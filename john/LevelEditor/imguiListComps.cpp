/******************************************************************************/
/*!
\file		imguiListComps.cpp
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
#include "StringConverter.h"
#include "ScriptFactory.h"

#define SELECTED_COLOUR 1.0f, 0.5f, 1.0f
#define TRANSLATE_COLOUR_X 1.0f,0.0f,0.0f
#define TRANSLATE_COLOUR_Y 0.0f,1.0f,0.0f
#define SCALE_COLOUR 1.0f,1.0f,0.0f
#define ROTATE_COLOUR 0.0f, 0.0f, 1.0f
#define COLLIDER_BOX_COLOUR 0.5f, 0.5f, 0.0f

namespace FwEngine
{
	namespace
	{
		static const int texSize = 32;
		static Vector3D startPos, startScale;
		static std::string tagName;
	}

	std::string ImguiMgr::ImGUICopyFile(std::string filename, std::string newFilePath)
	{
		std::string newName = filename.substr(filename.rfind("\\"), filename.size());
		CreateDirectory(UTF8_decode(newFilePath).c_str(), NULL);
		newFilePath.append(newName);
		CopyFile(UTF8_decode(filename).c_str(), UTF8_decode(newFilePath).c_str(), false);
		return newFilePath;
	}

	void ImguiMgr::ImGUIListComps(
		ComponentTransform& trn,
		ComponentCollision& col,
		ComponentPhysics& phy,
		ComponentRenderer& ren,
		ComponentAnimation& anim,
		ComponentText& text,
		ComponentAudio& audio,
		ComponentScript& script,
		ComponentEmitter& emitter
	)
	{
		ImGui::BeginChild("Components", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		{
			// Transform Component
			if (trn.isEnabled())
			{
				if (!trn._tag.empty())
					_tagList.emplace(trn._tag);
				FwEngine::Vector3D pos{ trn._currentPosition.x, trn._currentPosition.y, trn._currentPosition.z };

				CursorPos mousePos = _input->GetMouseWorldPos(_graphics->_cameraPos.x, _graphics->_cameraPos.y,
					_graphics->GetHeight(), _graphics->GetWidth(), _graphics->_zoom);

				Vector3D scale = trn._scale;

				if (scale.x < 0)
					scale.x = -scale.x;
				if (scale.y < 0)
					scale.y = -scale.y;

				Vector3D fMin = { pos.x - (scale.x * 0.5f), pos.y - (scale.y * 0.5f), 0 };
				Vector3D fMax = { pos.x + (scale.x * 0.5f), pos.y + (scale.y * 0.5f), 0 };

				// Draw Debugging
				if (col.isEnabled())
				{
					FwEngine::Vector3D vertexA = { pos.x + (scale.x * col._vertexA.x), pos.y + (scale.y * col._vertexA.y), 0 };
					FwEngine::Vector3D vertexB = { pos.x + (scale.x * col._vertexB.x), pos.y + (scale.y * col._vertexB.y), 0 };
					FwEngine::Vector3D vertexC = { pos.x + (scale.x * col._vertexC.x), pos.y + (scale.y * col._vertexC.y), 0 };
					FwEngine::Vector3D vertexD = { pos.x + (scale.x * col._vertexD.x), pos.y + (scale.y * col._vertexD.y), 0 };

					_mgr->_systemGraphics.DrawDebugLine(vertexA, vertexC, Vector3D(0, 1, 0), Vector3D(0, 1, 0));
					_mgr->_systemGraphics.DrawDebugLine(vertexC, vertexB, Vector3D(0, 1, 0), Vector3D(0, 1, 0));
					_mgr->_systemGraphics.DrawDebugLine(vertexB, vertexD, Vector3D(0, 1, 0), Vector3D(0, 1, 0));
					_mgr->_systemGraphics.DrawDebugLine(vertexD, vertexA, Vector3D(0, 1, 0), Vector3D(0, 1, 0));

					/*_mgr->_systemGraphics.DrawDebugLine(min, { min.x, max.y,0 }, FwMath::Vector3D(COLLIDER_BOX_COLOUR), FwMath::Vector3D(COLLIDER_BOX_COLOUR));
					_mgr->_systemGraphics.DrawDebugLine({ min.x, max.y,0 }, max, FwMath::Vector3D(COLLIDER_BOX_COLOUR), FwMath::Vector3D(COLLIDER_BOX_COLOUR));
					_mgr->_systemGraphics.DrawDebugLine(max, { max.x, min.y,0 }, FwMath::Vector3D(COLLIDER_BOX_COLOUR), FwMath::Vector3D(COLLIDER_BOX_COLOUR));
					_mgr->_systemGraphics.DrawDebugLine({ max.x, min.y,0 }, min, FwMath::Vector3D(COLLIDER_BOX_COLOUR), FwMath::Vector3D(COLLIDER_BOX_COLOUR));*/
					fMin = vertexA;
					fMax = vertexB;
				}
				else if (text.isEnabled())
				{
					pos = { pos.x, pos.y, 0 };
					scale = { (float)text.CalculateMaxRowWidth() * scale.x,
							  ((float)text.CountLines()) * text._pixel * scale.y,
							  0.0f };
					_mgr->_systemGraphics.DrawDebugBox(pos,
						scale, FwMath::Vector3D(SELECTED_COLOUR));
					FwEngine::Vector3D min = { pos.x - (scale.x * 0.5f), pos.y - (scale.y * 0.5f), 0 };
					FwEngine::Vector3D max = { pos.x + (scale.x * 0.5f), pos.y + (scale.y * 0.5f), 0 };
					fMin = min;
					fMax = max;
				}
				else
				{
					if (ren.isEnabled() && ren._ui)
					{
						pos.x = _mgr->_systemGraphics.GetWidth() * pos.x;
						pos.y = _mgr->_systemGraphics.GetHeight() * pos.y;

						scale.x = _mgr->_systemGraphics.GetWidth() * scale.x;
						scale.y = _mgr->_systemGraphics.GetHeight() * scale.y;

						_mgr->_systemGraphics.DrawDebugBox(pos, scale, FwMath::Vector3D(SELECTED_COLOUR));

						FwEngine::Vector3D min = { pos.x - (scale.x * 0.5f), pos.y - (scale.y * 0.5f), 0 };
						FwEngine::Vector3D max = { pos.x + (scale.x * 0.5f), pos.y + (scale.y * 0.5f), 0 };
						fMin = min;
						fMax = max;
					}
					else
					{
						_mgr->_systemGraphics.DrawDebugBox(pos, scale, FwMath::Vector3D(SELECTED_COLOUR));
					}
				}

				// Changes transform based on input
				{
					if (_input->GetKeyIf(IMOUSE_RBUTTON, SystemInput::PRESS))
						if (++_type > ROTATE)
							_type = 0;
					if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::RELEASE))
						_mouseUsed = false;
					CursorPos drag = _input->GetMouseWorldPosDragVec();
					switch (_type)
					{
					case TRANSLATE:
					{
						//_mgr->_systemGraphics.DrawDebugBox({ pos.x + scale.x, pos.y, 0 }, { scale.x + 20.0f, 20.0f, 0 }, FwMath::Vector3D(TRANSLATE_COLOUR_X));
						_mgr->_systemGraphics.DrawDebugLine(pos, { pos.x + scale.x, pos.y ,0 }, FwMath::Vector3D(TRANSLATE_COLOUR_X), FwMath::Vector3D(TRANSLATE_COLOUR_X));
						_mgr->_systemGraphics.DrawDebugLine(pos, { pos.x, pos.y + scale.y ,0 }, FwMath::Vector3D(TRANSLATE_COLOUR_Y), FwMath::Vector3D(TRANSLATE_COLOUR_Y));

						if (mousePos.x > fMin.x - drag.x && mousePos.x < fMax.x - drag.x)
							if (mousePos.y > fMin.y + drag.y && mousePos.y < fMax.y + drag.y)
								if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::PRESS) && !_mouseCaptured)
								{
									_undoTrans.emplace_back(_sObj, "translate", trn._currentPosition);
									startPos = { mousePos.x, mousePos.y, 0 };
									_mouseUsed = true;
								}
						if (_mouseUsed)
						{
							if (ren.isEnabled() && ren._ui)
							{
								trn._currentPosition += Vector3D{ drag.x / _mgr->_systemGraphics.GetWidth(), drag.y / _mgr->_systemGraphics.GetHeight(), 0 };
							}
							else
							{
								trn._currentPosition += Vector3D{ drag.x, drag.y, 0 };
							}

							if (audio.isEnabled())
							{
								audio.SetAudioPosition();
							}
						}

						break;
					}
					case SCALE:
					{
						/*_mgr->_systemGraphics.DrawDebugLine(pos, fMax, FwMath::Vector3D(SCALE_COLOUR), FwMath::Vector3D(SCALE_COLOUR));
						_mgr->_systemGraphics.DrawDebugLine(pos, fMin, FwMath::Vector3D(SCALE_COLOUR), FwMath::Vector3D(SCALE_COLOUR));
						_mgr->_systemGraphics.DrawDebugLine(pos, { fMin.x, fMax.y, 0 }, FwMath::Vector3D(SCALE_COLOUR), FwMath::Vector3D(SCALE_COLOUR));
						_mgr->_systemGraphics.DrawDebugLine(pos, { fMax.x, fMin.y, 0 }, FwMath::Vector3D(SCALE_COLOUR), FwMath::Vector3D(SCALE_COLOUR));*/

						_mgr->_systemGraphics.DrawDebugLine(fMin, fMax, FwMath::Vector3D(SCALE_COLOUR), FwMath::Vector3D(SCALE_COLOUR));
						_mgr->_systemGraphics.DrawDebugLine({ fMin.x, fMax.y, 0.0f }, { fMax.x, fMin.y,0.0f }, FwMath::Vector3D(SCALE_COLOUR), FwMath::Vector3D(SCALE_COLOUR));

						if (mousePos.x > fMin.x - drag.x && mousePos.x < fMax.x - drag.x)
							if (mousePos.y > fMin.y + drag.y && mousePos.y < fMax.y + drag.y)
								if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::PRESS) && !_mouseCaptured)
								{
									_undoTrans.emplace_back(_sObj, "scale", trn._scale);
									_mouseUsed = true;
									startPos = { mousePos.x, mousePos.y, 0 };
									startScale = trn._scale;
								}

						if (_mouseUsed)
						{
							Vector3D lr;
							if (text.isEnabled())
							{
								lr = { (float)drag.x * 2.0f / text._pixel, -(float)drag.y * 2.0f / text._pixel, 0.0f };
							}
							else if (ren.isEnabled() && ren._ui)
							{
								lr = { (float)drag.x / _mgr->_systemGraphics.GetWidth(), -(float)drag.y / _mgr->_systemGraphics.GetHeight(), 0.0f };
							}
							else
							{
								lr = { (float)drag.x * 2.0f, -(float)drag.y * 2.0f, 0.0f };
							}

							if (startPos.x < pos.x)
								lr.x = -lr.x;
							if (startPos.y > pos.y)
								lr.y = -lr.y;
							if (startScale.x < 0)
								lr.x = -lr.x;
							if (startScale.y < 0)
								lr.y = -lr.y;
							trn._scale += lr;
						}

						break;
					}
					case SCALECOLLISION:
					{
						_mgr->_systemGraphics.DrawDebugLine(fMin, fMax, FwMath::Vector3D(0, 0, 1), FwMath::Vector3D(0, 0, 1));
						_mgr->_systemGraphics.DrawDebugLine({ fMin.x, fMax.y, 0.0f }, { fMax.x, fMin.y,0.0f }, FwMath::Vector3D(0, 0, 1), FwMath::Vector3D(0, 0, 1));


						if (mousePos.x > fMin.x - drag.x && mousePos.x < fMax.x - drag.x)
							if (mousePos.y > fMin.y + drag.y && mousePos.y < fMax.y + drag.y)
								if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::PRESS) && !_mouseCaptured)
								{
									_mouseUsed = true;
									startPos = { mousePos.x, mousePos.y, 0 };
									startScale = trn._scale;
								}

						if (_mouseUsed)
						{
							Vector3D lr;

							lr = { (float)drag.x * 0.2f, -(float)drag.y * 0.2f, 0.0f };

							if (startPos.x < pos.x)
								lr.x = -lr.x;
							if (startPos.y > pos.y)
								lr.y = -lr.y;
							if (startScale.x < 0)
								lr.x = -lr.x;
							if (startScale.y < 0)
								lr.y = -lr.y;
							col._vertexA -= lr / 200;
							col._vertexB += lr / 200;
							col._vertexC = { col._vertexB.x,col._vertexA.y,0 };
							col._vertexD = { col._vertexA.x,col._vertexB.y,0 };
						}

						break;

					}
					case ROTATE:
					{
						float circleSize = 30.0f;

						float s = sin(-trn._rotation * 3.1415f / 180.0f); float c = cos(-trn._rotation * 3.1415f / 180.0f);
						float xNew = -scale.y * 0.5f * s; float yNew = scale.y * 0.5f * c;
						Vector3D mousePos3D = { mousePos.x, mousePos.y, 0 };

						_mgr->_systemGraphics.DrawDebugCircle(pos + Vector3D{ xNew, yNew, 0.0f }, circleSize, FwMath::Vector3D(ROTATE_COLOUR));
						_mgr->_systemGraphics.DrawDebugLine(pos, pos + Vector3D{ xNew, yNew, 0.0f }, FwMath::Vector3D(ROTATE_COLOUR), FwMath::Vector3D(ROTATE_COLOUR));

						if (FwMath::Vector3DLength(
							mousePos3D - (pos + Vector3D{ xNew, yNew, 0.0f }))
							< circleSize)
							if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::PRESS) && !_mouseCaptured)
							{
								_undoTrans.emplace_back(_sObj, "rotate", Vector3D{ trn._rotation, 0, 0 });
								_mouseUsed = true;
							}

						if (_mouseUsed)
						{
							Vector3D lineUp = { 0.0f, 1.0f, 0.0f };
							Vector3D toMouse = mousePos3D - pos;

							float dotProd = FwMath::Vector3DDotProduct(lineUp, toMouse);
							float mag = FwMath::Vector3DLength(toMouse);

							trn._rotation = acosf(dotProd / mag) * 180.0f / 3.1415f;
							if (mousePos3D.x < pos.x)
								trn._rotation = -trn._rotation;
						}

						break;
					}
					default:
						break;
					}
				}

				// Tag Popup
				{
					ImGui::Text("Tag: "); ImGui::SameLine(); ImGui::Text(trn._tag.c_str());

					if (ImGui::Button("Change Tag"))
						ImGui::OpenPopup("ChangeTag##Trn");
					ImGui::SameLine();
					if (ImGui::Button("Add Tag"))
						ImGui::OpenPopup("AddTag##Trn");
					ImGui::SameLine();
					if (ImGui::Button("Remove Tag"))
						ImGui::OpenPopup("RemoveTag##Trn");

					if (ImGui::BeginPopup("ChangeTag##Trn"))
					{
						for (std::string i : _tagList)
						{
							if (i != trn._tag)
								if (ImGui::Selectable(i.c_str()))
								{
									trn._tag = i;
									ImGui::CloseCurrentPopup();
								}
						}
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("AddTag##Trn"))
					{
						ImGui::InputText("##TagName", &tagName);
						if (ImGui::Button("Add##TagName"))
						{
							_tagList.emplace(tagName);
							tagName.clear();
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("RemoveTag##Trn"))
					{
						decltype(_tagList)::iterator i;
						for (i = ++std::begin(_tagList); i != std::end(_tagList); ++i)
						{
							if (ImGui::Selectable(i->c_str()))
							{
								ImGui::CloseCurrentPopup();
								break;
							}
						}
						if (i != std::end(_tagList))
							_tagList.erase(i);
						ImGui::EndPopup();
					}
				}
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode(std::string("Transform##").append(std::to_string(_sObj)).c_str()))
				{

					if (ren.isEnabled() && ren._ui)
					{
						ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.4f);

						ImGui::Text("\tPosition");
						ImGui::DragScalar("##posX", ImGuiDataType_Float, &trn._currentPosition.x, 0.01f);
						ImGui::SameLine();
						ImGui::DragScalar("##posY", ImGuiDataType_Float, &trn._currentPosition.y, 0.01f);

						ImGui::Text("\tRotation");
						ImGui::DragScalar("##Rotation", ImGuiDataType_Float, &trn._rotation, 0.1f);

						ImGui::Text("\tScale");
						ImGui::DragScalar("##scaleX", ImGuiDataType_Float, &trn._scale.x, 0.01f);
						ImGui::SameLine();
						ImGui::DragScalar("##scaleY", ImGuiDataType_Float, &trn._scale.y, 0.01f);

						ImGui::PopItemWidth();
					}
					else
					{
						ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.4f);

						ImGui::Text("\tPosition");
						ImGui::DragScalar("##posX", ImGuiDataType_Float, &trn._currentPosition.x, 1.0f);
						ImGui::SameLine();
						ImGui::DragScalar("##posY", ImGuiDataType_Float, &trn._currentPosition.y, 1.f);

						ImGui::Text("\tRotation");
						ImGui::DragScalar("##Rotation", ImGuiDataType_Float, &trn._rotation, 0.1f);

						ImGui::Text("\tScale");

						ImGui::DragScalar("##scaleX", ImGuiDataType_Float, &trn._scale.x, 0.5f);
						ImGui::SameLine();
						ImGui::DragScalar("##scaleY", ImGuiDataType_Float, &trn._scale.y, 0.5f);

						ImGui::PopItemWidth();
					}

					if (ImGui::Button("Snap to nearest Grid"))
					{
						float xDiff = fmod(trn._currentPosition.x, _mgr->_map.MAP_SCALE);
						float yDiff = fmod(trn._currentPosition.y, _mgr->_map.MAP_SCALE);
						trn._currentPosition.x = trn._currentPosition.x + ((_mgr->_map.MAP_SCALE * 0.5f < xDiff) ? _mgr->_map.MAP_SCALE - xDiff : -xDiff);
						trn._currentPosition.y = trn._currentPosition.y + ((_mgr->_map.MAP_SCALE * 0.5f < yDiff) ? _mgr->_map.MAP_SCALE - yDiff : -yDiff);
					}

					ImGui::TreePop();
				}
			}

			// Collision Component
			if (col.isEnabled())
			{
				if (ImGui::TreeNode(std::string("Collision##").append(std::to_string(_sObj)).c_str()))
				{
					ImGui::Text("\tMinimum Extent");
					float fPos[2] = { col._vertexA.x, col._vertexA.y };
					ImGui::InputFloat2("##Min1", fPos);
					col._vertexA = { fPos[0], fPos[1], 0 };


					ImGui::Text("\tMaximum Extent");
					fPos[0] = col._vertexB.x; fPos[1] = col._vertexB.y;
					ImGui::InputFloat2("##Max1", fPos);
					col._vertexB = { fPos[0], fPos[1], 0 };

					col._vertexC = { col._vertexB.x,col._vertexA.y,0 };
					col._vertexD = { col._vertexA.x,col._vertexB.y,0 };
					ImGui::TreePop();
				}
			}

			// Physics Component
			if (phy.isEnabled())
			{
				if (ImGui::TreeNode(std::string("Physics##").append(std::to_string(_sObj)).c_str()))
				{
					ImGui::Text("\tMass");
					ImGui::InputFloat("##Mass", &phy._mass);
					ImGui::Checkbox("Is Static", &phy._isStatic);

					ImGui::TreePop();
				}
			}

			// Renderer Component
			if (ren.isEnabled())
			{
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode(std::string("Renderer##").append(std::to_string(_sObj)).c_str()))
				{
					ImGui::Image((void*)(intptr_t)ren.GetTexture(), { texSize, texSize });
					ImGui::SameLine();
					if (ImGui::Button("..."))
					{
						COMDLG_FILTERSPEC rgSpec[] =
						{
							{ L"*.png", L"*.png" },
							{ L"*.jpg", L"*.jpg" },
							{ L"*.dds", L"*.dds" },
							{ L"All files (*.*)" , L"*.*"}
						};
						// Open to set texture
						//WINDOWMAIN->DisableClose();
						std::string filename = WindowsOpenFileBox(_handle, rgSpec, 4);
						if (!filename.empty())
						{
							filename = ImGUICopyFile(filename, ".\\Resources\\Sprites\\Renderer");
							//WINDOWMAIN->EnableClose();
							if (!filename.empty())
							{
								ren.SetTexture(filename);
								//GRAPHICS->AddRenderer(&ren);
							}
						}
					}
					ImGui::SameLine();
					ImGui::Text("Add new Texture");

					if (ImGui::InputInt("Layer", (int*)&ren._layer))
					{
						GRAPHICS->AddRenderer(&ren);
					}

					ImGui::Checkbox("Flip X", &ren._flipX);
					ImGui::SameLine();
					ImGui::Checkbox("Flip Y", &ren._flipY);

					if (!emitter.isEnabled())
					{
						ImGui::Checkbox("UI", &ren._ui);
						float arr[4] = { ren._colour.x, ren._colour.y, ren._colour.z, ren._colour.w };
						ImGui::ColorEdit4("RGBA", arr);
						ren._colour = { arr[0], arr[1], arr[2], arr[3] };
					}

					ImGui::TreePop();
				}
			}

			// Animation Component
			if (anim.isEnabled())
			{
				if (ImGui::TreeNode("Animation"))
				{
					//anim._animationFileName string to spritesheet name
					//ImGui::Image((void*)(intptr_t)RESOURCE->GetTexture(), 
						//{ ImGui::GetWindowContentRegionWidth() * 0.4f, ImGui::GetWindowContentRegionWidth() * 0.4f });
					/*ImGui::Image((void*)(intptr_t)anim.GetSpritesheetID(), { ImGui::GetWindowContentRegionWidth() * 0.4f, ImGui::GetWindowContentRegionWidth() * 0.4f });

					ImGui::Text(anim._animationFileName.c_str());
					if (ImGui::Button("..."))
					{
						COMDLG_FILTERSPEC rgSpec[] =
						{
							{ L"*.png", L"*.png" },
							{ L"*.jpg", L"*.jpg" },
							{ L"*.dds", L"*.dds" },
							{ L"All files (*.*)" , L"*.*"}
						};
						// Open to set texture
						//WINDOWMAIN->DisableClose();
						std::string filename = WindowsOpenFileBox(_handle, rgSpec, 4);
						if (!filename.empty())
						{
							filename = ImGUICopyFile(filename, ".\\Resources\\Sprites\\Animation");
							//WINDOWMAIN->EnableClose();
							if (!filename.empty())
							{
								anim._animationFileName = filename;
								anim.SetSpritesheet(filename);
							}
						}
					}
					*/
					//anim._maxRow number of animations so that vertical cutting is correct;
					ImGui::InputInt("Row", &anim._maxRow);

					if (anim._maxRow != anim._animationNames.size())
					{
						anim.CalculateOffsets();
						if (anim._animationNames.size() < anim._maxRow)
						{
							int toAdd = anim._maxRow - (int)anim._animationNames.size();
							for (int i = 0; i < toAdd; ++i)
							{
								anim._animationNames.emplace_back();
								anim._animationsFrames.emplace_back();
								anim._frameTimes.emplace_back();
							}
						}
						else
						{
							int toRemove = (int)anim._animationNames.size() - anim._maxRow;
							for (int i = 0; i < toRemove; ++i)
							{
								anim._animationNames.pop_back();
								anim._animationsFrames.pop_back();
								anim._frameTimes.pop_back();
							}
						}
					}

					//anim._maxColumn number of animation frame so that horizontal cutting is correct
					if (ImGui::InputInt("Column", &anim._maxColumn))
					{
						anim.CalculateOffsets();
					}

					//Here loop
					if (ImGui::TreeNode("Animation Names"))
					{
						int nameI = 0;
						for (std::string& name : anim._animationNames)
						{
							if (ImGui::CollapsingHeader(std::string("Animation: ").append(std::to_string(nameI)).c_str()))
							{
								ImGui::InputText(
									std::string("##AnimName").append(std::to_string(nameI)).c_str(),
									&name);
							}
							++nameI;
						}

						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Animation Frames"))
					{
						int nameI = 0;
						//anim._animationsFrames action has how many frames
						for (int& animFrame : anim._animationsFrames)
						{
							if (ImGui::CollapsingHeader(std::string("").append(anim._animationNames[nameI].c_str()).append("##AnimF").append(std::to_string(nameI)).c_str()))
							{
								//ImGui::Text(animFrame.first.c_str());
								ImGui::InputInt(std::string("##NumFrames").append(std::to_string(nameI)).c_str(), &animFrame);
							}
							++nameI;
						}
						nameI = 0;
						ImGui::TreePop();
					}
					//anim._frameTime action frame delay
					if (ImGui::TreeNode("Animation Frame Times"))
					{
						int nameI = 0;
						//anim._animationsFrames action has how many frames
						for (float& animTime : anim._frameTimes)
						{
							if (ImGui::CollapsingHeader(std::string("").append(anim._animationNames[nameI].c_str()).append("##AnimT").append(std::to_string(nameI)).c_str()))
							{
								//ImGui::Text(animFrame.first.c_str());
								ImGui::InputFloat(std::string("##FrameTime").append(std::to_string(nameI)).c_str(), &animTime);
							}
							++nameI;
						}
						nameI = 0;
						ImGui::TreePop();
					}
					//anim._currentAnimationName current animation name that is playing AKA starting animation
					if (ImGui::CollapsingHeader(std::string("Current Animation: ").append(anim._currentAnimationName).c_str()))
					{
						int nameI = 0;
						for (std::string& name : anim._animationNames)
						{
							bool curFrame = name._Equal(anim._currentAnimationName);
							if (ImGui::Selectable(std::string("").append(name.c_str()).append("##currentAnim").append(std::to_string(nameI)).c_str(), &curFrame))
								anim._currentAnimationName = name;
							++nameI;
						}
						nameI = 0;
					}

					//anim._defaultAnimationName default animation name that is playing AKA starting animation
					if (ImGui::CollapsingHeader(std::string("Default Animation: ").append(anim._defaultAnimationName).c_str()))
					{
						int nameI = 0;
						for (std::string& name : anim._animationNames)
						{
							bool defaultFrame = name._Equal(anim._defaultAnimationName);
							if (ImGui::Selectable(std::string("").append(name.c_str()).append("##defaultAnim").append(std::to_string(nameI)).c_str(), &defaultFrame))
							{
								anim._defaultAnimationName = name;
								anim.PlayAnimation();
							}
							++nameI;
						}
						nameI = 0;
					}

					//anim._play is animation currently playing?
					if (ImGui::Button("Preview Animation"))
					{
						std::string name = anim._currentAnimationName;
						anim._currentAnimationName = "";
						anim.PlayAnimation(name, anim._loop, anim._reverse);
					}

					//anim._play is animation currently playing?
					if (ImGui::Button("Stop Animation"))
					{
						anim.StopAnimation();
					}
					//anim._loop loop the animation or not
					ImGui::Checkbox("Loop Anim", &anim._loop);
					ImGui::Checkbox("Reverse Anim", &anim._reverse);
					//anim._currentFrame current animation frame playing might be removed later
					ImGui::TreePop();
				}
			}

			if (emitter.isEnabled())
			{
				if (ImGui::TreeNode("Emitter"))
				{
					const char* emitterType[TOTAL_SHAPE] = { "Sprite", "Circle", "Cone", "Directional" };
					const char* currentEmitter = (emitter._type >= 0 && emitter._type < TOTAL_SHAPE) ? emitterType[emitter._type] : "";
					ImGui::SliderInt("Type", (int*)&emitter._type, 0, TOTAL_SHAPE - 1, currentEmitter);

					ImGui::InputFloat("Duration Time", &emitter._durationPerCycle);
					ImGui::InputFloat("Emission Rate", &emitter._emissionRate);
					if (ImGui::InputInt("Max Particles", &emitter._maxParticles))
					{
						emitter.UpdateSize();
					}

					if (emitter._type == CIRCLE || emitter._type == CONE)
					{
						ImGui::InputFloat("Angle", &emitter._spawnAngle);
						ImGui::InputFloat("Radius", &emitter._radius);
					}
					if (ImGui::TreeNode("Particle Size"))
					{
						ImGui::Checkbox("Random Size", &emitter._randomizeSize);
						if (emitter._randomizeSize)
						{
							ImGui::InputFloat("Minimum Size", &emitter._minParticleSize);
							ImGui::InputFloat("Maximum Size", &emitter._maxParticleSize);
						}
						else
						{
							ImGui::InputFloat("Size", &emitter._maxParticleSize);
						}
						ImGui::TreePop();
					}


					if (ImGui::TreeNode("Particle Speed"))
					{
						ImGui::Checkbox("Random Speed", &emitter._randomizeSpeed);
						if (emitter._randomizeSpeed)
						{
							ImGui::InputFloat("Minimum Speed", &emitter._minParticleSpeed);
							ImGui::InputFloat("Maximum Speed", &emitter._maxParticleSpeed);
						}
						else
						{
							ImGui::InputFloat("Speed", &emitter._maxParticleSpeed);
						}
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Particle Lifespan"))
					{
						ImGui::Checkbox("Random LifeSpan", &emitter._randomizeLifespan);
						if (emitter._randomizeLifespan)
						{
							ImGui::InputFloat("Minimum LifeSpan", &emitter._minLifespan);
							ImGui::InputFloat("Maximum LifeSpan", &emitter._maxLifespan);
						}
						else
						{
							ImGui::InputFloat("LifeSpan", &emitter._maxLifespan);
						}
						ImGui::TreePop();
					}

					if (!emitter._colourOverTime)
					{
						if (ImGui::TreeNode("Particle Colour"))
						{
							ImGui::Checkbox("Random Colour", &emitter._randomizeColour);
							if (emitter._randomizeColour)
							{
								float colourA[4] = { emitter._colourA.x, emitter._colourA.y, emitter._colourA.z, emitter._colourA.w };
								ImGui::ColorEdit4("Colour Range Low", colourA);
								emitter._colourA = { colourA[0], colourA[1], colourA[2], colourA[3] };

								float colourB[4] = { emitter._colourB.x, emitter._colourB.y, emitter._colourB.z, emitter._colourB.w };
								ImGui::ColorEdit4("Colour Range High", colourB);
								emitter._colourB = { colourB[0], colourB[1], colourB[2], colourB[3] };
							}
							else
							{
								float colour[4] = { emitter._colourB.x, emitter._colourB.y, emitter._colourB.z, emitter._colourB.w };
								ImGui::ColorEdit4("Colour", colour);
								emitter._colourB = { colour[0], colour[1], colour[2], colour[3] };
							}
							ImGui::TreePop();
						}
					}

					ImGui::Checkbox("PreWarm", &emitter._preWarm);
					ImGui::Checkbox("Loop", &emitter._loop);
					ImGui::Checkbox("Burst", &emitter._burst);
					ImGui::Checkbox("Play", &emitter._play);

					if (emitter._type == CONE)
					{
						ImGui::Checkbox("Reverse", &emitter._reverse);
					}

					if (ImGui::Checkbox("Follow", &emitter._follow))
					{
						if (emitter._play)
						{
							emitter.Stop();
							emitter.Play();
						}
					}
					ImGui::Checkbox("Fade", &emitter._fade);

					if (ImGui::TreeNode("Special Behaviour"))
					{
						ImGui::Checkbox("Velocity over time", &emitter._velocityOverTime);
						if (emitter._velocityOverTime)
						{
							ImGui::InputFloat("X", &emitter._velocity.x);
							ImGui::InputFloat("Y", &emitter._velocity.y);
						}

						ImGui::Checkbox("Size over time", &emitter._sizeOverTime);
						ImGui::Checkbox("Speed over time", &emitter._speedOverTime);
						ImGui::Checkbox("Colour over time", &emitter._colourOverTime);
						if (emitter._colourOverTime)
						{
							float colourStart[4] = { emitter._colourStart.x, emitter._colourStart.y, emitter._colourStart.z, emitter._colourStart.w };
							ImGui::ColorEdit4("Colour Start", colourStart);
							emitter._colourStart = { colourStart[0], colourStart[1], colourStart[2], colourStart[3] };

							float colourEnd[4] = { emitter._colourEnd.x, emitter._colourEnd.y, emitter._colourEnd.z, emitter._colourEnd.w };
							ImGui::ColorEdit4("Colour End", colourEnd);
							emitter._colourEnd = { colourEnd[0], colourEnd[1], colourEnd[2], colourEnd[3] };
						}

						ImGui::TreePop();
					}
					//Debug
					//ImGui::Text(std::to_string(emitter.GetTimePassed()).c_str());
					//ImGui::Text(std::to_string(emitter.GetAlive()).c_str());

					if (ImGui::Button("Preview Particle"))
					{
						emitter.Play();
					}

					if (ImGui::Button("Stop Animation"))
					{
						emitter.Stop();
					}

					ImGui::TreePop();
				}
			}
			// Text Component
			if (text.isEnabled())
			{
				if (ImGui::TreeNode("Text"))
				{
					ImGui::Text(text._fontType.c_str());
					if (ImGui::Button("...##Font"))
					{
						COMDLG_FILTERSPEC rgSpec[] =
						{
							{ L"*.ttf", L"*.ttf" }
						};
						// Open to set texture
						//WINDOWMAIN->DisableClose();
						std::string filename = WindowsOpenFileBox(_handle, rgSpec, 1);
						//WINDOWMAIN->EnableClose();

						if (!filename.empty())
						{
							filename = ImGUICopyFile(filename, ".\\Resources\\Fonts");
							text._fontType = filename;
							text.SetFont(text._fontType, text._pixel);
						}
					}
					ImGui::SameLine();
					ImGui::Text("Select Font");

					int* pixSize = (int*)&text._pixel;
					int prev = *pixSize;
					ImGui::InputInt("Pixel Size", pixSize);
					if (prev != *pixSize)
						text.SetFont(text._fontType, text._pixel);

					ImGui::InputTextMultiline("##Text", &text._displayText);
					float color[4] = { text._colour.x, text._colour.y, text._colour.z, text._colour.w };
					ImGui::ColorEdit4("Text Color", color);
					text._colour = { color[0], color[1], color[2], color[3] };
					ImGui::TreePop();
				}
			}

			// Audio Component
			if (audio.isEnabled())
			{
				if (ImGui::TreeNode("Audio"))
				{
					float min = 0.0f, max = 1.0f;
					if (ImGui::SliderScalar("Volume", ImGuiDataType_Float, &audio._volume, &min, &max))
					{
						audio.SetVolume();
					}

					if (audio._3dSound)
					{
						if (ImGui::InputFloat("Min##min", &audio._min))
						{
							audio.SetAudioMinMax();
						}
						if(ImGui::InputFloat("Max##max", &audio._max))
						{
							audio.SetAudioMinMax();
						}
					}

					if (ImGui::Button("Add Audio"))
					{
						audio.AddNewAudio();
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete Audio"))
						ImGui::OpenPopup("DeleteAudioPopup");

					if (ImGui::BeginPopup("DeleteAudioPopup"))
					{
						size_t end = audio._audioNames.size();
						for (size_t i = 0; i < end; ++i)
						{
							if (ImGui::Selectable(audio._audioNames[i].c_str()))
							{
								audio.DeleteAudio(i);

								ImGui::CloseCurrentPopup();
								break;
							}
						}
						ImGui::EndPopup();
					}

					if (ImGui::TreeNode("Audio File Names"))
					{
						size_t end = audio._audioFileNames.size();
						for (size_t i = 0; i != end; ++i)
						{
							if (ImGui::CollapsingHeader(audio._audioNames[i].c_str()))
							{
								if (ImGui::Button((std::string("...##audiofile").append(std::to_string(i)).c_str())))
								{
									COMDLG_FILTERSPEC rgSpec[] =
									{
										{ L"*.ogg", L"*.ogg" },
										{ L"*.wav", L"*.wav" }
									};
									// Open to set texture
									//WINDOWMAIN->DisableClose();
									std::string filename = WindowsOpenFileBox(_handle, rgSpec, 2);

									//WINDOWMAIN->EnableClose();

									if (!filename.empty())
									{
										filename = ImGUICopyFile(filename, ".\\Resources\\Audio");
										audio._audioFileNames[i] = filename;
										audio.SetSound(filename);
									}
								}
								ImGui::SameLine();
								ImGui::Text(audio._audioFileNames[i].c_str());
							}
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Audio Names"))
					{
						size_t end = audio._audioNames.size();
						for (size_t i = 0; i != end; ++i)
						{
							if (ImGui::CollapsingHeader(std::string("Audio Names: ").append(std::to_string(i)).c_str()))
							{
								ImGui::InputText(
									std::string("##AudioName").append(std::to_string(i)).c_str(),
									&audio._audioNames[i]);
							}
						}

						ImGui::TreePop();
					}

					if (ImGui::CollapsingHeader(std::string("Current Audio: ").append(audio._currentAudio).c_str()))
					{
						size_t end = audio._audioFileNames.size();
						for (size_t i = 0; i != end; ++i)
						{
							bool currAudio = audio._audioNames[i]._Equal(audio._currentAudio);
							if (ImGui::Selectable(std::string("").append(audio._audioNames[i].c_str()).append("##currentAudio").append(std::to_string(i)).c_str(), &currAudio))
								audio._currentAudio = audio._audioNames[i];
						}
					}

					if (ImGui::CollapsingHeader(std::string("Default Audio: ").append(audio._defaultAudio).c_str()))
					{
						size_t end = audio._audioNames.size();
						for (size_t i = 0; i != end; ++i)
						{
							bool defaultAudio = audio._audioNames[i]._Equal(audio._defaultAudio);
							if (ImGui::Selectable(std::string("").append(audio._audioNames[i].c_str()).append("##defaultAudio").append(std::to_string(i)).c_str(), &defaultAudio))
							{
								audio._defaultAudio = audio._audioNames[i];
								//audio.Play();
							}
						}
					}


					if (ImGui::Button("Preview Sound"))
					{
						audio.Play(audio._currentAudio, audio._loop, true);
					}
					ImGui::SameLine();
					if (ImGui::Button("Stop Sound"))
					{
						audio.StopSound();
					}
					ImGui::Checkbox("Loop Audio", &audio._loop);
					ImGui::Checkbox("Mute Audio", &audio._mute);
					if (ImGui::Checkbox("3D Mode", &audio._3dSound))
					{
						audio.Toggle3D();
					}
					if (audio._mute)
					{
						audio.MuteSound();
					}
					else
					{
						audio.UnmuteSound();
					}

					ImGui::TreePop();
				}
			}

			// Script Component
			if (script.isEnabled())
			{
				if (ImGui::TreeNode("Script"))
				{
					ScriptGongChang listOf(script);

					ImGui::Separator();

					for (IScript* s : script.GetListOfScripts())
					{
						ImGui::Text(s->_name.c_str());
						
						for (auto fVar : s->_floatVars)
							ImGui::InputFloat(fVar.first.c_str(), fVar.second);
						for (auto iVar : s->_intVars)
							ImGui::InputInt(iVar.first.c_str(), iVar.second);
						for (auto sVar : s->_stringVars)
						{
							if (std::string(sVar.first.c_str()).find("Text") != std::string::npos)
							{
								ImGui::Text((*sVar.second).c_str());
								if (ImGui::Button("Add Text Input##dialogue"))
								{
									COMDLG_FILTERSPEC rgSpec[] =
									{
										{ L"*.txt", L"*.txt" }
									};
									std::string filename = WindowsOpenFileBox(_handle, rgSpec, 1);
									if (!filename.empty())
									{
										filename = ImGUICopyFile(filename, ".\\Resources\\Text");

										if (!filename.empty())
										{
											*sVar.second = filename;
										}
									}
								}
							}
							else
							{
								ImGui::InputText(sVar.first.c_str(), sVar.second);
							}
						}
						for (auto bVar : s->_boolVars)
							ImGui::Checkbox(bVar.first.c_str(), bVar.second);

						ImGui::Separator();
					}

					if (ImGui::Button("Add Script"))
						ImGui::OpenPopup("AddScriptPop");

					if (ImGui::BeginPopup("AddScriptPop"))
					{
						for (std::pair<int, std::string> i : listOf.scriptType)
						{
							bool exists = false;
							for (std::pair<std::string, std::string> j : script.GetParams().second)
							{
								if (j.second == i.second)
								{
									exists = true;
									break;
								}
							}
							if (exists)
								continue;
							if (ImGui::Selectable(i.second.c_str()))
							{
								script.AttachScript(i.second);
								ImGui::CloseCurrentPopup();
								break;
							}
						}
						ImGui::EndPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("Delete Script"))
						ImGui::OpenPopup("DeleteScriptPop");

					if (ImGui::BeginPopup("DeleteScriptPop"))
					{
						for (IScript* i : script.GetListOfScripts())
						{
							if (ImGui::Selectable(i->_name.c_str()))
							{
								script.DeleteScript(i->_name);
								ImGui::CloseCurrentPopup();
								break;
							}
						}
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
			}
		}
		ImGui::EndChild();
	}
}