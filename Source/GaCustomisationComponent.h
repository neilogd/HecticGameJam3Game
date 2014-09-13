/**************************************************************************
*
* File:		GaCustomisationComponent.h
* Author:	Daniel de Zwart
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GGaCustomisationComponent_H__
#define __GaCustomisationComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaCustomisationComponent > GaCustomisationComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaCustomisationComponent
class GaCustomisationComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaCustomisationComponent, ScnComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

private:

};

#endif

