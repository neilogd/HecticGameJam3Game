/**************************************************************************
*
* File:		GaSwarmElementComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaSwarmElementComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

#include "GaSwarmManagerComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaSwarmElementComponent );

void GaSwarmElementComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Manager_", &GaSwarmElementComponent::Manager_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaSwarmElementComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 1 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmElementComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSwarmElementComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSwarmElementComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Manager_ = Parent->getComponentAnyParentByType< GaSwarmElementComponent >();

	// TODO: Register with manager or something.
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaSwarmElementComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
	
	// TODO: Unregister with manager or something.
}
