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
#include "GaTankComponent.h"
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
	void								initialise( );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

	void								registerElement( GaSwarmElementComponentRef element );
	void								deregisterElement( GaSwarmElementComponentRef element );

	MaVec2d								defaultMovement( MaVec2d Move, GaSwarmElementComponent* Element );

private:
	SwarmElementList SwarmElements;

	BcF32 VelocityNeighbourDistance_;
	BcF32 PositionNeighbourDistance_;
	BcF32 SeparationDistance_;
	BcF32 EdgeDistance_;
	
	GaTankComponentRef					TankComponent_;

public:
	MaVec2d								getAverageVelocity( MaVec2d Position, BcU8 Mask, BcF32 Range = 250.0f );
	MaVec2d								getAveragePosition( MaVec2d Position, BcU8 Mask, BcF32 Range = 250.0f );
	MaVec2d								getSeparation(  MaVec2d Position, BcU8 Mask, BcF32 Range = 250.0f );
	MaVec2d								getAverageAcceleration( BcU8 Mask );
	MaVec2d								capVector( MaVec2d vector, BcF32 MaxMagnitude );
	MaVec2d								forceAwayFromNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask );
	MaVec2d								forceTowardsNearbyUnits( GaSwarmElementComponentRef Unit, BcU8 UnitCount, BcU8 Mask, BcF32 Range );
	SwarmElementList					getNearbyUnits( MaVec2d Position, BcU8 UnitCount, BcU8 Mask );
	bool								unitTypeExists( BcU8 Mask );
public:
	static const BcU8 PLAYER = 0x01;
	static const BcU8 ENEMY = 0x02;
	static const BcU8 FOOD = 0x04;
};

#endif

