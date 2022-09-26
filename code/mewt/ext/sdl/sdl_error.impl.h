
#pragma once

#include "mewt/diag/source_location.h"

namespace mewt::ext::sdl
{

	void throwError(diag::SourceLocation loc = diag::SourceLocation::current());

	inline void checkStatus(int status, diag::SourceLocation loc = diag::SourceLocation::current())
	{
		if (status != 0)
			throwError(loc);
	}

	inline auto checkPointer(auto* object, diag::SourceLocation loc = diag::SourceLocation::current())
		 -> auto*
	{
		if (object == nullptr)
			throwError(loc);
		return object;
	}

}
