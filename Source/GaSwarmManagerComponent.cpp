/**************************************************************************
*
* File:		GaSwarmManagerComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaSwarmManagerComponent.h"
#include "GaPlayerComponent.h"
#include "GaFishComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"
#include "System/Debug/DsCore.h"
#include "Base/BcMath.h"
#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaSwarmManagerComponent );

void GaSwarmManagerComponent::StaticRegisterClass()
{	ReField* Fields[] =
	{
		new ReField( "VelocityNeighbourDistance_", &GaSwarmManagerComponent::VelocityNeighbourDistance_ , DsCore::DsCoreSerialised ),
		new ReField( "PositionNeighbourDistance_", &GaSwarmManagerComponent::PositionNeighbourDistance_ , DsCore::DsCoreSerialised ),
		new ReField( "SeparationDistance_", &GaSwarmManagerComponent::SeparationDistance_, DsCore::DsCoreSerialised ),
		new ReField( "EdgeDistance_", &GaSwarmManagerComponent::EdgeDistance_, DsCore::DsCoreSerialised ),
	};
	ReRegisterClass< GaSwarmManagerComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmManagerComponent::initialise( const Json::Value& Object )
{
	initialise( );
	if (Object["velocityneighbourdistance"] != Json::ValueType::nullValue)
	{
		VelocityNeighbourDistance_ = (float)Object["velocityneighbourdistance"].asDouble();
	}

	if (Object["positionneighbourdistance"] != Json::ValueType::nullValue)
	{
		PositionNeighbourDistance_ = (float)Object["positionneighbourdistance"].asDouble();
	}
	if (Object["separationdistance"] != Json::ValueType::nullValue)
	{
		SeparationDistance_ = (float)Object["separationdistance"].asDouble();
	}	
	if (Object["edgedistance"] != Json::ValueType::nullValue)
	{
		EdgeDistance_ = (float)Object["edgedistance"].asDouble();
	}
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmManagerComponent::initialise( )
{
	VelocityNeighbourDistance_ = 250.0f;
	PositionNeighbourDistance_ = 250.0f;
	SeparationDistance_ = 100.0f;
	EdgeDistance_ = 20.0f;
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSwarmManagerComponent::update( BcF32 Tick )
{
	if (!TankComponent_.isValid())
	{
		TankComponent_ = getParentEntity()->getComponentAnyParentByType<GaTankComponent>();
	}
	bool hasTank = TankComponent_.isValid();
	MaVec2d CentralPosition;
	if (TankComponent_.isValid())
	{
		CentralPosition = TankComponent_->getCentralPosition();
	}
	Super::update( Tick );
	BcU32 size = this->SwarmElements.size();
	for ( BcU32 Idx = 0; Idx < size; ++Idx )
	{
		auto SwarmElement = SwarmElements[Idx];
		MaVec2d move = SwarmElement->getVelocity();

		move = defaultMovement( move, SwarmElement );

		// Do tank keeping in!
		if (hasTank)
		{
			BcF32 X = SwarmElement->getPosition().x();
			BcF32 Y = SwarmElement->getPosition().y();
			BcF32 TankL = TankComponent_->getParentEntity()->getWorldPosition().x();
			BcF32 TankR = TankComponent_->getParentEntity()->getWorldPosition().x() + TankComponent_->getDimensions().x();
			BcF32 TankB = TankComponent_->getParentEntity()->getWorldPosition().y();
			BcF32 TankT = TankComponent_->getParentEntity()->getWorldPosition().y() + TankComponent_->getDimensions().y();
			BcF32 TankLInnerEdge = TankL + EdgeDistance_;
			BcF32 TankRInnerEdge = TankR - EdgeDistance_;
			BcF32 TankBInnerEdge = TankB + EdgeDistance_;
			BcF32 TankTInnerEdge = TankT - EdgeDistance_;

			BcF32 PushAmount = 10.0f;
			if( X < TankLInnerEdge )
			{
				auto Amount = BcClamp( ( TankLInnerEdge - X ) / ( TankLInnerEdge - TankL ), 0.0f, 1.0f );
				move += MaVec2d( PushAmount, 0.0f ) * Amount;
			}
			if( X > TankRInnerEdge )
			{
				auto Amount = BcClamp( ( X - TankRInnerEdge ) / ( TankR - TankRInnerEdge ), 0.0f, 1.0f );
				move -= MaVec2d( PushAmount, 0.0f ) * Amount;
			}

			if( Y < TankBInnerEdge )
			{
				auto Amount = BcClamp( ( TankBInnerEdge - Y ) / ( TankBInnerEdge - TankB ), 0.0f, 1.0f );
				move += MaVec2d( 0.0f, PushAmount ) * Amount;
			}
			if( Y > TankTInnerEdge )
			{
				auto Amount = BcClamp( ( Y - TankTInnerEdge ) / ( TankT - TankTInnerEdge ), 0.0f, 1.0f );
				move -= MaVec2d( 0.0f, PushAmount ) * Amount;
			}
		}

		//move += forceAwayFromNearbyUnits( SwarmElements[Idx], 5, SwarmElements[Idx]->getUnitMask() ) * 0.5f;
		SwarmElements[Idx]->stageVelocity( move );
		SwarmElements[Idx]->commitChanges();
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSwarmManagerComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaSwarmManagerComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// defaultMovement
MaVec2d GaSwarmManagerComponent::defaultMovement( MaVec2d Move, GaSwarmElementComponent* Element )
{
	// Is attacking?
	auto AttackElement = Element->getAttackTarget();
	BcBool Attacking = AttackElement != nullptr;
	MaVec2d TargetPosition = Attacking ? AttackElement->getPosition() : MaVec2d( 0.0f, 0.0f );
	BcBool MoveToTarget = BcFalse;

	// If we're an enemy, move to player thing.
	if( Element->getUnitMask() == ENEMY )
	{
		if( unitTypeExists( FOOD ) )
		{
			Move += forceTowardsNearbyUnits( Element, 1, FOOD, FLT_MAX );
		}

		// Move away from crowd
		Move += getSeparation( Element->getPosition(), Element->getUnitMask() | PLAYER, SeparationDistance_ ) * 0.05f;
		// Adjust towards average velocity of crowd
		Move += getAverageVelocity( Element->getPosition(), Element->getUnitMask(), VelocityNeighbourDistance_ ).normal() * 0.01f;
		// Move towards centre of mass of crowd
		Move += ( ( getAveragePosition( Element->getPosition(), Element->getUnitMask(), PositionNeighbourDistance_ ) - Element->getPosition() ).normal() ) * 0.075f;

		// Move towards player.
		auto player = getNearbyUnits( Element->getPosition(), 1, PLAYER );
		if( player.size() > 0 )
		{
			if( ( player[ 0 ]->getPosition() - Element->getPosition() ).magnitude() < 25.0f )
			{
				Move += ( ( player[ 0 ]->getPosition() - Element->getPosition() ).normal() ) * 1.5f;
			}
		}

		// if attacking.
		if( Attacking )
		{
			MoveToTarget = BcTrue;
		}
	}

	// if we're a player...
	if( Element->getUnitMask() == PLAYER )
	{
		GaPlayerComponentRef Player = Element->getParentEntity()->getComponentByType< GaPlayerComponent >();

		// Move towards food.
		if( !Attacking && unitTypeExists( FOOD ) )
		{
			Move += forceTowardsNearbyUnits( Element, 1, FOOD, 128.0f ) * 4.0f;
		}

		// Move away from crowd
		Move += getSeparation( Element->getPosition(), ENEMY, SeparationDistance_ ) * 0.01f;

		MoveToTarget = BcTrue;

		if( !Attacking )
		{
			TargetPosition = Player->getTargetPosition();
		}
	}

	// if we're food...
	if( Element->getUnitMask() == FOOD )
	{
		Move *= 0.1f;
	}


	if( MoveToTarget )
	{
		// Move towards target.
		BcF32 TargetApproachRadius = Attacking ? 64.0f : 32.0f;
		BcF32 TargetAttackRadius = 256.0f;

		//BcF32 

		auto TargetVector = ( TargetPosition - Element->getPosition() );
		auto TargetDistance = TargetVector.magnitude();

		// When nearing attack, cancel out some of the previous velocity to narrow in a bit harder.
		BcF32 AttackSpeedMultiplier = 0.2f;
		if( Attacking && TargetDistance < TargetAttackRadius )
		{
			Move *= 0.9f;
			AttackSpeedMultiplier = 2.0f;
		}

		// Saunter around the target, or move in for kill.
		if( TargetDistance > TargetApproachRadius )
		{
			Move += ( TargetVector.normal() ) * ( AttackSpeedMultiplier );
		}

		// Slow down a bit.
		Move = Move - ( Move * 0.01f );
	}

	return Move;
}


MaVec2d GaSwarmManagerComponent::getAverageVelocity( MaVec2d Position, BcU8 Mask, BcF32 Range )
{
	int counter = 0;
	MaVec2d velocity(0.0f, 0.0f);
	for (auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u)
	{
		if (((*u)->getUnitMask() == Mask) && ((Position - (*u)->getPosition()).magnitude() <= Range))
		{
			velocity += (*u)->getVelocity();
			++counter;
		}
	}
	if (counter == 0) // Just a little hack to stop division by zero
		counter = 1;
	return velocity / (float)counter;
}

MaVec2d GaSwarmManagerComponent::getAveragePosition( MaVec2d Position, BcU8 Mask, BcF32 Range )
{
	int counter = 0;
	MaVec2d position(0.0f, 0.0f);
	for (auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u)
	{
		if (((*u)->getUnitMask() == Mask) && ((Position - (*u)->getPosition()).magnitude() <= Range))
		{
			MaVec2d pos = (*u)->getPosition();
			position += MaVec2d(pos.x(), pos.y());
			++counter;
		}
	}
	if (counter == 0) // Just a little hack to stop division by zero
		counter = 1;
	return position / (float)counter;
}

MaVec2d GaSwarmManagerComponent::getAverageAcceleration( BcU8 Mask )
{
	int counter = 0;
	MaVec2d velocity(0.0f, 0.0f);
	for (auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u)
	{
		if ((*u)->getUnitMask() == Mask)
		{
			velocity += (*u)->getVelocity();
			++counter;
		}
	}
	if (counter == 0) // Just a little hack to stop division by zero
		counter = 1;
	return velocity / (float)counter;

}

MaVec2d GaSwarmManagerComponent::capVector( MaVec2d vector, BcF32 MaxMagnitude )
{
	BcF32 magnitude = vector.magnitude();
	magnitude = magnitude > MaxMagnitude ? MaxMagnitude : magnitude;
	return vector / magnitude;
}

MaVec2d GaSwarmManagerComponent::forceAwayFromNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask )
{
	return -forceTowardsNearbyUnits( Unit, UnitCount, Mask, FLT_MAX );

}

MaVec2d GaSwarmManagerComponent::forceTowardsNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask, BcF32 Range )
{
	SwarmElementList elements = getNearbyUnits(Unit->getPosition(), UnitCount, Mask);
	MaVec2d average( 0.0f, 0.0f );
	BcF32 avgCount = 0.0f;
	for ( BcU32 Idx = 0; Idx < elements.size(); ++Idx )
	{
		BcF32 Distance = (elements[Idx]->getPosition() - average).magnitude();
		if( Distance < Range )
		{
			average += elements[Idx]->getPosition();
			avgCount += 1.0f;
		}
	}
	average = (average / avgCount);

	
	if( avgCount > 0.0f )
	{
		return -(Unit->getPosition() - average).normal();
	}
	else
	{
		return MaVec2d( 0.0f, 0.0f );
	}
}

SwarmElementList GaSwarmManagerComponent::getNearbyUnits( MaVec2d Position, BcU8 UnitCount, BcU8 Mask, BcF32 Range )
{
	SwarmElementList units;
	SwarmElementList ret;
	for ( auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u )
	{
		if ((*u)->getUnitMask() == Mask)
		{
			units.push_back(*u);
		}
	}
	std::sort( units.begin(), units.end(), [ Position ]( GaSwarmElementComponentRef a, GaSwarmElementComponentRef b )
	{
		MaVec2d vecA = (*a).getPosition() - Position;
		MaVec2d vecB = (*b).getPosition() - Position;
		return ( vecA.magnitude() < vecB.magnitude() );
	});
	for ( BcU32 Idx = 0; (Idx < units.size()) && ( Idx < UnitCount); ++Idx )
	{
		MaVec2d vec = units[Idx]->getPosition() - Position;
		if( vec.magnitude() < Range )
		{
			ret.push_back(units[Idx]);
		}
	}
	return ret;
}

void GaSwarmManagerComponent::registerElement( GaSwarmElementComponentRef element )
{
	BcAssert( element.isValid() );
	SwarmElements.push_back(element);
}

void GaSwarmManagerComponent::deregisterElement( GaSwarmElementComponentRef element )
{
	for ( auto iter = SwarmElements.begin(); iter != SwarmElements.end(); ++iter )
	{
		if ((*iter)->getUniqueId() == (*element).getUniqueId())
		{
			SwarmElements.erase(iter);
			return;
		}
	}

}

bool GaSwarmManagerComponent::unitTypeExists( BcU8 Mask )
{
	for ( BcU32 Idx = 0; Idx < SwarmElements.size(); ++Idx )
	{
		if ( SwarmElements[Idx]->getUnitMask() & Mask )
			return true;
	}
	return false;
}

MaVec2d GaSwarmManagerComponent::getSeparation( MaVec2d Position, BcU8 Mask, BcF32 Range )
{
	MaVec2d Output(0.0f, 0.0f);
	for (auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u)
	{
		MaVec2d AvoidPos = ( *u )->getPosition();
		auto Distance = ( Position - AvoidPos ).magnitude();
		if ((((*u)->getUnitMask() & Mask) != 0 ) && Distance <= Range)
		{
			// Divide down to 0-1.
			Distance /= Range;
			Output += -( MaVec2d( AvoidPos.x() - Position.x(), AvoidPos.y() - Position.y() ) ) * BcClamp( 1.0f - Distance, 0.0f, 1.0f );
		}
	}

	return Output;
}