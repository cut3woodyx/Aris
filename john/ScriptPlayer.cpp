#include "ScriptPlayer.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include "Reaction.h"
#include <math.h>
#include "SystemManager.h"
#include "Math_CollisionSAT.h"

FwEngine::ScriptPlayer::ScriptPlayer(ComponentScript& comp) : 
	abletojump {false},
	pan{ false },
	JumpForce{ Vector3D{0,1,0},200000, 0.085f },
	WalkRight(Vector3D{ 1,0,0 }, WalkingSpeed, 0.085f),
	WalkLeft(Vector3D{ -1,0,0 }, WalkingSpeed, 0.085f),
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
	_health(sysMgr._playerMgr.health),
	isBlue{sysMgr._playerMgr.isBlue},
	originalPosition(0)
{
}

FwEngine::ScriptPlayer::~ScriptPlayer()
{
}

void FwEngine::ScriptPlayer::Init()
{
	gfx._cameraPos = colTransform._currentPosition + FwMath::Vector3D{1000.0f,400.0f,0.0f};
	gfx._cameraPos.z = 10.0f;
	gfx._zoom = 3.0f;
	gfx.UpdateZoom();
	gfx.UpdateCameraView();
	//colPhy = _components.getComponent<ComponentPhysics>();
	originalPosition = colTransform._currentPosition;
	//sysMgr._map._height;
	//sysMgr._map._width;
	//sysMgr._map.MAP_SCALE;

	float halfMapScale = sysMgr._map.MAP_SCALE * 0.5f;

	FwMath::Vector3D verticeA(-halfMapScale, -halfMapScale, 0.0f);
	FwMath::Vector3D verticeB(sysMgr._map._width * sysMgr._map.MAP_SCALE - halfMapScale, sysMgr._map._height * sysMgr._map.MAP_SCALE - halfMapScale, 0.0f);
	camMgr.SetBounds(verticeA, verticeB);
	camMgr._defaultZoom = 2.0f;

	// init player vars
	_health = 5;
	isBlue = false;

	colAnimation.SetSpritesheet("Resources/Sprites/Character/char_Aris_combinedBlue_ss_002.png");
	colAnimation.SetSpritesheet("Resources/Sprites/Character/char_Aris_combinedRed_ss_002.png");
	colAnimation.PlayAnimation("idle", true);
	abletojump = false;

	colPhy.AddForce(JumpForce);
	colPhy.AddForce(WalkLeft);
	colPhy.AddForce(WalkRight);
}


