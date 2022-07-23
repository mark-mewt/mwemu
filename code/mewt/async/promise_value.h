
#pragma once

#include "types/non_movable.h"

#include <optional>

namespace mewt::async
{

   template<typename _ReturnType>
   struct promise_value_t : non_movable_t
   {
      std::optional<_ReturnType> _return_value;
      inline void return_value(_ReturnType v) { _return_value.emplace(std::move(v)); }
   };

   template<>
   struct promise_value_t<void> : non_movable_t
   {
      inline void return_void() { }
   };

}
