#include "stdafx.h"
#include "System.h"

void SystemInitSetting::Add(const std::string & name, int value)
{
	settings.push_back(std::make_pair(name, value));
}

System::System()
{
}

System::~System()
{
}

bool System::Init(const std::string &name, SystemSetting &setting)
{
	_name = name;
	//Setting 을 해석하여 알맞게 channel에 add를 한다??
	_channel.Broadcast<System::SystemInitialize>(SystemInitialize(this));
	return true;
}

void System::ShutDown()
{
	_channel.Broadcast<System::SystemShutDown>(SystemShutDown(this));
}

void System::Update(float deltaTime)
{
}