void FwEngine::ScriptPlayer::Update(float dt)
{

	if ((colCollision.collisionFlag & FwMath::COLLISION_BOTTOM) || (colCollision.previousCollisionFlag & FwMath::COLLISION_BOTTOM))
	{
		abletojump = true;
	}
	else
	{
		abletojump = false;
	}

	if ((inp)(IKEY_SPACE, SystemInput::PRESS))
	{
		if (abletojump)
		{
			JumpForce.SetActivate();
			abletojump = false;
		}
		//pan = true;

	}
	//if (colCollision.collisionFlag || colCollision.previousCollisionFlag)
	//{
		if ((inp)(IKEY_A, SystemInput::HOLD))
		{
			if (colPhy.isEnabled())
				WalkLeft.SetActivate();
		}
		if ((inp)(IKEY_D, SystemInput::HOLD))
		{
			if (colPhy.isEnabled())
				WalkRight.SetActivate();
		}
	//}
	//else
	//{
	//	if ((inp)(IKEY_A, SystemInput::HOLD))
	//	{
	//		if (colPhy.isEnabled())
	//			colPhy.SetSpeed(FwMath::Vector3D(-100, 0, 0));
	//	}
	//	if ((inp)(IKEY_D, SystemInput::HOLD))
	//	{
	//		if (colPhy.isEnabled())
	//			colPhy.SetSpeed(FwMath::Vector3D(100, 0, 0));
	//	}
	//}


	if((inp)(IMOUSE_RBUTTON,SystemInput::PRESS))
	{
		isBlue = !isBlue;

		if (isBlue)
		{
			comAudio._volume = 1.0f;
			comAudio.SetVolume();
			comAudio.Play("ChangeSound", false);
			colAnimation.SetSpritesheet("Resources/Sprites/Character/char_Aris_combinedBlue_ss_002.png");
		}
		else
		{
			comAudio._volume = 1.0f;
			comAudio.SetVolume();
			comAudio.Play("ChangeSound", false);
			colAnimation.SetSpritesheet("Resources/Sprites/Character/char_Aris_combinedRed_ss_002.png");
		}
	}

	if ((inp)(IKEY_X, SystemInput::PRESS))
	{
		--_health;
	}


	if (std::abs(colTransform._currentPosition.y - gfx._cameraPos.y) < 0.1f) 
	{
		pan = true;
	}

	if (pan == true) 
	{
		//gfx._cameraPos = colTransform._currentPosition + FwMath::Vector3D{ 500.0f,400.0f,0.0f };
		//gfx._cameraPos.z = 10.0f;
		//gfx.UpdateCameraView();
	}

	/*if ((inp)(IKEY_UP, SystemInput::HOLD))
	{
		gfx._cameraPos.y = gfx._cameraPos.y + 10.0f;
		gfx.UpdateCameraView();
	}
	if ((inp)(IKEY_DOWN, SystemInput::HOLD))
	{
		gfx._cameraPos.y = gfx._cameraPos.y - 10.0f;
		gfx.UpdateCameraView();
	}
	if ((inp)(IKEY_LEFT, SystemInput::HOLD))
	{
		gfx._cameraPos.x = gfx._cameraPos.x - 10.0f;
		gfx.UpdateCameraView();
	}
	if ((inp)(IKEY_RIGHT, SystemInput::HOLD))
	{
		gfx._cameraPos.x = gfx._cameraPos.x + 10.0f;
		gfx.UpdateCameraView();
	}*/
	//gfx._cameraPos = colTransform._currentPosition;
	//gfx._cameraPos.z = 10.0;
	//gfx.UpdateCameraView();
	if (colRender.isEnabled())
	{

		if (colAnimation.isEnabled())
		{
			if ((inp)(IKEY_D, SystemInput::HOLD))
				colRender._flipX = false;
			else if ((inp)(IKEY_A, SystemInput::HOLD))
				colRender._flipX = true;

			if (! (colCollision.collisionFlag || colCollision.previousCollisionFlag))
			{
				colAnimation.PlayAnimation("midair", true);
			}
			else
			{
				if ((inp)(IKEY_D, SystemInput::HOLD))
				{
					if (gfx._testState == SystemGraphics::TRANSLATE)
						if (colAnimation._currentAnimationName == "idle" || !colAnimation.IsPlaying())
						{
							//comAudio.Play("WalkSound", true);
							colAnimation.PlayAnimation("walk", true);
						}
				}
				else if ((inp)(IKEY_A, SystemInput::HOLD))
				{
					if (gfx._testState == SystemGraphics::TRANSLATE)
						if (colAnimation._currentAnimationName == "idle" || !colAnimation.IsPlaying())
						{
							//comAudio.Play("WalkSound", true);
							colAnimation.PlayAnimation("walk", true);
						}
				}
				else if ((inp)(IKEY_A, SystemInput::RELEASE))
				{
					comAudio.StopSound();
				}
				else if ((inp)(IKEY_D, SystemInput::RELEASE))
				{
					comAudio.StopSound();
				}
				else if ((inp)(IMOUSE_LBUTTON, SystemInput::HOLD) && abletojump)
				{
					comAudio._volume = 1.0f;
					comAudio.SetVolume();
					comAudio.Play("BlastSound", false);
					colAnimation.PlayAnimation("ability", false);
				}
				else if (!colAnimation.IsPlaying() || colAnimation._currentAnimationName == "walk")
				{
					colAnimation.PlayAnimation("idle", true);
				}

				if (colPhy._velocity.y > 0 && colAnimation._currentAnimationName != "jump" 
					&& colAnimation._currentAnimationName != "midair")
					colAnimation.PlayAnimation("jump", false);
				else if (abletojump 
					&& (colAnimation._currentAnimationName == "midair" || colAnimation._currentAnimationName == "jump"))
				{
					colAnimation.PlayAnimation("land", false);
					colAnimation.SetAnimationFrame(4);
				}
			}
			/*
			if (colAnimation._currentAnimationName != "jump")
			{
				if ((inp)(IKEY_A, SystemInput::HOLD) && gfx._testState == SystemGraphics::TRANSLATE)
				{
					colAnimation.PlayAnimation("walk", true);
					colRender._flipX = true;
				}
				else if ((inp)(IKEY_D, SystemInput::HOLD) && gfx._testState == SystemGraphics::TRANSLATE)
				{
					colAnimation.PlayAnimation("walk", true);
					colRender._flipX = false;
				}
				else if ((inp)(IMOUSE_LBUTTON, SystemInput::HOLD))
				{
					colAnimation.PlayAnimation("ability", false);
				}
				else if (colAnimation._currentAnimationName != "idle")
				{
					colAnimation.PlayAnimation("idle", true);
				}
			}
			else
			{
				if (abletojump)
				{
					colAnimation.PlayAnimation("land", false);
				}
				
			}
			*/
			

		}
		/*
		if ((inp)(IKEY_W, SystemInput::HOLD) && gfx._testState == SystemGraphics::SCALE)
		{
			colRender.GetTransform()->_scale.y += 1.0f;
		}
		if ((inp)(IKEY_A, SystemInput::HOLD) && gfx._testState == SystemGraphics::SCALE)
		{
			colRender.GetTransform()->_scale.x -= 1.0f;
		}
		if ((inp)(IKEY_S, SystemInput::HOLD) && gfx._testState == SystemGraphics::SCALE)
		{
			colRender.GetTransform()->_scale.y -= 1.0f;
		}
		if ((inp)(IKEY_D, SystemInput::HOLD) && gfx._testState == SystemGraphics::SCALE)
		{
			colRender.GetTransform()->_scale.x += 1.0f;
		}

		if ((inp)(IKEY_A, SystemInput::HOLD) && gfx._testState == SystemGraphics::ROTATE)
		{
			colRender.GetTransform()->_rotation -= 15.0f;
		}
		if ((inp)(IKEY_D, SystemInput::HOLD) && gfx._testState == SystemGraphics::ROTATE)
		{
			colRender.GetTransform()->_rotation += 15.0f;
		}

		if ((inp)(IKEY_8, SystemInput::PRESS))
		{
			gfx._testState = SystemGraphics::TRANSLATE;
		}
		if ((inp)(IKEY_9, SystemInput::PRESS))
		{
			gfx._testState = SystemGraphics::ROTATE;
		}
		if ((inp)(IKEY_0, SystemInput::PRESS))
		{
			gfx._testState = SystemGraphics::SCALE;
		}
		
		if ((inp)(IKEY_Z, SystemInput::PRESS))
		{
			SaveScreenshot();
		}
		*/

	}

	//// PLAYER LOSS
	if ((inp)(IKEY_J, SystemInput::PRESS))
	{
		sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERDIED;
		sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;
	}
	//// PLAYER WON
	if ((inp)(IKEY_H, SystemInput::PRESS))
	{
		sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERWON;
		sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;
	}
	if (sysMgr._menustateNext == sysMgr.MENUSTATE_PLAYERDIED)
	{
		if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
		{
			sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
			sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_RELOAD;
		}
	}
	if (sysMgr._menustateNext == sysMgr.MENUSTATE_PLAYERWON)
	{
		if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
		{
			sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
			sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_NEXTLEVEL;
		}
	}

	// middle mouse btn to move camera to a position then back
	if ((inp)(IMOUSE_MBUTTON, SystemInput::HOLD))
	{
		camMgr.ActionCamTo(sysMgr._cursorPosition, 1.0f); // move camera to the cursor position, zoom in to 0.5, minimum 2s
	}
	camMgr.Update(dt);
	gfx.UpdateZoom();
	gfx.UpdateCameraView();
}

