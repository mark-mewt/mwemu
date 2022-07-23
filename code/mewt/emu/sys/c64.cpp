
#include "emu/sys/c64.h"
#include "emu/clock.h"
#include "emu/cpu/mos_6502.h"
#include "emu/gpu/vic2.h"
#include "diag/log.h"
#include "async/awaitable_func.h"

namespace mewt::emu::sys
{

	void c64_t::run_sys()
	{
      clock_source_t clock;
      cpu::mos6502_t cpu(clock);
      gpu::vic2_t gpu(clock);
      logger().log("%s: %d", __FUNCTION__, 0);
      auto cr = cpu.run_cpu();
      auto gr = gpu.run_gpu();
      //run_async([&]() { return cpu.cpu_runner(); });
      //run_async([&]() { gpu.gpu_runner(); });
      logger().log("%s: %d", __FUNCTION__, 1);
      clock.run();
      logger().log("%s: %d", __FUNCTION__, 2);
   }

}

