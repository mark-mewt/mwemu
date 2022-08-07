
#pragma once

namespace mewt::types {

	template <typename _Type>
	requires(std::is_same_v<_Type, std::decay_t<_Type>>)
	struct default_traits {
	};

	template <typename _Type>
	requires(std::is_same_v<_Type, std::decay_t<_Type>>)
	struct class_id {
		using type_t = _Type;
	};
	template <typename _Type>
	using class_id_of = class_id<std::decay_t<_Type>>;
	
	template <typename _Type>
	constexpr auto get_type_traits(class_id<_Type>) -> class_id<default_traits<_Type>>;

	template <typename _Type>
	constexpr auto dummy_arg() -> _Type;

	namespace adl {
		template<typename _Type>
		struct trait_detector {
			using traits_t = typename decltype(get_type_traits(class_id_of<_Type>()))::type_t;
		};
	}

	template <typename _Type>
	using traits = typename adl::trait_detector<_Type>::traits_t;
	//struct traits : public default_traits<_Type> {
	//};

	template <typename _Type>
	constexpr _Type copy_of(const _Type& object) { return object; }

}
