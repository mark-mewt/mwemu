
#pragma once

#ifdef __clang__
#include <stdint.h>
namespace std {
	struct source_location {
		[[nodiscard]] static constexpr source_location current(const uint_least32_t _Line_ = __builtin_LINE(),
			 const uint_least32_t _Column_ = __builtin_COLUMN(), const char* const _File_ = __builtin_FILE(),
			 const char* const _Function_ = __builtin_FUNCTION()) noexcept {
			source_location _Result;
			_Result._Line = _Line_;
			_Result._Column = _Column_;
			_Result._File = _File_;
			_Result._Function = _Function_;
			return _Result;
		}

		[[nodiscard]] constexpr source_location() noexcept = default;

		[[nodiscard]] constexpr uint_least32_t line() const noexcept { return _Line; }
		[[nodiscard]] constexpr uint_least32_t column() const noexcept { return _Column; }
		[[nodiscard]] constexpr const char* file_name() const noexcept { return _File; }
		[[nodiscard]] constexpr const char* function_name() const noexcept { return _Function; }

	private:
		uint_least32_t _Line{};
		uint_least32_t _Column{};
		const char* _File = "";
		const char* _Function = "";
	};
}
#endif

#include <source_location>

namespace mewt::diag {

	class log_context_t {

	public:
		log_context_t(std::source_location loc);

		void log(const char* fmt, ...);

	private:
		std::source_location _loc;
	};

	log_context_t logger(std::source_location loc = std::source_location::current());

}

namespace mewt {
	using diag::logger;
}
