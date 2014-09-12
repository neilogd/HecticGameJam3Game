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

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

private:
	ReObjectRef< class GaSwarmManagerComponent > Manager_;
};

#endif

