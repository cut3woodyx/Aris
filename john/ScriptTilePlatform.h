#pragma once

#include "IScript.h"
#include "SystemManager.h"

constexpr auto STRING_SCRIPT_TILEPLATFORM = "tilePlatform";

namespace FwEngine
{
	class ScriptTilePlatform : public IScript
	{
		Map& _tileMap;

		int _posX;
		int _posY;
		int _width;
		int _height;

		std::string _textureName;
		int _textureID;

		std::vector<std::vector<int>> _columns;

		void GenerateTiles();

	public:
		ScriptTilePlatform(ComponentScript& comp);

		virtual ~ScriptTilePlatform();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void PushVars() override;

		void BreakLeftColumn();
		void BreakRightColumn();
		void BreakTopRow();
		void BreakBottomRow();

		void MakeLeftColumn();
		void MakeRightColumn();
		void MakeTopRow();
		void MakeBottomRow();

		int GetPositionX();
		int GetPositionY();
		int GetHeight();
		int GetWidth();
	};
}