void FwEngine::ScriptPlayer::Free()
{
}

void FwEngine::ScriptPlayer::Destroy()
{
}

void FwEngine::ScriptPlayer::PlayerGetDamaged(int damage)
{

	_health -= damage;
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
		colTransform._currentPosition = originalPosition;
	}

}

void FwEngine::ScriptPlayer::OnCollisionExit(size_t)
{
}

void FwEngine::ScriptPlayer::OnCollisionStay(size_t id)
{
	ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);

	if (otherTransform._tag=="metal")
	{
		ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
		ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
		StackingResponse_NonElastic(&colPhy, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);

		if ((colCollision.collisionFlag & FwMath::COLLISION_LEFT) || (colCollision.previousCollisionFlag & FwMath::COLLISION_LEFT) ||
			(colCollision.collisionFlag & FwMath::COLLISION_RIGHT) || (colCollision.previousCollisionFlag & FwMath::COLLISION_RIGHT))
		{
			otherPhysics._velocity.x = -otherPhysics._velocity.x;
			colTransform._currentPosition = colPhy._previousPosition;
		}
		else if((colCollision.collisionFlag & FwMath::COLLISION_BOTTOM))
		{
			if (otherPhysics._velocity.y > 0)
			{
				(otherPhysics._velocity.y = -otherPhysics._velocity.y);
			}	
		}
		else if ((colCollision.collisionFlag & FwMath::COLLISION_TOP) || (colCollision.previousCollisionFlag & FwMath::COLLISION_TOP))
		{
			otherPhysics._velocity.y = 0;
		}
		
		
	}


	if (otherTransform._tag == "heavy metal")
	{
		ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
		ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);

		StackingResponse(&colPhy,&colTransform,&colCollision,&otherPhysics,&otherTransform,&otherCollision);

		colPhy._velocity.y = 0;

		if ((colCollision.collisionFlag & FwMath::COLLISION_LEFT) || (colCollision.previousCollisionFlag & FwMath::COLLISION_LEFT))
		{
			colAnimation.PlayAnimation("hangon", true);
			colRender._flipX = false;
			if ((inp)(IKEY_SPACE, SystemInput::PRESS))
			{
				colPhy.AddForce((DirectionalForce(FwMath::Vector3D(1.0f, 0.3f, 0), 10000.0f, 0.1f)));
				comAudio.Play("PipeSound", false);
				comAudio._volume = 0.5f;
				comAudio.SetVolume();
			}
		}
		if ((colCollision.collisionFlag & FwMath::COLLISION_RIGHT) || (colCollision.previousCollisionFlag & FwMath::COLLISION_RIGHT))
		{
			colAnimation.PlayAnimation("hangon", true);
			colRender._flipX = true;
			if ((inp)(IKEY_SPACE, SystemInput::PRESS))
			{
				colPhy.AddForce((DirectionalForce(FwMath::Vector3D(-1.0f, 0.3f, 0), 10000.0f, 0.1f)));
				comAudio.Play("PipeSound", false);
				comAudio._volume = 0.5f;
				comAudio.SetVolume();
			}
		}
	}
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

