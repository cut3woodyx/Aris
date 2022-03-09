/******************************************************************************/
/*!
\file		ScriptPlayer.cpp
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
#include "ScriptPlayer.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include "Reaction.h"
#include <math.h>
#include "SystemManager.h"
#include "Math_CollisionSAT.h"
#include "SystemScript.h"
#include "ScriptWinZone.h"


constexpr int SCENETIMER = 500; // timer after enter portal
constexpr int MOVEMENTLAG = 60;
constexpr int DAMAGETIMER = 8 * CLOCKS_PER_SEC;
constexpr float CAMERA_DEFAULT_ZOOM = 2.0f;

constexpr float CAMSHAKEEVENT_FREQ = 3000.0f;
constexpr float CAMSHAKEEVENT_FREQ_S = 1000.0f;

constexpr int TIMERCOLORFREQ = 100;


#define WALKLEFT IKEY_A
#define WALKRIGHT IKEY_D
#define LOOKUP IKEY_W
#define INTERACT IKEY_W
#define JUMP IKEY_SPACE
#define WALLJUMP IKEY_SPACE
#define ATTRACT IMOUSE_LBUTTON
#define REPEL IMOUSE_RBUTTON


//SHORT CUT KEYS
#define FULLSCREEN IKEY_1
#define CAMERASHAKE IKEY_8
#define CAMERAPOINTTO IMOUSE_MBUTTON

//CHEAT CODES!
#define TELEPORT IKEY_T
#define GETKEY IKEY_8
#define PLAYERWON IKEY_O


FwEngine::ScriptPlayer::ScriptPlayer(ComponentScript& comp) :


	//**************************************Components*****************************************// 
	IScript(STRING_SCRIPT_PLAYER, comp),
	colPhy(_components.getComponent<ComponentPhysics>()),
	colAnimation(_components.getComponent<ComponentAnimation>()),
	colCollision(_components.getComponent<ComponentCollision>()),
	colRender(_components.getComponent<ComponentRenderer>()),
	colTransform(_components.getComponent<ComponentTransform>()),
	comAudio(_components.getComponent<ComponentAudio>()),
	inp(_components.getSystemInput()),
	gfx(_components.getSystemGraphics()),
	sysMgr(_components.getSystemManager()),
	camMgr(gfx._cameraPos, colTransform._currentPosition, gfx._zoom),
	objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
	sysScr(_components.getSystemScript()),



	//**************************************Movement Variables*****************************************// 
	abletojump{ false },
	WalkingSpeed{ 15000 },
	JumpForce{ Vector3D{0,1,0},10000, 0.10f },
	WalkRight(Vector3D{ 1,0,0 }, WalkingSpeed, 0.085f),
	WalkLeft(Vector3D{ -1,0,0 }, WalkingSpeed, 0.085f),
	WallPushForceRight{ Vector3D{1,0,0},20000,0.125f },
	WallPushForceLeft{ Vector3D{-1,0,0},20000,0.125f },
	WallJumpForceRight{ Vector3D{0.5,0.25,0},50000, 0.1285f },
	WallJumpForceLeft{ Vector3D{-0.5,0.25,0},50000, 0.1285f },
	StickWallForceRight{ Vector3D{ 1,0,0 }, 10000, 60.0f },
	StickWallForceLeft{ Vector3D{ -1,0,0 }, 10000, 60.0f },

	playerMsg(nullptr),

	//**************************************Script Variables*****************************************// 
	_leftBackground{ nullptr },
	_rightBackground{ nullptr },
	isBlue{ sysMgr._playerMgr.isBlue },
	facing{ false },
	up{ false },
	_health(sysMgr._playerMgr.health),
	_fullHealth{ 5 },
	_deathAudio{ false },
	_playerState{ IDLE },
	originalPosition(0),
	_keys{ sysMgr._playerMgr.keys },
	_movementTimer{ MOVEMENTLAG },
	_sceneChangeTimer(SCENETIMER),
	_damageTimer(DAMAGETIMER),
	_onEnterSceneChange(true),
	_eventCountDown(nullptr),
	_eventCountDownT(nullptr),
	_eventCountDownObj(nullptr),
	_isTimerWhite(true),
	_timerColor(TIMERCOLORFREQ),
	_playerLandingParticle{ nullptr },
	_playerJumpParticle{ nullptr },
	_playerJumpNParticle{ nullptr },
	_blastRed{ nullptr },
	_blastBlue{ nullptr },
	_blastSub{ nullptr },
	_blastSubB{ nullptr },
	_changeEffect{ nullptr },
	_keyUI{ nullptr },

	// thingy
	_screenShakeEventTimer{ (int)CAMSHAKEEVENT_FREQ },
	_screenShakeEventTimer_short{ (int)CAMSHAKEEVENT_FREQ_S }
{
}

FwEngine::ScriptPlayer::~ScriptPlayer()
{
}

