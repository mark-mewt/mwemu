
#pragma once

#include <tuple>

namespace mewt::types
{

	template<typename _Derived>
	class unique_referencable
	{

	public:

		class reference_t
		{
		public:
			inline reference_t() = default;
			inline reference_t(_Derived& object) : _object(&object) { }
			reference_t(const reference_t&) = delete;
			inline reference_t(reference_t&& rhs)
				: _object(rhs._object)
			{
				if (_object)
					_object->_reference = this;
				rhs._object = nullptr;
			}
			reference_t& operator = (const reference_t&) = delete;
			inline reference_t& operator = (reference_t&& rhs)
			{
				if (_object)
					_object->_reference = nullptr;
				_object = rhs._object;
				if (_object)
					_object->_reference = this;
				rhs._object = nullptr;
				return *this;
			}
			inline ~reference_t()
			{
				if (_object)
					_object->_reference = nullptr;
			}
			inline bool is_valid() const { return _object != nullptr; }
			inline _Derived* operator -> () const { return _object; }
		private:
			friend unique_referencable<_Derived>;
			_Derived* _object = nullptr;
		};

		unique_referencable() = default;
		unique_referencable(const unique_referencable&) = delete;
		inline unique_referencable(unique_referencable&& rhs)
			: _reference(rhs._reference)
		{
			if (_reference)
				_reference->_object = this_object();
			rhs._reference = nullptr;
		}
		unique_referencable& operator = (const unique_referencable&) = delete;
		inline unique_referencable& operator = (unique_referencable&& rhs)
		{
			if (_reference)
				_reference->_object = nullptr;
			_reference = rhs._reference;
			if (_reference)
				_reference->_object = this;
			rhs._reference = nullptr;
			return *this;
		}

		inline ~unique_referencable()
		{
			if (_reference)
				_reference->_object = nullptr;
		}

		inline bool has_reference() const { return _reference != nullptr; }
	private:

		reference_t* _reference = nullptr;
		inline _Derived* this_object() { return static_cast<_Derived*>(this); }

	};

	template<typename _Type>
	inline auto make_unique_reference()
	{
		_Type t;
		typename _Type::reference_t ref(t);
		return std::tuple(std::move(t), std::move(ref));
	}

}
