/**************************************************************************
*
* File:		GaGuiComponent.cpp
* Author:	Daniel de Zwart
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaPlayerComponent.h"
#include "GaGuiComponent.h"
#include "GaPlayerComponent.h"
#include "GaFishComponent.h"
#include "GaSwarmElementComponent.h"
#include "GaCannonComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnSpriteComponent.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"
#include "System/Os/OsEvents.h"

#include "System/Debug/DsCore.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaGuiComponent );

void GaGuiComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Canvas_", &GaGuiComponent::Canvas_, bcRFF_TRANSIENT ),
		new ReField( "Player_", &GaGuiComponent::Player_, bcRFF_TRANSIENT ),
		new ReField( "Swarm_", &GaGuiComponent::Swarm_, bcRFF_TRANSIENT ),
		new ReField( "HealthSprite_", &GaGuiComponent::HealthSprite_, bcRFF_TRANSIENT ),
		new ReField( "Background_", &GaGuiComponent::Background_, bcRFF_TRANSIENT ),
		new ReField( "HealthBar_", &GaGuiComponent::HealthBar_, bcRFF_TRANSIENT ),
		new ReField( "Pointer_", &GaGuiComponent::Pointer_, bcRFF_TRANSIENT ),
		new ReField( "PointerOffset_", &GaGuiComponent::PointerOffset_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "AssetOffset_", &GaGuiComponent::AssetOffset_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "RotationAmount_", &GaGuiComponent::RotationAmount_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "RotateLeft_", &GaGuiComponent::RotateLeft_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
		new ReField( "RotateRight_", &GaGuiComponent::RotateRight_, ReFieldFlags::bcRFF_DEBUG_EDIT ),
	};

	ReRegisterClass< GaGuiComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// ScnShaderViewUniformBlockData
REFLECTION_DEFINE_BASIC( GaGuiShaderUniformBlockData );

void GaGuiShaderUniformBlockData::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "VariableNameHere_",				&GaGuiShaderUniformBlockData::VariableNameHere_ )/*,
		new ReField( "ProjectionTransform_",			&ScnShaderViewUniformBlockData::ProjectionTransform_ ),
		new ReField( "InverseViewTransform_",			&ScnShaderViewUniformBlockData::InverseViewTransform_ ),
		new ReField( "ViewTransform_",					&ScnShaderViewUniformBlockData::ViewTransform_ ),
		new ReField( "ClipTransform_",					&ScnShaderViewUniformBlockData::ClipTransform_ )/**/
	};
		
	ReRegisterClass< GaGuiShaderUniformBlockData >( Fields );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaGuiComponent::initialise()
{
	UniformBuffer_ = nullptr;
	AssetOffset_ = MaVec2d( 0, 0 );
	PointerOffset_ = MaVec2d( 0, 0 );
	RotationAmount_ = 0.0f;
	RotateLeft_ = 0.0f;
	RotateRight_ = 1.2f;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaGuiComponent::initialise( const Json::Value& Object )
{
	initialise();
	if (Object[ "assetoffset" ].type() != Json::nullValue )
	{
		AssetOffset_ = MaVec2d( Object[ "assetoffset" ].asCString() );
	}	
	if (Object[ "pointeroffset" ].type() != Json::nullValue )
	{
		PointerOffset_ = MaVec2d( Object[ "pointeroffset" ].asCString() );
	}
	if (Object[ "rotateleft" ].type() != Json::nullValue )
	{
		RotateLeft_ = (float)( Object[ "rotateleft" ].asDouble() );
	}
	if (Object[ "rotateright" ].type() != Json::nullValue )
	{
		RotateRight_ = (float)( Object[ "rotateright" ].asDouble() );
	}
}

//////////////////////////////////////////////////////////////////////////
// preUpdate
//virtual
void GaGuiComponent::preUpdate( BcF32 Tick )
{
	Super::preUpdate( Tick );

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaGuiComponent::update( BcF32 Tick )
{
	// Player is attached before cannons are to tanks. Need to get rid of the
	// delayed attachment in the engine to fix this properly.

	Super::update( Tick );


	if (!Player_.isValid())
	{
		Player_ = ParentEntity_->getParentEntity()->getComponentByType<GaPlayerComponent>();
	}

	if( !Swarm_.isValid() )
	{
		Swarm_ = ParentEntity_->getParentEntity()->getComponentByType<GaSwarmElementComponent>();
	}

	if (Swarm_.isValid())
	{
		HealthBar_ = ParentEntity_->getComponentByType<ScnSpriteComponent>( BcName( "HealthBar", 0 ) );
		Background_ = ParentEntity_->getComponentByType<ScnSpriteComponent>( BcName( "Background", 0 ) );
		Pointer_ = ParentEntity_->getComponentByType<ScnSpriteComponent>( BcName( "FoodPointer", 0 ) );
		Fish_ = ParentEntity_->getParentEntity()->getComponentByType<GaFishComponent>( );
		if( Player_->getCannon().isValid() )
		{
			GaCannonComponentRef cannon = Player_->getCannon()->getComponentByType<GaCannonComponent>( );

			RotationAmount_ = 0.0f;
			if ( cannon.isValid() )
			{
				BcF32 MinSize = 0.75f;
				BcF32 TotalRange = ( cannon->getRequiredSize() - MinSize ) * 2.0f;
				BcF32 requiredSize = cannon->getRequiredSize();
				BcF32 currentSize = Fish_->getFishSize();
				RotationAmount_ = BcClamp( ( currentSize - MinSize ) / TotalRange, 0.0f, 1.0f);
			}
		}


		BcF32 health = Swarm_->getHealth();
		BcF32 maxHealth = Swarm_->getMaxHealth();
		float ratio = health / maxHealth;
		float width = 640.0f * ratio;
		MaVec2d ScreenBottom = MaVec2d(0.0f, -(BcF32)OsCore::pImpl()->getClient(0)->getHeight() / 2);
		if (HealthBar_.isValid())
		{
			HealthBar_->setSize( MaVec2d( width, -256 ) );
			HealthBar_->setPosition( AssetOffset_ + ScreenBottom );
		}
		if (Background_.isValid())
		{
			Background_->setPosition( AssetOffset_ + ScreenBottom );
		}
		if (Pointer_.isValid())
		{
			Pointer_->setPosition( AssetOffset_ + ScreenBottom + PointerOffset_ );
			BcF32 rotate = RotateRight_ * RotationAmount_  + RotateLeft_ * ( 1.0f - RotationAmount_ );
			Pointer_->setRotation( rotate );
		}
	}
	
	GaGuiShaderUniformBlockData block;
	block.VariableNameHere_ = MaVec4d(1.0f, 0,0,0);
	/**RsCore::pImpl()->updateBuffer( 
		UniformBuffer_,
		0, sizeof( block ),
		RsResourceUpdateFlags::ASYNC,
		[ block ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			BcMemCopy( Lock.Buffer_, &block, sizeof( block ) );
		} );
		/**/
}

//////////////////////////////////////////////////////////////////////////
// postUpdfate
//virtual
void GaGuiComponent::postUpdate( BcF32 Tick )
{
	Super::postUpdate( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaGuiComponent::onAttach( ScnEntityWeakRef Parent )
{

	Super::onAttach( Parent );

	Player_ = ParentEntity_->getParentEntity()->getComponentByType<GaPlayerComponent>();

	ScnMaterialComponentRef MaterialComponent_ =  ParentEntity_->getComponentByType<ScnMaterialComponent>("GuiMaterialComponent_2");


	/*UniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( GaGuiShaderUniformBlockData ) ) );
	auto UniformBlock = MaterialComponent_->findUniformBlock( "GuiUniformBlock" );
	BcAssert( UniformBlock != BcErrorCode );
	MaterialComponent_->setUniformBlock( UniformBlock, UniformBuffer_ );/**/

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaGuiComponent::onDetach( ScnEntityWeakRef Parent )
{

	RsCore::pImpl()->destroyResource( UniformBuffer_ );
	Super::onDetach( Parent );

}