void FwEngine::ScriptPlayer::Init()
{
	isBlue = true;
	inp.SetCursor(true);
	gfx._cameraPos = colTransform._currentPosition + FwMath::Vector3D{ 1000.0f,400.0f,0.0f };
	gfx._cameraPos.z = 10.0f;
	gfx._zoom = 3.0f;
	gfx.UpdateZoom();
	gfx.UpdateCameraView();
	
	//colPhy = _components.getComponent<ComponentPhysics>();
	originalPosition = colTransform._currentPosition;
	//sysMgr._map._height;
	//sysMgr._map._width;
	//sysMgr._map.MAP_SCALE;
	

	


	/// Particles 
	_playerLandingParticle = objectPool.FindGameObjectByName<ComponentEmitter>("LandingParticle");
	_playerJumpParticle = objectPool.FindGameObjectByName<ComponentEmitter>("JumpParticle");
	_playerJumpNParticle = objectPool.FindGameObjectByName<ComponentEmitter>("HopParticle");
	_blastRed = objectPool.FindGameObjectByName<ComponentEmitter>("BlastEffect_r");
	_blastBlue = objectPool.FindGameObjectByName<ComponentEmitter>("BlastEffect_b");
	_blastSub = objectPool.FindGameObjectByName<ComponentEmitter>("BlastEffect_Sub");
	_blastSubB = objectPool.FindGameObjectByName<ComponentEmitter>("BlastEffect_SubB");
	_changeEffect = objectPool.FindGameObjectByName<ComponentEmitter>("ChangeEffect");
	_keyUI = objectPool.FindGameObjectByTag<ComponentAnimation>("KeyUI");

	_blastRed->_follow = true;
	_blastBlue->_follow = true;
	_blastRed->_reverse = true;
	_blastBlue->_reverse = false;
	_blastRed->_preWarm = false;
	_blastBlue->_preWarm = false;
	_changeEffect->_follow = true;
	float halfMapScale = sysMgr._map.MAP_SCALE * 0.5f;

	FwMath::Vector3D verticeA(-halfMapScale, -halfMapScale, 0.0f);
	FwMath::Vector3D verticeB(sysMgr._map._width * sysMgr._map.MAP_SCALE - halfMapScale, sysMgr._map._height * sysMgr._map.MAP_SCALE - halfMapScale, 0.0f);
	camMgr.SetBounds(verticeA, verticeB);
	camMgr._defaultZoom = CAMERA_DEFAULT_ZOOM;
	// init player vars
	//_health = _fullHealth;
	_health = sysMgr._playerMgr.health;
	//isBlue = false;

	colRender.SetTexture("Resources/Sprites/Character/char_Aris_combinedRed_ss_003.png");
	colRender.SetTexture("Resources/Sprites/Character/char_Aris_combinedBlue_ss_003.png");
	colAnimation.PlayAnimation("idle", true);
	abletojump = false;

	colPhy.AddForce(JumpForce);
	colPhy.AddForce(WalkLeft);
	colPhy.AddForce(WalkRight);
	colPhy.AddForce(WallPushForceRight);
	colPhy.AddForce(WallPushForceLeft);
	colPhy.AddForce(WallJumpForceRight);
	colPhy.AddForce(WallJumpForceLeft);
	colPhy.AddForce(StickWallForceRight);
	colPhy.AddForce(StickWallForceLeft);

	// Message
	playerMsg = reinterpret_cast<ScriptMessage_Player*>(sysScr._sms.TryRegister<ScriptMessage_Player>("player"));

	playerMsg->movementParticle = false;
	playerMsg->velocity = FwMath::Vector3D(0.0f, 0.0f, 0.0f);

	// position
	std::vector<ComponentTransform>* tc = objectPool.GetContainerPtr<ComponentTransform>();
	for (ComponentTransform& t : *tc)
	{
		std::string tag = t._tag;
		if (tag.size() < 7)
			continue;
		if (tag.substr(0, 7).find("Portal_") != std::string::npos)
		{
			tag = tag.substr(7, tag.size());

			if (tag._Equal(sysMgr._prevScene))
			{
				// use that pos

				ScriptWinZone* wz = reinterpret_cast<ScriptWinZone*>(
					sysScr.FindScript((int)t.getParentGameObject()->GetIndex(), STRING_SCRIPT_WINZONE));

				if (wz)
					colTransform._currentPosition = t._currentPosition + FwMath::Vector3D(wz->_x, wz->_y, 0.0f);

				break;
			}
		}
	}

	// cannot find, use default pos



	// event

	// event
	if (sysMgr._playerMgr.keys >= 3 && sysMgr._playerMgr.isShakeEvent)
	{
		// shake event
		//sysMgr._playerMgr.isShakeEvent = true;
		//_screenShakeEventTimer.reset();
		//_screenShakeEventTimer_short.reset();


		// timer event
		//sysMgr._playerMgr.isTimerEvent = true;
		//sysMgr._playerMgr.eventTimer.reset();

		if (sysMgr._playerMgr.isTimerEvent)
		{
			_eventCountDown = sysMgr.AddTextObject();

			_eventCountDown->_displayText = sysMgr._playerMgr.eventTimer.getRemainingTime_s();

			_eventCountDownObj = _eventCountDown->getParentGameObject();

			_eventCountDownT = objectPool.GetComponent<ComponentTransform>(_eventCountDownObj->GetIndex());
			_eventCountDownT->_currentPosition.x = gfx._cameraPos.x;
			_eventCountDownT->_currentPosition.y = gfx._cameraPos.y + gfx.GetHeight() * gfx._zoom * 0.4f;

			_timerColor.reset();
		}

	}

}


