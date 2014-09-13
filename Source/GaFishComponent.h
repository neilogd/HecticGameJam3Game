/**************************************************************************
*
* File:		GaFishComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaFishComponent_H__
#define __GaFishComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaFishComponent > GaFishComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaFishComponent
class GaFishComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaFishComponent, ScnComponent );

	void initialise();
	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

	BcF32 getFishSize() const;

	BcU32 getMaxHealth() const;
	BcU32 getHealth() const;

	void updateSwarmManagerRef( class GaSwarmManagerComponent* SwarmManager );

private:
	BcF32 Size_;
	BcF32 SizeIncreaseMultiplier_;
	BcF32 EatDistance_;
	BcF32 EatSpeed_;	
	BcU32 Health_;
	BcU32 MaxHealth_;

	ReObjectRef< class GaSwarmManagerComponent > SwarmManager_;

	std::vector< MaVec2d > SpriteSizes_;
	std::vector< ReObjectRef< class ScnSpriteComponent > > Sprites_;
};

#endif

