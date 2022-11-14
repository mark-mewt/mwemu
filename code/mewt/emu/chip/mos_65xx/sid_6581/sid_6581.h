
#pragma once

#include "mewt/async/future_promise.decl.h"
#include "mewt/emu/chip/clock/clock.decl.h"
#include "mewt/emu/chip/mos_65xx/mos_65xx.h"
#include "mewt/types/address_space.h"
#include "mewt/emu/mem/interface/interface.RegisterStruct.h"

#include <array>

namespace mewt::emu::chip::mos_65xx
{

	class SID
	{
	private:
		const clock_source_t& _clock;

		static constexpr mem::BusSpec<SID> kSIDBusSpec { .address_bits = { 5 } };

		friend constexpr auto getBusSpecForDevice(types::traits::TypeId<SID> /*unused*/)
		{
			return kSIDBusSpec;
		}

			struct Registers
		{
			// https://www.c64-wiki.com/wiki/SID
			struct Voice
			{
				Byte _frequency_low;
				Byte _frequency_high;
				Byte _pulse_wave_duty_cycle_low;
				Byte _pulse_wave_duty_cycle_high;
				Byte _control_register;
				Byte _attack_decay;
				Byte _sustain_release;
			};

			std::array<Voice, 3> _voice;
			Byte _filter_cutoff_freq_low;
			Byte _filter_cutoff_freq_high;
			Byte _filter_resonance_routing;
			Byte _filter_mode_volume;
			Byte _paddle_x;
			Byte _paddle_y;
			Byte _oscillator_voice_3;
			Byte _envelope_voice_3;
		};

			#if 0
		class IOController // : public mem::IMemoryInterface<SID>
		{
		private:
			// SID& _sys;


			#if 0
			friend constexpr auto
			declareEquivalentSpaces(types::traits::TypeId<mem::AddressSpace<kSIDBusSpec>> /*unused*/,
											types::traits::TypeId<types::traits::Layout<Regs>> /*unused*/)
			{
				return true;
			}
			#endif

			static constexpr size_t kExpectedRegsSize = 0x1d;
			static_assert(sizeof(Regs) == kExpectedRegsSize);
			Regs _regs {};

		public:
			// IOController(SID& sys)
			//: _sys(sys)
			//{
			//}

			auto read(Address address) -> Data final;
			void write(Address address, Data data) final;
		};
		#endif

		using MemoryController = mem::interface::RegisterStruct<Registers>;
		MemoryController _memoryController;
		//IOController _io_controller;
		//{ *this };

	public:
		using Address = typename MemoryController::Address;
		explicit SID(const clock_source_t& clock);
		auto readMem() -> async::Future<>;
		auto runInst() -> async::Future<>;
		auto runGPU() -> async::Future<>;

		auto memoryController() -> MemoryController& { return _memoryController; }

	};

}
