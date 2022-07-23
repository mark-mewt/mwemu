
#include "diag/log.h"

#include <stdarg.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace mewt::diag
{

   log_context_t::log_context_t(std::source_location loc)
      : _loc(loc)
   {
   }

   void log_context_t::log(const char* fmt, ...)
	{
      va_list arg_ptr;
      char buf[1024];
      va_start(arg_ptr, fmt);
      vsprintf_s(buf, fmt, arg_ptr);
      va_end(arg_ptr);
      char buf2[1024];
      sprintf_s(buf2, "%s(%d): [%s] %s\n", _loc.file_name(), _loc.line(), _loc.function_name(), buf);
      ::OutputDebugStringA(buf2);
   }

	log_context_t logger(std::source_location loc) { return loc; }

}
