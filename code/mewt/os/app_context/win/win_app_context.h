
#pragma once

#include "mewt/os/app_context/app_context.h"
#include "mewt/os/base/win_base.h"

namespace mewt::os::win {

	class app_context_t : public app_context_i {

	public:
		inline const auto& init_data() const { return _init_data; }

	private:
		struct init_data_t {
			HINSTANCE _instance;
			HINSTANCE _previous_instance;
			PSTR _command_line;
			int _show_command;
		};

		init_data_t _init_data;

		inline app_context_t(init_data_t init_data) : _init_data(init_data) {}

		friend int APIENTRY ::WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ PSTR cmdline, _In_ int cmdshow);
	};

}
