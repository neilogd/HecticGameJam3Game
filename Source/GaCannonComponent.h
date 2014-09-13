/**************************************************************************
*
* File:		GaCannonComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaCannonComponent_H__
#define __GaCannonComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaCannonComponent > GaCannonComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaCannonComponent
class GaCannonComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaCannonComponent, ScnComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

private:

};

#endif

