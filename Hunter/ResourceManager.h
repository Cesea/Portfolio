#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#if defined FindResource
# undef FindResource
#endif

#include "SingletonBase.h"

#include "stdafx.h"

#include <unordered_map>
#include <string>

template< typename T, typename A >
class ResourceManager : public SingletonBase<A>
{
protected:
	typedef std::unordered_map< std::string, T *> ResourceMap;
	//typedef std::unordered_map< std::string, T *>::iterator ResourceMapIter;

	ResourceMap	_resources;

public:
	ResourceManager(void);
	~ResourceManager(void);

	//파일 경로로 리소스를 불러온다.
	T *CreateResource(const std::string &filePath, void* pParam = NULL);
	T *FindResource(const std::string &key);

	//특정 리소스 해제
	void RemoveResource(const std::string &filePath);

	//모든 리소스 해제
	void ClearResource();


	uint32 GetCurrentResourceID() { return _pCurrentResource->GetID(); }
	const T *GetCurrentResource() { return _pCurrentResource; }
protected:

	//로드 방식만 재정의 
	virtual T *LoadResource(const std::string &filePath, void* pParam = NULL) = 0;

	//해재 방식만 재정의
	virtual void ReleaseResource(T *data) = 0;
	virtual void ReleaseResource(const std::string &key) = 0;
	virtual bool32 ChangeCurrentResource(T *pResource) = 0;

	T *_pCurrentResource{};
};

template< typename T, typename A >
ResourceManager<T, A>::ResourceManager(void)
{}


template< typename T, typename A >
ResourceManager<T, A>::~ResourceManager(void)
{
	//모든 리소스를 턴다.
	this->ClearResource();
}


template<typename T, typename A>
T *ResourceManager<T, A>::CreateResource(const std::string &filePath, void* pParam)
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//이미 로딩되었는지 확인
	pIter = _resources.find(filePath);

	//해당 filePath 가 맵에 추가 되어있지 않다면...
	if (pIter == _resources.end())
	{
		//LoadResource 함수로 새로운 Resource 로딩
		T *newResource = LoadResource(filePath, pParam);

		//리소스 로딩이 실패되었다면 0 리턴
		if (newResource == 0)
			return 0;

		//Map Table 에 추가
		_resources.insert(std::make_pair(filePath, newResource));

		return newResource;
	}

	//여기까지왔다면 이미 있다는거
	return pIter->second;
}

template<typename T, typename A>
inline T * ResourceManager<T, A>::FindResource(const std::string & key)
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//이미 로딩되었는지 확인
	pIter = _resources.find(filePath);

	//해당 filePath 가 맵에 추가 되어있지 않다면...
	if (pIter == _resources.end())
	{
		return nullptr;
	}
	else
	{
		return pIter->second;
	}
}


//특정 리소스 해제
template<typename T, typename A>
void ResourceManager<T, A>::RemoveResource(const std::string &filePath)
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//이미 로딩되었는지 확인
	pIter = _resources.find(filePath);

	//존재한다면...
	if (pIter != _resources.end())
	{
		//찾은 리소스 해재...
		ReleaseResource(pIter->second);

		//Iterator 가 가리키는 곳을 삭제.
		_resources.remove(pIter);
	}
}


//모든 리소스 해제
template<typename T, typename A>
void ResourceManager<T, A>::ClearResource()
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//Binary RedBlack Tree 로 되어있는 map에 다음과 같이 순차접근으로 모두 해제할때
	//중위 접근으로 털린다.
	for (pIter = _resources.begin(); pIter != _resources.end(); ++pIter)
	{
		ReleaseResource(pIter->second);
	}

	//Table
	_resources.clear();
}

#endif