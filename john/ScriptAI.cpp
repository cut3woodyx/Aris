#include "ScriptAI.h"
#include "DebugDiagnostic.h"

#include "AIChase.h"
#include "AIRoaming.h"
#include "AIExplode.h"

#include "AIBoss1Idle.h"
#include "AIBoss1Vulnerable.h"
#include "AIBoss1Attack.h"
#include "AIBoss1Charge.h"
#include "AIBoss1Damaged.h"
#include "AIBoss1VField.h"

namespace FwEngine
{
	std::vector<std::string> TagsToPush_
	{
		"player",
		"metal"
	};

	// Add Scripts here
	AIScript::AIMap AIScript::AllAIBases_
	{
	{"Roaming", std::make_shared<AIRoaming>()},
	{"Chase", std::make_shared<AIChase>()},
	{"Explode", std::make_shared<AIExplode>()},
	{STRING_AI_BOSS_1_IDLE,		std::make_shared<AIBoss1Idle>()},
	{STRING_AI_BOSS_1_VULN,		std::make_shared<AIBoss1Vulnerable>()},
	{STRING_AI_BOSS_1_ATTACK,	std::make_shared<AIBoss1Attack>()},
	{STRING_AI_BOSS_1_DAMAGED,	std::make_shared<AIBoss1Damaged>()},
	{STRING_AI_BOSS_1_CHARGE,	std::make_shared<AIBoss1Charge>()},
	{STRING_AI_BOSS_1_VFIELD,	std::make_shared<AIBoss1VField>()}
	};

	std::vector<std::string> Tokenize(std::string const& text, std::string const& separator)
	{
		std::vector<std::string> result;
		std::string tmp = text;

		size_t first_pos = 0;
		size_t second_pos = tmp.find(separator);

		while (second_pos != std::string::npos)
		{
			if (first_pos != second_pos)
			{
				std::string word = tmp.substr(first_pos, second_pos - first_pos);
				result.push_back(word);
			}
			tmp = tmp.substr(second_pos + separator.length());
			second_pos = tmp.find(separator);
		}

		result.push_back(tmp);

		return result;
	}

	AIScript::AIScript(ComponentScript& comp) 
		: IScript(STRING_SCRIPT_AI, comp), _aiBases{ {"Empty", nullptr} }, _curAI{ nullptr }, _curAIName{"Empty"}
	{
		AddStringVar("List of AI", &_listOfAI);
		AddStringVar("List of Variables (1)", &_listOfVars);
	}

	AIScript::~AIScript()
	{
	}

	void AIScript::AddAI(std::string aiName, std::string var)
	{
		if (!aiName.empty() && (AllAIBases_.find(aiName) != std::end(AllAIBases_)))
		{
			_aiBases.emplace(aiName, AllAIBases_[aiName]->Clone(this, &_components, var));
			if (!_curAI)
			{
				_curAI = _aiBases[aiName];
				_curAIName = aiName;
			}
		}
	}

	void AIScript::RemoveAI(std::string aiName)
	{
		_aiBases.erase(aiName);
	}

	void AIScript::RunAI(std::string name)
	{
		if (name != _curAIName && _aiBases.find(name) != std::end(_aiBases))
		{
			_curAI->Free();
			_curAIName = name;
			_curAI = _aiBases.find(name)->second;
			if (_curAI)
				_curAI->Init();
		}
	}

	void AIScript::UpdateAI(std::string aiName, float dt)
	{
		if (aiName != _curAIName && _aiBases.find(aiName) != std::end(_aiBases))
		{
			_aiBases.find(aiName)->second->Update(dt);
		}

		// Reflects values in Level Editor in realtime - Inefficient, do not need to use
		/*
		// Rewrites variables
		std::string newVars;
		std::vector<std::string> tokens = Tokenize(_listOfAI, " ");
		for (decltype(tokens)::iterator i = std::begin(tokens); i != std::end(tokens); ++i)
		{
			newVars.append(_aiBases[(*i)]->_variable);
			newVars.append(" ");
		}
		_listOfVars = newVars;
		*/
	}

	std::shared_ptr<AIBase> AIScript::GetAI(std::string aiName)
	{
		if (_aiBases.find(aiName) != std::end(_aiBases))
			return _aiBases[aiName];
		return nullptr;
	}

	ComponentScript* AIScript::GetCompScript()
	{
		return &_components;
	}

	void AIScript::Init()
	{
		// Add AI here
		if (_aiBases.size() == 1)
		{
			size_t val = 0;
			std::vector<std::string> tokens = Tokenize(_listOfAI, " ");
			std::vector<std::string> tokenVars = Tokenize(_listOfVars, " ");

			for (size_t i = 0; i < tokens.size(); ++i)
			{
				std::string var;
				if (i < tokenVars.size())
					var = tokenVars[i];
				AddAI(tokens[i], var);
			}
			val++;
		}
		if (_curAI)
			_curAI->Init();
	}

	void AIScript::Update(float dt)
	{
		if (_curAI)
			_curAI->Update(dt);
	}

	void AIScript::Free()
	{
		if (_curAI)
		{
			_curAI->Free();

			// Rewrites variables
			std::string newVars;
			std::vector<std::string> tokens = Tokenize(_listOfAI, " ");
			for (decltype(tokens)::iterator i = std::begin(tokens); i != std::end(tokens); ++i)
			{
				newVars.append(_aiBases[(*i)]->_variable);
				newVars.append(" ");
			}
			_listOfVars = newVars;
		}
	}

	void AIScript::Destroy()
	{
	}

	void AIScript::OnCollisionEnter(size_t id)
	{
		if (_curAI)
			_curAI->OnCollisionEnter(id);
	}

	void AIScript::OnCollisionExit(size_t id)
	{
		if (_curAI)
			_curAI->OnCollisionExit(id);
	}

	void AIScript::OnCollisionStay(size_t id)
	{
		if (_curAI)
			_curAI->OnCollisionStay(id);
	}
	void AIScript::PushVars()
	{
		size_t totalSize = _originalVars.size();
		size_t halfSize = totalSize / 2;

		while (_originalVars.size() > halfSize) {
			_listOfAI.append(_originalVars[0]).append(" ");
			_originalVars.erase(std::begin(_originalVars));
		}
		while (_originalVars.size() > 0) {
			_listOfVars.append(_originalVars[0]).append(" ");
			_originalVars.erase(std::begin(_originalVars));
		}

		if (_listOfAI.size())
			_listOfAI.erase(_listOfAI.end() - 1);
		if (_listOfVars.size())
			_listOfVars.erase(_listOfVars.end() - 1);
	}
}