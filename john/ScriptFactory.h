/******************************************************************************/
/*!
\file		ScriptFactory.h
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include <string>

#include "IScript.h"
#include "Blast.h"

#include "ScriptPlayer.h"
#include "ScriptPlayerUI.h"
#include "ScriptTestObjectOne.h"
#include "ScriptTrigger.h"
#include "ScriptMetal.h"
#include "ScriptStart.h"
#include "ScriptNonMetal.h"
#include "ScriptSwitch.h"

#include "ScriptMainMenu.h"
#include "ScriptMenuUIButton.h"
#include "ScriptDebug.h"
#include "ScriptUIText.h"
#include "ScriptKillZone.h"
#include "ScriptMagneticLight.h"

#include "ScriptDialogue.h"
#include "ScriptWinZone.h"

#include "ScriptAI.h"
#include "ScriptSpikeBox.h"
#include "ScriptSpawn.h"
#include "ScriptEndZone.h"

#include "ScriptTemperature.h"
#include "ScriptForceField.h"
#include "ScriptTilePlatform.h"

#include "ScriptMovingPlatform.h"

namespace FwEngine
{
//
//	/*! \brief iterates until N is 0
//		and TFirst contains the type we want
//*/
//	template <size_t N, typename TFirst, typename ...TRest>
//	struct getType
//	{
//		//using type = getType<N - 1, TRest...>;
//		using type = typename getType<N - 1, TRest...>::type;
//		using thisType = TFirst;
//	};
//
//	/*! \brief specialised template for endpoint when N = 0
//	*/
//	template <typename TFirst, typename ...TRest>
//	struct getType<0, TFirst, TRest...>
//	{
//		using type = TFirst;
//		using thisType = TFirst;
//	};
//
//	/*! \brief helper type for getType
//	*/
//	template <size_t N, typename ...Ts>
//	using getType_t = typename getType<N, Ts...>::thisType;
//
//	//template <typename ...Ts>
//	//struct storeType
//	//{
//	//	using type = typename getType_t<N, Ts...>;
//	//};



	class ComponentScript;

	class ScriptGongChang
	{
		ComponentScript& _compscr;

	public:
		std::unordered_map<int, std::string> scriptType;

		//using scriptTypes = getType <
		//	13,
		//	ScriptPlayer,
		//	ScriptPlayerUI,
		//	ScriptTestObjectOne,
		//	ScriptMainMenu,
		//	ScriptMenuUIButton,
		//	ScriptDebug,
		//	ScriptUIText,
		//	ScriptBlast,
		//	ScriptTrigger,
		//	ScriptMetal,
		//	ScriptKillZone,
		//	ScriptStart,
		//	ScriptMagneticLight,
		//	ScriptBackgroundMovement
		//>;

		ScriptGongChang(ComponentScript& compscr) : _compscr(compscr)
		{
			scriptType.try_emplace(0, STRING_SCRIPT_PLAYER);
			scriptType.try_emplace(1, STRING_SCRIPT_PLAYERUI);
			scriptType.try_emplace(2, STRING_SCRIPT_TESTOBJECTONE);
			scriptType.try_emplace(3, STRING_SCRIPT_MAINMENU);
			scriptType.try_emplace(4, STRING_SCRIPT_MENUUIBUTTON);
			scriptType.try_emplace(5, STRING_SCRIPT_DEBUG);
			scriptType.try_emplace(6, STRING_SCRIPT_UITEXT);
			scriptType.try_emplace(7, STRING_SCRIPT_BLAST);
			scriptType.try_emplace(8, STRING_SCRIPT_TRIGGER);
			scriptType.try_emplace(9, STRING_SCRIPT_METAL);
			scriptType.try_emplace(10, STRING_SCRIPT_KILL_ZONE);
			scriptType.try_emplace(11, STRING_SCRIPT_START);
			scriptType.try_emplace(12, STRING_SCRIPT_LIGHT);
			//scriptType.try_emplace(13, STRING_SCRIPT_BACKGROUND);
			scriptType.try_emplace(14, STRING_SCRIPT_DIALOGUE);
			scriptType.try_emplace(15, STRING_SCRIPT_WINZONE);

			scriptType.try_emplace(16, STRING_SCRIPT_AI);
			scriptType.try_emplace(17, STRING_SCRIPT_NON_METAL);
			scriptType.try_emplace(18, STRING_SCRIPT_SWITCH);
			scriptType.try_emplace(19, STRING_SCRIPT_SPAWN);
			scriptType.try_emplace(20, STRING_SCRIPT_ENDZONE);

			scriptType.try_emplace(21, STRING_SCRIPT_TEMPERATURE);
			scriptType.try_emplace(22, STRING_SCRIPT_TILEPLATFORM);
			scriptType.try_emplace(23, STRING_SCRIPT_SPIKE_BOX);


			scriptType.try_emplace(24, STRING_SCRIPT_MOVINGPLATFORM);
			scriptType.try_emplace(25, STRING_SCRIPT_FORCE_FIELD);
			//AddType<ScriptPlayer>(STRING_SCRIPT_PLAYER);
			//AddType<ScriptPlayerUI>(STRING_SCRIPT_PLAYERUI);
			//AddType<ScriptTestObjectOne>(STRING_SCRIPT_TESTOBJECTONE);
			//AddType<ScriptMainMenu>(STRING_SCRIPT_MAINMENU);
			//AddType<ScriptMenuUIButton>(STRING_SCRIPT_MENUUIBUTTON);
			//AddType<ScriptDebug>(STRING_SCRIPT_DEBUG);
			//AddType<ScriptUIText>(STRING_SCRIPT_UITEXT);
			//AddType<ScriptBlast>(STRING_SCRIPT_BLAST);
			//AddType<ScriptTrigger>(STRING_SCRIPT_TRIGGER);
			//AddType<ScriptTrigger>(STRING_SCRIPT_METAL);
			//AddType<ScriptTrigger>(STRING_SCRIPT_KILL_ZONE);
			//AddType<ScriptStart>(STRING_SCRIPT_START);
			//AddType<ScriptMagneticLight>(STRING_SCRIPT_LIGHT);
		}

		template <typename T>
		void AddType(std::string name)
		{
			T* add = new T(_compscr);
			scriptType.try_emplace(name, add);
		}

		//template <typename T>
		//IScript* BuildType()
		//{
		//	//return reinterpret_cast<IScript*>(new typename std::remove_pointer<T>::type(_compscr));
		//	return reinterpret_cast<IScript*>(new typename std::remove_pointer<T>::type(_compscr));
		//}

		IScript* CreateType(std::string script, ComponentScript& compscr)
		{
			//int key = 0;
			//for (auto& i : scriptType) {
			//	if (i.second == script) {
			//		key = i.first;
			//		break;
			//	}
			//}

			//_compscr = compscr; // 

			//auto scr = scriptType.at(name).get();
			//return BuildType<decltype(scr)>(); // kennut

			//return
			//	reinterpret_cast<IScript*>(new 
			//		typename getType<>::thisType(compscr)
			//	);

			if (script._Equal(STRING_SCRIPT_PLAYER))
			{
				ScriptPlayer* newScript = new ScriptPlayer(compscr);
				//newScript->Init();
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_PLAYERUI))
			{
				ScriptPlayerUI* newScript = new ScriptPlayerUI(compscr);
				//newScript->Init();
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_TESTOBJECTONE))
			{
				ScriptTestObjectOne* newScript = new ScriptTestObjectOne(compscr);
				//newScript->Init();
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_MAINMENU))
			{
				ScriptMainMenu* newScript = new ScriptMainMenu(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_MENUUIBUTTON))
			{
				ScriptMenuUIButton* newScript = new ScriptMenuUIButton(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_DEBUG))
			{
				ScriptDebug* newScript = new ScriptDebug(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_UITEXT))
			{
				ScriptUIText* newScript = new ScriptUIText(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_BLAST))
			{
				ScriptBlast* newScript = new ScriptBlast(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_TRIGGER))
			{
				ScriptTrigger* newScript = new ScriptTrigger(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_METAL))
			{
				ScriptMetal* newScript = new ScriptMetal(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_KILL_ZONE))
			{
				ScriptKillZone* newScript = new ScriptKillZone(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_START))
			{
				ScriptStart* newScript = new ScriptStart(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_LIGHT))
			{
				ScriptMagneticLight* newScript = new ScriptMagneticLight(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			/*
			if (script._Equal(STRING_SCRIPT_BACKGROUND))
			{
				ScriptBackgroundMovement* newScript = new ScriptBackgroundMovement(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			*/
			if (script._Equal(STRING_SCRIPT_DIALOGUE))
			{
				ScriptDialogue* newScript = new ScriptDialogue(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_WINZONE))
			{
				ScriptWinZone* newScript = new ScriptWinZone(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_AI))
			{
				AIScript* newScript = new AIScript(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_NON_METAL))
			{
				ScriptNonMetal* newScript = new ScriptNonMetal(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}

			if (script._Equal(STRING_SCRIPT_SWITCH))
			{
				ScriptSwitch* newScript = new ScriptSwitch(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_SPAWN))
			{
				ScriptSpawn* newScript = new ScriptSpawn(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_ENDZONE))
			{
				ScriptEndZone* newScript = new ScriptEndZone(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_TEMPERATURE))
			{
				ScriptTemperature* newScript = new ScriptTemperature(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_SPIKE_BOX))
			{
				ScriptSpikeBox* newScript = new ScriptSpikeBox(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_TILEPLATFORM))
			{
				ScriptTilePlatform* newScript = new ScriptTilePlatform(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_MOVINGPLATFORM))
			{
				ScriptMovingPlatform* newScript = new ScriptMovingPlatform(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			if (script._Equal(STRING_SCRIPT_FORCE_FIELD))
			{
				ScriptForceField* newScript = new ScriptForceField(compscr);
				return reinterpret_cast<IScript*>(newScript);
			}
			//throw;
			return nullptr;
		}


	};
	





}
