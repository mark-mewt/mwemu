
#pragma once

namespace mewt
{

   struct non_movable_t
   {
      constexpr non_movable_t() = default;
      constexpr non_movable_t(const non_movable_t&) = delete;
      constexpr non_movable_t(non_movable_t&&) = delete;
      constexpr non_movable_t& operator = (const non_movable_t&) = delete;
      constexpr non_movable_t& operator = (non_movable_t&&) = delete;
      inline ~non_movable_t() = default;
   };

}
