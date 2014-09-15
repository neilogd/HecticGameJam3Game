/**************************************************************************
*
* File:		GaFoodComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaFoodComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnSpriteComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"
#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaFoodComponent );

void GaFoodComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Size_", &GaFoodComponent::Size_, DsCore::DsCoreSerialised ),
		new ReField( "Life_", &GaFoodComponent::Life_, DsCore::DsCoreSerialised ),
		new ReField( "Sprite_", &GaFoodComponent::Sprite_, DsCore::DsCoreSerialised ),
		new ReField( "Rotation_", &GaFoodComponent::Rotation_, DsCore::DsCoreSerialised ),
		new ReField( "Sprites_", &GaFoodComponent::Sprites_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "SpriteSizes_", &GaFoodComponent::SpriteSizes_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
	};
	
	ReRegisterClass< GaFoodComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 1 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFoodComponent::initialise()
{
	Size_ = 0.0f;
	Life_ = 1.0f;
	Sprite_ = 0;
	Rotation_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFoodComponent::initialise( const Json::Value& Object )
{
	initialise();

	Size_ = BcF32( Object[ "size" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaFoodComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	Life_ -= Tick * 0.1f;
	Rotation_ += Tick;
	// Update sprite sizes.
	for( BcU32 Idx = 0; Idx < Sprites_.size(); ++Idx )
	{
		Sprites_[ Idx ]->setSpriteIndex( Sprite_ );
		Sprites_[ Idx ]->setRotation( Rotation_ );
		Sprites_[ Idx ]->setSize( SpriteSizes_[ Idx ] * Size_ );
		Sprites_[ Idx ]->setColour( RsColour( 1.0f, 1.0f, 1.0f, BcClamp( Life_ * 2.0f, 0.0f, 1.0f ) ) );
	}

	if( Life_ < 0.0f )
	{
		ScnCore::pImpl()->removeEntity( getParentEntity() );
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaFoodComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Sprite_ = BcRandom::Global.randRange( 0, 17 );


	// Grab all sprites for scaling.
	BcU32 Idx = 0;
	while( auto SpriteComponent = Parent->getComponentByType< ScnSpriteComponent >( Idx++ ) )
	{
		Sprites_.push_back( SpriteComponent );
		SpriteSizes_.push_back( SpriteComponent->getSize() );
	}
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaFoodComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// tryEat
BcF32 GaFoodComponent::tryEat( BcF32 Amount )
{
	if( Amount < Size_ )
	{
		Size_ -= Amount;
		return Amount;
	}

	ScnCore::pImpl()->removeEntity( getParentEntity() );

	return Size_;
}
