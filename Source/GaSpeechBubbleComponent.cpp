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

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

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
		new ReField( "Visible_", &GaSpeechBubbleComponent::Visible_, DsCore::DsCoreSerialised ),
		new ReField( "FontOffset_", &GaSpeechBubbleComponent::FontOffset_, DsCore::DsCoreSerialised ),
		new ReField( "VisibleTime_", &GaSpeechBubbleComponent::VisibleTime_, DsCore::DsCoreSerialised ),
		new ReField( "TimeBeenVisible_", &GaSpeechBubbleComponent::TimeBeenVisible_, DsCore::DsCoreSerialised ),
		new ReField( "Text_", &GaSpeechBubbleComponent::Text_, DsCore::DsCoreSerialised ),
		new ReField( "TargetEntity_", &GaSpeechBubbleComponent::TargetEntity_, bcRFF_TRANSIENT ),
		new ReField( "SpriteOffset_", &GaSpeechBubbleComponent::SpriteOffset_,  DsCore::DsCoreSerialised )
	};

	ReRegisterClass< GaSpeechBubbleComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSpeechBubbleComponent::initialise()
{
	VisibleTime_ = 5.0f;
	TimeBeenVisible_ = 0.0f;
	RequiredSize_ = 0.0f;
	Visible_ = true;
	/*Text_.push_back(" ");
	Text_.push_back("I must escape");
	Text_.push_back("escape this");
	Text_.push_back("cage!");/**/

	FontOffset_ = MaVec2d( -165.0f, -210.0f );
	SpriteOffset_ = MaVec2d( -340.0f, 350.0f );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSpeechBubbleComponent::initialise( const Json::Value& Object )
{
	GaSpeechBubbleComponent::initialise();

	if ( Object[ "requiredSize" ].type() != Json::nullValue )
		RequiredSize_ = BcF32( Object[ "requiredsize" ].asDouble() );

	if ( Object[ "visibletime" ].type() != Json::nullValue )
		VisibleTime_ = BcF32( Object[ "visibletime" ].asDouble() );

	if ( Object[ "visible" ].type() != Json::nullValue )
		Visible_ = ( Object[ "visible" ].asBool() );
	if ( Object[ "spriteoffset" ].type() != Json::nullValue )
		SpriteOffset_ = MaVec2d( Object[ "spriteoffset" ].asCString() );

	if ( Object[ "text" ].type() != Json::nullValue )
	{
		std::string text = Object[ "text" ].asString();
		setText(text);
		
	}
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
		SpeechBubble_->setColour( RsColour( 1, 1, 1, Visible_ ? 0.8 : 0 ) );
	}
	if ( !TargetEntity_.isValid() )
	{
		// We aren't even gonna bother
		return;
	}

	MaMat4d TextScaleMatrix;
	//TextScaleMatrix.scale( MaVec4d( 0.04f, 0.04f, 1.0f, 1.0f ) );
	TextScaleMatrix.scale( MaVec4d( 1.00f, -1.00f, 1.0f, 1.0f ) );

	FontComponent_->setAlphaTestStepping( MaVec2d( 0.4f, 0.45f ) );


	if (Visible_)
	{
		TimeBeenVisible_ += Tick;
		if ( TimeBeenVisible_ > VisibleTime_ )
			Visible_ = false;
		//MaMat4d Matrix = getParentEntity()->getWorldMatrix();
		//Matrix = Canvas_->popMatrix();
		Canvas_->pushMatrix( TextScaleMatrix );
	
		MaVec2d Size;
		MaVec3d worldPos = TargetEntity_->getWorldPosition();
		MaVec2d Position( 0 , 0 );
		MaVec2d localPos  = SpeechBubble_->getPosition();
		localPos  = MaVec2d(TargetEntity_->getWorldPosition().x(), -TargetEntity_->getWorldPosition().y() ) + FontOffset_;
		SpeechBubble_->setPosition( MaVec2d( TargetEntity_->getWorldPosition().x(), TargetEntity_->getWorldPosition().y() )  + SpriteOffset_ );
		for( BcU32 Idx = 0; Idx < Text_.size(); ++Idx )
		{
			const auto& Option( Text_[ Idx ] );
			const auto Colour = RsColour::BLACK;
			Size = FontComponent_->drawCentered( Canvas_, localPos + Position, Text_[ Idx ] , Colour, 280 );
			Position += MaVec2d( 0.0f, Size.y() );
		}



		Canvas_->popMatrix();
	}
	//Canvas_->pushMatrix( Matrix );
	//Canvas_->setMatrix( Matrix );
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

//////////////////////////////////////////////////////////////////////////
// show
void GaSpeechBubbleComponent::show( BcF32 VisibleTime )
{
	Visible_ = true;
	VisibleTime_ = VisibleTime;
	TimeBeenVisible_ = 0.0f;
}

void GaSpeechBubbleComponent::setText(std::string Text)
{
	Text_.clear();
	boost::split(Text_, Text, boost::is_any_of("/"));
}

void GaSpeechBubbleComponent::setTarget( ScnEntityRef Target )
{
	TargetEntity_ = Target;
}
