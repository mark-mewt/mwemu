
#pragma once

#include "mewt/async/event.h"
#include "mewt/gfx/image.h"
#include "mewt/types/interface.h"

namespace mewt::emu
{

	class IHost : public types::IInterface
	{
	public:
		struct Config
		{
			gfx::Image::Size display_size;
		};
		struct Frame;

		struct Events
		{
			async::Event<Config> initialising;
			async::Event<Frame> need_frame;
		};
		[[nodiscard]] inline auto events() -> Events& { return _events; }

		void runEmuHost();

		// IHost(IHost&&) = delete;
		// IHost(const IHost&) = delete;
		// auto operator=(IHost&&) = delete;
		// auto operator=(const IHost&) = delete;

		// virtual ~IHost() = default;

	protected:
		virtual void initHost() = 0;
		[[nodiscard]] inline auto hostConfig() const -> const Config& { return _host_config; }

	private:
		Config _host_config;
		Events _events;
	};

}