void FwEngine::ScriptPlayer::Update(float dt)
{
	//Blue Particles
	_blastBlue->GetTransform()->_currentPosition = colTransform._currentPosition;
	_blastBlue->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.1f;

	if (facing && !up)
	{
		_blastBlue->GetTransform()->_currentPosition.x += colTransform._scale.x * 0.5f;
		_blastBlue->GetTransform()->_rotation = 0.0f;
	}
	else  if (!facing && !up)
	{
		_blastBlue->GetTransform()->_currentPosition.x -= colTransform._scale.x * 0.5f;
		_blastBlue->GetTransform()->_rotation = 180.0f;
	}
	if (up)
	{
		_blastBlue->GetTransform()->_currentPosition.y += colTransform._scale.y * 0.5f;
		_blastBlue->GetTransform()->_rotation = -90.0f;
	}

	///Red Particle
	_blastRed->GetTransform()->_currentPosition = colTransform._currentPosition;
	_blastRed->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.1f;

	if (facing && !up)
	{
		//_blastRed->GetTransform()->_currentPosition.x += (colTransform._scale.x * 0.5f + 200);
		_blastRed->GetTransform()->_rotation = 0.0f;
	}
	else  if (!facing && !up)
	{
		//_blastRed->GetTransform()->_currentPosition.x -= (colTransform._scale.x * 0.5f + 200);
		_blastRed->GetTransform()->_rotation = 180.0f;
	}
	if (up)
	{
		_blastRed->GetTransform()->_currentPosition.y += (colTransform._scale.y * 0.5f + 200);
		_blastRed->GetTransform()->_rotation = -90.0f;
	}



	// Sub emitter Red
	_blastSub->GetTransform()->_currentPosition = colTransform._currentPosition;
	_blastSub->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.1f;

	if (facing && !up)
	{
		_blastSub->GetTransform()->_currentPosition.x += colTransform._scale.x * 0.5f;
		_blastSub->GetTransform()->_rotation = 0.0f;
	}
	else  if (!facing && !up)
	{
		_blastSub->GetTransform()->_currentPosition.x -= colTransform._scale.x * 0.5f;
		_blastSub->GetTransform()->_rotation = 180.0f;
	}
	if (up)
	{
		_blastSub->GetTransform()->_currentPosition.y += colTransform._scale.y * 0.5f;
		_blastSub->GetTransform()->_rotation = -90.0f;
	}

	_blastSub->_reverse = true;
	_blastSub->_spawnAngle = 65;

	// sub emitter blue
	_blastSubB->GetTransform()->_currentPosition = colTransform._currentPosition;
	_blastSubB->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.1f;

	if (facing && !up)
	{
		_blastSubB->GetTransform()->_currentPosition.x += colTransform._scale.x * 0.5f;
		_blastSubB->GetTransform()->_rotation = 0.0f;
	}
	else  if (!facing && !up)
	{
		_blastSubB->GetTransform()->_currentPosition.x -= colTransform._scale.x * 0.5f;
		_blastSubB->GetTransform()->_rotation = 180.0f;
	}
	if (up)
	{
		_blastSubB->GetTransform()->_currentPosition.y += colTransform._scale.y * 0.5f;
		_blastSubB->GetTransform()->_rotation = -90.0f;
	}

	if (facing)
	{
		_blastSubB->_velocity = { 1,0,0 };
	}
	else
	{
		_blastSubB->_velocity = { -1,0,0 };
	}


	// Change effect
	_changeEffect->GetTransform()->_currentPosition = colTransform._currentPosition;
	_changeEffect->GetTransform()->_currentPosition.y += colTransform._scale.y * 0.1f;


	//setting gamestate
	if (sysMgr._menustateNext == sysMgr.MENUSTATE_PLAYERDIED)
	{
		if ((inp)(IKEY_RETURN, SystemInput::PRESS) || (inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
		{
			sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
			sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_RELOAD;
		}
		return;
	}

	// screen shake eventtt
	//if (sysMgr._playerMgr.isRestrickMovement)
	if (sysMgr._playerMgr.keys >= 3 && !sysMgr._playerMgr.isShakeEvent)
	{
		// shake event
		sysMgr._playerMgr.isShakeEvent = true;
		_screenShakeEventTimer.reset();
		_screenShakeEventTimer_short.reset();


		// timer event
		sysMgr._playerMgr.isTimerEvent = true;
		sysMgr._playerMgr.eventTimer.reset();

		_eventCountDown = sysMgr.AddTextObject();

		_eventCountDown->_displayText = sysMgr._playerMgr.eventTimer.getRemainingTime_s();

		_eventCountDownObj = _eventCountDown->getParentGameObject();

		_eventCountDownT = objectPool.GetComponent<ComponentTransform>(_eventCountDownObj->GetIndex());
		_eventCountDownT->_currentPosition.x = gfx._cameraPos.x;
		_eventCountDownT->_currentPosition.y = gfx._cameraPos.y + gfx.GetHeight() * gfx._zoom * 0.4f;

		_timerColor.reset();
	}


	if (sysMgr._playerMgr.isTimerEvent)
	{
		if (_eventCountDownT)
		{
			_eventCountDownT->_currentPosition.x = gfx._cameraPos.x;
			_eventCountDownT->_currentPosition.y = gfx._cameraPos.y + gfx.GetHeight() * gfx._zoom * 0.4f;
		}
		if (_eventCountDown)
		{
			_eventCountDown->_displayText = sysMgr._playerMgr.eventTimer.getRemainingTime_s();


			if (_timerColor.checkTrigger())
			{
				_timerColor.reset();
				if (_isTimerWhite)
				{
					_eventCountDown->_colour = FwMath::Vector4D(255, 255, 255, 255);
				}
				else
				{
					_eventCountDown->_colour = FwMath::Vector4D(255, 0, 0, 255);
				}
				_isTimerWhite = !_isTimerWhite;
			}

		}

		if (sysMgr._playerMgr.isTimerUp)
		{
			if (_eventCountDown)
			{
				_eventCountDown->_displayText = std::string("SYSTEM SHUTTING DOWN");
				_eventCountDown->_fontType= std::string(".\\Resources\\Fonts\\Bandwidth.ttf");

				
				if (_timerColor.checkTrigger())
				{
					_timerColor.reset();
					if (_isTimerWhite)
					{
						_eventCountDown->_colour = FwMath::Vector4D(0, 0, 0, 255);
					}
					else
					{
						_eventCountDown->_colour = FwMath::Vector4D(255, 0, 0, 255);
					}
					_isTimerWhite = !_isTimerWhite;
				}

			}


			// minus hp every 10? sec
			if (_damageTimer.checkTrigger())
			{
				_damageTimer.reset();
				--_health;
			}
		} 
		else if (sysMgr._playerMgr.eventTimer.checkTrigger())
		{
			//clock_t elapsed = sysMgr._playerMgr.eventTimer.getElapsedTime();

			sysMgr._playerMgr.isTimerUp = true;
			_damageTimer.reset();

		}

	}

	if (sysMgr._playerMgr.isShakeEvent)
	{
		if (!sysMgr._playerMgr.isTimerEvent)
		{
			if(_eventCountDown)
				_eventCountDown->_colour = FwMath::Vector4D(255, 255, 255, 255);
		}

		if (_screenShakeEventTimer.checkTrigger())
		{
			camMgr._cameraShake.Start(500, 100, 400, PlayerCameraPositionManager::CameraShake::CSH_SIDEWAYSX);

			_screenShakeEventTimer.reset();
		}

		if (_screenShakeEventTimer.checkTrigger())
		{
			camMgr._cameraShake.Start(300, 100, 400, PlayerCameraPositionManager::CameraShake::CSH_SIDEWAYSX);

			_screenShakeEventTimer_short.reset();
		}
	}



	//check if the player is able to jump
	if ((colCollision.collisionFlag & FwMath::COLLISION_BOTTOM) || (colCollision.previousCollisionFlag & FwMath::COLLISION_BOTTOM))
	{
		abletojump = true;
	}
	else
	{
		abletojump = false;
	}

	//////////////////////////////////////Player controls here//////////////////////////////////////

	//////////////move logic start///////////

	if (_playerState != INTERACTING)
	{
		if ((inp)(LOOKUP, SystemInput::PRESS))
		{

			if (abletojump)
			{
				_playerState = LOOKUPSTART;
			}

			up = true;
		}
		else if ((inp)(LOOKUP, SystemInput::HOLD))
		{

			if (abletojump)
			{
				camMgr.ActionCamTo(camMgr._playercurrentPosition + Vector3D(0, 200, 0), camMgr._defaultZoom); // Look Up cam
			}

			if (!colAnimation.IsPlaying() && abletojump)
			{
				_playerState = LOOKINGUP;
			}
		}
		/*else*/ if ((inp)(LOOKUP, SystemInput::RELEASE))
		{
			if (abletojump)
			{
				_playerState = LOOKUPEND;
			}
		}
	}


	if ((inp)(WALKLEFT, SystemInput::PRESS))
	{
		facing = false;
		up = false;
		_movementTimer.reset();

		if (_playerState == HANGING_RIGHT_LOOKRIGHT)
		{
			camMgr.ActionCamTo(camMgr._playercurrentPosition - Vector3D(200, 0, 0), camMgr._defaultZoom, 1);
		}
	}
	if ((inp)(WALKRIGHT, SystemInput::PRESS))
	{
		facing = true;
		up = false;
		_movementTimer.reset();

		if (_playerState == HANGING_LEFT_LOOKLEFT)
		{
			camMgr.ActionCamTo(camMgr._playercurrentPosition + Vector3D(200, 0, 0), camMgr._defaultZoom, 1);
		}
	}

	if ((inp)(WALKLEFT, SystemInput::HOLD) && _movementTimer.checkTrigger())
	{
		if (_playerState != HANGING_LEFT_LOOKLEFT)
		{
			WalkLeft.SetActivate();
		}

		if (_playerState != MIDAIR &&
			_playerState != HANGING_RIGHT_LOOKRIGHT &&
			_playerState != HANGING_LEFT_LOOKLEFT &&
			_playerState != HANGING_RIGHT_LOOKUP &&
			_playerState != HANGING_LEFT_LOOKUP &&
			_playerState != CEILING_CLIMB)
		{
			_playerState = WALK;
		}

	}
	if ((inp)(WALKRIGHT, SystemInput::HOLD) && _movementTimer.checkTrigger())
	{
		if (_playerState != HANGING_RIGHT_LOOKRIGHT)
		{
			WalkRight.SetActivate();
		}

		if (_playerState != MIDAIR &&
			_playerState != HANGING_RIGHT_LOOKRIGHT &&
			_playerState != HANGING_LEFT_LOOKLEFT &&
			_playerState != HANGING_RIGHT_LOOKUP &&
			_playerState != HANGING_LEFT_LOOKUP &&
			_playerState != CEILING_CLIMB)
		{
			_playerState = WALK;
		}

	}
	//////////////move logic end///////////

	//////////////jump logic start///////////
	if ((inp)(JUMP, SystemInput::PRESS))
	{
		if (abletojump &&
			_playerState != HANGING_RIGHT_LOOKRIGHT &&
			_playerState != HANGING_LEFT_LOOKLEFT &&
			_playerState != CEILING_CLIMB)
		{
			if (_playerJumpNParticle)
			{
				_playerJumpNParticle->GetTransform()->_currentPosition = colTransform._currentPosition;
				_playerJumpNParticle->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.5f;
				_playerJumpNParticle->Play();
			}
			_playerState = MIDAIR;

			JumpForce.SetActivate();
			abletojump = false;
		}
	}
	//////////////jump logic end///////////


	//////////////ability logic start///////////

	/////////////attract logic start/////////////
	if ((inp)(ATTRACT, SystemInput::PRESS) && abletojump)
	{
		colRender.SetTexture("Resources/Sprites/Character/char_Aris_combinedRed_ss_003.png");
		isBlue = false;

		if (abletojump)
		{
			_playerState = ATTRACT_START;
		}

		if (_changeEffect)
		{

			_changeEffect->_colourStart = { 1,0,0,1 };
			_changeEffect->_colourEnd = { 1,0,0,0 };
			_changeEffect->Play();
		}

		_blastBlue->Stop();
		_blastSubB->Stop();


	}
	else if ((inp)(ATTRACT, SystemInput::HOLD))
	{
		if (!colAnimation.IsPlaying() && abletojump)
		{
			_playerState = ATTRACTING;
		}
		
		if ((inp)(WALKLEFT, SystemInput::HOLD) ||
			(inp)(WALKRIGHT, SystemInput::HOLD))
		{
			if (_playerState != CEILING_CLIMB &&
				_playerState != HANGING_LEFT_LOOKLEFT &&
				_playerState != HANGING_LEFT_LOOKRIGHT &&
				_playerState != HANGING_RIGHT_LOOKLEFT &&
				_playerState != HANGING_RIGHT_LOOKRIGHT &&
				_playerState != HANGING_LEFT_LOOKUP &&
				_playerState != HANGING_RIGHT_LOOKUP)
			{
				_playerState = ATTRACT_MOVE;
			}
		}

		if ((inp)(WALKLEFT, SystemInput::RELEASE) ||
			(inp)(WALKRIGHT, SystemInput::RELEASE))
		{
			_playerState = ATTRACTING;
		}

		char check = colCollision.checkCollisionWith("heavy metal");
		if ((inp)(LOOKUP, SystemInput::HOLD) &&
			check != 'T' &&
			check != 'L' &&
			check != 'R')
		{
			_playerState = ATTRACT_UP;
		}

		if (_blastRed && !_blastRed->_play && _playerState != CEILING_CLIMB)
		{
			_blastRed->Play();
			_blastSub->Play();
		}
		else if (_playerState == CEILING_CLIMB)
		{
			_blastRed->Stop();
			_blastSub->Stop();
		}
	}
	/*else*/ if ((inp)(ATTRACT, SystemInput::RELEASE))
	{
		if (abletojump)
		{
			_playerState = ATTRACT_END;
		}
		_blastRed->Stop();
		_blastSub->Stop();
	}
	/////////////attract logic end/////////////



	/////////////repel logic start/////////////
	if ((inp)(REPEL, SystemInput::PRESS))
	{
		colRender.SetTexture("Resources/Sprites/Character/char_Aris_combinedBlue_ss_003.png");
		isBlue = true;

		if (abletojump)
		{
			_playerState = REPEL_START;
		}

		if (_changeEffect)
		{
			_changeEffect->_colourStart = { 0,0,1,1 };
			_changeEffect->_colourEnd = { 0,0,1,0 };
			_changeEffect->Play();
		}
		_blastRed->Stop();
		_blastSub->Stop();
	}
	else if ((inp)(REPEL, SystemInput::HOLD))
	{
		if (!colAnimation.IsPlaying() && abletojump)
		{
			_playerState = REPELING;
		}

		if ((inp)(WALKLEFT, SystemInput::HOLD) ||
			(inp)(WALKRIGHT, SystemInput::HOLD))
		{
			if (_playerState != CEILING_CLIMB &&
				_playerState != HANGING_LEFT_LOOKLEFT &&
				_playerState != HANGING_LEFT_LOOKRIGHT &&
				_playerState != HANGING_RIGHT_LOOKLEFT &&
				_playerState != HANGING_RIGHT_LOOKRIGHT &&
				_playerState != HANGING_LEFT_LOOKUP &&
				_playerState != HANGING_RIGHT_LOOKUP)
			{
				_playerState = REPEL_MOVE;
			}
		}

		if ((inp)(WALKLEFT, SystemInput::RELEASE) ||
			(inp)(WALKRIGHT, SystemInput::RELEASE))
		{
			_playerState = REPELING;
		}

		if (_blastBlue && !_blastBlue->_play && _playerState != CEILING_CLIMB)
		{
			_blastBlue->Play();
			_blastSubB->Play();
		}
	}
	 if ((inp)(REPEL, SystemInput::RELEASE))
	{
		if (abletojump)
		{
			_playerState = REPEL_END;
		}
		_blastBlue->Stop();
		_blastSubB->Stop();
	}
	/////////////attract logic end/////////////



	//////////////ability logic end///////////

	//////////////interact logic start//////////


	if ((inp)(INTERACT, SystemInput::PRESS) && abletojump)
	{
		if (colCollision.isCollidingWith("switch"))
		{
			_playerState = INTERACTING;
		}
	}

	//////////////interact logic end//////////

	//player in midair
	if (colCollision.checkInAir())
	{
		_playerState = MIDAIR;
	}

	colCollision.checkCollisionWith("");

	//PLayer landed from midair
	if (colCollision.checkLanding())
	{
		_playerState = LANDING;
		
		//////////////land logic here///////////
		if (-colPhy._preVelocity.y > 1500)
		{
			camMgr._cameraShake.Start(static_cast<int>(-colPhy._preVelocity.y / 4), 200, 200, PlayerCameraPositionManager::CameraShake::CSH_SIDEWAYSX);
			comAudio._volume = 1.0f;
			comAudio.SetVolume();
			comAudio.Play("BoomSound", false);
			//Particle play
			//Position particle at bottom of sprite
			if (_playerLandingParticle)
			{
				_playerLandingParticle->GetTransform()->_currentPosition = colTransform._currentPosition;
				_playerLandingParticle->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.5f;
				_playerLandingParticle->Play();
			}
		}
		if (-colPhy._preVelocity.y > 2200)
		{
			camMgr._cameraShake.Start(static_cast<int>(-colPhy._preVelocity.y / 3), 100, 400, PlayerCameraPositionManager::CameraShake::CSH_SIDEWAYSX);
			_health--;
		}
		//////////////land logic end///////////
	}

	//// PLAYER LOSS
	if (_health == 0)
	{
		inp.SetCursor(true);
		_playerState = DEATH;

		//////////////death logic here///////////
		sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERDIED;
		sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;

		if (!_deathAudio)
		{
			comAudio._volume = 1.0f;
			comAudio.SetVolume();
			comAudio.Play("DeathSound", false);
			_deathAudio = true;
		}
		/////////////death logic end/////////////
	}

	//Animation rendered here
	if (colRender.isEnabled())
	{

		if (colAnimation.isEnabled())
		{
			if (facing)
			{
				colRender._flipX = false;
			}
			else
			{
				colRender._flipX = true;
			}

			switch (_playerState)
			{
			case IDLE:
				colAnimation.PlayAnimation("idle", true);
				break;
			case WALK:
				colAnimation.PlayAnimation("walk", true, 0, 7);
				break;
			case MIDAIR:
				colAnimation.PlayAnimation("jump", false, 7, 11);
				break;

			case LANDING:
				colAnimation.PlayAnimation("jump", false, 13, 16);
				break;

			case INTERACTING:
				colAnimation.PlayAnimation("interact", false, 0, 14);
				break;
			case LOOKUPSTART:
				colAnimation.PlayAnimation("look", false, 0, 2);
				break;
			case LOOKINGUP:
				colAnimation.PlayAnimation("look", true, 3, 3);
				break;
			case LOOKUPEND:
				colAnimation.PlayAnimation("look", false, 4, 5);
				break;

			case ATTRACT_START:
				comAudio._volume = 1.0f;
				comAudio.SetVolume();
				comAudio.Play("BlastSound", false);
				colAnimation.PlayAnimation("ability", false, 0, 6);
				break;

			case ATTRACTING:
				colAnimation.PlayAnimation("ability", true, 7, 10);
				break;

			case ATTRACT_END:
				colAnimation.PlayAnimation("ability", false, 11, 13);
				_blastRed->Stop();
				break;

			case ATTRACT_MOVE:
				colAnimation.PlayAnimation("abilitywalk", true, 0, 7);
				break;

			case ATTRACT_UP:
				colAnimation.PlayAnimation("abilitywalk", false, 9, 16);
				break;

			case REPEL_START:
				comAudio._volume = 1.0f;
				comAudio.SetVolume();
				comAudio.Play("RepelSound", false);
				colAnimation.PlayAnimation("ability", false, 0, 9);
				break;

			case REPELING:
				colAnimation.PlayAnimation("ability", true, 10, 11);

				break;

			case REPEL_END:
				colAnimation.PlayAnimation("ability", false, 12, 13);
				_blastBlue->Stop();
				break;

			case REPEL_MOVE:
				colAnimation.PlayAnimation("abilitywalk", true, 0, 7);
				break;

			case HANGING_LEFT_LOOKLEFT:
				//play hang on animation which share row with walk
				colRender._flipX = true;
				colAnimation.PlayAnimation("ceilingclimb", true, 9, 14);
				break;

			case HANGING_RIGHT_LOOKRIGHT:
				colRender._flipX = false;
				colAnimation.PlayAnimation("ceilingclimb", true, 9, 14);
				break;

			case HANGING_LEFT_LOOKRIGHT:
				//play hang on animation which share row with walk
				colRender._flipX = true;
				colAnimation.PlayAnimation("walk", true, 10, 14);
				break;

			case HANGING_RIGHT_LOOKLEFT:
				colRender._flipX = false;
				colAnimation.PlayAnimation("walk", true, 10, 14);
				break;

			case HANGING_LEFT_LOOKUP:
				//play hang on animation which share row with walk
				colRender._flipX = true;
				colAnimation.PlayAnimation("look", true, 11, 16);
				break;

			case HANGING_RIGHT_LOOKUP:
				colRender._flipX = false;
				colAnimation.PlayAnimation("look", true, 11, 16);
				break;


			case CEILING_CLIMB:
				if (((inp)(WALKLEFT, SystemInput::HOLD) || (inp)(WALKRIGHT, SystemInput::HOLD)))
				{
					colAnimation.PlayAnimation("ceilingclimb", true);
				}
				else
				{
					colAnimation.PlayAnimation("ceilingclimb", false, 1, 1);
				}
				break;

			case DEATH:
				colAnimation.PlayAnimation("damage", false);
				break;

			default:
				throw;
			}
		}
	}

	//reset player state
	if (_playerState == WALK ||
		_playerState == LANDING ||
		_playerState == HANGING_LEFT_LOOKRIGHT||
		_playerState == HANGING_RIGHT_LOOKLEFT||
		_playerState == HANGING_LEFT_LOOKLEFT ||
		_playerState == HANGING_RIGHT_LOOKRIGHT ||
		_playerState == REPEL_END ||
		_playerState == ATTRACT_END ||
		_playerState == LOOKUPEND)
	{
		_playerState = IDLE;
	}
	///////////////////////////////////////////////hotkeys here////////////////////////////////////////////////

	//fullscreen
	if ((inp)(FULLSCREEN, SystemInput::PRESS))
	{
		WINDOWMAIN->ToggleFullScreen();
	}

	// teleport
	if ((inp)(TELEPORT, SystemInput::PRESS))
	{
		inp.SetCursor(true);
		colTransform._currentPosition = sysMgr._cursorPosition;
	}

	// middle mouse btn to move camera to a position then back
	if ((inp)(CAMERAPOINTTO, SystemInput::HOLD))
	{
		camMgr.ActionCamTo(sysMgr._cursorPosition, 1.0f); // move camera to the cursor position
	}

	if ((inp)(IKEY_H, SystemInput::PRESS))
	{
		inp.SetCursor(true);
		_health--;
	}


	// 
	if ((inp)(CAMERASHAKE, SystemInput::PRESS))
	{
		//camMgr._cameraShake.Start(1500.0f, 100.0f, 1000.0f, PlayerCameraPositionManager::CameraShake::CSH_SIDEWAYSX);
		camMgr._cameraShake.Start(500, 100, 400, PlayerCameraPositionManager::CameraShake::CSH_SIDEWAYSX);
	}
	if ((inp)(GETKEY, SystemInput::PRESS))
	{
		_keys++;
		_health++;

	}
	// PLAYER WON
	/*if ((inp)(PLAYERWON, SystemInput::PRESS) /*|| colCollision.isCollidingWith("winning condition")*///)
	/*{
		sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERWON;
		sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;
	}*/

	////////////////////////////////////////////////////camera here/////////////////////////////////////////////
	//camera bounding

	/////////////////////////////////////Game State Update here/////////////////////////////
	if (sysMgr._menustateNext == sysMgr.MENUSTATE_PLAYERWON)
	{
		inp.SetCursor(true);

		if (_onEnterSceneChange)
		{
			_onEnterSceneChange = !_onEnterSceneChange;
			_sceneChangeTimer.reset();
		}
		else
		{
			if (_sceneChangeTimer.checkTrigger() || (inp)(IKEY_RETURN, SystemInput::PRESS))
			{
				// triggered
				sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
				//sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_NEXTLEVEL;
				sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_NEXTSCENE;
				_onEnterSceneChange = !_onEnterSceneChange;
			}
		}
	}

	if (sysMgr._gamestateNext == sysMgr.GAMESTATE_PAUSE)
	{
		inp.SetCursor(true);
		// pause
		camMgr.ActionCamTo(
			camMgr._playercurrentPosition //+ 
			/*FwMath::Vector3D(colTransform._scale.x, colTransform._scale.y * 0.2f, 0.0f)*/,
			0.4f, 0.0f, 10.0f);
	}

	camMgr.Update(dt);
	gfx.UpdateZoom();
	gfx.UpdateCameraView();

	// Message

	playerMsg->movementParticle = false;
	playerMsg->velocity = colPhy._velocity;
}

void FwEngine::ScriptPlayer::Free()
{
	colPhy.ListOfDirectionalForces.clear();
	colPhy.ListOfMagneticForces.clear();
	colPhy.ListOfRotationalForces.clear();
}

void FwEngine::ScriptPlayer::Destroy()
{

}

void FwEngine::ScriptPlayer::Unload()
{
	if (_eventCountDown)
		_eventCountDown->disable();
	if (_eventCountDownObj)
		_eventCountDownObj->disable();
	if (_eventCountDownT)
		_eventCountDownT->disable();
}

void FwEngine::ScriptPlayer::PlayerGetDamaged(int damage)
{

	_health -= damage;

	comAudio._volume = 1.0f;
	comAudio.SetVolume();
	comAudio.Play("HitSound", false);
}

int* FwEngine::ScriptPlayer::GetKey()
{
	return &_keys;
}

void FwEngine::ScriptPlayer::OnCollisionEnter(size_t id)
{
	ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
	if (otherTransform._tag == "kill_zone")
	{
		--_health;
		comAudio._volume = 1.0f;
		comAudio.SetVolume();
		comAudio.Play("HitSound", false);
	}

	if (otherTransform._tag == "AED")
	{
		if (_health < _fullHealth)
		{
			ComponentAudio& AED = objectPool.GetContainerPtr<ComponentAudio>()->at(id);
			otherTransform.getParentGameObject()->disable();
			AED.Play("AEDSound", false);
			_health++;
		}
	}

}

void FwEngine::ScriptPlayer::OnCollisionExit(size_t id)
{
	ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
	if (otherTransform._tag == "heavy metal")
	{
		StickWallForceLeft.DeActivate();
		StickWallForceRight.DeActivate();
	}
}

void FwEngine::ScriptPlayer::OnCollisionStay(size_t id)
{
	ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);

	if (otherTransform._tag == "keys")
	{

		ComponentAudio& key = objectPool.GetContainerPtr<ComponentAudio>()->at(id);
		ComponentAnimation& keyAnim = objectPool.GetContainerPtr<ComponentAnimation>()->at(id);

		keyAnim.PlayAnimation("activate", false, 0, 9);


		if (!keyAnim.IsPlaying())
		{
			key.Play("KeySound", false);

			_keys++;

			sysMgr._playerMgr.addKeyLocation(sysMgr._currentScene);

			if (_keys == 1)
			{
				_keyUI->PlayAnimation("card0", false);
			}
			else if (_keys == 2)
			{
				_keyUI->PlayAnimation("card1", false);
			}
			else if (_keys == 3)
			{
				_keyUI->PlayAnimation("card2", false);
			}
			otherTransform.getParentGameObject()->disable();
		}

	}

	if (otherTransform._tag == "metal")
	{
		ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
		ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
		StackingResponse_NonElastic(&colPhy, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);
	}


	if (otherTransform._tag == "heavy metal")
	{
		ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
		ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);

		StackingResponse(&colPhy, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);

		colPhy._velocity.y = 0;
		colTransform._currentPosition.y = colPhy._previousPosition.y;
		JumpForce.DeActivate();
		char check = colCollision.Collision_Check_List.find("heavy metal")->second;
		if (check == 'L')
		{
			StickWallForceLeft.SetActivate();
			_playerState = HANGING_LEFT_LOOKLEFT;

			if ((inp)(LOOKUP, SystemInput::HOLD))
			{
				_playerState = HANGING_LEFT_LOOKUP;
			}

			if ((inp)(WALLJUMP, SystemInput::PRESS))
			{
				WallJumpForceRight.SetActivate();
				if (_playerJumpParticle)
				{
					_playerJumpParticle->GetTransform()->_currentPosition = colTransform._currentPosition;
					_playerJumpParticle->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.1f;
					_playerJumpParticle->GetTransform()->_rotation = 160;
					_playerJumpParticle->Play();
				}
				comAudio.Play("PipeSound", false);
				comAudio._volume = 0.5f;
				comAudio.SetVolume();
			}
			else if ((inp)(REPEL, SystemInput::PRESS))
			{
				WallPushForceRight.SetActivate();
				comAudio.Play("PipeSound", false);
				comAudio._volume = 0.5f;
				comAudio.SetVolume();
			}
		}
		if (check == 'R')
		{
			StickWallForceRight.SetActivate();

			_playerState = HANGING_RIGHT_LOOKRIGHT;

			if ((inp)(LOOKUP, SystemInput::HOLD))
			{
				_playerState = HANGING_RIGHT_LOOKUP;
			}


			if ((inp)(WALLJUMP, SystemInput::PRESS))
			{
				WallJumpForceLeft.SetActivate();
				if (_playerJumpParticle)
				{
					_playerJumpParticle->GetTransform()->_currentPosition = colTransform._currentPosition;
					_playerJumpParticle->GetTransform()->_currentPosition.y -= colTransform._scale.y * 0.1f;
					_playerJumpParticle->GetTransform()->_rotation = 25;
					_playerJumpParticle->Play();
				}
				comAudio.Play("PipeSound", false);
				comAudio._volume = 0.5f;
				comAudio.SetVolume();
			}
			else if ((inp)(ATTRACT, SystemInput::PRESS))
			{
				WallPushForceLeft.SetActivate();
				comAudio.Play("PipeSound", false);
				comAudio._volume = 0.5f;
				comAudio.SetVolume();
			}
		}
		if ((colCollision.checkCollisionWith("heavy metal") == 'T') ||
			(colCollision.prevCheckCollisionWith("heavy metal") == 'T'))
		{
			up = true;
			_playerState = CEILING_CLIMB;
			if ((inp)(ATTRACT, SystemInput::RELEASE))
			{
				_playerState = MIDAIR;
			}
		}
	}


	if (otherTransform._tag == "non metal")
	{
		ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
		ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
		StackingResponse_NonElastic(&colPhy, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);
	}
	if (otherTransform._tag == "Door")
	{
		ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
		NoPhysicsUnpenetrableObject(&colPhy, &colTransform, &colCollision, &otherTransform, &otherCollision);
	}


	//if (otherTransform._tag == "mplatform")
	//{
	//	ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
	//	ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
	//	StackingResponse_NonElastic(&colPhy, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);
	//}

}

