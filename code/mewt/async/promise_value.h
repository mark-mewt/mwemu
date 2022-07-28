
#pragma once

#include "types/non_movable.h"
#include "async/future.h"

#include <optional>

namespace mewt::async
{

   template<typename _ReturnType>
   struct promise_value_base_t : non_movable_t
   {
      future<_ReturnType>::reference_t _future;
   };

   template<typename _ReturnType>
   struct promise_value_t : public promise_value_base_t<_ReturnType>
   {
      inline void return_value(_ReturnType v) { this->_future->_return_value.emplace(std::move(v)); }
   };

   template<>
   struct promise_value_t<void> : promise_value_base_t<void>
   {
      inline void return_void() { this->_future->_is_complete = true; }
   };

}
