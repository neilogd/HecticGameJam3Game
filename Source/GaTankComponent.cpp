/**************************************************************************
*
* File:		GaTankComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTankComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaTankComponent );

void GaTankComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Canvas_", &GaTankComponent::Canvas_, bcRFF_TRANSIENT ),
		new ReField( "WaterMaterial_", &GaTankComponent::WaterMaterial_, bcRFF_TRANSIENT ),
		new ReField( "GlassMaterial_", &GaTankComponent::GlassMaterial_, bcRFF_TRANSIENT ),
		new ReField( "WaterMaterialName_", &GaTankComponent::WaterMaterialName_ ),
		new ReField( "GlassMaterialName_", &GaTankComponent::GlassMaterialName_ ),
		new ReField( "Dimensions_", &GaTankComponent::Dimensions_ )
	};

	ReRegisterClass< GaTankComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTankComponent::initialise( const Json::Value& Object )
{
	WaterMaterialName_ = Object[ "watermaterial" ].asCString();
	GlassMaterialName_ = Object[ "glassmaterial" ].asCString();
	Dimensions_ = Object[ "dimensions" ].asCString();
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaTankComponent::update( BcF32 Tick )
{
	ScnEntityWeakRef Entity = getParentEntity();
	const MaMat4d& Matrix = Entity->getWorldMatrix();

	// Push matrix onto canvas.
	Canvas_->pushMatrix( Matrix );

	Canvas_->setMaterialComponent( GlassMaterial_ );
	Canvas_->drawBox( 
		MaVec2d( 0.0f, 0.0f ), 
		Dimensions_,
		RsColour( 0.5f, 1.0f, 1.0f, 0.2f ),
		0 );

	Canvas_->popMatrix();

	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTankComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	// Find a canvas to use for rendering (someone in ours, or our parent's hierarchy).
	Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >( 0 );
	BcAssertMsg( Canvas_.isValid(), "Sprite component needs to be attached to an entity with a canvas component in any parent!" );

	// Find a canvas to use for rendering (someone in ours, or our parent's hierarchy).
	WaterMaterial_ = Parent->getComponentAnyParentByType< ScnMaterialComponent >( WaterMaterialName_ );
	GlassMaterial_ = Parent->getComponentAnyParentByType< ScnMaterialComponent >( GlassMaterialName_ );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTankComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// getDimensions
const MaVec2d& GaTankComponent::getDimensions() const
{
	return Dimensions_;
}
