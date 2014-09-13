/**************************************************************************
*
* File:		GaCustomisationComponent.cpp
* Author:	Daniel de Zwart 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaCustomisationComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource GaCustomisationComponent.
DEFINE_RESOURCE( GaCustomisationComponent );

void GaCustomisationComponent::StaticRegisterClass()
{
	ReRegisterClass< GaCustomisationComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaCustomisationComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaCustomisationComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaCustomisationComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaCustomisationComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
