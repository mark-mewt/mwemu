
#pragma once
/*
#include <utility>

namespace mewt::types {

	struct delegate_t {
		virtual void invoke() = 0;
		virtual ~delegate_t() = default;
	};	// #todo: move elsewhere

	struct allocator_t {
		virtual void* allocate(size_t size, std::align_val_t align) = 0;
		virtual void deallocate(void* ptr) = 0;
		virtual ~allocator_t() = default;
		static allocator_t& get_global(); // bad
	}; // #todo: move elsewhere

	template <typename _Type>
	class data_stack_ptr {

	public:

		inline data_stack_ptr() noexcept = default;
		inline data_stack_ptr(_Type* object, delegate_t* destroyer) : _object(object), _destroyer(destroyer) {}
		inline data_stack_ptr(const data_stack_ptr&) = delete;
		inline data_stack_ptr(data_stack_ptr&& rhs) : _object(rhs._object), _destroyer(rhs._destroyer) {
			rhs._object = nullptr;
			rhs._destroyer = nullptr;
		}
		template<typename _OtherType>
		inline data_stack_ptr(data_stack_ptr<_OtherType>&& rhs) : _object(rhs._object), _destroyer(rhs._destroyer) {
			rhs._object = nullptr;
			rhs._destroyer = nullptr;
		}

		inline ~data_stack_ptr() {
			if (_destroyer)
				_destroyer->invoke();
		}

		inline data_stack_ptr& operator=(const data_stack_ptr&) = delete;
		inline data_stack_ptr& operator=(data_stack_ptr&& rhs) {
			if (_destroyer)
				_destroyer->invoke();
			_object = rhs._object;
			_destroyer = rhs._destroyer;
			rhs._object = nullptr;
			rhs._destroyer = nullptr;
			return *this;
		}

		inline const _Type* operator->() const { return _object; }
		inline _Type* operator->() { return _object; }

		template<typename ... _Args>
		inline static data_stack_ptr allocate(allocator_t& allocator, _Args&&... args) {
			// #todo: move all this and the utility include to a .impl.h file
			struct destroyer_t : public delegate_t {
				allocator_t& _allocator;
				_Type* _object;
				void* _mem;
				size_t _size;
				destroyer_t(allocator_t& allocator, _Type * object, void* mem, size_t size)
					 : _allocator(allocator), _object(object), _mem(mem), _size(size) {

				}
				void invoke() override final {
					_object->~_Type();
					_allocator.deallocate(_mem);
				}
			};
			size_t align = std::max(alignof(destroyer_t), alignof(_Type));
			size_t object_size = (sizeof(_Type) + align - 1) & ~(align - 1);
			size_t total_size = object_size + sizeof(destroyer_t);
			void* mem = allocator.allocate(total_size, (std::align_val_t)align);
			_Type* object = new (mem) _Type(std::forward<_Args>(args)...);
			destroyer_t* destroyer = new ((char*)mem + object_size) destroyer_t{allocator, object, mem, total_size};
			return data_stack_ptr(object, destroyer);
		}

	private:
		_Type* _object = nullptr;
		delegate_t* _destroyer = nullptr;
		template <typename _OtherType>
		friend class data_stack_ptr;

	};

}*/

