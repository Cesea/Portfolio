#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "ResourceManager.h"
#include "Effect.h"

#include <map>

class EffectManager : public ResourceManager<Effect>
{
public:
	EffectManager();
	virtual ~EffectManager();
	EffectHandle Create(const std::string &fileName);

	virtual bool32 Destroy(EffectHandle handle);
private:
};


#endif