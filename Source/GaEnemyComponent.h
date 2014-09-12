/**************************************************************************
*
* File:		GaEnemyComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaEnemyComponent_H__
#define __GaEnemyComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaEnemyComponent > GaEnemyComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaEnemyComponent
class GaEnemyComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaEnemyComponent, ScnComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

private:

};

#endif

