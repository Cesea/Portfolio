#ifndef LOAD_ITEM_H
#define LOAD_ITEM_H

#include <string>
#include <vector>

enum LOADING_KIND
{
	LOADING_MODEL,
	LOADING_SOUND,
};


struct ModelResource
{
	ModelResource() {}

	bool Init(const std::string &fileName, const std::string &keyName, float scale, float rotationY);

	std::string _fileName;
	std::string _keyName;
	float _scale;
	float _rotationY;
};

struct SoundResource
{
	SoundResource() {}

	bool Init(const std::string &keyName, const std::string &fileName,
		bool bgm, bool loop, float minDist, float maxDist = 0.0f);

	std::string _fileName;
	std::string _keyName;
	bool _isBackground;
	bool _loop;
	float _min;
	float _max;
};

class ModelLoadItem
{
public:
	ModelLoadItem() {}
	bool Init(const std::string &fileName, const std::string &keyName, bool isStatic, float scale, float rotationY);
	const ModelResource &GetResource() { return _resource; }
	bool IsStatic() { return _isStatic; }

private :
	ModelResource _resource;

	bool _isStatic;
};


class SoundLoadItem
{
public :
	SoundLoadItem() {}
	bool Init(const std::string &fileName, const std::string &keyName, 
		bool bgm, bool loop, float minDist, float maxDist = 0.0f);
	const SoundResource &GetResource() { return _resource; }
private :
	SoundResource _resource;
};

class Loading
{
public :
	Loading() {}
	virtual ~Loading() { }

	void LoadSoundResources(const std::string &fileName, const std::string &keyName, 
		bool bgm, bool loop, float min = 0.0f, float max = 0.0f);
	void LoadModelResources(const std::string &fileName, const std::string &keyName, 
		bool isStatic, float scale, float rotationY);

	float LoadNext(void);

	void Release();

private :

	int32 _modelCurrent{};
	int32 _soundCurrent{};

	bool _loadModel{};
	bool _loadSound{};

	std::vector<ModelLoadItem *> _modelLoadItems;
	std::vector<SoundLoadItem *> _soundLoadItems;

	int32 _totalSize{};
};



#endif