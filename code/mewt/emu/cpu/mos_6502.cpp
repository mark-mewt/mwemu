

#include <coroutine>

struct awaitable_t
{
};

template<typename ic_t>
struct promise_t
{
   constexpr auto initial_suspend() { return std::suspend_never{}; }
   constexpr auto final_suspend() noexcept { return std::suspend_never{}; }
};

template<typename ic_t>
struct std::coroutine_traits<awaitable_t, ic_t&>
{
   using promise_type = promise_t<ic_t>;
};

struct tick_t
{
   constexpr bool await_ready() { return false; }
};

class cpu_t
{
public:
   awaitable_t read_mem()
   {
      co_await tick_t{};
   }
   awaitable_t write_mem()
   {
      co_await tick_t{};
   }
   awaitable_t run_inst()
   {
      co_await read_mem();
      co_await tick_t{};
      co_await write_mem();
   }
};

