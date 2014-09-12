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
	
};

#endif

