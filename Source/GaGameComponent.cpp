/**************************************************************************
*
* File:		GaGameComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaGameComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"
#include "System/Sound/SsCore.h"

#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaGameComponent );

void GaGameComponent::StaticRegisterClass()
{
	ReRegisterClass< GaGameComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaGameComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaGameComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaGameComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
	playSound( "MusicGame", BcTrue );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaGameComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// playSound
void GaGameComponent::playSound( std::string Name, BcBool Looping )
{
	auto SoundEmitter = getParentEntity()->getComponentAnyParentByType< ScnSoundEmitterComponent >();

	ScnSoundRef Sound;
	if( CsCore::pImpl()->requestResource( "sounds", Name, Sound ) )
	{
		SoundEmitter->play( Sound );
	}
}

//////////////////////////////////////////////////////////////////////////
// stopAllSounds
void GaGameComponent::stopAllSounds()
{
	auto SoundEmitter = getParentEntity()->getComponentAnyParentByType< ScnSoundEmitterComponent >();
	SoundEmitter->stopAll();
}
