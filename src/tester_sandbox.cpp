#include "tester.h"
#include "sandboxed_api/util/flag.h"
#include "absl/memory/memory.h"
#include "sandboxed_api/sandbox2/executor.h"
#include "sandboxed_api/sandbox2/limits.h"
#include "sandboxed_api/sandbox2/policy.h"
#include "sandboxed_api/sandbox2/policybuilder.h"
#include "sandboxed_api/sandbox2/sandbox2.h"
#include "sandboxed_api/sandbox2/util/bpf_helper.h"

#include <iostream>
#include <vector>

std::unique_ptr<sandbox2::Policy> GetPolicy() {
	return sandbox2::PolicyBuilder()
      .AllowExit()
      .AllowStaticStartup()
      .AllowDynamicStartup()
      .EnableNamespaces()
      .BuildOrDie();
}

void Tester::test(binary_t* target,size_t size, Statistics *statsContainer)
{
	FILE *fp = fopen("target","wb");
	if (!fp)
		throw "Error during writing of file";
	fwrite(target,sizeof(binary_t),size,fp); //writes binary
	fclose(fp);
	std::string path = "target";
	std::vector<std::string> args = {path};
	std::vector<std::string> envs = {};
	auto executor = absl::make_unique<sandbox2::Executor>(path, args, envs);
	 executor
      // Sandboxing is enabled by the sandbox itself. The sandboxed binary is
      // not aware that it'll be sandboxed.
      // Note: 'true' is the default setting for this class.
      ->set_enable_sandbox_before_exec(true)
      .limits()
      // Remove restrictions on the size of address-space of sandboxed
      // processes.
      ->set_rlimit_as(RLIM64_INFINITY)
      // Kill sandboxed processes with a signal (SIGXFSZ) if it writes more than
      // these many bytes to the file-system.
      .set_rlimit_fsize(1024)
      // The CPU time limit.
      .set_rlimit_cpu(60)
      .set_walltime_limit(absl::Seconds(30));


	auto *comms = executor->ipc()->comms();

	sandbox2::Sandbox2 s2(std::move(executor),GetPolicy());
	//run the sandboxee
	std::chrono::high_resolution_clock::time_point start_time = 
std::chrono::high_resolution_clock::now();
	auto result = s2.Run();
	std::cout << result.ToString() << std::endl;
	std::chrono::high_resolution_clock::time_point stop_time = 
std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = 
std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - 
start_time);
	statsContainer->setTime(time.count());
}
