/**************************************************************************
*
* File:		GaPlayerComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaPlayerComponent.h"
#include "GaGameComponent.h"
#include "GaCannonComponent.h"
#include "GaFishComponent.h"
#include "GaTankComponent.h"
#include "GaSwarmManagerComponent.h"
#include "GaSwarmElementComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"
#include "System/Os/OsEvents.h"

#include "System/Debug/DsCore.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"
#include "GaSpeechBubbleComponent.h"
//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaPlayerComponent );

void GaPlayerComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Canvas_", &GaPlayerComponent::Canvas_, bcRFF_TRANSIENT ),
		new ReField( "ViewMatrix_", &GaPlayerComponent::ViewMatrix_, DsCore::DsCoreSerialised ),
		new ReField( "TargetPosition_", &GaPlayerComponent::TargetPosition_, DsCore::DsCoreSerialised ),
		new ReField( "PlayerState_", &GaPlayerComponent::PlayerState_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "OriginalSize_", &GaPlayerComponent::OriginalSize_, DsCore::DsCoreSerialised ),
		new ReField( "JumpHeight_", &GaPlayerComponent::JumpHeight_, DsCore::DsCoreSerialised ),
		new ReField( "JumpTimer_", &GaPlayerComponent::JumpTimer_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "JumpSpeed_", &GaPlayerComponent::JumpSpeed_, DsCore::DsCoreSerialised ),
		new ReField( "JumpStart_", &GaPlayerComponent::JumpStart_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "JumpEnd_", &GaPlayerComponent::JumpEnd_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "CannonTimer_", &GaPlayerComponent::CannonTimer_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "CannonSpeed_", &GaPlayerComponent::CannonSpeed_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "CannonSuckDistance_", &GaPlayerComponent::CannonSuckDistance_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "CannonStart_", &GaPlayerComponent::CannonStart_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "CannonEnd_", &GaPlayerComponent::CannonEnd_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "TankIndex_", &GaPlayerComponent::TankIndex_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised ),
		new ReField( "Tank_", &GaPlayerComponent::TankIndex_, bcRFF_TRANSIENT  ),
		new ReField( "Cannon_", &GaPlayerComponent::TankIndex_, bcRFF_TRANSIENT ),
		new ReField( "FirstUpdate_", &GaPlayerComponent::FirstUpdate_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaPlayerComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaPlayerComponent::initialise()
{
	TargetPosition_ = MaVec2d( 0.0f, 0.0f );
	PlayerState_ = PlayerState::IDLE;

	OriginalSize_ = 1.0f;
	JumpHeight_ = 512.0f;
	JumpTimer_ = 0.0f;
	JumpSpeed_ = 0.5f;
	JumpStart_ = MaVec2d( 0.0f, 0.0f );
	JumpEnd_ = MaVec2d( 0.0f, 0.0f );

	CannonTimer_ = 0.0f;
	CannonSpeed_ = 1.0f;
	CannonSuckDistance_ = 128.0f;
	CannonStart_ = MaVec2d( 0.0f, 0.0f );
	CannonEnd_ = MaVec2d( 0.0f, 0.0f );
	TankIndex_ = 0;

	FirstUpdate_ = true;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaPlayerComponent::initialise( const Json::Value& Object )
{
	initialise();
}

//////////////////////////////////////////////////////////////////////////
// preUpdate
//virtual
void GaPlayerComponent::preUpdate( BcF32 Tick )
{
	Super::preUpdate( Tick );

	ViewMatrix_.translation( -getParentEntity()->getWorldPosition() );
	Canvas_->setViewMatrix( ViewMatrix_ );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaPlayerComponent::update( BcF32 Tick )
{
	if ( FirstUpdate_ )
	{
		GaSpeechBubbleComponentRef bubble = getParentEntity()->getComponentAnyParentByType<GaSpeechBubbleComponent>();
		if (bubble.isValid())
		{
			FirstUpdate_ = false;
			bubble->setTarget( getParentEntity() );
			bubble->setText( "I must/escape this/horrid cage" );
			bubble->show();
		}
	}
	// Player is attached before cannons are to tanks. Need to get rid of the
	// delayed attachment in the engine to fix this properly.
	if( Cannon_ == nullptr && Tank_->getComponentByType< GaTankComponent >()->HasCannon_ )
	{
		jumpTank( TankIndex_ );
	}

	switch( PlayerState_ )
	{
	case PlayerState::IDLE:
		{	
			auto SwarmElement = getParentEntity()->getComponentByType< GaSwarmElementComponent >();
			if( SwarmElement->getAttackTarget() )
			{
				TargetPosition_ = SwarmElement->getAttackTarget()->getPosition();
			}

			if( Cannon_ != nullptr )
			{
				BcF32 Distance = ( getPosition() - getCannonPosition() ).magnitude();

				if( Distance < CannonSuckDistance_ )
				{
					// Do suck later, go straight to load.
					PlayerState_ = PlayerState::CANNON_SUCK;

					auto TankDimensions = Tank_->getComponentByType< GaTankComponent >()->getDimensions();

					CannonTimer_ = 0.0f;
					CannonStart_ = getCannonPosition();
					CannonEnd_ = getCannonPosition() + MaVec2d( 0.0f, 480.0f * 2.0f );
				}
			}
		}
		break;

	case PlayerState::CANNON_SUCK:
		{
			BcAssert( Cannon_ != nullptr );
			TargetPosition_ = getCannonPosition();
			BcF32 Distance = ( getPosition() - getCannonPosition() ).magnitude();
			if( Distance < 70.0f )
			{
				auto GameComponent = getParentEntity()->getComponentAnyParentByType< GaGameComponent >();
				GameComponent->playSound( "SoundSuck", BcFalse );
				PlayerState_ = PlayerState::CANNON_LOAD;
			}			
		}
		break;

	case PlayerState::CANNON_LOAD:
		{
			BcAssert( Cannon_ != nullptr );
						
			BcF32 ClampedTimer = BcSmoothStep( std::min( CannonTimer_, 1.0f ) );
			MaVec2d Position;
			Position.lerp( CannonStart_, CannonEnd_, ClampedTimer );

			MaVec3d NewPosition = getParentEntity()->getWorldPosition();
			getParentEntity()->setWorldPosition(
				NewPosition * 0.5f + MaVec3d( Position, 0.0f ) * 0.5f );

			if( CannonTimer_ > 1.0f )
			{
				TargetPosition_ = getPosition();

				auto FishComponent = getParentEntity()->getComponentByType< GaFishComponent >();
				auto CannonComponent = Cannon_->getComponentByType< GaCannonComponent >();

				if( FishComponent->getFishSize() < CannonComponent->getRequiredSize() )
				{
					jumpTank( TankIndex_, BcTrue );
				}
				else
				{
					jumpTank( TankIndex_ + 1, BcTrue );
				}
			}

			CannonTimer_ += Tick * CannonSpeed_;
		}
		break;

	case PlayerState::JUMP:
		{
			BcF32 ClampedTimer = BcSmoothStep( std::min( JumpTimer_, 1.0f ) );
			MaVec2d Position;
			Position.lerp( JumpStart_, JumpEnd_, ClampedTimer );

			Position.y( Position.y() + std::sin( ClampedTimer * BcPI ) * JumpHeight_ );

			auto Fish = getParentEntity()->getComponentByType< GaFishComponent >();
			Fish->setFishSize( Fish->getFishSize() - Tick * 0.5f );
			
			getParentEntity()->setWorldPosition(
				MaVec3d( Position, 0.0f ) );

			if( JumpTimer_ > 1.0f )
			{
				TargetPosition_ = MaVec2d( 
					getParentEntity()->getWorldPosition().x(), 
					getParentEntity()->getWorldPosition().y() );
				PlayerState_ = PlayerState::IDLE;
				Tank_->getComponentByType<GaTankComponent>()->receiveFish();

				if( !Tank_->getComponentByType< GaTankComponent >()->HasCannon_ )
				{
					auto GameComponent = getParentEntity()->getComponentAnyParentByType< GaGameComponent >();
					GameComponent->stopAllSounds();
					GameComponent->playSound( "MusicWin", BcFalse );

					getParentEntity()->getComponentByType< GaSwarmElementComponent >()->UnitMask_ = GaSwarmManagerComponent::WINNER;

					BcU32 Idx = 0;
					while( auto SpriteComponent = getParentEntity()->getComponentByType< ScnSpriteComponent >( Idx++ ) )
					{
						SpriteComponent->setAnimation( "none" );
						SpriteComponent->setAnimation( "dead" );
					}
				}
			}

			JumpTimer_ += Tick * JumpSpeed_;
		}
		break;
	}
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// postUpdfate
//virtual
void GaPlayerComponent::postUpdate( BcF32 Tick )
{
	Super::postUpdate( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaPlayerComponent::onAttach( ScnEntityWeakRef Parent )
{
	OsEventInputMouse::Delegate OnMouseDown = OsEventInputMouse::Delegate::bind< GaPlayerComponent, &GaPlayerComponent::onMouseDown >( this );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, OnMouseDown );

	// Find a canvas to use for rendering (someone in ours, or our parent's hierarchy).
	Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >( 0 );
	BcAssertMsg( Canvas_.isValid(), "Player component needs to be attached to an entity with a canvas component in any parent!" );

	OriginalSize_ = getParentEntity()->getComponentByType< GaFishComponent >()->getFishSize();

	// Jump tank 0.
	jumpTank( 0 );

	// Start in centre of tank.
	auto TankDimensions = 
		Tank_->getComponentByType< GaTankComponent >()->getDimensions();

	getParentEntity()->setWorldPosition(
		( MaVec3d( 
			TankDimensions.x(),
			TankDimensions.y(), 
			0.0f ) * 0.5f ) +
		( Tank_->getParentEntity()->getWorldPosition() ) );

	TargetPosition_ = MaVec2d( 
		getParentEntity()->getWorldPosition().x(), 
		getParentEntity()->getWorldPosition().y() );

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaPlayerComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
	OsCore::pImpl()->unsubscribeAll( this );
}

//////////////////////////////////////////////////////////////////////////
// onMouseDown
eEvtReturn GaPlayerComponent::onMouseDown( EvtID ID, const OsEventInputMouse& Event )
{
	GaSwarmElementComponentRef ref = getParentEntity()->getComponentByType<GaSwarmElementComponent>();
	if ( ref.isValid() )
	{
		if ((ref->UnitMask_ ==GaSwarmManagerComponent::DEAD) || (ref->UnitMask_ == GaSwarmManagerComponent::WINNER))
		{
			ScnEntityRef lajdfk = getParentEntity()->getComponentAnyParentByType<GaGameComponent>()->getParentEntity();
			// Load game
			ScnEntitySpawnParams StartGameEntityParams =
			{
				"default", "CustomisationEntity", "CustomisationEntity_0",
				MaMat4d(),
				lajdfk->getParentEntity(),
				nullptr
			};
			ScnCore::pImpl()->removeEntity(lajdfk);
			ScnCore::pImpl()->spawnEntity( StartGameEntityParams );
		}
	}

	// Offset mouse postion.
	MaVec2d MousePosition( MaVec2d( BcF32( Event.MouseX_ ), BcF32( Event.MouseY_ ) ) );
	
	auto Client = OsCore::pImpl()->getClient( 0 );

	MousePosition -= MaVec2d( BcF32( Client->getWidth() ), BcF32( Client->getHeight() ) ) * 0.5f;
	MousePosition = MaVec2d( MousePosition.x(), -MousePosition.y() );
	
	MaMat4d InverseViewMatrix = ViewMatrix_;
	InverseViewMatrix.inverse();
	MousePosition = MousePosition * InverseViewMatrix;

	auto SwarmElement = getParentEntity()->getComponentByType< GaSwarmElementComponent >();
	
	// Do die sound.
	auto GameComponent = getParentEntity()->getComponentAnyParentByType< GaGameComponent >();
	
	// Target position.
	if( Event.ButtonCode_ == 0 )
	{
		TargetPosition_ = MousePosition;

		SwarmElement->setAttackTarget( nullptr );
	}

	// Check if we're touching an enemy.
	if( Tank_ != nullptr )
	{
		auto SwarmManager = Tank_->getComponentAnyParentByType< GaSwarmManagerComponent >();
		auto Units = SwarmManager->getNearbyUnits( MousePosition, 1, GaSwarmManagerComponent::ENEMY, 64.0f );
		if( Units.size() > 0 )
		{
			SwarmElement->setAttackTarget( Units[ 0 ] );
			GameComponent->playSound( "SoundAggro", BcFalse );
		}
		else
		{
			GameComponent->playSound( "SoundMove", BcFalse );
		}
	}


	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// jumpTank
void GaPlayerComponent::jumpTank( BcU32 TankIndex, BcBool Force )
{
	if( Tank_ != nullptr )
	{
		Tank_->getComponentByType< GaSwarmManagerComponent >()->deregisterElement( 
			getParentEntity()->getComponentByType< GaSwarmElementComponent >() );
	}

	Tank_ = getParentEntity()->getComponentAnyParentByType< ScnEntity >( BcName( "TankEntity", TankIndex ) );
	Cannon_ = Tank_->getComponentByType< ScnEntity >( "CannonEntity_0" );

	getParentEntity()->getComponentByType< GaFishComponent >()->updateSwarmManagerRef( 
		Tank_->getComponentAnyParentByType< GaSwarmManagerComponent >() );

	
	if( Tank_ != nullptr )
	{
		Tank_->getComponentByType< GaSwarmManagerComponent >()->registerElement( 
			getParentEntity()->getComponentByType< GaSwarmElementComponent >() );
	}

	if( TankIndex != TankIndex_ || Force )
	{
		auto GameComponent = getParentEntity()->getComponentAnyParentByType< GaGameComponent >();
		GameComponent->playSound( "SoundCannon", BcFalse );

		PlayerState_ = PlayerState::JUMP;
		TankIndex_ = TankIndex;

		BcAssertMsg( Tank_ != nullptr, "Implement end game condition here :)" );

		auto TankDimensions = Tank_->getComponentByType< GaTankComponent >()->getDimensions();

		JumpTimer_ = 0.0f;
		JumpStart_ = MaVec2d( 
			getParentEntity()->getLocalPosition().x(), 
			getParentEntity()->getLocalPosition().y() );
		JumpEnd_ = MaVec2d( 
			Tank_->getLocalPosition().x(), 
			Tank_->getLocalPosition().y() ) + TankDimensions * 0.5f;
	}
	else
	{
		PlayerState_ = PlayerState::IDLE;
	}
}

//////////////////////////////////////////////////////////////////////////
// getTargetPosition
const MaVec2d& GaPlayerComponent::getTargetPosition() const
{
	return TargetPosition_;
}

//////////////////////////////////////////////////////////////////////////
// getCannon
ScnEntityRef GaPlayerComponent::getCannon()
{
	return Cannon_;
}

//////////////////////////////////////////////////////////////////////////
// getPosition
MaVec2d GaPlayerComponent::getPosition() const
{
	return MaVec2d( 
		getParentEntity()->getWorldPosition().x(), 
		getParentEntity()->getWorldPosition().y() );
}

//////////////////////////////////////////////////////////////////////////
// getCannonPosition
MaVec2d GaPlayerComponent::getCannonPosition() const
{
	if( Cannon_ != nullptr )
	{
		return MaVec2d(
			Cannon_->getWorldPosition().x(),
			Cannon_->getWorldPosition().y() ) + MaVec2d( 200.0f, 210.0f ); // hack offset to scaled up sprite entry point.
	}
	return MaVec2d();
}
