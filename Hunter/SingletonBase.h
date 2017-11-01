#ifndef SINGLETON_BASE_H
#define SINGLETON_BASE_H


template<typename T>
class SingletonBase
{
protected:
	static T*		_pInstance;		//인스턴스 포인터

protected:
	SingletonBase(void) {};
	~SingletonBase(void) {};

public:

	//인스턴스 얻기 함수
	static T* GetInstance(void);

	//인스턴스 해제 함수
	static void ReleaseInstance(void);
};

//인스턴스 포인터 초기화
template<typename T>
T* SingletonBase<T>::_pInstance = NULL;



//인스턴스 얻기 함수
template<typename T>
T* SingletonBase<T>::GetInstance(void)
{
	if (_pInstance == NULL)
		_pInstance = new T;

	return _pInstance;
}

//인스턴스 해제 함수
template<typename T>
void SingletonBase<T>::ReleaseInstance(void)
{
	if (_pInstance) {
		delete _pInstance;
		s_pInstance = NULL;
	}
}
#endif