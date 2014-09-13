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

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"
#include "System/Debug/DsCore.h"
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
		TankComponent_ = getParentEntity()->getParentEntity()->getComponentByType<GaTankComponent>();
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
		MaVec2d move = SwarmElements[Idx]->getVelocity();
		if (unitTypeExists( FOOD ))
		{
			move = forceTowardsNearbyUnits( SwarmElements[Idx], 1, FOOD );
		}
		move += getAverageVelocity(SwarmElements[Idx]->getPosition(), SwarmElements[Idx]->getUnitMask(), VelocityNeighbourDistance_).normal();
		move += (getAveragePosition( SwarmElements[Idx]->getPosition(), SwarmElements[Idx]->getUnitMask(), PositionNeighbourDistance_ ) - SwarmElements[Idx]->getPosition()).normal();
		move += getSeparation( SwarmElements[Idx]->getPosition(), SwarmElements[Idx]->getUnitMask(), SeparationDistance_ ).normal() * 2.01f;

		if (SwarmElements[Idx]->getUnitMask() == ENEMY)
		{
			auto player = getNearbyUnits( SwarmElements[Idx]->getPosition(), 1, PLAYER );
			if ( player.size() > 0 )
			{
				if ((player[0]->getPosition() - SwarmElements[Idx]->getPosition()).magnitude() < 25.0f)
				{
					move += ((player[0]->getPosition() - SwarmElements[Idx]->getPosition()).normal()) * 1.5f;
				}
			}
		}
		if (hasTank)
		{
			if (SwarmElements[Idx]->getPosition().x() - CentralPosition.x() - TankComponent_->getDimensions().x() * 0.5f < EdgeDistance_)
			{
				move += MaVec2d(200,0);
			}
			if (CentralPosition.x() + TankComponent_->getDimensions().x() * 0.5f  - SwarmElements[Idx]->getPosition().x() < EdgeDistance_)
			{
				move += MaVec2d(-200,0);
			}
			if (SwarmElements[Idx]->getPosition().y() - CentralPosition.y() - TankComponent_->getDimensions().y() * 0.5f < EdgeDistance_)
			{
				move += MaVec2d(0,200);
			}
			if (CentralPosition.y() + TankComponent_->getDimensions().y() * 0.5f  - SwarmElements[Idx]->getPosition().y() < EdgeDistance_)
			{
				move += MaVec2d(0,-200);
			}
		}

		//move += forceAwayFromNearbyUnits( SwarmElements[Idx], 5, SwarmElements[Idx]->getUnitMask() ) * 0.5f;
		SwarmElements[Idx]->stageVelocity( move.normal() );
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
	return -forceTowardsNearbyUnits( Unit, UnitCount, Mask );

}

MaVec2d GaSwarmManagerComponent::forceTowardsNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask )
{
	SwarmElementList elements = getNearbyUnits(Unit->getPosition(), UnitCount, Mask);
	MaVec2d average( 0.0f, 0.0f );
	for ( BcU32 Idx = 0; Idx < elements.size(); ++Idx )
	{
		average += elements[Idx]->getPosition();
	}
	average = (average / (BcF32)elements.size());
	return -(Unit->getPosition() - average).normal();
}

SwarmElementList GaSwarmManagerComponent::getNearbyUnits( MaVec2d Position, BcU8 UnitCount, BcU8 Mask )
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
		ret.push_back(units[Idx]);
	}
	return ret;
}

void GaSwarmManagerComponent::registerElement( GaSwarmElementComponentRef element )
{
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

MaVec2d GaSwarmManagerComponent::getSeparation(  MaVec2d Position, BcU8 Mask, BcF32 Range )
{
	int counter = 0;
	MaVec2d position(0.0f, 0.0f);
	for (auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u)
	{
		if (((*u)->getUnitMask() == Mask) && ((Position - (*u)->getPosition()).magnitude() <= Range))
		{
			MaVec2d pos = (*u)->getPosition();
			position += MaVec2d(pos.x() - Position.x(), pos.y() - Position.y());
			++counter;
		}
	}
	if (counter == 0) // Just a little hack to stop division by zero
		counter = 1;
	counter = -counter;
	return position / (float)counter;
}