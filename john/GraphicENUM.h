/******************************************************************************/
/*!
\file		GraphicENUM.h
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
enum GraphicLayer
{
	BACKGROUND,
	DEFAULT, //Tilemap layer or any platform layers
	PLAYER,
	FOREGROUND, 
	UI,

	TOTAL_LAYER = 5
};

enum MeshType
{
	SPRITE,
	FONT,
	PARTICLES,

	TOTAL_MESH = 3
};