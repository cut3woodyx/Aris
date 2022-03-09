#pragma once

#include "IScript.h"

constexpr auto STRING_SCRIPT_PLAYER = "player";

namespace FwEngine
{
	class ScriptPlayer : public IScript
	{
	protected:
		bool abletojump;
		bool pan;
		float WalkingSpeed;
		DirectionalForce JumpForce;
		DirectionalForce WalkRight;
		DirectionalForce WalkLeft;
		//bool south;

		ComponentPhysics& colPhy;
		ComponentAnimation& colAnimation;
		ComponentCollision& colCollision;
		ComponentRenderer& colRender;
		ComponentTransform& colTransform;
		ComponentAudio& comAudio;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;
		GameObjectPool& objectPool;

		bool& isBlue;
		int& _health;

		FwMath::Vector3D originalPosition;
		class PlayerCameraPositionManager
		{
			FwMath::Vector3D& _cameraPosition;
			FwMath::Vector3D& _playercurrentPosition;
			float& _zoom;

			FwMath::Vector3D _verticeA;
			FwMath::Vector3D _verticeB;

			bool _isAction;
			FwMath::Vector3D _destinationPos;
			float _destionationZoom;
			float _timer;
		public:
			float _MaxDistance;
			float _MinDistance;
			float _MaxVelocity;
			float _MinVelocity;

			float _defaultZoom;

			void SetBounds(FwMath::Vector3D _verticeA, FwMath::Vector3D _verticeB);

			PlayerCameraPositionManager(FwMath::Vector3D& _cameraPosition, FwMath::Vector3D& _playercurrentPosition, float& zoom);
			~PlayerCameraPositionManager() = default;
			
			void ActionCamTo(FwMath::Vector3D position, float zoom = 1.0f, float time = 0.0f);

			void Update(float dt);
		};
		PlayerCameraPositionManager camMgr;

	public:
		ScriptPlayer(ComponentScript& comp);

		virtual ~ScriptPlayer();

		virtual void Init() override;
		
		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		void PlayerGetDamaged(int health);

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
	};
}
