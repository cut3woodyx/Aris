/******************************************************************************/
/*!
\file		ScriptMagneticLight.cpp
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include "IScript.h"

constexpr auto STRING_SCRIPT_LIGHT = "light";

namespace FwEngine
{
	class ScriptMagneticLight : public IScript
	{
	protected:
		ComponentTransform& calTrans;
		ComponentRenderer& calRen;
		ComponentCollision& calCol;
		GameObjectPool& objectPool;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;
		ComponentTransform* _blastTrans;
		ComponentCollision* _blastCol;
		ComponentRenderer* _blastRen;
		ComponentTransform* _playerCol;

		std::vector<Vector3D> _initVertexes;
		float _xScale;
		float _yScale;

	public:

		ScriptMagneticLight(ComponentScript& comp);

		virtual ~ScriptMagneticLight();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

	};

}
