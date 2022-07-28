
#pragma once

#include "types/unique_referencable.h"

#include <optional>
#include <coroutine>

namespace mewt::async
{

   template<typename _ReturnType>
   struct future : public types::unique_referencable<future<_ReturnType>>
   {
      std::optional<_ReturnType> _return_value;
      inline bool is_complete() const { return _return_value.has_value(); }
      inline void set_complete() { throw std::exception("uh?"); }
      std::coroutine_handle<> _continuation;
   };

   template<>
   struct future<void> : public types::unique_referencable<future<void>>
   {
      bool _is_complete = false;
      inline bool is_complete() const { return _is_complete; }
      inline void set_complete() { _is_complete = true; }
      std::coroutine_handle<> _continuation;
   };

}
