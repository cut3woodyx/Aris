/******************************************************************************/
/*!
\file		ScriptPlayer.h
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

#include "IScript.h"
#include <time.h>
#include <functional>
constexpr auto STRING_SCRIPT_PLAYER = "player";


namespace FwEngine
{
	class SystemScript;

	enum PLAYER_STATE
	{
		IDLE,
		WALK,
		MIDAIR,
		LANDING,
		INTERACTING,
		ATTRACT_START,
		ATTRACTING,
		ATTRACT_END,
		ATTRACT_MOVE,
		ATTRACT_UP,
		REPEL_START,
		REPELING,
		REPEL_END,
		REPEL_MOVE,
		ABILITY_END,
		HANGING_LEFT_LOOKRIGHT,
		HANGING_LEFT_LOOKUP,
		HANGING_LEFT_LOOKLEFT,
		HANGING_RIGHT_LOOKLEFT,
		HANGING_RIGHT_LOOKUP,
		HANGING_RIGHT_LOOKRIGHT,
		LOOKUPSTART,
		LOOKINGUP,
		LOOKUPEND,
		CEILING_CLIMB,
		DEATH
	};

	class ScriptPlayer : public IScript
	{
	protected:
		//**************************************Components*****************************************// 
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
		SystemScript& sysScr;


		//**************************************Movement Variables*****************************************// 
		bool abletojump;
		float WalkingSpeed;
		DirectionalForce JumpForce;
		DirectionalForce WalkRight;
		DirectionalForce WalkLeft;
		DirectionalForce WallPushForceRight;
		DirectionalForce WallPushForceLeft;
		DirectionalForce WallJumpForceRight;
		DirectionalForce WallJumpForceLeft;
		DirectionalForce StickWallForceRight;
		DirectionalForce StickWallForceLeft;

		//**************************************Script Variables*****************************************// 
		ComponentTransform* _leftBackground;
		ComponentTransform* _rightBackground;
		bool& isBlue;
		bool facing;
		bool up;
		int _fullHealth;
		bool _deathAudio;
		unsigned int _playerState;
		FwMath::Vector3D originalPosition;
		
		//used in ScriptCollectables.h
		int& _keys;
		Timer _movementTimer;
		Timer _sceneChangeTimer;
		bool _onEnterSceneChange;
		Timer _damageTimer;
		bool _isTimerWhite;
		Timer _timerColor;

		GameObject* _eventCountDownObj;
		ComponentText* _eventCountDown;
		ComponentTransform* _eventCountDownT;

		ComponentEmitter* _playerLandingParticle;
		ComponentEmitter* _playerJumpParticle;
		ComponentEmitter* _playerJumpNParticle;
		ComponentEmitter* _blastRed;
		ComponentEmitter* _blastBlue;
		ComponentEmitter* _blastSub;
		ComponentEmitter* _blastSubB;
		ComponentEmitter* _changeEffect;

		ScriptMessage_Player* playerMsg;

		ComponentAnimation* _keyUI;


		//**************************************Camera Variables*****************************************// 
		class PlayerCameraPositionManager
		{
		public:
			class CameraShake
			{
			public:
				enum CAMSHAKEMODE
				{
					CSH_RANDOM,
					CSH_SIDEWAYSX
				};
			private:
				// general vars
				CAMSHAKEMODE _csmode;
				Timer _cshakeDuration;
				Timer _cshakePeriod;
				bool _isActive;
				bool _isActivate;
				int _magnitude;
				//mode vars
				bool _direction;
			public:
				FwMath::Vector3D _shakeVector;

				CameraShake();
				void CameraShakeUpdate();
				void Start(int magnitude, int period, int duration, CAMSHAKEMODE csmode = CSH_RANDOM);
				void Stop();
			};
		private:

			FwMath::Vector3D& _cameraPosition;
			float& _zoom;


			FwMath::Vector3D _verticeA;
			FwMath::Vector3D _verticeB;

			bool _isAction;
			FwMath::Vector3D _destinationPos;
			float _destionationZoom;
			float _timer;
		public:
			FwMath::Vector3D& _playercurrentPosition;
			float _MaxDistance;
			float _MinDistance;
			float _MaxVelocity;
			float _MinVelocity;

			float _VelocityScaleX;
			float _VelocityScaleY;
			float _VelocityScaleZ;

			float _defaultZoom;

			CameraShake _cameraShake;

			void SetBounds(FwMath::Vector3D _verticeA, FwMath::Vector3D _verticeB);
			PlayerCameraPositionManager(FwMath::Vector3D& _cameraPosition, FwMath::Vector3D& _playercurrentPosition, float& zoom);
			~PlayerCameraPositionManager() = default;

			void ActionCamTo(FwMath::Vector3D position, float zoom = 1.0f, float time = 0.0f, float velocityScale = 1.0f);

			void Update(float dt);
		};
		PlayerCameraPositionManager camMgr;
		Timer _screenShakeEventTimer;
		Timer _screenShakeEventTimer_short;

	public:
		int& _health;
		
		
		ScriptPlayer(ComponentScript& comp);

		virtual ~ScriptPlayer();

		virtual void Init() override;
		
		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void Unload() override;

		void PlayerGetDamaged(int health);
		int* GetKey();

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void OnEnterScn() override;
	};
}