void FwEngine::ScriptPlayer::OnEnterScn()
{
	// Message
	playerMsg = reinterpret_cast<ScriptMessage_Player*>(sysScr._sms.TryRegister<ScriptMessage_Player>("player"));

	playerMsg->movementParticle = false;
	playerMsg->velocity = FwMath::Vector3D(0.0f, 0.0f, 0.0f);


	// health
	if (_health == 0)
	{
		_health = _fullHealth;
		_health = sysMgr._playerMgr.health;
	}
	else
	{
		_health = sysMgr._playerMgr.health;
	}


	if (sysMgr._playerMgr.isTimerUp)
	{

		sysMgr._playerMgr.eventTimer.reset();
	}

	//// position
	//std::vector<ComponentTransform>* tc = objectPool.GetContainerPtr<ComponentTransform>();
	//for (ComponentTransform& t : *tc)
	//{
	//	std::string tag = t._tag;
	//	if (tag.size() < 7)
	//		continue;
	//	if (tag.substr(0, 7).find("Portal_") != std::string::npos)
	//	{
	//		tag = tag.substr(7, tag.size());

	//		if (tag._Equal(sysMgr._prevScene))
	//		{
	//			// use that pos

	//			ScriptWinZone* wz = reinterpret_cast<ScriptWinZone*>(
	//				sysScr.FindScript((int)t.getParentGameObject()->GetIndex(), STRING_SCRIPT_WINZONE));
	//			
	//			if(wz)
	//				colTransform._currentPosition = t._currentPosition + FwMath::Vector3D(wz->_x, wz->_y, 0.0f);

	//			break;
	//		}
	//		
	//	}
	//}

	//// cannot find, use default pos
	//

}

