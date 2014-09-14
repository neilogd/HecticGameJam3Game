/**************************************************************************
*
* File:		GaSwarmElementComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaSwarmElementComponent_H__
#define __GaSwarmElementComponent_H__

#include "Psybrus.h"
#include "Math/MaVec2d.h"
//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaSwarmElementComponent > GaSwarmElementComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaSwarmElementComponent
class GaSwarmElementComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaSwarmElementComponent, ScnComponent );

	void initialise();
	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

	BcU8 getUnitMask();

	MaVec2d getAcceleration();
	void stageAcceleration( MaVec2d acceleration );
	MaVec2d getVelocity();
	void stageVelocity( MaVec2d acceleration );
	void commitChanges();
	MaVec2d getPosition();

	void setAttackTarget( GaSwarmElementComponent* AttackTarget );
	GaSwarmElementComponent* getAttackTarget() const;
	
	BcU32 getMaxHealth() const;
	BcU32 getHealth() const;

public:
	ReObjectRef< class GaSwarmManagerComponent > Manager_;
	BcU8 UnitMask_;
	MaVec2d Acceleration_;
	MaVec2d Velocity_;

	MaVec2d StagedAcceleration_;
	MaVec2d StagedVelocity_;

	BcBool VelocityDriven_;
	BcF32 MaxSpeed_;

	BcU32 Health_;
	BcU32 MaxHealth_;

	BcF32 AttackSpeed_;
	BcF32 AttackTimer_;
	BcF32 AttackDistance_;

	GaSwarmElementComponent* AttackTarget_;

};

#endif

