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
#include "GaGameComponent.h"
#include "GaFoodComponent.h"
#include "GaPlayerComponent.h"
#include "GaSpeechBubbleComponent.h"
#include "GaSwarmManagerComponent.h"
#include "GaSwarmElementComponent.h"
#include "GaCannonComponent.h"

#include "System/Scene/Rendering/ScnSpriteComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"
#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaFishComponent );

void GaFishComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Size_", &GaFishComponent::Size_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "SizeIncreaseMultiplier_", &GaFishComponent::SizeIncreaseMultiplier_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "EatDistance_", &GaFishComponent::EatDistance_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "EatSpeed_", &GaFishComponent::EatSpeed_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "SwarmManager_", &GaFishComponent::SwarmManager_, bcRFF_TRANSIENT | ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "Sprites_", &GaFishComponent::Sprites_, bcRFF_TRANSIENT | ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "SpriteSizes_", &GaFishComponent::SpriteSizes_, bcRFF_TRANSIENT | ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "XScale_", &GaFishComponent::XScale_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "LoseWeightTimer_", &GaFishComponent::LoseWeightTimer_, bcRFF_TRANSIENT | ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "EatSoundTimer_", &GaFishComponent::EatSoundTimer_, bcRFF_TRANSIENT | ReFieldFlags::bcRFF_DEBUG_EDIT ),
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
	XScale_ = -1.0f;
	LoseWeightTimer_ = 0.0f;
	EatSoundTimer_ = 0.0f;
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

	BcBool WasTooSmall = BcFalse;

	GaPlayerComponentRef player = getParentEntity()->getComponentByType<GaPlayerComponent>();
	if (player.isValid())
	{
		if( player->getCannon().isValid() )
		{
			GaCannonComponentRef cannon = player->getCannon()->getComponentByType<GaCannonComponent>();

			GaSpeechBubbleComponentRef bubble = getParentEntity()->getComponentAnyParentByType<GaSpeechBubbleComponent>();
			if ( bubble.isValid() && cannon.isValid() && ( cannon->getRequiredSize() > Size_ ) )
			{
				WasTooSmall = BcTrue;
			}
		}
	}

	auto Element = getParentEntity()->getComponentByType< GaSwarmElementComponent >();

	if( Element->getUnitMask() != GaSwarmManagerComponent::DEAD )
	{
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

					if( Element->getUnitMask() == GaSwarmManagerComponent::PLAYER )
					{
						EatSoundTimer_ -= Tick;
						if( EatSoundTimer_ < 0.0f )
						{
							EatSoundTimer_ += BcRandom::Global.randRealRange( 0.4f, 0.6f );
							auto GameComponent = getParentEntity()->getComponentAnyParentByType< GaGameComponent >();
							GameComponent->playSound( "SoundEat", BcFalse );
						}
					}
				}
			}
		}
	}

	// Slowly size down.
	if( LoseWeightTimer_ < 0.0f )
	{
		Size_ = Size_ - ( 1.0f * Tick * 0.03f );
	}
	else
	{
		LoseWeightTimer_ -= Tick;
	}

	if (player.isValid())
	{
		if( player->getCannon().isValid() )
		{
			GaCannonComponentRef cannon = player->getCannon()->getComponentByType<GaCannonComponent>();
		
			GaSpeechBubbleComponentRef bubble = getParentEntity()->getComponentAnyParentByType<GaSpeechBubbleComponent>();
			if ( bubble.isValid() && cannon.isValid() )
			{
				if( ( cannon->getRequiredSize() <= Size_ ) && WasTooSmall )
				{
					bubble->setTarget( getParentEntity() );
					bubble->setText( "I've eaten/enough to/escape!" );
					bubble->show(3.2f);
					LoseWeightTimer_ = 3.0f;
				}
				else if ( ( cannon->getRequiredSize() > Size_ ) && !WasTooSmall )
				{
					bubble->setTarget( getParentEntity() );
					bubble->setText( "Too slow/must eat/more :(" );
					bubble->show(3.2f);
				}
			}
		}
	}

	// Clamp to range.
	Size_ = BcClamp( Size_, 0.75f, 2.0f );

	// Do the flip and stuff.
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
		if( Element->getUnitMask() == GaSwarmManagerComponent::DEAD )
		{
			FishVelRot += BcPIDIV2;
		}
		else
		{
			FishVelRot = BcLerp( -FishVelRot, FishVelRot, ( XScale_ + 1.0f ) * 0.5f );
		}

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
		SpriteComponent->setAnimation( "idle" );
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
// setFishSize
void GaFishComponent::setFishSize( BcF32 Size )
{
	Size_ = Size;
}

//////////////////////////////////////////////////////////////////////////
// updateSwarmManagerRef
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
