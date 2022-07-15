

#include <coroutine>

struct promise_t;

struct func_awaiter_t
{
   constexpr bool await_ready() { return false; }
   void await_resume() { _continuation = {}; }
   bool await_suspend(std::coroutine_handle<> continuation)
   {
      _continuation = continuation;
      return true;
   }
private:
   std::coroutine_handle<void> _continuation;
};

struct awaitable_func_t
{
   using promise_type = promise_t;
   awaitable_func_t(std::coroutine_handle<promise_t> continuation)
   {
   }
   func_awaiter_t operator co_await ()
   {
      return { };
   }
};

struct promise_t
{
   constexpr auto initial_suspend() { return std::suspend_never{}; }
   constexpr auto final_suspend() noexcept { return std::suspend_never{}; }
   //constexpr void get_return_object() { }
   void return_void() { /* do we need to indicate we're done? */ }
   auto get_return_object() { return std::coroutine_handle<promise_t>::from_promise(*this); }
   void unhandled_exception() { /* do something here */ }
};

struct clock_tick_awaiter_t
{
   constexpr bool await_ready() { return false; }
   void await_resume() { _continuation = {}; }
   bool await_suspend(std::coroutine_handle<> continuation)
   {
      _continuation = continuation;
      return true;
   }
private:
   std::coroutine_handle<void> _continuation;
};

struct clock_tick_t
{
   clock_tick_awaiter_t operator co_await ()
   {
      return { };
   }
};

struct clock_source_t
{
   clock_tick_t next_tick() const
   {
      return { };
   }
};

class cpu_t
{
private:
   clock_source_t _master_clock;
public:
   awaitable_func_t read_mem()
   {
      co_await _master_clock.next_tick();
   }
   awaitable_func_t write_mem()
   {
      co_await _master_clock.next_tick();
   }
   awaitable_func_t run_inst()
   {
      co_await read_mem();
      co_await _master_clock.next_tick();
      co_await write_mem();
   }
};

int main()
{
   cpu_t cpu;
   cpu.run_inst();
   return 0;
}