/**************************************************************************
*
* File:		GaCannonComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaCannonComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaCannonComponent );

void GaCannonComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "RequiredSize_", &GaCannonComponent::RequiredSize_, ReFieldFlags::bcRFF_DEBUG_EDIT )
	};

	ReRegisterClass< GaCannonComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaCannonComponent::initialise()
{
	RequiredSize_ = 0.0f;

}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaCannonComponent::initialise( const Json::Value& Object )
{
	GaCannonComponent::initialise();

	RequiredSize_ = BcF32( Object[ "requiredsize" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaCannonComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaCannonComponent::onAttach( ScnEntityWeakRef Parent )
{



	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaCannonComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// getRequiredSize
BcF32 GaCannonComponent::getRequiredSize() const
{
	return RequiredSize_;
}
