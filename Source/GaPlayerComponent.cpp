/**************************************************************************
*
* File:		GaPlayerComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaPlayerComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"
#include "System/Os/OsEvents.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaPlayerComponent );

void GaPlayerComponent::StaticRegisterClass()
{
	ReRegisterClass< GaPlayerComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaPlayerComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaPlayerComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaPlayerComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	OsEventInputMouse::Delegate OnMouseDown = OsEventInputMouse::Delegate::bind< GaPlayerComponent, &GaPlayerComponent::onMouseDown >( this );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, OnMouseDown );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaPlayerComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
	OsCore::pImpl()->unsubscribeAll( this );
}

//////////////////////////////////////////////////////////////////////////
// onMouseDown
eEvtReturn GaPlayerComponent::onMouseDown( EvtID ID, const OsEventInputMouse& Event )
{

	return evtRET_PASS;
}

