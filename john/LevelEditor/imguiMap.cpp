/******************************************************************************/
/*!
\file		imguiMap.cpp
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
#include "Map.h"
#include "LESysMgr.h"
#include "WindowsDialogBox.h"

namespace FwEngine
{
	namespace
	{
		static const int tileSize = 32;

		static int newMapWidth;
		static int newMapHeight;
		static float newMapTileSize;

		static bool newMapClear = false;

		static int curBrushIndex = -1;
		static bool curBrushCol = 0;
	}

	void ImguiMgr::ImGUIMap()
	{
		//ImGui::SetNextWindowSize({ _clientRect.right * 0.2f, _clientRect.bottom * 0.4f }, ImGuiCond_Always);
		//ImGui::SetNextWindowPos({0, _clientRect.bottom * 0.6f });

		ImGui::Begin("Map", nullptr);// , (bool*)0,
		//	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		//	| ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::IsWindowCollapsed())
		{
			ImGui::End();
			return;
		}

		Map& map = _mgr->_map;

		int& height = map._height;
		int& width = map._width;

		//========================================= Map creation =========================================
		if (ImGui::Button("New Map"))
		{
			newMapWidth = width;
			newMapHeight = height;
			newMapTileSize = map.MAP_SCALE;
			ImGui::OpenPopup("New Map?");
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Texture##TILE"))
		{
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"*.png", L"*.png" },
				{ L"*.jpg", L"*.jpg" },
			};
			// Open to set texture
			//WINDOWMAIN->DisableClose();
			std::string filename = WindowsOpenFileBox(_handle, rgSpec, 2);
			//WINDOWMAIN->EnableClose();
			if (!filename.empty())
			{
				// Copy file
				try
				{
					filename = ImGUICopyFile(filename, ".\\Resources\\Sprites\\Map");
					GLuint texture = RESOURCE->GetTexture(filename);
					if (std::find(std::begin(map._tileTex), std::end(map._tileTex), texture) == std::end(map._tileTex))
					{
						map._tileTex.push_back(RESOURCE->GetTexture(filename));
					}
				}
				catch (...)
				{
					ImGui::End();
					return;
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Texture##TILE"))
			ImGui::OpenPopup("DeleteTileTexture?");

		if (ImGui::BeginPopupModal("DeleteTileTexture?", NULL))
		{
			ImGui::Text("Are you certain? The texture will be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			// Display textures to delete
			for (size_t i = 0; i < map._tileTex.size(); ++i)
			{
				if (ImGui::ImageButton((void*)(intptr_t)map._tileTex[i], { tileSize, tileSize }))
				{
					// Delete usable texture
					map._tileTex.erase(std::begin(map._tileTex) + i);
					// Resets the map
					for (int& j : map._mapArray)
					{
						if (j == i + 10)
							j = 0;
						else if (j > i + 10)
							--j;
					}
					ImGui::CloseCurrentPopup();
					break;
				}
				if (i%5 || !i)
					ImGui::SameLine();
			}
			ImGui::Separator();
			ImGui::SetItemDefaultFocus();
			//ImGui::SameLine();
			if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		// New Map
		if (ImGui::BeginPopupModal("New Map?", NULL))
		{
			if (newMapWidth < 0)
				newMapWidth = 0;
			if (newMapHeight < 0)
				newMapHeight = 0;
			if (newMapTileSize < 0)
				newMapTileSize = 0;
			ImGui::InputInt("New Width", &newMapWidth);
			ImGui::InputInt("New Height", &newMapHeight);
			ImGui::InputFloat("New Tile Size", &newMapTileSize);
			ImGui::Checkbox("Clear Map", &newMapClear);
			ImGui::Text("Are you certain? The current map may be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK"))
			{
				// Build Map
				std::vector<int> oldArr = map._mapArray;
				std::vector<int> oldArrCol = map._mapArrayCol;
				int oldH = map._height;
				int oldW = map._width;
				map._mapArray.clear();
				map._mapArray.resize((size_t)newMapHeight * newMapWidth);

				map._mapArrayCol.clear();
				map._mapArrayCol.resize((size_t)newMapHeight* newMapWidth);

				map._height = newMapHeight;
				map._width = newMapWidth;

				int checkH = (oldH < newMapHeight) ? oldH : newMapHeight;
				int checkW = (oldW < newMapWidth) ? oldW : newMapWidth;

				// Clear Data if needed
				if (!newMapClear)
				{
					for (int h = 0; h < checkH; ++h)
						for (int w = 0; w < checkW; ++w)
						{
							map._mapArray[(size_t)h * newMapWidth + w] = oldArr[(size_t)h * oldW + w];
							map._mapArrayCol[(size_t)h * newMapWidth + w] = oldArrCol[(size_t)h * oldW + w];
						}
				}

				map.MAP_SCALE = newMapTileSize;
				map.MAP_SCALE_HF = map.MAP_SCALE * 0.5f;
				map.MAP_SCALE_INVERSE = 1.0f / newMapTileSize;
				map.MAP_SCALE_P = newMapTileSize / 10000.0f;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}


		//========================================= Map Editor =========================================

		ImGui::BeginChild("##MapChildWin",
			ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 85.0f), true, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::Separator();
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

		if (map._mapArray.size())
		{
			// Display Map
			for (int h = height - 1; h >= 0; --h)
			{
				for (int w = 0; w < width; ++w)
				{
					ImGui::SameLine(0, 0);
					ImGui::PushID(h * width + w);

					unsigned int index = map._mapArray[(size_t)h * width + w];
					int& colVal = map._mapArrayCol[(size_t)h * width + w];
					bool pressed = false;

					// Like why even -10 like why are you doing this to me
					if (!index)
					{
						// Press to Change tile
						if (colVal)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 1.0f });
							if (ImGui::Button("", { tileSize, tileSize }))
							{
								pressed = true;
								colVal = curBrushCol;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if (ImGui::Button("", { tileSize, tileSize }))
							{
								pressed = true;
								colVal = curBrushCol;
							}
						}
					}
					else
					{
						// Press to Change tile
						if (colVal)
						{
							if (ImGui::ImageButton((void*)(intptr_t)map._tileTex[index - 10], { tileSize, tileSize }, { 0,0 }, { 1,1 }, 0))
							{
								pressed = true;
								colVal = curBrushCol;
							}
						}
						else
						{
							if (ImGui::ImageButton((void*)(intptr_t)map._tileTex[index - 10], { tileSize - 8, tileSize - 6 }))
							{
								pressed = true;
								colVal = curBrushCol;
							}
						}
					}

					// Changing occurs here
					if (pressed)
					{
						if (curBrushIndex == -1)
							map._mapArray[(size_t)h * width + w] = 0;
						else
							map._mapArray[(size_t)h * width + w] = curBrushIndex + 10;
					}

					ImGui::PopID();

				}
				ImGui::Separator();
			}
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();

		if (ImGui::Button("Brush"))
			ImGui::OpenPopup("ChangeTilePopup##2");

		if (ImGui::BeginPopup("ChangeTilePopup##2"))
		{
			if (!map._tileTex.empty())
			{
				// Change brush (stamp)
				if (curBrushIndex != -1)
					ImGui::Image((void*)(intptr_t)map._tileTex[curBrushIndex], { tileSize, tileSize }, { 0,0 }, { 1,1 });
				else
					ImGui::Image((void*)(intptr_t)0, { tileSize, tileSize }, { 0,0 }, { 1,1 });

				if (ImGui::ImageButton(0, { tileSize * 0.5f, tileSize * 0.5f }))
				{
					curBrushIndex = -1;
				}
				ImGui::SameLine();
				ImGui::Checkbox("Collidable", &curBrushCol);

				ImGui::Separator();

				int tileIndex = 0;
				for (GLuint i : map._tileTex)
				{
					if (!(tileIndex % 5))
						ImGui::Separator();
					ImGui::SameLine(0, 0);

					if (ImGui::ImageButton((void*)(intptr_t)i, { tileSize * 0.5f, tileSize * 0.5f }))
						curBrushIndex = tileIndex;

					++tileIndex;
				}
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}