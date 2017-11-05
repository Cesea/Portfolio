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

	//���� ��η� ���ҽ��� �ҷ��´�.
	T *CreateResource(const std::string &filePath, void* pParam = NULL);
	T *FindResource(const std::string &key);

	//Ư�� ���ҽ� ����
	void RemoveResource(const std::string &filePath);

	//��� ���ҽ� ����
	void ClearResource();


	uint32 GetCurrentResourceID() { return _pCurrentResource->GetID(); }
	const T *GetCurrentResource() { return _pCurrentResource; }
protected:

	//�ε� ��ĸ� ������ 
	virtual T *LoadResource(const std::string &filePath, void* pParam = NULL) = 0;

	//���� ��ĸ� ������
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
	//��� ���ҽ��� �ϴ�.
	this->ClearResource();
}


template<typename T, typename A>
T *ResourceManager<T, A>::CreateResource(const std::string &filePath, void* pParam)
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//�̹� �ε��Ǿ����� Ȯ��
	pIter = _resources.find(filePath);

	//�ش� filePath �� �ʿ� �߰� �Ǿ����� �ʴٸ�...
	if (pIter == _resources.end())
	{
		//LoadResource �Լ��� ���ο� Resource �ε�
		T *newResource = LoadResource(filePath, pParam);

		//���ҽ� �ε��� ���еǾ��ٸ� 0 ����
		if (newResource == 0)
			return 0;

		//Map Table �� �߰�
		_resources.insert(std::make_pair(filePath, newResource));

		return newResource;
	}

	//��������Դٸ� �̹� �ִٴ°�
	return pIter->second;
}

template<typename T, typename A>
inline T * ResourceManager<T, A>::FindResource(const std::string & key)
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//�̹� �ε��Ǿ����� Ȯ��
	pIter = _resources.find(filePath);

	//�ش� filePath �� �ʿ� �߰� �Ǿ����� �ʴٸ�...
	if (pIter == _resources.end())
	{
		return nullptr;
	}
	else
	{
		return pIter->second;
	}
}


//Ư�� ���ҽ� ����
template<typename T, typename A>
void ResourceManager<T, A>::RemoveResource(const std::string &filePath)
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//�̹� �ε��Ǿ����� Ȯ��
	pIter = _resources.find(filePath);

	//�����Ѵٸ�...
	if (pIter != _resources.end())
	{
		//ã�� ���ҽ� ����...
		ReleaseResource(pIter->second);

		//Iterator �� ����Ű�� ���� ����.
		_resources.remove(pIter);
	}
}


//��� ���ҽ� ����
template<typename T, typename A>
void ResourceManager<T, A>::ClearResource()
{
	std::unordered_map<std::string, T *>::iterator pIter;

	//Binary RedBlack Tree �� �Ǿ��ִ� map�� ������ ���� ������������ ��� �����Ҷ�
	//���� �������� �и���.
	for (pIter = _resources.begin(); pIter != _resources.end(); ++pIter)
	{
		ReleaseResource(pIter->second);
	}

	//Table
	_resources.clear();
}

#endif