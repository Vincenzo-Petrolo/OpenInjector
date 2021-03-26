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


std::unique_ptr<sandbox2::Policy> GetPolicy() {
	 sandbox2::PolicyBuilder builder;
	std::cout << "Policy crafted" << std::endl;
	 return builder
	.EnableNamespaces()
	.DangerDefaultAllowAll()
	.BuildOrDie();
}

Tester::Tester(binary_t *goldenBinary,size_t size)
{
	std::cout << "Tester" << std::endl;
	this->goldenStatistics = new Statistics();
	test(goldenBinary,size,this->goldenStatistics);
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
	.set_rlimit_cpu(60)
	.set_walltime_limit(absl::Seconds(30));
	std::cout << "test" << std::endl;
	
	std::cout << "prima di pol" << std::endl;
	auto policy = GetPolicy();
	std::cout << "getpol" << std::endl;
	sandbox2::Sandbox2 s2(std::move(executor), std::move(policy));

	std::cout << "dopo s2" << std::endl;
#if 0
	std::chrono::high_resolution_clock::time_point start_time = 
	std::chrono::high_resolution_clock::now();
	auto result = s2.Run();
	std::cout << "dopo run" << std::endl;
	std::chrono::high_resolution_clock::time_point stop_time = 
	std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = 
std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - 
start_time);
	std::cout << "after run" << std::endl;
	statsContainer->setTime(time.count());
	std::cout << "Result : " << result.ToString() << std::endl;
#endif
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

std::string Tester::statsConstructor(Tester::mode_t mode)
{
	std::string statistics;
	std::string averages;
	std::string result;
	int i = 0;
	double tot_time = 0;
	double average = 0;
	for (Statistics *s : this->faultyStatistics) {
#define DBG 0
#if DBG
		if (s->getResult().final_status() == sandbox2::Result::OK) {
#endif
		statistics.append("\"#").append(std::to_string(i)).append("\",") 
		.append(std::to_string(s->getTime()))
		.append("\n");
		i++;
		tot_time += s->getTime();
#if DBG
		} else {
		statistics.append("\"#").append(std::to_string(i)).append("\",")
		.append(s->getResult().StatusEnumToString(
			s->getResult().final_status()
		))
		.append("\n");
		}
#endif
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

void Tester::stringifyStats()
{
	std::cout << "===============Statistics===============" << std::endl;
	std::cout << statsConstructor(O_SHORT) << std::endl;
	std::cout << "===============End===============" << std::endl;

}

void Tester::writeCSV(Tester::mode_t mode)
{
	std::fstream stats_out;
	stats_out.open("statistics.csv",std::ios::out);
	stats_out << statsConstructor(mode) << std::endl;
}
