/**************************************************************************
*
* File:		MainGame.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		Main Game Entrypoint.
*		
*
*
* 
**************************************************************************/

#include "Psybrus.h"

#include "System/Content/CsCore.h"
#include "System/Os/OsCore.h"
#include "System/Scene/ScnCore.h"

//////////////////////////////////////////////////////////////////////////
// GPsySetupParams
PsySetupParams GPsySetupParams( "Hectic Game Jam 3: Underwater", psySF_GAME_DEV, 1.0f / 60.0f );	

//////////////////////////////////////////////////////////////////////////
// PsyGameInit
void PsyGameInit()
{

}

//////////////////////////////////////////////////////////////////////////
// PsyLaunchGame
void PsyLaunchGame()
{
	ScnEntitySpawnParams ScreenEntityParams = 
	{
		"default", "ScreenEntity", "ScreenEntity_0",
		MaMat4d(),
		nullptr,
		[]( ScnEntity* ParentEntity )
		{
			ScnEntitySpawnParams CameraEntityParams =
			{
				"default", "CameraEntity", "CameraEntity_0",
				MaMat4d(),
				ParentEntity,
				nullptr
			};

			ScnCore::pImpl()->spawnEntity( CameraEntityParams );

			ScnEntitySpawnParams GameEntityParams =
			{
				"default", "GameEntity", "GameEntity_0",
				MaMat4d(),
				ParentEntity,
				[]( ScnEntity* ParentEntity )
				{
					ScnEntitySpawnParams TankEntityParams =
					{
						"default", "TankEntity", "TankEntity_0",
						MaMat4d(),
						ParentEntity,
					};

					ScnCore::pImpl()->spawnEntity( TankEntityParams );

					ScnEntitySpawnParams PlayerEntityParams =
					{
						"default", "PlayerEntity", "PlayerEntity_0",
						MaMat4d(),
						ParentEntity,
						nullptr
					};

					ScnCore::pImpl()->spawnEntity( PlayerEntityParams );
				}
			};

			ScnCore::pImpl()->spawnEntity( GameEntityParams );
		}
	};

	ScnCore::pImpl()->spawnEntity( ScreenEntityParams );
}
