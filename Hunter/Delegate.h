#ifndef DELEGATE_H
#define DELEGATE_H

template <typename returnType, typename ... params>
class Delegate
{
private:
	typedef returnType(*Type)(void *callee, params ...);
public:
	Delegate()
	{}
	void Init(void *callee, Type function)
	{
		fpCallee = callee;
		fpFunctionPointer = function;
	}
	template <class T, returnType (T::*Method)(params ...)>
	static inline Delegate<returnType, params ... > FromFunction(T *callee)
	{
		Delegate<returnType, params ...> result;
		result.Init(callee, &MethodCaller<T, Method>);
		return result;
	}

	template<class T, returnType(T::*Method)(params ...)>
	void Bind(T *callee)
	{
		Init(callee, &MethodCaller<T, Method>);
	}

	returnType Invoke(params... xs) const
	{
		return (*fpFunctionPointer)(fpCallee, xs...);
	}

	bool operator== (const Delegate &other)
	{
		bool result = false;
		if ((fpCallee == other.fpCallee) && (fpFunctionPointer == other.fpFunctionPointer))
		{
			result = true;
		}
		return result;
	}

	Delegate &operator = (const Delegate &other)
	{
		fpCallee = other.fpCallee;
		fpFunctionPointer = other.fpFunctionPointer;
		return *this;
	}

private:
	void *fpCallee;
	Type fpFunctionPointer;

	template <class T, returnType(T::*Method)(params ...)>
	static returnType MethodCaller(void *callee, params ...xs)
	{
		T *t = static_cast<T *>(callee);
		return (t->*Method)(xs...);
	}
};


#endif