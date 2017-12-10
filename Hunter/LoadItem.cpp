#include "stdafx.h"
#include "LoadItem.h"


bool SoundResource::Init(const std::string & keyName, const std::string & fileName, 
	bool bgm, bool loop, float minDist, float maxDist)
{
	_keyName = keyName;
	_fileName = fileName;

	_isBackground = bgm;
	_loop = loop;
	_min = minDist;
	_max = maxDist;
	return true;
}

bool ModelResource::Init(const std::string & fileName, const std::string & keyName, float scale, float rotationY)
{
	_fileName = fileName;
	_keyName = keyName;
	_scale = scale;
	_rotationY = rotationY;
	return true;
}


bool ModelLoadItem::Init(const std::string & fileName, const std::string & keyName, bool isStatic,
	float scale, float rotationY)
{
	_resource._fileName = fileName;
	_resource._keyName = keyName;
	_resource._scale = scale;
	_resource._rotationY = rotationY;

	_isStatic = isStatic;
	return true;
}

bool SoundLoadItem::Init(const std::string & fileName, const std::string & keyName, 
	bool bgm, bool loop, float minDist, float maxDist)
{
	_resource._fileName = fileName;
	_resource._keyName = keyName;
	_resource._isBackground = bgm;
	_resource._loop = loop;
	_resource._min = minDist;
	_resource._max = maxDist;
	return true;
}

void Loading::LoadSoundResources(const std::string & fileName, const std::string & keyName, 
	bool bgm, bool loop, float min, float max)
{
	SoundLoadItem *newItem = new SoundLoadItem;
	newItem->Init(fileName, keyName, bgm, loop, min, max);
	_soundLoadItems.push_back(newItem);
	_totalSize++;
}

void Loading::LoadModelResources(const std::string & fileName, const std::string & keyName, 
	bool isStatic, float scale, float rotationY)
{
	ModelLoadItem *newItem = new ModelLoadItem;
	newItem->Init(fileName, keyName, isStatic, scale, rotationY);
	_modelLoadItems.push_back(newItem);
	_totalSize++;
}

float Loading::LoadNext(void)
{
	float result = 0.0f;
	if (!_loadSound)
	{
		if (_soundCurrent >= _soundLoadItems.size())
		{
			_loadSound = true;
		}
		else
		{
			SoundLoadItem* item = _soundLoadItems[_soundCurrent];
			const SoundResource &refResource = item->GetResource();
			if (item->GetResource()._min == 0.0f)
			{
				SOUNDMANAGER->AddSound(refResource._keyName, refResource._fileName,
					refResource._isBackground, refResource._loop);
			}
			else
			{
				SOUNDMANAGER->AddSound3D(refResource._keyName, refResource._fileName,
					refResource._isBackground, refResource._loop, refResource._min, refResource._max);
			}
			_soundCurrent++;
		}

	}
	else if(!_loadModel)
	{
		if (_modelCurrent >= _modelLoadItems.size())
		{
			_loadModel = true;
		}
		else
		{
			ModelLoadItem* item = _modelLoadItems[_modelCurrent];
			const ModelResource &refResource = item->GetResource();
			if (item->IsStatic())
			{
				Matrix correctionMat;
				Matrix rotationCorrection;
				MatrixRotationY(&rotationCorrection, refResource._rotationY);
				MatrixScaling(&correctionMat, refResource._scale, refResource._scale, refResource._scale);
				MatrixMultiply(&correctionMat, &correctionMat, &rotationCorrection);
				VIDEO->CreateStaticXMesh(refResource._fileName, &correctionMat, refResource._keyName);
			}
			else
			{
				Matrix correctionMat;
				Matrix rotationCorrection;
				MatrixRotationY(&rotationCorrection, refResource._rotationY);
				MatrixScaling(&correctionMat, refResource._scale, refResource._scale, refResource._scale);
				MatrixMultiply(&correctionMat, &correctionMat, &rotationCorrection);
				VIDEO->CreateSkinnedXMesh(
					refResource._fileName, &correctionMat, refResource._keyName);
			}
			_modelCurrent++;
		}
	}

	result = ((float)(_modelCurrent + _soundCurrent) / (float)_totalSize);

	return result;
}

void Loading::Release()
{
	for (uint32 i = 0; i < _modelLoadItems.size(); ++i)
	{
		SAFE_DELETE(_modelLoadItems[i]);
	}
	_modelLoadItems.clear();


	for (uint32 i = 0; i < _soundLoadItems.size(); ++i)
	{
		SAFE_DELETE(_soundLoadItems[i]);
	}
	_soundLoadItems.clear();
}
