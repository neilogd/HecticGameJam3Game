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
#include "System/Scene/Rendering/ScnCanvasComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"
#include "System/Os/OsEvents.h"

#include "System/Debug/DsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaPlayerComponent );

void GaPlayerComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Canvas_", &GaPlayerComponent::Canvas_, bcRFF_TRANSIENT ),
		new ReField( "ViewMatrix_", &GaPlayerComponent::ViewMatrix_, DsCore::DsCoreSerialised ),
		new ReField( "TargetPosition_", &GaPlayerComponent::TargetPosition_, DsCore::DsCoreSerialised ),
	};

	ReRegisterClass< GaPlayerComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaPlayerComponent::initialise( const Json::Value& Object )
{
	TargetPosition_ = MaVec2d( 0.0f, 0.0f );
}

//////////////////////////////////////////////////////////////////////////
// preUpdate
//virtual
void GaPlayerComponent::preUpdate( BcF32 Tick )
{
	Super::preUpdate( Tick );

	ViewMatrix_.translation( -getParentEntity()->getWorldPosition() );
	Canvas_->setViewMatrix( ViewMatrix_ );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaPlayerComponent::update( BcF32 Tick )
{
	getParentEntity()->setLocalPosition( 
		MaVec3d( TargetPosition_, 0.0f ) );

	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// postUpdfate
//virtual
void GaPlayerComponent::postUpdate( BcF32 Tick )
{
	Super::postUpdate( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaPlayerComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	OsEventInputMouse::Delegate OnMouseDown = OsEventInputMouse::Delegate::bind< GaPlayerComponent, &GaPlayerComponent::onMouseDown >( this );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, OnMouseDown );

	// Find a canvas to use for rendering (someone in ours, or our parent's hierarchy).
	Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >( 0 );
	BcAssertMsg( Canvas_.isValid(), "Player component needs to be attached to an entity with a canvas component in any parent!" );

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
	// Offset mouse postion.
	MaVec2d MousePosition( MaVec2d( BcF32( Event.MouseX_ ), BcF32( Event.MouseY_ ) ) );
	
	auto Client = OsCore::pImpl()->getClient( 0 );

	MousePosition -= MaVec2d( BcF32( Client->getWidth() ), BcF32( Client->getHeight() ) ) * 0.5f;
	MousePosition = MaVec2d( MousePosition.x(), -MousePosition.y() );
	
	MaMat4d InverseViewMatrix = ViewMatrix_;
	InverseViewMatrix.inverse();
	MousePosition = MousePosition * InverseViewMatrix;

	TargetPosition_ = MousePosition;

	return evtRET_PASS;
}

