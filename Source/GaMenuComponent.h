/**************************************************************************
*
* File:		GaMenuComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaMenuComponent_H__
#define __GaMenuComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaMenuComponent > GaMenuComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaMenuComponent
class GaMenuComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaMenuComponent, ScnComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

private:

};

#endif

