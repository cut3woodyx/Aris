/******************************************************************************/
/*!
\file		imguiList.cpp
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "imguiMgr.h"
#include "LESysMgr.h"
#include "imgui_stdlib.h"
#include "WindowsDialogBox.h"
#include "Math_CollisionAABB.h"

#define TRANSLATE_COLOUR_X 1.0f,0.0f,0.0f
#define TRANSLATE_COLOUR_Y 0.0f,1.0f,0.0f

namespace FwEngine
{
	namespace
	{
		static std::string searchObj = "";
		static bool sortList = false;
		static bool searchByTag = false;
		static std::set<size_t> multiSelectObjs;
		static Vector3D startMouse;
	}
	
	void ImguiMgr::ImGUIList()
	{
		//ImGui::SetNextWindowSize({ _clientRect.right *0.30f, (float)_clientRect.bottom - 20.0f}, ImGuiCond_Always);
		//ImGui::SetNextWindowPos({ _clientRect.right * 0.70f, 20.0f });
		
		ImGui::Begin("Objects", nullptr);//, (bool*)0, 
			//ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		if (ImGui::IsWindowCollapsed())
		{
			ImGui::End();
			return;
		}

		std::vector<GameObject>& pool = *_mgr->_gameObjectPool.GetContainerPtr<GameObject>();
		std::vector<ComponentTransform>& trnPool = *_mgr->_gameObjectPool.GetContainerPtr<ComponentTransform>();

		ImGui::Text("Sort");
		ImGui::SameLine();
		ImGui::Checkbox("##SortObj", &sortList);
		ImGui::SameLine();
		ImGui::Text("Search");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() * 0.5f);
		ImGui::InputText("##SearchObj", &searchObj);
		ImGui::SameLine();
		ImGui::Text("Tag");
		ImGui::SameLine();
		ImGui::Checkbox("##SearchByTag", &searchByTag);
		
		ImGui::BeginChild("Object List", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.4f, ImGui::GetWindowHeight() - 60.0f), 
			true, ImGuiWindowFlags_HorizontalScrollbar);

		// Add/Delete Objects
		{
			if (ImGui::Button("Add Obj##ObjectList"))
			{
				int index = (int)_mgr->_gameObjectPool.AddGameObject()->GetIndex();
				(*_mgr->_gameObjectPool.GetContainerPtr<ComponentTransform>())[index].enable();
				_sObj = index;
			}

			ImGui::SameLine();

			if (ImGui::Button("Delete##ObjectList"))
				if (_sObj >= 0)
					ImGui::OpenPopup("Delete Object?");

			if (_input->GetKeyIf(IKEY_DELETE, SystemInput::PRESS))
				if (_sObj >= 0)
					ImGui::OpenPopup("Delete Object?");

			if (ImGui::BeginPopupModal("Delete Object?", NULL))
			{
				ImGui::Text("Are you certain? Deleted Objects may not be recoverable.\nThis operation cannot be undone!\n\n");
				ImGui::Separator();

				if (ImGui::Button("OK"))
				{
					_mgr->_gameObjectPool.DestroyGameObject(_sObj);
					_sObj = -1;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}

		// Swap object data
		auto swapObject = [&](size_t index_1, size_t index_2)
		{
			GameObjectPool sPool = _mgr->_gameObjectPool;
			std::vector<IComponent*> comps;

			comps.push_back(&(*sPool.GetContainerPtr<ComponentTransform>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentCollision>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentPhysics>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentRenderer>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentText>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentScript>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentAnimation>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentAudio>())[index_1]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentEmitter>())[index_1]);

			std::vector<IComponent*> comps2;

			comps2.push_back(&(*sPool.GetContainerPtr<ComponentTransform>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentCollision>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentPhysics>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentRenderer>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentText>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentScript>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentAnimation>())[index_2]);
			comps2.push_back(&(*sPool.GetContainerPtr<ComponentAudio>())[index_2]);
			comps.push_back(&(*sPool.GetContainerPtr<ComponentEmitter>())[index_2]);

			paramPair obj1 = GetObjectParam(index_1);
			paramPair obj2 = GetObjectParam(index_2);

			for (IComponent* i : comps)
			{
				i->disable();
				i->setParentGameObject(&pool[index_1]);

				if (obj2.second.find(i->GetParams().first) != std::end(obj2.second))
					i->Init(obj2.second.find(i->GetParams().first)->second);
			}
			for (IComponent* i : comps2)
			{
				i->disable();
				i->setParentGameObject(&pool[index_2]);

				if (obj1.second.find(i->GetParams().first) != std::end(obj1.second))
					i->Init(obj1.second.find(i->GetParams().first)->second);
			}

			std::string someName = pool[index_1].GetName();
			pool[index_1].SetName(pool[index_2].GetName());
			pool[index_2].SetName(someName);
		};

		// Swap up or down
		if (ImGui::Button("Up"))
		{
			int nextObj = _sObj - 1;
			while (nextObj >= 0)
			{
				if (pool[nextObj]._isActive)
				{
					swapObject(_sObj, nextObj);
					_sObj = nextObj;
					break;
				}
				--nextObj;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Down"))
		{
			int nextObj = _sObj + 1;
			if (nextObj >= 0)
			{
				while (nextObj < pool.size() - 1)
				{
					if (pool[nextObj]._isActive)
					{
						swapObject(_sObj, nextObj);
						_sObj = nextObj;
						break;
					}
					++nextObj;
				}
			}
		}

		ImGui::BeginChild("##ObjectList",
			ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 65.0f), true, ImGuiWindowFlags_HorizontalScrollbar);


		// Object selection on Object list
		auto selectObjs = [&](int index)
		{
			std::string objName;
			if (searchByTag)
				objName = trnPool[index]._tag;
			else
				objName = pool[index].GetName();
			if (!searchByTag && objName.empty())
			{
				objName = "Object ";
				objName.append(std::to_string(index));
			}

			if (searchObj.empty()
				|| objName.find(searchObj) != std::string::npos)
			{
				if (ImGui::Selectable(std::string(objName).append("##").append(std::to_string(index)).c_str(),
					multiSelectObjs.find(index) != std::end(multiSelectObjs),
					ImGuiSelectableFlags_PressedOnClick))
				{
					_sObj = index;
					_sPrefab = -1;

					if (_input->GetKeyIf(IKEY_CTRL, SystemInput::HOLD))
					{
						if (multiSelectObjs.find(index) == std::end(multiSelectObjs))
							multiSelectObjs.emplace(index);
						else
							multiSelectObjs.erase(multiSelectObjs.find(index));
					}
					else if (_input->GetKeyIf(IKEY_SHIFT, SystemInput::HOLD))
					{
						int start = (int)(*std::begin(multiSelectObjs));
						if (start > index)
							std::swap(start, index);
						multiSelectObjs.clear();
						for (size_t i =start; i <= index; ++i)
							multiSelectObjs.emplace(i);
					}
					else
					{
						multiSelectObjs.clear();
						multiSelectObjs.emplace(index);
					}
				}

				// Drag Drop into Prefabs
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("PREFAB_ADDDROP", (void*)&pool[_sObj], sizeof(std::string));
					ImGui::Text(objName.c_str());
					ImGui::EndDragDropSource();
				}
			}
		};

		// Display all Active Objects
		if (!sortList)
		{
			for (int n = 0; n < pool.size(); ++n)
			{
				if (!pool[n]._isActive)
					continue;
				//ImGui::Checkbox(std::to_string(n).c_str(), &pool[n]._isActive);
				//ImGui::SameLine();

				selectObjs(n);
			}
		}
		else
		{
			std::multimap<std::string, int> sortedMap;
			for (int n = 0; n < pool.size(); ++n)
			{
				if (!pool[n]._isActive)
					continue;
				std::string base = pool[n].GetName();
				std::for_each(std::begin(base), std::end(base),
					[](char& c)
					{
						c = static_cast<char>(std::tolower(c));
					});
				sortedMap.emplace(base, n);
			}
			for (std::pair<std::string, int> i : sortedMap)
			{
				int n = i.second;
				selectObjs(n);
			}
		}
		ImGui::EndChild();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Inspector", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		// if selected object does not exist in multi, clear and add in
		if (multiSelectObjs.find(_sObj) == std::end(multiSelectObjs))
		{
			multiSelectObjs.clear();
			multiSelectObjs.emplace(_sObj);
		}

		// Components
		if (_sObj>=0)
		{
			ComponentTransform& trn = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentTransform>())[_sObj];
			ComponentCollision& col = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentCollision>())[_sObj];
			ComponentPhysics& phy = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentPhysics>())[_sObj];
			ComponentRenderer& ren = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentRenderer>())[_sObj];
			ComponentAnimation& anim = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentAnimation>())[_sObj];
			ComponentText& text = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentText>())[_sObj];
			ComponentAudio& audio = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentAudio>())[_sObj];
			ComponentScript& script = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentScript>())[_sObj];
			ComponentEmitter& emitter = (*_mgr->_gameObjectPool.GetContainerPtr<ComponentEmitter>())[_sObj];

			std::map<std::string, IComponent&> allComps;
			allComps.emplace(STRING_COMPONENT_TRANSFORM, trn); 
			allComps.emplace(STRING_COMPONENT_COLLISION, col);
			allComps.emplace(STRING_COMPONENT_PHYSICS, phy); 
			allComps.emplace(STRING_COMPONENT_RENDERER, ren); 
			allComps.emplace(STRING_COMPONENT_ANIMATION, anim);
			allComps.emplace(STRING_COMPONENT_TEXT, text);
			allComps.emplace(STRING_COMPONENT_AUDIO, audio);
			allComps.emplace(STRING_COMPONENT_SCRIPT, script);
			allComps.emplace(STRING_COMPONENT_EMITTER, emitter);

			std::string objName = pool[_sObj].GetName();
			ImGui::InputText(std::string("ID: ").append(std::to_string(_sObj)).c_str(), &objName);
			if (!objName._Equal(pool[_sObj].GetName()))
				pool[_sObj].SetName(objName);


			// Add/Delete Components
			{
				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("AddCompPopUp");
				ImGui::SameLine();
				if (ImGui::Button("Delete Component"))
					ImGui::OpenPopup("DelCompPopUp");

				if (ImGui::BeginPopup("AddCompPopUp"))
				{
					for (std::pair<std::string, IComponent&> i : allComps)
					{
						if (!i.second.isEnabled())
							if (ImGui::Selectable(i.first.c_str()))
							{
								i.second.enable();
								i.second.setParentGameObject(&(*_mgr->_gameObjectPool.GetGameObject(_sObj)));

								if (i.first == STRING_COMPONENT_RENDERER)
								{
									GRAPHICS->AddRenderer(reinterpret_cast<ComponentRenderer*>(&i.second));
								}
								else if(i.first == STRING_COMPONENT_TEXT)
								{
									GRAPHICS->AddText(reinterpret_cast<ComponentText*>(&i.second));
								}
								ImGui::CloseCurrentPopup();
								break;
							}
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopup("DelCompPopUp"))
				{
					for (std::pair<std::string, IComponent&> i : allComps)
					{
						if (i.second.isEnabled())
							if(!i.first._Equal(STRING_COMPONENT_TRANSFORM))
								if (ImGui::Selectable(i.first.c_str()))
								{
									i.second.disable();

									if (i.first == STRING_COMPONENT_RENDERER)
									{
										GRAPHICS->DeleteRenderer(reinterpret_cast<ComponentRenderer*>(&i.second));
									}
									else if (i.first == STRING_COMPONENT_TEXT)
									{
										GRAPHICS->DeleteText(reinterpret_cast<ComponentText*>(&i.second));
									}

									if (i.first == STRING_COMPONENT_AUDIO)
									{
										ComponentAudio* toStopAudio = reinterpret_cast<ComponentAudio*>(&i.second);
										toStopAudio->StopSound();
									}
									ImGui::CloseCurrentPopup();
									break;
								}
					}
					ImGui::EndPopup();
				}
			}

			//ImGui::SameLine();
			// Focus on selected Object

			// Shortcuts
			if (_sObj >= 0)
			{
				if (_input->GetKeyIf(IKEY_CTRL, SystemInput::HOLD))
				{
					if (_input->GetKeyIf(IKEY_F, SystemInput::PRESS))
					{
						_graphics->_cameraPos = { trn._currentPosition.x, trn._currentPosition.y, _graphics->_cameraPos.z };
						_graphics->UpdateCameraView();
					}
					if (_input->GetKeyIf(IKEY_V, SystemInput::PRESS))
					{
						CustomTextSerializer saveObjs;
						std::vector<paramPair> fullObjMap;

						paramPair singleObj = GetObjectParam(_sObj);
						fullObjMap.push_back(singleObj);

						saveObjs.CustomSaveObjComponent(
							".\\dupeObj.txt",
							fullObjMap);

						GameObject* obj = _mgr->_gameObjectFactory.CreateSingleSerializedGameObject(".\\dupeObj.txt");
						std::string copyObj = obj->GetName();
						if (copyObj.find("_copy", copyObj.size() - 6) == std::string::npos)
							obj->SetName(copyObj.append("_copy"));

						DeleteFile(L".\\dupeObj.txt");

						_sObj = (int)obj->GetIndex();
					}
				}
			}

			// Multi-selection
			if (multiSelectObjs.size()<2)
				ImGUIListComps(trn, col, phy, ren, anim, text, audio, script, emitter);
			else
			{
				// Multiselect here
				float highestX = 0, lowestX = 0, highestY = 0, lowestY = 0;

				GameObjectPool goPool = _mgr->_gameObjectPool;

				if (!_input->GetKeyIf(IKEY_CTRL, SystemInput::HOLD) &&
					_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::PRESS)
					&& !_mouseCaptured && !_mouseUsed)
					_mouseUsed = true;

				if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::RELEASE))
					_mouseUsed = false;

				CursorPos mousePos = _input->GetMouseWorldPos(_graphics->_cameraPos.x, _graphics->_cameraPos.y,
					_graphics->GetHeight(), _graphics->GetWidth(), _graphics->_zoom);

				// Moving all objects
				std::for_each(std::begin(multiSelectObjs), std::end(multiSelectObjs),
				[&](size_t i)
				{
					ComponentTransform& cool = goPool.GetContainerPtr<ComponentTransform>()->at(i);
					if (highestX < cool._currentPosition.x)
						highestX = cool._currentPosition.x;
					if (highestY < cool._currentPosition.y)
						highestY = cool._currentPosition.y;
					if (lowestX > cool._currentPosition.x || lowestX == 0)
						lowestX = cool._currentPosition.x;
					if (lowestY > cool._currentPosition.y || lowestY == 0)
						lowestY = cool._currentPosition.y;

					Vector3D pos = cool._currentPosition;
					Vector3D scale = cool._scale;

					FwEngine::Vector3D vertexA = { pos.x + (scale.x * col._vertexA.x), pos.y + (scale.y * col._vertexA.y), 0 };
					FwEngine::Vector3D vertexB = { pos.x + (scale.x * col._vertexB.x), pos.y + (scale.y * col._vertexB.y), 0 };
					FwEngine::Vector3D vertexC = { pos.x + (scale.x * col._vertexC.x), pos.y + (scale.y * col._vertexC.y), 0 };
					FwEngine::Vector3D vertexD = { pos.x + (scale.x * col._vertexD.x), pos.y + (scale.y * col._vertexD.y), 0 };

					_mgr->_systemGraphics.DrawDebugLine(vertexA, vertexC, Vector3D(0, 1, 0), Vector3D(0, 1, 0));
					_mgr->_systemGraphics.DrawDebugLine(vertexC, vertexB, Vector3D(0, 1, 0), Vector3D(0, 1, 0));
					_mgr->_systemGraphics.DrawDebugLine(vertexB, vertexD, Vector3D(0, 1, 0), Vector3D(0, 1, 0));
					_mgr->_systemGraphics.DrawDebugLine(vertexD, vertexA, Vector3D(0, 1, 0), Vector3D(0, 1, 0));

					if (_mouseUsed)
					{
						CursorPos drag = _input->GetMouseWorldPosDragVec();
						cool._currentPosition += Vector3D{ drag.x, drag.y, 0 };
					}
				});

				Vector3D center = { lowestX + (highestX - lowestX) * 0.5f, lowestY + (highestY - lowestY) * 0.5f, 0 };

				_mgr->_systemGraphics.DrawDebugLine(center,
					{ center.x + center.x * 1.25f, center.y ,0 }, FwMath::Vector3D(TRANSLATE_COLOUR_X), FwMath::Vector3D(TRANSLATE_COLOUR_X));
				_mgr->_systemGraphics.DrawDebugLine(center,
					{ center.x, center.y + center.y * 1.25f ,0 }, FwMath::Vector3D(TRANSLATE_COLOUR_Y), FwMath::Vector3D(TRANSLATE_COLOUR_Y));
							}
		}
		ImGui::EndChild();

		if (!_playEngine)
		{
			// Selecting objects by clicking on them directly 
			CursorPos mousePos = _input->GetMouseWorldPos(_graphics->_cameraPos.x, _graphics->_cameraPos.y,
				_graphics->GetHeight(), _graphics->GetWidth(), _graphics->_zoom);

			if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::PRESS))
				startMouse = { mousePos.x, mousePos.y,0 };

			for (size_t i = 0; i < pool.size(); ++i)
			{
				if (!pool[i]._isActive)
					continue;
				ComponentTransform& tran = _mgr->_gameObjectPool.GetContainerPtr<ComponentTransform>()->at(i);
				ComponentCollision& cool = _mgr->_gameObjectPool.GetContainerPtr<ComponentCollision>()->at(i);
				ComponentText& texxt = _mgr->_gameObjectPool.GetContainerPtr<ComponentText>()->at(i);

				Vector3D pos = tran._currentPosition;
				Vector3D scale = tran._scale;

				if (scale.x < 0)
					scale.x = -scale.x;
				if (scale.y < 0)
					scale.y = -scale.y;

				Vector3D fMin = { pos.x - (scale.x * 0.5f), pos.y - (scale.y * 0.5f), 0 };
				Vector3D fMax = { pos.x + (scale.x * 0.5f), pos.y + (scale.y * 0.5f), 0 };

				// Draw Debugging
				if (cool.isEnabled())
				{
					Vector3D vertexA = { pos.x + (scale.x * cool._vertexA.x), pos.y + (scale.y * cool._vertexA.y), 0 };
					Vector3D vertexB = { pos.x + (scale.x * cool._vertexB.x), pos.y + (scale.y * cool._vertexB.y), 0 };
					Vector3D vertexC = { pos.x + (scale.x * cool._vertexC.x), pos.y + (scale.y * cool._vertexC.y), 0 };
					Vector3D vertexD = { pos.x + (scale.x * cool._vertexD.x), pos.y + (scale.y * cool._vertexD.y), 0 };

					fMin = vertexA;
					fMax = vertexB;
				}
				else if (texxt.isEnabled())
				{
					pos = { pos.x, pos.y, 0 };
					scale = { (float)texxt.CalculateMaxRowWidth() * scale.x,
							  ((float)texxt.CountLines()) * texxt._pixel * scale.y,
							  0.0f };
					FwEngine::Vector3D min = { pos.x - (scale.x * 0.5f), pos.y - (scale.y * 0.5f), 0 };
					FwEngine::Vector3D max = { pos.x + (scale.x * 0.5f), pos.y + (scale.y * 0.5f), 0 };
					fMin = min;
					fMax = max;
				}

				if (!_input->GetKeyIf(IKEY_CTRL, SystemInput::HOLD) &&
					_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::RELEASE)
					&& !_mouseCaptured
					&& (startMouse.x == mousePos.x && startMouse.y == mousePos.y))
				{
					if (mousePos.x > fMin.x&& mousePos.x < fMax.x
						&& mousePos.y > fMin.y&& mousePos.y < fMax.y)
					{
						if (multiSelectObjs.find(i) != std::end(multiSelectObjs))
							continue;
						if (!_input->GetKeyIf(IKEY_SHIFT))
							multiSelectObjs.clear();
						multiSelectObjs.emplace(i);
						_sObj = (int)i;
						break;
					}
					else
					{
						if (!_input->GetKeyIf(IKEY_SHIFT))
						{
							if (i == pool.size() - 1)
							{
								multiSelectObjs.clear();
								_sObj = -1;
							}
						}
					}
				}
			}
		}

		

		ImGui::End();
	}
}