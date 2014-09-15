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
#include "System/Scene/Rendering/ScnSpriteComponent.h"
#include "System/OS/OsCore.h"
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
	void								initialise(  );

	virtual void						update( BcF32 Tick );
	
	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );
	eEvtReturn							onMouseDown( EvtID ID, const OsEventInputMouse& Event );
private:
	ScnSpriteComponentRef Images_[ 6 ];
	BcU32 ImageIdx_;
	bool ShowCredits_;
	bool InGame_;
	void								spawnGame();
	
};

#endif

