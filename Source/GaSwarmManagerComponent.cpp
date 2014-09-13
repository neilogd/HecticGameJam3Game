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

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaSwarmManagerComponent );

void GaSwarmManagerComponent::StaticRegisterClass()
{
	ReRegisterClass< GaSwarmManagerComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmManagerComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSwarmManagerComponent::update( BcF32 Tick )
{
	Super::update( Tick );
	BcU32 size = this->SwarmElements.size();
	for ( BcU32 Idx = 0; Idx < size; ++Idx )
	{
		MaVec2d move( 0, 0 );
		move = (SwarmElements[Idx]->getPosition() - this->getAveragePosition(PLAYER)).normal();

		SwarmElements[Idx]->stageVelocity(move);
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

MaVec2d GaSwarmManagerComponent::getAverageVelocity( BcU8 Mask )
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
	return velocity / (float)counter;
}

MaVec2d GaSwarmManagerComponent::getAveragePosition( BcU8 Mask )
{
	int counter = 0;
	MaVec2d position(0.0f, 0.0f);
	for (auto u = SwarmElements.begin(); u != SwarmElements.end(); ++u)
	{
		if ((*u)->getUnitMask() == Mask)
		{
			MaVec3d pos = (*u)->getParentEntity()->getWorldPosition();
			position += MaVec2d(pos.x(), pos.y());
			++counter;
		}
	}
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
	MaVec2d average;
	for ( BcU32 Idx = 0; Idx < elements.size(); ++Idx )
	{
		average += elements[Idx]->getPosition();
	}
	average = (average / (BcF32)elements.size());
	return (Unit->getPosition() - average).normal();
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
