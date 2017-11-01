#ifndef SINGLETON_BASE_H
#define SINGLETON_BASE_H


template<typename T>
class SingletonBase
{
protected:
	static T*		_pInstance;		//�ν��Ͻ� ������

protected:
	SingletonBase(void) {};
	~SingletonBase(void) {};

public:

	//�ν��Ͻ� ��� �Լ�
	static T* GetInstance(void);

	//�ν��Ͻ� ���� �Լ�
	static void ReleaseInstance(void);
};

//�ν��Ͻ� ������ �ʱ�ȭ
template<typename T>
T* SingletonBase<T>::_pInstance = NULL;



//�ν��Ͻ� ��� �Լ�
template<typename T>
T* SingletonBase<T>::GetInstance(void)
{
	if (_pInstance == NULL)
		_pInstance = new T;

	return _pInstance;
}

//�ν��Ͻ� ���� �Լ�
template<typename T>
void SingletonBase<T>::ReleaseInstance(void)
{
	if (_pInstance) {
		delete _pInstance;
		s_pInstance = NULL;
	}
}
#endif