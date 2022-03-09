/******************************************************************************/
/*!
\file		Reaction.cpp
\project	Aris
\author 	Primary: Wang YiDi
\par    	email: w.yidi\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Reaction.h"
#include <iostream>
#include "Math_CollisionSAT.h"
class DirectionalForce;

void FwEngine::StackingResponse
(	ComponentPhysics* phys1, ComponentTransform* trans1, 
	ComponentCollision* coll1, ComponentPhysics* phys2, 
	ComponentTransform* trans2, ComponentCollision* coll2)
{
	if (!phys1->isEnabled() && phys2->isEnabled())
	{
		std::swap(phys1, phys2);
		std::swap(trans1, trans2);
	}

	if (phys1->isEnabled() && !phys2->isEnabled())
	{
		float objALeft = abs(coll1->_vertexA.x * trans1->_scale.x);
		float objARight = abs(coll1->_vertexB.x * trans1->_scale.x);
		float objABot = abs(coll1->_vertexA.y * trans1->_scale.y);
		float objATop = abs(coll1->_vertexB.y * trans1->_scale.y);

		float objBLeft = abs(coll2->_vertexA.x * trans2->_scale.x);
		float objBRight = abs(coll2->_vertexB.x * trans2->_scale.x);
		float objBBot = abs(coll2->_vertexA.y * trans2->_scale.y);
		float objBTop = abs(coll2->_vertexB.y * trans2->_scale.y);

		int flag = 0;
		FwMath::checkCollisionSides(*coll1,*trans1,*coll2,*trans2,flag);
		//if the object 1 is on top
		if (flag & FwMath::COLLISION_BOTTOM)
		{

			phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
			phys1->SetPosition({ trans1->_currentPosition.x, trans2->_currentPosition.y + objBTop + objABot, trans1->_currentPosition.z });
		}
		//if object 1 is at bottom
		if(flag & FwMath::COLLISION_TOP)
		{
			phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
			phys1->SetPosition({ trans1->_currentPosition.x, trans2->_currentPosition.y - objATop - objBBot, trans1->_currentPosition.z });
		}
		//if object 1 is at left
		if (flag & FwMath::COLLISION_RIGHT)
		{
			phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
			phys1->SetPosition({ trans2->_currentPosition.x - objBLeft - objARight , trans1->_currentPosition.y, trans1->_currentPosition.z });
		} 
		//if object 1 is at right
		if (flag & FwMath::COLLISION_LEFT)
		{
			phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
			phys1->SetPosition({ trans2->_currentPosition.x + objBRight + objALeft + 1.0f , trans1->_currentPosition.y, trans1->_currentPosition.z });
		}
	}
	else if (phys1->isEnabled())
	{
		/*
		elastic collision
		0.5f * m_1 * v_1i * v_1i + 0.5f m_2 * v_2i * v_2i = 0.5f * m_1 * v_1f * v_1f + 0.5f m_2 * v_2f * v_2f

		inelastic collision
		v_f = (m_1 * v_1 + m_2 * v_2) / (m_1 + m_2)
		*/
		float elasticity = 1.0f;
		int side = 0;

		//phys1->SetPosition(phys1->_previousPosition);
		//phys2->SetPosition(phys2->_previousPosition);

		//const float SMALL = abs( (trans1->_scale.x + trans1->_scale.y) * 0.0001f );
		const float SMALL_1X = abs(phys1->_velocity.x * 0.0001f);
		const float SMALL_1Y = abs(phys1->_velocity.y * 0.0001f);
		const float SMALL_2X = abs(phys2->_velocity.x * 0.0001f);
		const float SMALL_2Y = abs(phys2->_velocity.y * 0.0001f);


		float objALeft = abs(coll1->_vertexA.x * trans1->_scale.x);
		float objARight = abs(coll1->_vertexB.x * trans1->_scale.x);
		float objABot = abs(coll1->_vertexA.y * trans1->_scale.y);
		float objATop = abs(coll1->_vertexB.y * trans1->_scale.y);

		float objBLeft = abs(coll2->_vertexA.x * trans2->_scale.x);
		float objBRight = abs(coll2->_vertexB.x * trans2->_scale.x);
		float objBBot = abs(coll2->_vertexA.y * trans2->_scale.y);
		float objBTop = abs(coll2->_vertexB.y * trans2->_scale.y);

		float distX = abs(trans1->_currentPosition.x - trans2->_currentPosition.x);
		float distY = abs(trans1->_currentPosition.y - trans2->_currentPosition.y);


		//FwEngine::Vector3D velocityRelative = phys1->_velocity - phys2->_velocity;

		float diffA = abs(distX - objALeft - objBRight);
		float diffB = abs(distX - objBLeft - objARight);

		float diffC = abs(distY - objATop - objBBot);
		float diffD = abs(distY - objBTop - objABot);

		float diff = diffA < diffB ? diffA : diffB;


		if (diff < diffC && diff < diffD)
		{
			if ((trans1->_currentPosition.x - trans2->_currentPosition.x) > 0)
			{
				side = 1;
				// obj 1 is on the right
				if (phys2->isEnabled())
				{
					trans1->_currentPosition.x = (
						phys2->_previousPosition.x + objALeft + objBRight );

					trans2->_currentPosition.x = (
						phys1->_previousPosition.x - objBLeft - objARight );
				}
				else
				{
					phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
					phys1->SetPosition({ phys1->_previousPosition.x, trans1->_currentPosition.y, trans1->_currentPosition.z });
				}
			}
			else
			{
				side = 2;
				// obj 1 is on the left
				if (phys2->isEnabled())
				{
					trans1->_currentPosition.x = (
						phys2->_previousPosition.x - objARight - objBLeft );
					trans2->_currentPosition.x = (
						phys1->_previousPosition.x + objBRight + objALeft );
				}
				else
				{
					phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
					phys1->SetPosition({ phys1->_previousPosition.x, trans1->_currentPosition.y, trans1->_currentPosition.z });
				}
			}
		}
		else if ((trans1->_currentPosition.y - trans2->_currentPosition.y) > 0)
		{
			side = 3;
			// obj 1 is on the top
			if (phys2->isEnabled())
			{
				trans1->_currentPosition.y = (
					//trans2->_currentPosition.y + objABot + objBTop );
					phys2->_previousPosition.y + objABot + objBTop );
				trans2->_currentPosition.y = (
					phys1->_previousPosition.y - objBTop - objABot );
			}
			else
			{
				phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
				phys1->SetPosition({ trans1->_currentPosition.x, phys1->_previousPosition.y, trans1->_currentPosition.z });
			}
		}
		else
		{
			side = 4;
			// obj 1 is on the bottom
			if (phys2->isEnabled())
			{
				trans1->_currentPosition.y = (
					phys2->_previousPosition.y - objATop - objBBot );
				trans2->_currentPosition.y = (
					phys1->_previousPosition.y + objBBot + objATop );
			}
			else
			{
				phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
				phys1->SetPosition({ trans1->_currentPosition.x, phys1->_previousPosition.y, trans1->_currentPosition.z });
			}
		}

		if (phys2->isEnabled())
		{
			if (elasticity == 0.0f)
			{
				FwEngine::Vector3D velocityFinal;
				velocityFinal = (phys1->_mass * phys1->_velocity + phys2->_mass * phys2->_velocity) / (phys1->_mass + phys2->_mass);
				phys1->SetVelocity(velocityFinal);
				phys2->SetVelocity(velocityFinal);
			}


			if (elasticity > 0.0f)
			{
				//FwEngine::Vector3D initHmvs_1 = 0.5f * phys1->_mass * phys1->_velocity * phys1->_velocity;
				//FwEngine::Vector3D initHmvs_2 = 0.5f * phys2->_mass * phys2->_velocity * phys2->_velocity;
				float vel1;
				float vel2;
				float initHmvs_1;
				float initHmvs_2;
				if (side == 1 || side == 2)
				{
					//side

					initHmvs_1 = 0.5f * phys1->_mass * phys1->_velocity.x * phys1->_velocity.x;
					initHmvs_2 = 0.5f * phys2->_mass * phys2->_velocity.x * phys2->_velocity.x;
					vel1 = phys1->_velocity.x * phys1->_velocity.x;
					vel2 = phys2->_velocity.x * phys2->_velocity.x;

					if (vel1 > vel2)
					{
						// 1 > 2, assume 1 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys2->_mass);
						if (phys1->_velocity.x < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys2->SetVelocity(FwEngine::Vector3D(vel_sqr, phys2->_velocity.y, 0.0f));
						phys1->SetVelocity(FwEngine::Vector3D(0.0f, phys2->_velocity.y, 0.0f));
					}
					else
					{
						// 1 < 2, assume 2 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys1->_mass);
						if (phys2->_velocity.x < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys1->SetVelocity(FwEngine::Vector3D(vel_sqr, phys1->_velocity.y, 0.0f));
						phys2->SetVelocity(FwEngine::Vector3D(0.0f, phys2->_velocity.y, 0.0f));
					}
				}
				else
				{
					// top/bot

					initHmvs_1 = 0.5f * phys1->_mass * phys1->_velocity.y * phys1->_velocity.y;
					initHmvs_2 = 0.5f * phys2->_mass * phys2->_velocity.y * phys2->_velocity.y;
					vel1 = phys1->_velocity.y * phys1->_velocity.y;
					vel2 = phys2->_velocity.y * phys2->_velocity.y;

					if (vel1 > vel2)
					{
						// 1 > 2, assume 1 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys2->_mass);
						if (phys1->_velocity.y < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys2->SetVelocity(FwEngine::Vector3D(phys2->_velocity.x, vel_sqr, 0.0f));
						phys1->SetVelocity(FwEngine::Vector3D(phys1->_velocity.x, 0.0f, 0.0f));
					}
					else
					{
						// 1 < 2, assume 2 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys1->_mass);
						if (phys2->_velocity.y < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys1->SetVelocity(FwEngine::Vector3D(phys1->_velocity.x, vel_sqr, 0.0f));
						phys2->SetVelocity(FwEngine::Vector3D(phys2->_velocity.x, 0.0f, 0.0f));
					}
				}
			}
		}
	}
}

void FwEngine::StackingResponse_Elastic
(	ComponentPhysics* phys1, ComponentTransform* trans1,
	ComponentCollision* coll1, ComponentPhysics* phys2,
	ComponentTransform* trans2, ComponentCollision* coll2)
{
	if (!phys1->isEnabled() && phys2->isEnabled())
	{
		std::swap(phys1, phys2);
		std::swap(trans1, trans2);
	}

	if (phys1->isEnabled() && !phys2->isEnabled())
	{
		float objALeft = abs(coll1->_vertexA.x * trans1->_scale.x);
		float objARight = abs(coll1->_vertexB.x * trans1->_scale.x);
		float objABot = abs(coll1->_vertexA.y * trans1->_scale.y);
		float objATop = abs(coll1->_vertexB.y * trans1->_scale.y);

		float objBLeft = abs(coll2->_vertexA.x * trans2->_scale.x);
		float objBRight = abs(coll2->_vertexB.x * trans2->_scale.x);
		float objBBot = abs(coll2->_vertexA.y * trans2->_scale.y);
		float objBTop = abs(coll2->_vertexB.y * trans2->_scale.y);

		int flag = 0;
		FwMath::checkCollisionSides(*coll1, *trans1, *coll2, *trans2, flag);
		//if the object 1 is on top
		if (flag & FwMath::COLLISION_BOTTOM)
		{

			phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
			phys1->SetPosition({ trans1->_currentPosition.x, trans2->_currentPosition.y + objBTop + objABot + 0.01f, trans1->_currentPosition.z });
		}
		//if object 1 is at bottom
		if (flag & FwMath::COLLISION_TOP)
		{
			phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
			phys1->SetPosition({ trans1->_currentPosition.x, trans2->_currentPosition.y - objATop - objBBot - 0.01f, trans1->_currentPosition.z });
		}
		//if object 1 is at left
		if (flag & FwMath::COLLISION_RIGHT)
		{
			phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
			phys1->SetPosition({ trans2->_currentPosition.x - objBLeft - objARight - 0.01f, trans1->_currentPosition.y, trans1->_currentPosition.z });
		}
		//if object 1 is at right
		if (flag & FwMath::COLLISION_LEFT)
		{
			phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
			phys1->SetPosition({ trans2->_currentPosition.x + objBRight + objALeft + 0.01f , trans1->_currentPosition.y, trans1->_currentPosition.z });
		}
	}
	else if (phys1->isEnabled())
	{
		/*
		elastic collision
		0.5f * m_1 * v_1i * v_1i + 0.5f m_2 * v_2i * v_2i = 0.5f * m_1 * v_1f * v_1f + 0.5f m_2 * v_2f * v_2f

		inelastic collision
		v_f = (m_1 * v_1 + m_2 * v_2) / (m_1 + m_2)
		*/
		float elasticity = 1.0f;
		int side = 0;

		//phys1->SetPosition(phys1->_previousPosition);
		//phys2->SetPosition(phys2->_previousPosition);

		//const float SMALL = abs( (trans1->_scale.x + trans1->_scale.y) * 0.0001f );
		const float SMALL_1X = abs(phys1->_velocity.x * 0.0001f);
		const float SMALL_1Y = abs(phys1->_velocity.y * 0.0001f);
		const float SMALL_2X = abs(phys2->_velocity.x * 0.0001f);
		const float SMALL_2Y = abs(phys2->_velocity.y * 0.0001f);


		float objALeft = abs(coll1->_vertexA.x * trans1->_scale.x);
		float objARight = abs(coll1->_vertexB.x * trans1->_scale.x);
		float objABot = abs(coll1->_vertexA.y * trans1->_scale.y);
		float objATop = abs(coll1->_vertexB.y * trans1->_scale.y);

		float objBLeft = abs(coll2->_vertexA.x * trans2->_scale.x);
		float objBRight = abs(coll2->_vertexB.x * trans2->_scale.x);
		float objBBot = abs(coll2->_vertexA.y * trans2->_scale.y);
		float objBTop = abs(coll2->_vertexB.y * trans2->_scale.y);

		float distX = abs(trans1->_currentPosition.x - trans2->_currentPosition.x);
		float distY = abs(trans1->_currentPosition.y - trans2->_currentPosition.y);


		//FwEngine::Vector3D velocityRelative = phys1->_velocity - phys2->_velocity;

		float diffA = abs(distX - objALeft - objBRight);
		float diffB = abs(distX - objBLeft - objARight);

		float diffC = abs(distY - objATop - objBBot);
		float diffD = abs(distY - objBTop - objABot);

		float diff = diffA < diffB ? diffA : diffB;


		if (diff < diffC && diff < diffD)
		{
			if ((trans1->_currentPosition.x - trans2->_currentPosition.x) > 0)
			{
				side = 1;
				// obj 1 is on the right
				if (phys2->isEnabled())
				{
					trans1->_currentPosition.x = (
						phys2->_previousPosition.x + objALeft + objBRight);

					trans2->_currentPosition.x = (
						phys1->_previousPosition.x - objBLeft - objARight);
				}
				else
				{
					phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
					phys1->SetPosition({ phys1->_previousPosition.x, trans1->_currentPosition.y, trans1->_currentPosition.z });
				}
			}
			else
			{
				side = 2;
				// obj 1 is on the left
				if (phys2->isEnabled())
				{
					trans1->_currentPosition.x = (
						phys2->_previousPosition.x - objARight - objBLeft);
					trans2->_currentPosition.x = (
						phys1->_previousPosition.x + objBRight + objALeft);
				}
				else
				{
					phys1->SetVelocity({ 0, phys1->_velocity.y, phys1->_velocity.z });
					phys1->SetPosition({ phys1->_previousPosition.x, trans1->_currentPosition.y, trans1->_currentPosition.z });
				}
			}
		}
		else if ((trans1->_currentPosition.y - trans2->_currentPosition.y) > 0)
		{
			side = 3;
			// obj 1 is on the top
			if (phys2->isEnabled())
			{
				// obj 1 is on the top
				trans1->_currentPosition.y = (
					trans2->_currentPosition.y + objABot + objBTop + 0.01f);

				phys1->_velocity.y *= 0.4f;
			}
			else
			{
				phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
				phys1->SetPosition({ trans1->_currentPosition.x, phys1->_previousPosition.y, trans1->_currentPosition.z });
			}
		}
		else
		{
			side = 4;
			// obj 1 is on the bottom
			if (phys2->isEnabled())
			{
				// obj 1 is on the bottom
				trans2->_currentPosition.y = (
					phys1->_previousPosition.y + objBBot + objATop);

				phys1->_velocity.y = 0;
				phys2->_velocity.y = 0;
			}
			else
			{
				phys1->SetVelocity({ phys1->_velocity.x, 0, phys1->_velocity.z });
				phys1->SetPosition({ trans1->_currentPosition.x, phys1->_previousPosition.y, trans1->_currentPosition.z });
			}
		}

		if (phys2->isEnabled())
		{
			if (elasticity == 0.0f)
			{
				FwEngine::Vector3D velocityFinal;
				velocityFinal = (phys1->_mass * phys1->_velocity + phys2->_mass * phys2->_velocity) / (phys1->_mass + phys2->_mass);
				phys1->SetVelocity(velocityFinal);
				phys2->SetVelocity(velocityFinal);
			}


			if (elasticity > 0.0f)
			{
				//FwEngine::Vector3D initHmvs_1 = 0.5f * phys1->_mass * phys1->_velocity * phys1->_velocity;
				//FwEngine::Vector3D initHmvs_2 = 0.5f * phys2->_mass * phys2->_velocity * phys2->_velocity;
				float vel1;
				float vel2;
				float initHmvs_1;
				float initHmvs_2;
				if (side == 1 || side == 2)
				{
					//side

					initHmvs_1 = 0.5f * phys1->_mass * phys1->_velocity.x * phys1->_velocity.x;
					initHmvs_2 = 0.5f * phys2->_mass * phys2->_velocity.x * phys2->_velocity.x;
					vel1 = phys1->_velocity.x * phys1->_velocity.x;
					vel2 = phys2->_velocity.x * phys2->_velocity.x;

					if (vel1 > vel2)
					{
						// 1 > 2, assume 1 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys2->_mass);
						if (phys1->_velocity.x < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys2->SetVelocity(FwEngine::Vector3D(vel_sqr, phys2->_velocity.y, 0.0f));
						phys1->SetVelocity(FwEngine::Vector3D(0.0f, phys2->_velocity.y, 0.0f));
					}
					else
					{
						// 1 < 2, assume 2 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys1->_mass);
						if (phys2->_velocity.x < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys1->SetVelocity(FwEngine::Vector3D(vel_sqr, phys1->_velocity.y, 0.0f));
						phys2->SetVelocity(FwEngine::Vector3D(0.0f, phys2->_velocity.y, 0.0f));
					}
				}
				else
				{
					// top/bot

					initHmvs_1 = 0.5f * phys1->_mass * phys1->_velocity.y * phys1->_velocity.y;
					initHmvs_2 = 0.5f * phys2->_mass * phys2->_velocity.y * phys2->_velocity.y;
					vel1 = phys1->_velocity.y * phys1->_velocity.y;
					vel2 = phys2->_velocity.y * phys2->_velocity.y;

					if (vel1 > vel2)
					{
						// 1 > 2, assume 1 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys2->_mass);
						if (phys1->_velocity.y < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys2->SetVelocity(FwEngine::Vector3D(phys2->_velocity.x, vel_sqr, 0.0f));
						phys1->SetVelocity(FwEngine::Vector3D(phys1->_velocity.x, 0.0f, 0.0f));
					}
					else
					{
						// 1 < 2, assume 2 becomes stationary
						float vel_sqr = (initHmvs_1 + initHmvs_2) / (0.5f * phys1->_mass);
						if (phys2->_velocity.y < 0)
						{
							vel_sqr = -sqrt(vel_sqr);
						}
						else
						{
							vel_sqr = sqrt(vel_sqr);
						}
						phys1->SetVelocity(FwEngine::Vector3D(phys1->_velocity.x, vel_sqr, 0.0f));
						phys2->SetVelocity(FwEngine::Vector3D(phys2->_velocity.x, 0.0f, 0.0f));
					}
				}
			}
		}
	}
}

void FwEngine::StackingResponse_NonElastic(
	ComponentPhysics* phy1, ComponentTransform* trans1, ComponentCollision* coll1, 
	ComponentPhysics* phy2, ComponentTransform* trans2, ComponentCollision* coll2)
{
	float objALeft = abs(coll1->_vertexA.x * trans1->_scale.x);
	float objARight = abs(coll1->_vertexB.x * trans1->_scale.x);
	float objABot = abs(coll1->_vertexA.y * trans1->_scale.y);
	float objATop = abs(coll1->_vertexB.y * trans1->_scale.y);

	float objBLeft = abs(coll2->_vertexA.x * trans2->_scale.x);
	float objBRight = abs(coll2->_vertexB.x * trans2->_scale.x);
	float objBBot = abs(coll2->_vertexA.y * trans2->_scale.y);
	float objBTop = abs(coll2->_vertexB.y * trans2->_scale.y);

	float distX = abs(trans1->_currentPosition.x - trans2->_currentPosition.x);
	float distY = abs(trans1->_currentPosition.y - trans2->_currentPosition.y);


	//FwEngine::Vector3D velocityRelative = phys1->_velocity - phys2->_velocity;

	float diffA = abs(distX - objALeft - objBRight);
	float diffB = abs(distX - objBLeft - objARight);

	float diffC = abs(distY - objATop - objBBot);
	float diffD = abs(distY - objBTop - objABot);

	float diff = diffA < diffB ? diffA : diffB;


	if (diff < diffC && diff < diffD)
	{
		//object1 is on the left

		trans1->_currentPosition.x = phy1->_previousPosition.x;
		trans2->_currentPosition.x = phy2->_previousPosition.x;

		distX = abs(trans1->_currentPosition.x - trans2->_currentPosition.x);
		diffA = distX - objALeft - objBRight;
		diffB = distX - objBLeft - objARight;

		if (trans1->_currentPosition.x - trans2->_currentPosition.x > 0)
		{
			if (diffA < 0)
			{
				trans1->_currentPosition.x = (
					phy2->_previousPosition.x + objALeft + objBRight);
				trans2->_currentPosition.x = (
					phy1->_previousPosition.x - objALeft - objBRight);
			}
		}
		//object1 is on the right
		else
		{
			if (diffB < 0)
			{
				trans1->_currentPosition.x = (
					phy2->_previousPosition.x - objARight - objBLeft);
				trans2->_currentPosition.x = (
					phy1->_previousPosition.x + objBRight + objALeft);
			}
		}
	}
	else if ((trans1->_currentPosition.y - trans2->_currentPosition.y) > 0)
	{
		// obj 1 is on the top
		trans1->_currentPosition.y = (
			trans2->_currentPosition.y + objABot + objBTop + 0.01f);
			
		phy1->_velocity.y *= 0.4f;
	}
	else
	{
		// obj 1 is on the bottom
		trans2->_currentPosition.y = (
			phy1->_previousPosition.y + objBBot + objATop);

		phy1->_velocity.y = 0;
		phy2->_velocity.y = 0;
	}
}

void FwEngine::StackingResponse_forcePushing(
	ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1, 
	ComponentPhysics* phys2, ComponentTransform* trans2, ComponentCollision* coll2)
{
	if (!phys1->isEnabled() && phys2->isEnabled())
	{
		std::swap(phys1, phys2);
		std::swap(trans1, trans2);
	}

	if (phys1->isEnabled() && !phys2->isEnabled())
	{
		FwMath::Vector3D relative_velocity = phys1->_velocity;
		FwMath::Vector3D axis1 = (
			coll1->_vertexB - coll1->_vertexA) * trans1->_scale / 2;
		FwMath::Vector3D axis2 = (coll2->_vertexB - coll2->_vertexA) * trans1->_scale / 2;
		FwMath::Vector3D distance_inbetween = trans2->_currentPosition - trans1->_currentPosition;
		FwMath::Vector3D penetration = (axis1 + axis2) - FwMath::Vector3D(fabsf(distance_inbetween.x), fabsf(distance_inbetween.y), 0);
		if (penetration.x < penetration.y)
		{
			trans1->_currentPosition = phys1->_previousPosition;
			phys1->_velocity.x = 0;
		}
		else
		{
			trans1->_currentPosition = phys1->_previousPosition;
			phys1->_velocity.y = 0;
		}
	}
}

void FwEngine::NoPhysicsUnpenetrableObject(
	ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1,
	ComponentTransform* trans2, ComponentCollision* coll2)
{
	(void)coll2;

	char Check = coll1->Collision_Check_List.find(trans2->_tag)->second;
	if ((Check == 'L' )||( Check == 'R'))
	{
		trans1->_currentPosition.x = phys1->_previousPosition.x;
		phys1->_velocity.x = 0;
	}
	if ((Check == 'B') || (Check == 'T'))
	{
		trans1->_currentPosition.y = phys1->_previousPosition.y;
		//trans1->_currentPosition.y = trans2->_currentPosition.y + abs(trans2->_scale.y * coll2->_vertexB.y);
		phys1->_velocity.y = 0;
	}
}
