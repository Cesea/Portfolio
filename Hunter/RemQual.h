#ifndef REM_QUAL_H
#define REM_QUAL_H

//Pointer Checking///////////////////////////////////////////////
template <typename T>
struct IsPointer
{
	static const bool value = false;
};
template <typename T>
struct IsPointer<T *>
{
	static const bool value = true;
};
template <typename T>
struct IsPointer<T &>
{
	static const bool value = false;
};

// Array Checking///////////////////////////////////////////////
template <typename T>
struct IsArray
{
	static const bool value = false;
};
template <typename T>
struct IsArray<T *>
{
	static const bool value = false;
};
template <typename T>
struct IsArray<T &>
{
	static const bool value = false;
};

#define INTERNAL_ARRAY_OVERLOAD( X ) \
	template <typename T> \
	struct RemQual<T[X]> \
	{ \
		typedef typename RemQual<T>::type type; \
	}; \
	template <typename T> \
	struct RemQual<const T[X]> \
	{ \
		typedef typename RemQual<T>::type type; \
	}; \
	template <typename T> \
	struct IsArray<T [X]> \
	{ \
		static const bool value = true; \
	}; \
	template <typename T> \
	struct IsArray<const T[X]> \
	{ \
		static const bool value = true; \
	};

#define ARRAY_OVERLOAD( ) \
	INTERNAL_ARRAY_OVERLOAD(__COUNTER__)


//Remove Qualifier///////////////////////////////////////////////
template <typename T>
struct RemQual
{
	typedef T type;
};

template <typename T>
struct RemQual <const T>
{
	typedef typename RemQual<T>::type type;
};

template <typename T> 
struct RemQual<T &>
{
	typedef typename RemQual<T>::type type;
};

template <typename T>
struct RemQual<const T &>
{
	typedef typename RemQual<T>::type type;
};

template <typename T>
struct RemQual<T &&>
{
	typedef typename RemQual<T>::type type;
};

template <typename T>
struct RemQual<const T *>
{
	typedef typename RemQual<T *>::type type;
};

ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );
ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );ARRAY_OVERLOAD( );

#endif