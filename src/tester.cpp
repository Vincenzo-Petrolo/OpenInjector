/*
 * OpenInjector is an open source and free framework, used to run
 * fault injection campaigns on binary executables, and to test
 * their errors resilience.
 * Copyright (C) 2020  Vincenzo Petrolo - vincenzo@kernel-space.org
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <fcntl.h>
#include <sys/resource.h>
#include <syscall.h>
#include <unistd.h>
#include <fstream>
#include <csignal>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <sys/stat.h>
#include <glog/logging.h>
#include "sandboxed_api/util/flag.h"
#include "absl/memory/memory.h"
#include "sandboxed_api/config.h"
#include "sandboxed_api/sandbox2/executor.h"
#include "sandboxed_api/sandbox2/limits.h"
#include "sandboxed_api/sandbox2/policy.h"
#include "sandboxed_api/sandbox2/policybuilder.h"
#include "sandboxed_api/sandbox2/result.h"
#include "sandboxed_api/sandbox2/sandbox2.h"
#include "sandboxed_api/sandbox2/util/bpf_helper.h"
#include "sandboxed_api/util/runfiles.h"

#include "tester.h"
#include "statistics.h"


std::unique_ptr<sandbox2::Policy> GetPolicy(Tester::policy_t p) {
	 sandbox2::PolicyBuilder builder;
	 
	 if (p == Tester::P_VERY_STRICT) {
		 return builder
		 .EnableNamespaces()
		 .BuildOrDie();
	 } else if (p == Tester::P_STRICT) {
		 return builder
		 .EnableNamespaces()
		 .AllowRead()
		 .AllowOpen()
		 .BuildOrDie();
	 } else if (p == Tester::P_NORMAL) {
		 return builder
		 .EnableNamespaces()
		 .AllowRead()
		 .AllowWrite()
		 .AllowOpen()
		 .BuildOrDie();
	 } else if (p == Tester::P_FLEXIBLE) {
		return builder
		.EnableNamespaces()
		.DangerDefaultAllowAll()
		.BuildOrDie();
	 }

	 return builder
	.EnableNamespaces()
	.DangerDefaultAllowAll()
	.BuildOrDie();
}

Tester::Tester(binary_t *goldenBinary,size_t size)
{
	this->injection_count = 0;
	this->goldenStatistics = new Statistics();
	this->policy = P_FLEXIBLE;
	this->timeout_ms = 1000000;
	/*Executing some times and getting only the last time*/
	test(goldenBinary,size,this->goldenStatistics);
	test(goldenBinary,size,this->goldenStatistics);
	test(goldenBinary,size,this->goldenStatistics);
	/*After executing golden binary setting to default
	 the execution time*/
	this->timeout_ms = GOLDEN_TIME_TIMES*this->goldenStatistics->getTime();
}

Tester::~Tester()
{

}

void Tester::test(binary_t* target,size_t size, Statistics *statsContainer)
{
	const std::string path = "target";
	const std::string mode = "0755";
	int i = strtoul(mode.c_str(),0,8);
	FILE *fp = fopen(path.c_str(),"wb");
	if (!fp)
		throw "Error during writing of file";
	fwrite(target,sizeof(binary_t),size,fp); //writes binary
	fclose(fp);
	chmod(path.c_str(),i);

	std::vector<std::string> args = {path};
	auto executor = absl::make_unique<sandbox2::Executor>(path, args);

	executor
	->set_enable_sandbox_before_exec(true)
	.limits()
	->set_rlimit_as(RLIM64_INFINITY)
	.set_rlimit_fsize(1024 * 1024)
	.set_walltime_limit(
		absl::Milliseconds(
		this->timeout_ms
		)
	);
	
	auto policy = GetPolicy(this->policy);
	sandbox2::Sandbox2 s2(std::move(executor), std::move(policy));

	std::chrono::high_resolution_clock::time_point start_time = 
	std::chrono::high_resolution_clock::now();
	auto result = s2.Run();
	std::chrono::high_resolution_clock::time_point stop_time = 
	std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration_tmp = 
	std::chrono::duration_cast<std::chrono::duration<double>>
	(stop_time - start_time);

	std::chrono::duration<double,std::milli> time = 
	std::chrono::duration_cast<
	std::chrono::milliseconds>(duration_tmp);
	statsContainer->setTime(time.count());
	statsContainer->setExitStatus(result.final_status());
	this->injection_count++;

	/*Append to temporary file*/
	appendStats(statsContainer);
}

