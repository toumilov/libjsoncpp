#pragma once

#include <algorithm>

namespace jsoncpp
{

/**
 * Max class - compile time maximum calculation
 */
template <typename T, T val, T ... values>
struct Max
{
	typedef T type;
};

template <typename T, T val>
struct Max<T, val>
{
	typedef T type;
	static constexpr T value = val;
};

template <typename T, T val1, T val2, T ... values>
struct Max<T, val1, val2, values...>
{
	typedef T type;
	static constexpr T value = val1 >= val2 ? Max<T, val1, values...>::value : Max<T, val2, values...>::value;
};

/**
 * TypeIndex
 */
template <typename ...Ts>
struct TypeIndex;

template <typename T, typename ...Ts>
struct TypeIndex<T, T, Ts...> : std::integral_constant<unsigned, 1>{};

template <typename T1, typename T2, typename ...Ts>
struct TypeIndex<T1, T2, Ts...> : std::integral_constant<unsigned, 1 + TypeIndex<T1, Ts...>::value>{};

/**
 * Variant class
 */

template <typename ...Types>
class Variant
{
public:
	Variant() :
		type_( invalid_type_ ),
		destructor_( nullptr ),
		copier_( nullptr )
	{}
	Variant( const Variant &v ) :
		type_( invalid_type_ ),
		destructor_( nullptr ),
		copier_( nullptr )
	{
		clone( v );
	}
	Variant( Variant &v ) :
		type_( invalid_type_ ),
		destructor_( nullptr ),
		copier_( nullptr )
	{
		clone( v );
	}
	template <class T>
	Variant( const T &value ) :
		type_( invalid_type_ ),
		destructor_( nullptr ),
		copier_( nullptr )
	{
		build( value );
	}

	~Variant()
	{
		clear();
	}

	Variant& operator=( const Variant &v )
	{
		clone( v );
		return *this;
	}

	Variant& operator=( const Variant &&v )
	{
		clone( std::move( v ) );
		return *this;
	}

	template <class T>
	Variant& set( const T &v )
	{
		clone( v );
		return *this;
	}

	template <class T>
	bool is()
	{
		return type_ == TypeIndex<T, Types...>::value;
	}

	template <class T>
	T* get()
	{
		if ( type_ != TypeIndex<T, Types...>::value ) { return nullptr; }
		return reinterpret_cast<T*>( &data_ );
	}

	template <class T>
	const T* get() const
	{
		if ( type_ != TypeIndex<T, Types...>::value ) { return nullptr; }
		return reinterpret_cast<const T*>( &data_ );
	}

	unsigned type_index() const
	{
		return type_;
	}

	void clear()
	{
		if ( type_ )
		{
			destructor_( &data_ );
			type_ = invalid_type_;
		}
	}

	bool empty() const
	{
		return type_ == invalid_type_;
	}

	template <class Visitor>
	void accept( Visitor &v ) const
	{
		Dispatcher<Visitor, Types...>::dispatch( *this, v );
	}

	template <typename T, class Visitor>
	void accept( const T *value, Visitor &v ) const
	{
		v.operator()( value );
	}

private:
	template <typename Visitor, typename ...Ts>
	struct Dispatcher;

	template <typename Visitor>
	struct Dispatcher<Visitor>
	{
		static void dispatch( const Variant&, Visitor &visitor )
		{
			visitor();
		}
	};

	template <typename Visitor, typename T, typename ...Ts>
	struct Dispatcher<Visitor, T, Ts...>
	{
		static void dispatch( const Variant &v, Visitor &visitor )
		{
			const T *value = v.get<T>();
			if ( value )
			{
				visitor.operator()( *value );
			}
			else
			{
				Dispatcher<Visitor, Ts...>::dispatch( v, visitor );
			}
		}
	};

	static constexpr auto data_size_ = Max<size_t, sizeof(Types)...>::value;
	static constexpr auto data_align_ = Max<size_t, alignof(Types)...>::value;
	static constexpr unsigned invalid_type_ = 0;
	typedef typename std::aligned_storage<data_size_, data_align_>::type DataType;
	DataType data_;
	unsigned type_;
	void (*destructor_)( void* );
	void (*copier_)( const void*, void* );

	template <typename T>
	void build( const T &value )
	{
		struct Util
		{
			static void copy( const void *src, void *dst )
			{
				new( reinterpret_cast<T*>( dst ) ) T( *reinterpret_cast<const T*>( src ) );
			}
			static void destroy( void *data )
			{
				reinterpret_cast<T*>( data )->~T();
			}
		};
		type_ = TypeIndex<T, Types...>::value;
		destructor_ = &Util::destroy;
		copier_ = &Util::copy;
		Util::copy( &value, &data_ );
	}

	void clone( const Variant &value )
	{
		if ( this == &value ) { return; }
		clear();
		if ( value.type_ )
		{
			value.copier_( &value.data_, &data_ );
			type_ = value.type_;
			destructor_ = value.destructor_;
			copier_ = value.copier_;
		}
	}

	void clone( const Variant &&value )
	{
		clear();
		if ( value.type_ )
		{
			value.copier_( &value.data_, &data_ );
			type_ = value.type_;
			destructor_ = value.destructor_;
			copier_ = value.copier_;
		}
	}
};

} // namespace jsoncpp
