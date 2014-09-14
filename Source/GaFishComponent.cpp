/**************************************************************************
*
* File:		GaFishComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaFishComponent.h"
#include "GaFoodComponent.h"
#include "GaSwarmManagerComponent.h"
#include "GaSwarmElementComponent.h"

#include "System/Scene/Rendering/ScnSpriteComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"
 
//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaFishComponent );

void GaFishComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Size_", &GaFishComponent::Size_, DsCore::DsCoreSerialised ),
		new ReField( "SizeIncreaseMultiplier_", &GaFishComponent::SizeIncreaseMultiplier_, DsCore::DsCoreSerialised ),
		new ReField( "EatDistance_", &GaFishComponent::EatDistance_, DsCore::DsCoreSerialised ),
		new ReField( "EatSpeed_", &GaFishComponent::EatSpeed_, DsCore::DsCoreSerialised ),
		new ReField( "SwarmManager_", &GaFishComponent::SwarmManager_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "Sprites_", &GaFishComponent::Sprites_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "SpriteSizes_", &GaFishComponent::SpriteSizes_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised )
	};
	
	ReRegisterClass< GaFishComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 10 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFishComponent::initialise()
{
	Size_ = 0.0f;
	SizeIncreaseMultiplier_ = 1.0f;
	EatDistance_ = 0.0f;
	SwarmManager_ = nullptr;
	RotationTimer_ = 0.0f;
	RotationSpeed_ = 1.0f;
	Rotation_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFishComponent::initialise( const Json::Value& Object )
{
	initialise();

	Size_ = BcF32( Object[ "size" ].asDouble() );
	SizeIncreaseMultiplier_ = BcF32( Object[ "sizeincreasemultiplier" ].asDouble() );
	EatDistance_ = BcF32( Object[ "eatdistance" ].asDouble() );
	EatSpeed_ = BcF32( Object[ "eatspeed" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaFishComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	if( SwarmManager_ != nullptr )
	{
		auto Foods = SwarmManager_->getNearbyUnits( 
			MaVec2d( 
				getParentEntity()->getWorldPosition().x(),
				getParentEntity()->getWorldPosition().y() ), 1, GaSwarmManagerComponent::FOOD );

		if( Foods.size() > 0 )
		{
			auto FoodSwarmElement = Foods[ 0 ];
			auto Distance = 
				( FoodSwarmElement->getParentEntity()->getWorldPosition() - getParentEntity()->getWorldPosition() ).magnitude();

			if( Distance < EatDistance_ )
			{
				auto Food = FoodSwarmElement->getParentEntity()->getComponentByType< GaFoodComponent >();
				auto AmountAte = Food->tryEat( Tick * EatSpeed_ );

				Size_ += AmountAte * SizeIncreaseMultiplier_;
			}
		}
	}

	// Do the flip and stuff.
	auto Element = getParentEntity()->getComponentByType< GaSwarmElementComponent >();
	MaVec2d Velocity;
	if( Element != nullptr )
	{
		Velocity = Element->getVelocity();
		BcF32 MoveTurnVelocity = 1.0f;
		if( Element->getVelocity().x() < -MoveTurnVelocity )
		{
			XScale_ += Tick * 16.0f;
		}
		else if( Element->getVelocity().x() > MoveTurnVelocity )
		{
			XScale_ -= Tick * 16.0f;
		}

		XScale_ = BcClamp( XScale_, -1.0f, 1.0f );
	}
	
	// Update sprite sizes.
	for( BcU32 Idx = 0; Idx < Sprites_.size(); ++Idx )
	{
		auto NewSize = SpriteSizes_[ Idx ] * Size_;

		NewSize.x( NewSize.x() * XScale_ );

		// Calculate angle to point at.
		auto FishVelRot = BcAtan2( Velocity.y(), BcAbs( Velocity.x() ) ) * 0.3f;

		// Interpolate for turning.
		FishVelRot = BcLerp( -FishVelRot, FishVelRot, ( XScale_ + 1.0f ) * 0.5f );

		Sprites_[ Idx ]->setSize( NewSize );
		Rotation_ = 
			( Rotation_ * 0.1f ) + 
			( FishVelRot + ( BcSin( RotationTimer_ ) * 0.02f ) * 0.9f );
		Sprites_[ Idx ]->setRotation( Rotation_ );
	}

	// Rotation.
	RotationTimer_ += Tick * RotationSpeed_ * Velocity.magnitude();
	if( RotationTimer_ > BcPIMUL2 )
	{
		RotationTimer_ -= BcPIMUL2;
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaFishComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

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
void GaFishComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// getFishSize
BcF32 GaFishComponent::getFishSize() const
{
	return Size_;
}


//////////////////////////////////////////////////////////////////////////
// getFishSize
void GaFishComponent::updateSwarmManagerRef( GaSwarmManagerComponent* SwarmManager )
{
	if( SwarmManager != nullptr )
	{
		SwarmManager_ = SwarmManager;
	}
	else
	{
		SwarmManager_ = getParentEntity()->getComponentAnyParentByType< GaSwarmManagerComponent >();
	}
}
