
#pragma once

#include "mewt/async/event.h"
#include "mewt/gfx/image.h"

namespace mewt::emu {

	class IHost
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
		Events events;

		void runEmuHost();

		virtual ~IHost() = default;

	protected:
		virtual void initHost() = 0;

		Config _host_config;
	};

}
