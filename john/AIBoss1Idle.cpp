#include "AIBoss1Idle.h"
#include "AIBoss1Charge.h"
#include "AIBoss1VField.h"

namespace FwEngine
{
	AIBoss1Idle::AIBoss1Idle(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ STRING_AI_BOSS_1_IDLE, ai, scr, var },
		//_compAnim{ nullptr },
		_idleTime{ 0 },
		_currentTime{ 0 },
		_bossPhase{ 0 }
	{
	}

	AIBoss1Idle::~AIBoss1Idle()
	{
	}

	void AIBoss1Idle::Init()
	{
		if (!_variable.empty())
		{
			std::vector<std::string> tokens = Tokenize(_variable, "#");

			int i = 0;
			if (tokens.size() > i)
				_idleTime = std::stof(tokens[i++]);
		}
		_currentTime = 0;
	}

	void AIBoss1Idle::Update(float dt)
	{
		_currentTime += dt;

		if (_currentTime > _idleTime)
		{
			// randomize and go to different AI
			switch (_bossPhase)
			{
			case 0:
			{
				_compAI->RunAI(STRING_AI_BOSS_1_CHARGE);
				break;
			}
			case 1:
			case 2:
			{
				if (std::rand() % 2)
				{
					_compAI->RunAI(STRING_AI_BOSS_1_CHARGE);
				}
				else
				{
					_compAI->RunAI(STRING_AI_BOSS_1_VFIELD);
				}
				break;
			}
			default:
			{
				// End sequence here
				_compAI->RunAI("Empty");
				break;
			}
			}
		}
	}

	void AIBoss1Idle::Free()
	{
	}

	void AIBoss1Idle::OnCollisionEnter(size_t)
	{
	}

	void AIBoss1Idle::OnCollisionExit(size_t)
	{
	}

	void AIBoss1Idle::OnCollisionStay(size_t)
	{
	}
}