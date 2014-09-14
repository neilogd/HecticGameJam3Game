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
#include "GaPlayerComponent.h"
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
	Text_.push_back("LIne 1");
	Text_.push_back("Line 2");
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
	if ( !FontComponent_.isValid() )
	{
		Canvas_ = ParentEntity_->getComponentAnyParentByType< ScnCanvasComponent >();
		FontComponent_ = ParentEntity_->getComponentAnyParentByType< ScnFontComponent >();
	}
	if ( !SpeechBubble_.isValid() )
	{
		SpeechBubble_ = ParentEntity_->getComponentByType<ScnSpriteComponent>( BcName( "SpeechBubbleComponent", 0 ) );
	}
	if ( SpeechBubble_.isValid() )
	{
		SpeechBubble_->setColour( RsColour( 1, 1, 1, Visible_ ? 1 : 0 ) );
	}

	MaMat4d TextScaleMatrix;
	//TextScaleMatrix.scale( MaVec4d( 0.04f, 0.04f, 1.0f, 1.0f ) );
	TextScaleMatrix.scale( MaVec4d( 1.04f, 1.04f, 1.0f, 1.0f ) );

	FontComponent_->setAlphaTestStepping( MaVec2d( 0.4f, 0.45f ) );

	Canvas_->pushMatrix( TextScaleMatrix );

	MaVec2d Position( 0.0f, 0.0f );
	MaVec2d Size;
	MaVec3d worldPos = getParentEntity()->getWorldPosition();
	MaVec2d localPos( worldPos.x(), worldPos.y() );
	for( BcU32 Idx = 0; Idx < Text_.size(); ++Idx )
	{
		const auto& Option( Text_[ Idx ] );
		const auto Colour = RsColour::BLUE;
		Size = FontComponent_->drawCentered( Canvas_, /*localPos + /**/ Position, Text_[ Idx ] , Colour, 280 );
		Position += MaVec2d( 0.0f, Size.y() );
	}



	Canvas_->popMatrix();
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSpeechBubbleComponent::onAttach( ScnEntityWeakRef Parent )
{



	Super::onAttach( Parent );
	Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >();
	FontComponent_ = ParentEntity_->getComponentAnyParentByType<ScnFontComponent>();
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
