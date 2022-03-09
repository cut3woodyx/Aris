/******************************************************************************/
/*!
\file		GameObject.cpp
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
#include "GameObjectFactory.h"
#include "Component.h"
#include "IComponent.h"

constexpr auto STRING_POSITION_X = "position_X";
constexpr auto STRING_POSITION_Y = "position_Y";

constexpr auto STRING_COMPONENT_COLLISION = "collision";
constexpr auto STRING_COMPONENT_PHYSICS = "physics";
constexpr auto STRING_COMPONENT_RENDERER = "renderer";
constexpr auto STRING_COMPONENT_TRANSFORM = "transform";
constexpr auto STRING_COMPONENT_ANIMATION = "animation";
constexpr auto STRING_COMPONENT_SCRIPT = "script";
constexpr auto STRING_COMPONENT_TEXT = "text";
constexpr auto STRING_COMPONENT_AUDIO = "audio";
constexpr auto STRING_COMPONENT_EMITTER = "emitter";

namespace FwEngine
{

	/////////////////////////
	// Component Builders

	void GameObjectFactory::BuildComponent(GameObject* newObj, ParamValueMap& paramValues, std::string componentName, bool isFab)
	{
		IComponent* comp = nullptr;

		comp = GetObjFromMap(componentName, newObj->GetIndex());
		if (comp)
		{
			comp->setParentGameObject(newObj);
			comp->Init(paramValues);
		}
		if (componentName._Equal(STRING_COMPONENT_RENDERER) && !isFab)
		{
			_systemGraphics->AddRenderer(static_cast<ComponentRenderer*>(comp));
		}
		else if (componentName._Equal(STRING_COMPONENT_TEXT) && !isFab)
		{
			_systemGraphics->AddText(static_cast<ComponentText*>(comp));
		}
		paramValues.clear();
	}


	// Component Builders END
	/////////////////////////

	std::string& GameObjectFactory::CerealiseGameObjects()
	{
		// cereal
		typedef std::pair<std::string, std::unordered_map<std::string, ParamValueMap>> Param_t;
		std::vector<Param_t> fullObjMap;

		std::vector<GameObject>* objPool = _currentPool->GetContainerPtr<GameObject>();
		GameObject* obj = &objPool->at(0);

		// this is so hardcoded, need to figure out a method to do this in a better way in the future
		ComponentTransform* compTrans = &_currentPool->GetContainerPtr<ComponentTransform>()->at(0);
		ComponentCollision* compCol = &_currentPool->GetContainerPtr<ComponentCollision>()->at(0);
		ComponentPhysics* compPhy = &_currentPool->GetContainerPtr<ComponentPhysics>()->at(0);
		ComponentRenderer* compRend = &_currentPool->GetContainerPtr<ComponentRenderer>()->at(0);
		ComponentText* compTex = &_currentPool->GetContainerPtr<ComponentText>()->at(0);
		ComponentScript* compScr = &_currentPool->GetContainerPtr<ComponentScript>()->at(0);
		ComponentAnimation* compAni = &_currentPool->GetContainerPtr<ComponentAnimation>()->at(0);
		ComponentAudio* compAud = &_currentPool->GetContainerPtr<ComponentAudio>()->at(0);
		ComponentEmitter* compEmitter = &_currentPool->GetContainerPtr<ComponentEmitter>()->at(0);

		for (size_t i = 0; i < objPool->size(); ++i, ++obj,
			++compTrans, ++compCol, ++compPhy, ++compRend, ++compTex, ++compScr, ++compAni, ++compAud, ++compEmitter)
		{
			// not active, skip
			if (!objPool->at(i)._isActive)
				continue;

			std::unordered_map<std::string, ParamValueMap> objMap;

			//std::vector<IComponent*> comps;

			if (compTrans->isEnabled())
				objMap.emplace(compTrans->GetParams());
			if (compCol->isEnabled())
				objMap.emplace(compCol->GetParams());
			if (compPhy->isEnabled())
				objMap.emplace(compPhy->GetParams());
			if (compRend->isEnabled())
				objMap.emplace(compRend->GetParams());
			if (compTex->isEnabled())
				objMap.emplace(compTex->GetParams());
			if (compScr->isEnabled())
				objMap.emplace(compScr->GetParams());
			if (compAni->isEnabled())
				objMap.emplace(compAni->GetParams());
			if (compAud->isEnabled())
				objMap.emplace(compAud->GetParams());
			if (compEmitter->isEnabled())
				objMap.emplace(compEmitter->GetParams());

			if (!obj->GetName().empty())
			{
				// want to save all obj, inc menu objs
				//if (obj->GetName().find("_Menu_") != std::string::npos)
				//	continue;
				fullObjMap.push_back(Param_t(obj->GetName(), objMap));
			}
			else
				fullObjMap.push_back(Param_t(std::string("Object").append(std::to_string(i)), objMap));
		}

		//return fullObjMap;

		// save

		//std::stringstream buff;
		_cerealise.clear();

		const std::string _start = "<";
		const std::string _end = ">";
		const std::string _breakName = "</>";

		for (int k = 0; k < fullObjMap.size(); ++k)
		{
			Param_t& comps = fullObjMap.at(k);

			// Name of Object
			//buff << _start << comps.first << _end << "\n";
			_cerealise.append(_start);
			_cerealise.append(comps.first);
			_cerealise.append(_end);
			_cerealise.append("\n");

			for (std::pair<std::string, ParamValueMap> i : comps.second)
			{
				// Name of Component
				//buff << "\t" << _start << i.first << _end << "\n";
				_cerealise.append("\t");
				_cerealise.append(_start);
				_cerealise.append(i.first);
				_cerealise.append(_end);
				_cerealise.append("\n");

				for (std::pair<std::string, std::string> j : i.second)
				{
					// Name of Component Variable
					//buff << "\t\t" << _start << j.first << _end << " " << j.second << "\n";
					_cerealise.append("\t\t");
					_cerealise.append(_start);
					_cerealise.append(j.first);
					_cerealise.append(_end);
					_cerealise.append(" ");
					_cerealise.append(j.second);
					_cerealise.append("\n");
				}
				//buff << "\t" << _breakName << "\n";
				_cerealise.append("\t");
				_cerealise.append(_breakName);
				_cerealise.append("\n");
			}
			//buff << _breakName << "\n";
			_cerealise.append(_breakName);
			_cerealise.append("\n");

		}


		//_buffer.clear();
		//Write(buff.str());
		//OpenWrite(filename);

		return _cerealise;
	}


	GameObjectFactory::GameObjectFactory() : _currentPool(nullptr), _systemGraphics{nullptr}
	{
		AddNameToMap<ComponentCollision>(STRING_COMPONENT_COLLISION);
		AddNameToMap<ComponentPhysics>(STRING_COMPONENT_PHYSICS);
		AddNameToMap<ComponentRenderer>(STRING_COMPONENT_RENDERER);
		AddNameToMap<ComponentTransform>(STRING_COMPONENT_TRANSFORM);
		AddNameToMap<ComponentAnimation>(STRING_COMPONENT_ANIMATION);
		AddNameToMap<ComponentScript>(STRING_COMPONENT_SCRIPT);
		AddNameToMap<ComponentText>(STRING_COMPONENT_TEXT);
		AddNameToMap<ComponentAudio>(STRING_COMPONENT_AUDIO);
		AddNameToMap<ComponentEmitter>(STRING_COMPONENT_EMITTER);
	}


	GameObjectFactory::~GameObjectFactory()
	{
	}

	void GameObjectFactory::SetCurrentPool(GameObjectPool * currentPool)
	{
		_currentPool = currentPool;
	}

	void GameObjectFactory::SetSystemGraphics(SystemGraphics* gfx)
	{
		_systemGraphics = gfx;
	}

	void GameObjectFactory::CreateSerialisedGameObjects(std::string fileName, bool isFab)
	{

		enum READSTATE
		{
			READ_START,
			READ_OBJECT,
			READ_END,
			READ_COMPONENT
		};

		// ADAPTED CODE =====================================================================

		typedef std::unordered_map<std::string, ParamValueMap> CompMaps;

		CustomTextSerializer readFromFile;
		CDEBUG_ASSERT(readFromFile.OpenRead(fileName), "Unable to Open File!");
		std::string gameObjName;
		std::unordered_map<std::string, ParamValueMap> compMap;

		while (readFromFile.CustomReadObjComponent(gameObjName, compMap))
		{
			GameObject* gameObject = _currentPool->AddGameObject();
			gameObject->SetName(gameObjName);

			CDEBUG_ASSERT(gameObject, "Game Object Not Created!");

			if (gameObject == nullptr) break; //

			CDEBUG_PRINT("----OBJECT :" + gameObjName);
			for (CompMaps::iterator i = std::begin(compMap); i != std::end(compMap); ++i)
			{
				// Gets the name for the component here.
				CDEBUG_PRINT("----COMPONENT :" + i->first);
				for (ParamValueMap::iterator y = std::begin(compMap[i->first]); y != std::end(compMap[i->first]); ++y)
				{
					// Gets the parameter name and parameter value here.
					CDEBUG_PRINT("----PARAM :" + y->first);
					CDEBUG_PRINT("----PARAM :" + y->second);
				}
				BuildComponent(gameObject, i->second, i->first, isFab);
			}
			CDEBUG_PRINT("NEXT OBJ");
		}
		CDEBUG_PRINT("READ END");
		// ADAPTED CODE =====================================================================

		/*
		TextSerializer readFromFile;
		readFromFile.OpenRead(fileName);
		std::string currentRead;
		READSTATE readState = READ_START;
		ParamValueMap paramValues;

		std::string componentName;
		GameObject* gameObject = nullptr;
		

		while (true)
		{
			
			readFromFile.ReadString(currentRead);
			//std::cout << "TEXT :" << currentRead << std::endl;

			switch (readState)
			{
			case READ_START:
				if (currentRead._Equal("END"))
				{
					readState = READ_END;

					// finished reading
				}
				else if (currentRead.at(0) == '<')
				{
					readState = READ_OBJECT;
					std::cout << "----OBJECT :" << currentRead << std::endl;

					// create gameObject

					gameObject = _currentPool->addGameObject();

					if (gameObject == nullptr)
					{
						// obj not created ***
						throw "GG";
					}

					// create gameObject END
				}
				break;
			case READ_OBJECT:
				if (currentRead._Equal("</>"))
				{
					readState = READ_START;
					std::cout << "NEXT OBJ" << std::endl;

					// object created with components //
				}
				else if (currentRead.at(0) == '<')
				{
					readState = READ_COMPONENT;
					currentRead = currentRead.substr(1, currentRead.find('>') - 1);
					std::cout << "----COMPONENT :" << currentRead << std::endl;
					componentName = currentRead;

					// got component name
				}
				break;
			case READ_COMPONENT:
				if (currentRead._Equal("</>"))
				{
					readState = READ_OBJECT;

					// read next object

					// write make a component and attach to current obj
					
					buildComponent(gameObject, paramValues, componentName);

				}
				else if (currentRead.at(0) == '<')
				{
					size_t endParamName = currentRead.find('>');
					std::string paramName = currentRead.substr(1, endParamName - 1);
					currentRead = currentRead.substr(endParamName + 1);
					std::cout << "----PARAM :" << paramName << std::endl;
					std::cout << "----PARAM :" << currentRead << std::endl;

					paramValues[paramName] = currentRead;

				}
				break;

			}

			if (readState == READ_END)
				break;
		}
		
		std::cout << "READ END" << std::endl;
		*/
	}

	GameObject* GameObjectFactory::CreateSingleSerializedGameObject(std::string fileName, bool isFab)
	{
		typedef std::unordered_map<std::string, ParamValueMap> CompMaps;

		CustomTextSerializer readFromFile;
		try
		{
			CDEBUG_ASSERT(readFromFile.OpenRead(fileName), "Unable to Open File!");
		}
		catch (...)
		{
			return nullptr;
		}
		std::string gameObjName;
		std::unordered_map<std::string, ParamValueMap> compMap;

		if (readFromFile.CustomReadObjComponent(gameObjName, compMap))
		{
			GameObject* gameObject = _currentPool->AddGameObject();
			gameObject->SetName(gameObjName);

			CDEBUG_ASSERT(gameObject, "Game Object Not Created!");
			for (CompMaps::iterator i = std::begin(compMap); i != std::end(compMap); ++i)
				BuildComponent(gameObject, i->second, i->first, isFab);
			return gameObject;
		}
		return nullptr;
	}

	//

	GameObject* GameObjectFactory::CreateGameObject_Test1(FwMath::Vector3D position, FwMath::Vector3D velocity, std::string path)
	{

		if (_currentPool == nullptr)
			return nullptr;

		//GameObject* newObj = _currentPool->AddGameObject();

		GameObject* newObj = CreateGameObject_Empty(FwMath::Vector3D(64.0f, 64.0f, 0.0f), position);

		if (newObj == nullptr)
		{
			// obj not created ***
			return nullptr;
		}

		//ComponentAnimation compAni(newObj);
		//compAni.enable();
		//currentPool->attachComponent(newObj->getIndex(), COMPONENT_ANIMATION, &compAni);

		ComponentCollision* compCol = 
			dynamic_cast<ComponentCollision*>(_currentPool->GetComponent<ComponentCollision>(newObj->GetIndex()));
		compCol->setParentGameObject(newObj);
		compCol->enable();
		//compCol->Init();
		compCol->_vertexA = FwMath::Vector3D(-0.5f, -0.5f, 1.0f);
		compCol->_vertexB = FwMath::Vector3D(0.5f, 0.5f, 1.0f);

		ComponentTransform* compTrans =
			dynamic_cast<ComponentTransform*>(_currentPool->GetComponent<ComponentTransform>(newObj->GetIndex()));
		//compTrans->setParentGameObject(newObj);
		//compTrans->enable();
		//compTrans->_currentPosition = position;
		//compTrans->_scale = FwMath::Vector3D(64.0f, 64.0f, 1.0f);
		//compTrans->_rotation = 0;

		ComponentPhysics* compPhy =
			dynamic_cast<ComponentPhysics*>(_currentPool->GetComponent<ComponentPhysics>(newObj->GetIndex()));
		compPhy->setParentGameObject(newObj);
		compPhy->enable();
		compPhy->_mass = 10;
		compPhy->_velocity = velocity;
		compPhy->_isStatic = false;
		compPhy->_previousPosition = position;

		compPhy->_transformComponent = compTrans;

		ComponentRenderer* compRender =
			dynamic_cast<ComponentRenderer*>(_currentPool->GetComponent<ComponentRenderer>(newObj->GetIndex()));
		compRender->setParentGameObject(newObj);
		compRender->enable();
		//compRender->_type = TEXTURE;
		compRender->_layer = 0;
		compRender->SetTexture(path);

		compRender->SetTransform(compTrans);

		ComponentAnimation* compAnimation =
			dynamic_cast<ComponentAnimation*>(_currentPool->GetComponent<ComponentAnimation>(newObj->GetIndex()));
		compRender->SetAnimation(compAnimation);

		_systemGraphics->AddRenderer(compRender);

		ComponentScript* compScript =
			dynamic_cast<ComponentScript*>(_currentPool->GetComponent<ComponentScript>(newObj->GetIndex()));
		compScript->setParentGameObject(newObj);
		compScript->enable();
		ParamValueMap paramMap;
		paramMap["script0"] = "testobjectone";
		compScript->Init(paramMap);

		compScript->InitAllScripts();

		return newObj;
	}

	GameObject* GameObjectFactory::CreateGameObject_Empty(FwMath::Vector3D scale, FwMath::Vector3D position)
	{
		GameObject* newObj = _currentPool->AddGameObject();

		if (newObj == nullptr)
		{
			// obj not created ***
			return nullptr;
		}

		newObj->enable();

		ComponentTransform* compTrans =
			dynamic_cast<ComponentTransform*>(_currentPool->GetComponent<ComponentTransform>(newObj->GetIndex()));
		compTrans->setParentGameObject(newObj);
		compTrans->enable();
		compTrans->_currentPosition = position;
		compTrans->_scale = scale; // FwMath::Vector3D(64.0f, 64.0f, 1.0f);
		compTrans->_rotation = 0;

		return newObj;
	}

	ComponentRenderer* GameObjectFactory::GameObject_AttachComponent_Renderer(size_t objId, std::string path, int layer)
	{
		if (_currentPool == nullptr)
			return nullptr;
	
		ComponentTransform* compTrans =
			dynamic_cast<ComponentTransform*>(_currentPool->GetComponent<ComponentTransform>(objId));

		ComponentRenderer* compRender =
			dynamic_cast<ComponentRenderer*>(_currentPool->GetComponent<ComponentRenderer>(objId));
		compRender->setParentGameObject(_currentPool->GetGameObject(objId));
		compRender->enable();
		compRender->_layer = layer;
		compRender->SetTexture(path);

		compRender->SetTransform(compTrans);		

		compRender->_flipX = false;
		compRender->_flipY = false;

		_systemGraphics->AddRenderer(compRender);

		return compRender;
	}

	ComponentText* GameObjectFactory::GameObject_AttachComponent_Text(size_t objId, std::string text)
	{
		if (_currentPool == nullptr)
			return nullptr;

		try 
		{

			ComponentText* compText =
				dynamic_cast<ComponentText*>(_currentPool->GetComponent<ComponentText>(objId));


			compText->setParentGameObject(_currentPool->GetGameObject(objId));

			compText->enable();
			compText->_fontType = std::string(".\\Resources\\Fonts\\arial.ttf");
			compText->_displayText = text;
			compText->_pixel = 64;

			compText->_colour = FwMath::Vector4D(
				1.0f,
				1.0f,
				1.0f,
				1.0f
			);

			ComponentTransform* compTrans =
				dynamic_cast<ComponentTransform*>(_currentPool->GetComponent<ComponentTransform>(objId));

			compText->SetTransform(compTrans);

			try
			{
				compText->SetFont(compText->_fontType, compText->_pixel);
			}
			catch (...)
			{
				//CDEBUG_PRINT(std::string(_fontType).append(" not found."));
				compText->SetFont("",0);
			}

			return compText;
		}
		catch (...)
		{
			return nullptr;
		}
	}

	ComponentScript* GameObjectFactory::GameObject_AttachComponent_Script(size_t objId, std::string script)
	{
		if (_currentPool == nullptr)
			return nullptr;

		ComponentScript* compScr =
			dynamic_cast<ComponentScript*>(_currentPool->GetComponent<ComponentRenderer>(objId));
		compScr->enable();
		compScr->setParentGameObject(_currentPool->GetGameObject(objId));
		compScr->AttachScript(script);

		return compScr;
	}

	// For Event Messaging Testing purposes
	void GameObjectFactory::CreateObject(InputMessage& msg)
	{
#ifdef  _DEBUG
		std::string toDebug;

		toDebug = "Cursor Pos on click: ";
		toDebug.append(std::to_string(msg._cursorPos.x));
		toDebug.append(" " + std::to_string(msg._cursorPos.y));

		CDEBUG_PRINT(toDebug.c_str());
#endif
		//RECT rect;
		//GetClientRect(GRAPHICS->GetHandle(), &rect);
		//LONG width = rect.right;
		//LONG height = rect.bottom;
		//if (msg._cursorPos.x < 0 || msg._cursorPos.x > width || msg._cursorPos.y < 0 || msg._cursorPos.y > height)
		//	return;

		if (msg._key == 2)
		{
			for (unsigned int i = 0; i < 1500; ++i)
				CreateGameObject_Test1(
					FwMath::Vector3D(
						(float)msg._cursorPos.x,
						(float)msg._cursorPos.y
						, 0.0f)
					,
					FwMath::Vector3D(0, 0, 0)
				);
		}
		else if (msg._key == 1)
		{
			CreateGameObject_Test1(
				FwMath::Vector3D(
					(float)msg._cursorPos.x,
					(float)msg._cursorPos.y
					, 0.0f),
				FwMath::Vector3D(0, 0, 0)
			);
		}
	}

}
