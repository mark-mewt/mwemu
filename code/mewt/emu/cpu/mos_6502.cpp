

#define __cpp_lib_coroutine 1

//#include <experimental/coroutine>

#include <coroutine>
#include <optional>

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace std::experimental
{
   using namespace std;
}

/*
* 
* ToDo:
* 
* - Move intrusive_stack out to another module (mewt::intrusive_stack)
* - Move non_movable_t out to another module (mewt::???)
* - Write a splitter for clock cycles, so we can run things on sub-cycles.
* - Can we separate the list of awaiters from the clock source (and remove the mutable)?
* - Allow awaitable func to return a value (eg, the loaded memory value)
* - Separate awaitable func and runnable func
* - Get C64 rom and start emulating it
* - Start emulating the Vic20
* - Enable all warnings in compiler - need to shift out the win32 stuff first
* - Make address sanitizer work
* - Compile with both clang and msvc
* 
*/

template<typename _ReturnType = void>
struct [[nodiscard]] awaitable_func_t;

template<typename _ReturnType>
struct func_awaiter_base_t
{
   func_awaiter_base_t(awaitable_func_t<_ReturnType>& func)
      : _func(func)
   {
   }
   constexpr bool await_ready() { return false; }
   bool await_suspend(std::coroutine_handle<> continuation);
protected:
   awaitable_func_t<_ReturnType>& _func;
};

template<typename _ReturnType>
struct func_awaiter_t : public func_awaiter_base_t<_ReturnType>
{
   using func_awaiter_base_t<_ReturnType>::func_awaiter_base_t;
   _ReturnType await_resume() { return *this->_func._promise._return_value; }
};

template<> // todo: we can merge this back into above using if constexpr
struct func_awaiter_t<void> : public func_awaiter_base_t<void>
{
   using func_awaiter_base_t<void>::func_awaiter_base_t;
   void await_resume() { }
};

struct non_movable_t
{
   constexpr non_movable_t() = default;
   constexpr non_movable_t(const non_movable_t&) = delete;
   constexpr non_movable_t(non_movable_t&&) = delete;
   constexpr non_movable_t& operator = (const non_movable_t&) = delete;
   constexpr non_movable_t& operator = (non_movable_t&&) = delete;
   constexpr ~non_movable_t() = default;
};

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

template<typename _InitialSuspend, typename _ReturnType>
struct promise_t : public promise_value_t<_ReturnType>
{
   auto initial_suspend() { return _InitialSuspend{}; }
   auto final_suspend() noexcept
   {
      // todo: because of noexcept, we really want to return an awaitable, which resumes the continuation for us when that is awaited
      if(_continuation)
         _continuation.resume();
      return std::suspend_never{};
   }
   auto& get_return_object() { return *this; }
   void unhandled_exception() { /* do something here */ }
   std::coroutine_handle<void> _continuation;
};

template<typename _ReturnType>
struct [[nodiscard]] awaitable_func_t
{
   using promise_type = promise_t<std::suspend_never, _ReturnType>;
   awaitable_func_t(promise_type& promise)
      : _promise(promise)
   {
   }
   func_awaiter_t<_ReturnType> operator co_await ()
   {
      return *this;
   }
   void set_continuation(std::coroutine_handle<> continuation)
   {
      _promise._continuation = continuation;
   }
   promise_type& _promise;
};

template<typename _ReturnType>
bool func_awaiter_base_t<_ReturnType>::await_suspend(std::coroutine_handle<> continuation)
{
   _func.set_continuation(continuation);
   //_continuation = continuation;
   // when _func is done, we need to resume _continuation
   // how do we know when _func is done?
   return true;
}

template<typename _ReturnType = void>
struct [[nodiscard]] runnable_func_t
{
   using promise_type = promise_t<std::suspend_always, _ReturnType>;
   runnable_func_t(promise_type& promise)
      : _initial(std::coroutine_handle<promise_type>::from_promise(promise))
   {
   }
   void start()
   {
      _initial.resume();
   }
private:
   std::coroutine_handle<promise_type> _initial;
};

class clock_tick_awaiter_t;
class clock_source_t;

class clock_tick_count_t
{
public:
   inline clock_tick_count_t(const clock_source_t& clock, size_t tick_count)
      : _clock(clock), _tick_count(tick_count)
   {
   }
   inline clock_tick_awaiter_t operator co_await ();
   inline auto& clock() const { return _clock; }
   size_t decrement() { return --_tick_count; }
private:
   const clock_source_t& _clock;
   size_t _tick_count;
};

template<typename _Elt>
class intrusive_stack
{
public:
   inline intrusive_stack() = default;
   inline intrusive_stack(const intrusive_stack&) = delete;
   inline intrusive_stack(intrusive_stack&& rhs) noexcept : _first(rhs._first) { rhs._first = nullptr; }
   inline intrusive_stack& operator = (const intrusive_stack&) = delete;
   inline intrusive_stack& operator = (intrusive_stack&& rhs) noexcept { _first = rhs._first; rhs._first = nullptr; }
   inline void push(_Elt& elt) noexcept
   {
      elt._next = _first;
      _first = std::addressof(elt);
   }
   inline bool is_empty() const noexcept { return _first == nullptr; }
   inline _Elt* pop() noexcept
   {
      auto* elt = _first;
      if(_first)
         _first = _first->_next;
      return elt;
   }
private:
   _Elt* _first = nullptr;
   friend _Elt;
   struct node_t
   {
      _Elt* _next = nullptr;
   };
};