void Tester::testFaulty(binary_t* target, size_t size)
{
	Statistics *s = new Statistics();
	this->test(target,size,s);
	this->faultyStatistics.push_back(s);
}

Statistics Tester::getGoldenStats()
{
	return *this->goldenStatistics;
}

/**TODO remove this function
 */
std::string Tester::statsConstructor(Tester::mode_t mode)
{
	std::string statistics;
	std::string averages;
	std::string result;
	int i = 0;
	double tot_time = 0;
	double average = 0;
	for (Statistics *s : this->faultyStatistics) {
		if (s->getExitStatus() == sandbox2::Result::OK) {
		statistics
		.append("\"#")
		.append(std::to_string(i))
		.append("\",") 
		.append(std::to_string(s->getTime()))
		.append("\n");
		tot_time += s->getTime();
		} else {
		statistics.append("\"#").append(std::to_string(i)).append("\",")
		.append(sandbox2::Result::StatusEnumToString(
			(sandbox2::Result::StatusEnum) s->getExitStatus()
		))
		.append("\n");
		}
		i++;
	}
	average = tot_time/i;
	averages.append("\"Average faulty time\",").append(std::to_string(average)).append("\n"); 
	averages.append("\"Golden time\",").append(std::to_string(goldenStatistics->getTime())).append("\n");
	if (mode == O_DETAILED) {
		result.append(statistics);
	}
	result.append(averages);
	return result;
}

void Tester::appendStats(Statistics* s)
{
	std::string line;
	
	if (s->getExitStatus() == sandbox2::Result::OK) {
		line
		.append("\"#")
		.append(std::to_string(this->injection_count))
		.append("\",") 
		.append(std::to_string(s->getTime()))
		.append("\n");
	} else {
		line
		.append("\"#")
		.append(std::to_string(this->injection_count))
		.append("\",")
		.append(sandbox2::Result::StatusEnumToString(
			(sandbox2::Result::StatusEnum) s->getExitStatus()
		))
		.append("\n");
		}
	std::fstream stats_out("statistics_backup.csv",std::fstream::in | 
						std::fstream::out | 
						std::fstream::app
		     );
	
	stats_out << line;
	
	stats_out.close();
}


void Tester::writeStats( Tester::file_t file_type, 
			 Tester::mode_t mode, 
			std::string filename
			)
{
	std::string time_section = "";
	
	
	/**Section where we process all final
	 * statistics
	 */
	// AVERAGE TIME OF EXECUTION - GOLDEN TIME
	double average_time = 0.0;
	int n = 0;
	for (Statistics *s : this->faultyStatistics) {
		if (s->getExitStatus() == sandbox2::Result::OK) {
			average_time += s->getTime();
			n++;
		}
	}
	average_time /= n;
	time_section
		.append("\"Average faulty time\",")
		.append(std::to_string(average_time))
		.append("\n"); 
	time_section
		.append("\"Golden time\",")
		.append(std::to_string(goldenStatistics->getTime()))
		.append("\n");
	
	// DETAILED STATS 
	std::string details = "";
	n = 0;
	if (mode == O_DETAILED) {
		for (Statistics *s : this->faultyStatistics) {
			details
			.append("\"#")
			//ID
			.append(std::to_string(n))
			.append("\",");
			if (s->getExitStatus() == sandbox2::Result::OK) {
				details
				//TIME
				.append(std::to_string(s->getTime()))
				//APPEND OTHERS FROM HERE..
				.append("\n");
			} else {
				details
				.append(sandbox2::Result::StatusEnumToString(
					(sandbox2::Result::StatusEnum)
					s->getExitStatus()
				))
				.append("\n");
			}
			n++;
		}
	}
	
	/*
	 * Saving all in the file
	 */
	std::fstream stats_out;
	
	if (file_type == F_STDOUT) {
		std::cout << details << std::endl;
		std::cout << time_section << std::endl;
	} else if (file_type == F_CSV) {
		stats_out = std::fstream(filename.c_str(),
					std::fstream::out);
		stats_out << details;
		stats_out << time_section;
	}

}

void Tester::setExecutionTimeout(int timeout)
{
	this->timeout_ms = timeout;
}


void Tester::setPolicy(Tester::policy_t p)
{
	this->policy = p;
}
