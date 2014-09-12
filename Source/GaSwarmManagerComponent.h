/**************************************************************************
*
* File:		GaSwarmManagerComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaSwarmManagerComponent_H__
#define __GaSwarmManagerComponent_H__

#include "Psybrus.h"
#include "GaSwarmElementComponent.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaSwarmManagerComponent > GaSwarmManagerComponentRef;
typedef std::vector<GaSwarmElementComponentRef> SwarmElementList;

//////////////////////////////////////////////////////////////////////////
// GaSwarmManagerComponent
class GaSwarmManagerComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaSwarmManagerComponent, ScnComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

private:
	SwarmElementList SwarmElements;

	MaVec2d								getAverageVelocity( BcU8 Mask );
	MaVec2d								getAveragePosition( BcU8 Mask );
	MaVec2d								getAverageAcceleration( BcU8 Mask );
	MaVec2d								capVector( MaVec2d vector, BcF32 MaxMagnitude );
	MaVec2d								forceAwayFromNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask = 0xFF );
	MaVec2d								forceTowardsNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask = 0xFF );
	SwarmElementList					getNearbyUnits( MaVec2d Position, BcU8 UnitCount, BcU8 Mask );
public:
	const BcU8 FOOD = 0xFF;
	const BcU8 PLAYER = 0x01;
};

#endif