class clock_tick_awaiter_t : public intrusive_stack<clock_tick_awaiter_t>::node_t
{
public:
   inline clock_tick_awaiter_t(clock_tick_count_t tick_count)
      : _tick_count(tick_count)
   {
   }
   constexpr bool await_ready() { return false; }
   void await_resume() { _continuation = {}; }
   bool await_suspend(std::coroutine_handle<> continuation);
//private: todo: make this whole class a private inner class, and eliminate any reason for the following to be private
   std::coroutine_handle<void> _continuation;
   clock_tick_count_t _tick_count;
   //clock_tick_awaiter_t* _next_awaiter = nullptr;
};

class clock_source_t
{
public:
   clock_tick_count_t next_tick() const
   {
      return { *this, 1 };
   }
   void add_awaiter(clock_tick_awaiter_t& awaiter) const
   {
      _awaiters.push(awaiter);
   }
   void run();
private:
   mutable intrusive_stack<clock_tick_awaiter_t> _awaiters;
   //mutable clock_tick_awaiter_t* _first_awaiter = nullptr;
};

bool clock_tick_awaiter_t::await_suspend(std::coroutine_handle<> continuation)
{
   _continuation = continuation;
   _tick_count.clock().add_awaiter(*this);
   return true;
}

inline clock_tick_awaiter_t clock_tick_count_t::operator co_await () { return *this; }

void clock_source_t::run()
{
   while (!_awaiters.is_empty())
   {
      auto prev_awaiters = std::move(_awaiters);
      while (auto* awaiter = prev_awaiters.pop())
      {
         if (awaiter->_tick_count.decrement() == 0)
            awaiter->_continuation.resume();
         else
            _awaiters.push(*awaiter);
      }

/*      auto* current_awaiter = _first_awaiter;
      auto** current_awaiter_ptr = std::addressof(_first_awaiter);
      while (current_awaiter)
      {
         auto* next_awaiter = current_awaiter->_next_awaiter;
         if (current_awaiter->_tick_count.decrement() == 0)
         {
            *current_awaiter_ptr = next_awaiter;
            current_awaiter->_next_awaiter = nullptr;
            auto d0 = current_awaiter->_continuation.done();
            current_awaiter->_continuation.resume();
            // appears to not always work when gpu and cpu are both present - may need to move assign the resume stack first, before resuming them all.
         }
         else
            current_awaiter_ptr = std::addressof(current_awaiter->_next_awaiter);
         current_awaiter = next_awaiter;
      }*/
   }
}

static void dlog(int line, const char* func, int idx)
{
   static int counter = 0;
   char buf[256];
   sprintf_s(buf, "[%d]: %s(%d) L: %d\n", counter++, func, idx, line);
   ::OutputDebugStringA(buf);
}

#define LOG(n) dlog(__LINE__, __FUNCTION__, n)

class cpu_t
{
private:
   const clock_source_t& _clock;
public:
   cpu_t(const clock_source_t& clock)
      : _clock(clock)
   {
      LOG(0);
   }
   awaitable_func_t<uint8_t> read_mem()
   {
      LOG(0);
      co_await _clock.next_tick();
      LOG(1);
      co_await _clock.next_tick();
      LOG(2);
      co_await _clock.next_tick();
      LOG(3);
      co_return 42;
   }
   awaitable_func_t<> write_mem(uint8_t v)
   {
      LOG(0);
      co_await _clock.next_tick();
      LOG(1);
      co_await _clock.next_tick();
      LOG(2);
      co_await _clock.next_tick();
      LOG(3);
   }
   awaitable_func_t<> run_inst()
   {
      LOG(0);
      auto v = co_await read_mem();
      LOG(1);
      co_await _clock.next_tick();
      LOG(2);
      co_await _clock.next_tick();
      LOG(3);
      co_await _clock.next_tick();
      LOG(4);
      co_await write_mem(v);
      LOG(5);
   }
   runnable_func_t<> cpu_runner()
   {
      LOG(0);
      co_await run_inst();
      LOG(1);
      co_await run_inst();
      LOG(2);
      co_await run_inst();
      LOG(3);
   }
};

class gpu_t
{
private:
   const clock_source_t& _clock;
public:
   gpu_t(const clock_source_t& clock)
      : _clock(clock)
   {
      LOG(0);
   }
   awaitable_func_t<> read_mem()
   {
      LOG(0);
      co_await _clock.next_tick();
      LOG(1);
      co_await _clock.next_tick();
      LOG(2);
   }
   awaitable_func_t<> run_inst()
   {
      LOG(0);
      co_await read_mem();
      LOG(1);
      co_await _clock.next_tick();
      LOG(2);
      co_await _clock.next_tick();
      LOG(3);
   }
   runnable_func_t<> gpu_runner()
   {
      LOG(0);
      co_await run_inst();
      LOG(1);
      co_await run_inst();
      LOG(2);
   }
};

int main()
{
   clock_source_t clock;
   cpu_t cpu(clock);
   gpu_t gpu(clock);
   LOG(0);
   cpu.cpu_runner().start();
   gpu.gpu_runner().start();
   LOG(1);
   clock.run();
   LOG(2);
   return 0;
}