//void FwEngine::ScriptPlayer::OnTriggerEnter(int id)
//{
//}
//
//void FwEngine::ScriptPlayer::OnTriggerExit(int id)
//{
//}
//
//void FwEngine::ScriptPlayer::OnTriggerStay(int id)
//{
//}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::SetBounds(FwMath::Vector3D verticeA, FwMath::Vector3D verticeB)
{
	_verticeA = verticeA;
	_verticeB = verticeB;
}

//constexpr float CAMERA_VELOCITYSCALE_DEFAULT = 1.0f;
constexpr float CAMERA_VELOCITYSCALEX_DEFAULT = 1.0f;
constexpr float CAMERA_VELOCITYSCALEY_DEFAULT = 1.0f;
constexpr float CAMERA_VELOCITYSCALEZ_DEFAULT = 1.0f;

FwEngine::ScriptPlayer::PlayerCameraPositionManager::PlayerCameraPositionManager(
	FwMath::Vector3D& cameraPosition,
	FwMath::Vector3D& playercurrentPosition,
	float& zoom
	) :
	_cameraPosition(cameraPosition), _playercurrentPosition(playercurrentPosition), _zoom(zoom),
	_isAction(false), _destinationPos(), _destionationZoom(0.0f), _timer(0.0f),
	_MaxDistance(5000.0f), _MinDistance(1.0f), _MaxVelocity(8000.0f), _MinVelocity(2.5f),
	_VelocityScaleX(CAMERA_VELOCITYSCALEX_DEFAULT), _VelocityScaleY(CAMERA_VELOCITYSCALEY_DEFAULT), _VelocityScaleZ(CAMERA_VELOCITYSCALEZ_DEFAULT),
	_defaultZoom(CAMERA_DEFAULT_ZOOM), _verticeA(FwMath::Vector3D()), _verticeB(FwMath::Vector3D()),
	_cameraShake()
{
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::ActionCamTo(FwMath::Vector3D position, float zoom, float time, float velocityScale)
{
	_isAction = true;
	_destinationPos = position;
	_destionationZoom = zoom;
	_timer = time;
	_VelocityScaleX = CAMERA_VELOCITYSCALEX_DEFAULT * velocityScale;
	_VelocityScaleY = CAMERA_VELOCITYSCALEY_DEFAULT * velocityScale;
	_VelocityScaleZ = CAMERA_VELOCITYSCALEZ_DEFAULT * velocityScale;
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::Update(float dt)
{
	float minZoom = 0.01f;
	float maxZoom = 500.0f;
	float closeDistance = 100.0f;

	auto limitBounds = [&](FwMath::Vector3D& pos)
	{
		//FwMath::Vector3D verticeA(_verticeA.x + GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeA.y + GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);
		//FwMath::Vector3D verticeB(_verticeB.x - GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeB.y - GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);

		FwMath::Vector3D verticeA(_verticeA.x + GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeA.y + GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);
		FwMath::Vector3D verticeB(_verticeB.x - GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeB.y - GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);

		//FwMath::Vector3D verticeA(_verticeA.x + GRAPHICS->GetWidth() * 0.5f /** _zoom*/, _verticeA.y + GRAPHICS->GetHeight() * 0.5f /** _zoom*/, 0.0f);
		//FwMath::Vector3D verticeB(_verticeB.x - GRAPHICS->GetWidth() * 0.5f /** _zoom*/, _verticeB.y - GRAPHICS->GetHeight() * 0.5f /** _zoom*/, 0.0f);

		//verticeA += FwMath::Vector3D((GRAPHICS->GetWidth() - (GRAPHICS->GetWidth() / _zoom)) *  0.5f, (GRAPHICS->GetHeight() - (GRAPHICS->GetHeight() / _zoom)) *  0.5f, 0.0f);
		//verticeB += FwMath::Vector3D((GRAPHICS->GetWidth() - (GRAPHICS->GetWidth() / _zoom)) * -0.5f, (GRAPHICS->GetHeight() - (GRAPHICS->GetHeight() / _zoom)) * -0.5f, 0.0f);

		if (pos.x < verticeA.x)
			pos.x = verticeA.x;

		if (pos.x > verticeB.x)
			pos.x = verticeB.x;

		if ((abs(_verticeB.x - _verticeA.x)) < (GRAPHICS->GetWidth() * _zoom))
			pos.x = (verticeA.x + verticeB.x) * 0.5f;

		if (pos.y < verticeA.y)
			pos.y = verticeA.y;

		if (pos.y > verticeB.y)
			pos.y = verticeB.y;

		if ((abs(_verticeB.y - _verticeA.y)) < (GRAPHICS->GetHeight() * _zoom))
			pos.y = (verticeA.y + verticeB.y) * 0.5f;

		return pos;
	};

	auto moveCamTo = [&](FwMath::Vector3D& dest)
	{
		limitBounds(dest);
		//limitBounds(dest, FwMath::Vector3D(1000.0f, 1000.0f, 0.0f));
		dest += _cameraShake._shakeVector;

		//if (_cameraShake._shakeVector.x + _cameraShake._shakeVector.y)

		if (dest.x != _cameraPosition.x || dest.y != _cameraPosition.y)
		{
			FwMath::Vector3D camToPlayer = dest - _cameraPosition;
			float vel_x;
			float vel_y;

			if (abs(camToPlayer.x) < _MinDistance)
			{
				_cameraPosition.x = dest.x;
				vel_x = 0;
			}
			else
			{
				if (abs(camToPlayer.x) > _MaxDistance)
				{
					vel_x = camToPlayer.x > 0 ? _MaxVelocity : -_MaxVelocity;
				}
				else
				{
					vel_x = camToPlayer.x / _MaxDistance * _MaxVelocity;
				}
			}

			if (abs(camToPlayer.y) < _MinDistance)
			{
				_cameraPosition.y = dest.y;
				vel_y = 0;
			}
			else
			{
				if (abs(camToPlayer.y > _MaxDistance))
				{
					vel_y = camToPlayer.y > 0 ? _MaxVelocity : -_MaxVelocity;
				}
				else
				{
					vel_y = camToPlayer.y / _MaxDistance * _MaxVelocity;
				}
			}

			//FwMath::Vector3D velocity(vel_x, vel_y, 0.0f);
			//_cameraPosition += (velocity * dt * _VelocityScaleX);

			_cameraPosition += FwMath::Vector3D(
				vel_x * dt * _VelocityScaleX,
				vel_y * dt * _VelocityScaleY,
				0.0f
				);

			//return (velocity.x == 0.0f && velocity.y == 0.0f);
			return (vel_x == 0.0f && vel_y == 0.0f);
		}
		return true;
	};

	auto zoomCamTo = [&](float dest)
	{
		if (_zoom != dest)
		{
			float zoomToPlayer = dest - _zoom;
			float vel_z;

			if (abs(zoomToPlayer) < minZoom)
			{
				_zoom = dest;
				return true;
			}
			else
			{
				if (abs(zoomToPlayer) > maxZoom)
				{
					vel_z = zoomToPlayer > 0 ? _MaxVelocity : -_MaxVelocity;
				}
				else
				{
					vel_z = zoomToPlayer / _MaxDistance * _MaxVelocity;
				}
			}
			_zoom += (vel_z * dt * _VelocityScaleZ);
			return (vel_z == 0.0f);
		}
		return true;
	};

	auto isCloseTo = [](float a, float b, float c)
	{
		return (abs(a - b) < c);
	};

	if (!_isAction)
	{
		FwMath::Vector3D destination = _playercurrentPosition;
		//FwMath::Vector3D verticeA(_verticeA.x + GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeA.y + GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);
		//FwMath::Vector3D verticeB(_verticeB.x - GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeB.y - GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);

		_VelocityScaleX = CAMERA_VELOCITYSCALEX_DEFAULT;
		_VelocityScaleY = CAMERA_VELOCITYSCALEY_DEFAULT;
		_VelocityScaleZ = CAMERA_VELOCITYSCALEZ_DEFAULT;

		moveCamTo(destination);
		zoomCamTo(_defaultZoom);
	}
	else
	{
		_timer -= dt;
		zoomCamTo(_destionationZoom);

		if
			(
				(
					moveCamTo(_destinationPos) ||
					(
						isCloseTo(_destinationPos.x, _cameraPosition.x, closeDistance) &&
						isCloseTo(_destinationPos.y, _cameraPosition.y, closeDistance)
						)
					)
				&& _timer <= 0.0f
				)
		{
			_isAction = false;
		}
	}

	_cameraShake.CameraShakeUpdate();
}

// camera shake

FwEngine::ScriptPlayer::PlayerCameraPositionManager::CameraShake::CameraShake() :
	_csmode(CSH_RANDOM), _cshakeDuration(0), _cshakePeriod(0), _isActive(false),
	_isActivate(false), _magnitude(0), _direction(true)
{
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::CameraShake::CameraShakeUpdate()
{
	if (_isActivate)
	{
		_cshakePeriod.reset();
		_cshakeDuration.reset();
		_isActive = true;
		_isActivate = false;
	}

	if (!_isActive) return;

	if (!_cshakeDuration.checkTrigger())
	{
		if (!_cshakePeriod.checkTrigger())
		{
			switch (_csmode)
			{
			case CSH_RANDOM:
			{
				int randomX = rand();
				randomX %= (_magnitude * 2);
				randomX -= _magnitude;
				int randomY = rand();
				randomY %= (_magnitude * 2);
				randomY -= _magnitude;
				_shakeVector = FwMath::Vector3D((float)randomX, (float)randomY, 0.0f);
			}
			break;
			case CSH_SIDEWAYSX:
			{
				int randomX = rand();
				_direction = randomX % 2;
				randomX %= (_magnitude);
				if (_direction)
					randomX *= -1;
				_direction = !_direction;

				int _magnitudeY = _magnitude / 2;
				int randomY = rand();
				randomY %= (_magnitudeY * 2);
				randomY -= _magnitudeY;

				_shakeVector = FwMath::Vector3D((float)randomX, (float)randomY, 0.0f);
			}
			break;
			}
		}
		else
		{
			_cshakePeriod.reset();
		}
	}
	else
	{
		_shakeVector = FwMath::Vector3D(0.0f, 0.0f, 0.0f);
		Stop();
	}
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::CameraShake::Start(int magnitude, int period, int duration, CAMSHAKEMODE csmode)
{
	_isActivate = true;
	_magnitude = magnitude;
	_cshakePeriod = Timer(period);
	_cshakeDuration = Timer(duration);
	_csmode = csmode;
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::CameraShake::Stop()
{
	_isActive = false;
}

