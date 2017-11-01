#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "ResourceManager.h"

#include "Effect.h"

class EffectManager : public ResourceManager<Effect, EffectManager>
{
public:
	EffectManager(void);
	~EffectManager(void);

	virtual Effect *LoadResource(const std::string &filePath, void* pParam = NULL);

	virtual void ReleaseResource(Effect *pData);
	virtual void ReleaseResource(const std::string &key);

};

#define EFFECTMANAGER (EffectManager::GetInstance())


#endif