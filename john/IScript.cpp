
#include "IScript.h"


std::string FwEngine::Timer::clockToString(clock_t time)
{
	int totalSeconds = abs(time / CLOCKS_PER_SEC);
	int secs = totalSeconds % 60;
	int mins = totalSeconds / 60;

	std::string str;
	str.append(std::to_string(mins));
	str.append(":");
	str.append(std::to_string(secs));

	return str;
}

FwEngine::Timer::Timer(int time) : _initTime(clock()), _setTime(time)
{
}

clock_t FwEngine::Timer::getElapsedTime()
{
	return clock() - _initTime;
}

std::string FwEngine::Timer::getElapsedTime_s()
{
	clock_t elapsedTime = clock() - _initTime;

	return clockToString(elapsedTime);
}

std::string FwEngine::Timer::getRemainingTime_s()
{
	clock_t elapsedTime = clock() - _initTime;
	clock_t remainingTime = _setTime - elapsedTime;

	return clockToString(remainingTime);
}

bool FwEngine::Timer::checkTrigger()
{
	return _setTime < getElapsedTime();
}

void FwEngine::Timer::reset()
{
	_initTime = clock();
}

// 

FwEngine::ScriptMessagingSystem::ScriptMessagingSystem()
{
}

FwEngine::ScriptMessagingSystem::~ScriptMessagingSystem()
{
	Free();
}

void FwEngine::ScriptMessagingSystem::Free()
{
	for (std::pair<std::string, ScriptMessageObject*> m : _messageBox)
	{
		delete m.second;
	}
	_messageBox.clear();
}

std::map<std::string, FwEngine::ScriptMessageObject*>::iterator FwEngine::ScriptMessagingSystem::FindAddress(std::string addressName)
{
	std::map<std::string, FwEngine::ScriptMessageObject*>::iterator itr = _messageBox.begin();
	while(itr != _messageBox.end())
	{
		if ((*itr).first._Equal(addressName))
			return itr;
		++itr;
	}
	return itr;
}

void FwEngine::ScriptMessagingSystem::RegisterAddress(std::string addressName)
{
	std::map<std::string, FwEngine::ScriptMessageObject*>::iterator address = FindAddress(addressName);
	if (address == _messageBox.end())
	{
		_messageBox.try_emplace(addressName, nullptr);
	}
}

FwEngine::ScriptMessageObject* FwEngine::ScriptMessagingSystem::RegisterMessage(std::string addressName, ScriptMessageObject* message)
{
	std::map<std::string, FwEngine::ScriptMessageObject*>::iterator address = FindAddress(addressName);

	if (address == _messageBox.end())
	{
		RegisterAddress(addressName);
		address = FindAddress(addressName);
	}

	// delete existing
	if ((*address).second != nullptr)
	{
		delete (*address).second;
	}

	// save
	(*address).second = message; 

	return message;
}

FwEngine::ScriptMessageObject* FwEngine::ScriptMessagingSystem::RegisterListener(std::string addressName)
{
	std::map<std::string, FwEngine::ScriptMessageObject*>::iterator address = FindAddress(addressName);

	// not found
	if (address == _messageBox.end())
		return nullptr;

	return (*address).second;
}