FwEngine::ScriptPlayer::PlayerCameraPositionManager::PlayerCameraPositionManager(FwMath::Vector3D& cameraPosition, FwMath::Vector3D& playercurrentPosition, float& zoom) :
	_cameraPosition(cameraPosition), _playercurrentPosition(playercurrentPosition), _zoom(zoom), _isAction(false), _destinationPos(), _destionationZoom(0.0f), _timer(0.0f),
	_MaxDistance(5000.0f), _MinDistance(1.0f), _MaxVelocity(8000.0f), _MinVelocity(10.0f), _defaultZoom(1.0f), _verticeA(FwMath::Vector3D()), _verticeB(FwMath::Vector3D())
{
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::ActionCamTo(FwMath::Vector3D position, float zoom, float time)
{
	_isAction = true;
	_destinationPos = position;
	_destionationZoom = zoom;
	_timer = time;
}

void FwEngine::ScriptPlayer::PlayerCameraPositionManager::Update(float dt)
{
	float minZoom = 0.01f;
	float maxZoom = 500.0f;
	float closeDistance = 100.0f;

	auto moveCamTo = [&](FwMath::Vector3D& dest)
	{
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

			FwMath::Vector3D velocity(vel_x, vel_y, 0.0f);

			_cameraPosition += velocity * dt;

			return (velocity.x == 0.0f && velocity.y == 0.0f);
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
			_zoom += vel_z * dt;
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
		FwMath::Vector3D verticeA(_verticeA.x + GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeA.y + GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);
		FwMath::Vector3D verticeB(_verticeB.x - GRAPHICS->GetWidth() * 0.5f * _zoom, _verticeB.y - GRAPHICS->GetHeight() * 0.5f * _zoom, 0.0f);

		if (destination.x < verticeA.x)
			destination.x = verticeA.x;

		if (destination.x > verticeB.x)
			destination.x = verticeB.x;

		if ((abs(_verticeB.x - _verticeA.x)) < (GRAPHICS->GetWidth() * _zoom))
			destination.x = (verticeA.x + verticeB.x) * 0.5f;

		if (destination.y < verticeA.y)
			destination.y = verticeA.y;

		if (destination.y > verticeB.y)
			destination.y = verticeB.y;

		if ((abs(_verticeB.y - _verticeA.y)) < (GRAPHICS->GetHeight() * _zoom))
			destination.y = (verticeA.y + verticeB.y) * 0.5f;

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
}
