/**************************************************************************
*
* File:		GaSpeechBubbleComponent.cpp
* Author:	Daniel de Zwart
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaSpeechBubbleComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaSpeechBubbleComponent );

void GaSpeechBubbleComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Visible_", &GaSpeechBubbleComponent::Visible_, DsCore::DsCoreSerialised )
	};

	ReRegisterClass< GaSpeechBubbleComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSpeechBubbleComponent::initialise()
{
	RequiredSize_ = 0.0f;
	Visible_ = true;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSpeechBubbleComponent::initialise( const Json::Value& Object )
{
	GaSpeechBubbleComponent::initialise();

	RequiredSize_ = BcF32( Object[ "requiredsize" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSpeechBubbleComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	if ( !SpeechBubble_.isValid() )
	{
		SpeechBubble_ = ParentEntity_->getComponentByType<ScnSpriteComponent>( BcName( "SpeechBubbleComponent", 0 ) );
	}
	if ( SpeechBubble_.isValid() )
	{
		SpeechBubble_->setColour( RsColour( 1, 1, 1, Visible_ ? 1 : 0 ) );
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSpeechBubbleComponent::onAttach( ScnEntityWeakRef Parent )
{



	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaSpeechBubbleComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// getRequiredSize
BcF32 GaSpeechBubbleComponent::getRequiredSize() const
{
	return RequiredSize_;
}
