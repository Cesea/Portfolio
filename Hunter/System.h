#ifndef SYSTEM_H
#define SYSTEM_H

#include "EventChannel.h"
#include "EventHandler.h"

#include <vector>
#include <string>

typedef std::pair<std::string, int> SystemSetting;

struct SystemInitSetting
{
	std::vector<SystemSetting> settings;
	void Add(const std::string &name, int value);
};

class System
{
public :
	System();
	virtual ~System();

	virtual bool Init(const std::string &name, SystemSetting &setting);
	virtual void ShutDown();

	virtual void Update(uint32 deltaMS);

	const std::string GetName() { return _name; }

	virtual EventChannel GetChannel() { return _channel; }

public :
	struct SystemInitialize
	{
		SystemInitialize(System *system)
			:pSystem(system)
		{}
		System *pSystem;
	};

	struct SystemShutDown
	{
		SystemShutDown(System *system)
			:pSystem(system)
		{}
		System *pSystem;
	};
private :

protected :
	EventChannel _channel;
	std::string _name;
};


#endif