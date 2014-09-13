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

private:
	BcF32 FishSize_;
};

#endif

