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
		new ReField( "HealthSprite_", &GaGuiComponent::HealthSprite_, bcRFF_TRANSIENT ),
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
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaGuiComponent::initialise( const Json::Value& Object )
{
	initialise();
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

	if( !Fish_.isValid() )
	{
		Fish_ = ParentEntity_->getParentEntity()->getComponentByType<GaFishComponent>();
	}

	if (Fish_.isValid())
	{
		BcF32 health = Fish_->getHealth();
		BcF32 maxHealth = Fish_->getMaxHealth();
		float ratio = health / maxHealth;
		float width = 512.f * ratio;
		ScnSpriteComponentRef sprite = ParentEntity_->getComponentByType<ScnSpriteComponent>(BcName("HealthBar",0));
		if (sprite.isValid())
		{
			sprite->setSize(MaVec2d(width, 64.0f));
		}
	}
	/*
	GaGuiShaderUniformBlockData block;
	block.VariableNameHere_ = MaVec4d(0.5f, 0,0,0);
	RsCore::pImpl()->updateBuffer( 
		UniformBuffer_,
		0, sizeof( block ),
		RsResourceUpdateFlags::ASYNC,
		[ block ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			BcMemCopy( Lock.Buffer_, &block, sizeof( block ) );
		} );
		*/
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

	ScnMaterialComponentRef MaterialComponent_ =  ParentEntity_->getComponentByType<ScnMaterialComponent>("GuiMaterialComponent_3");


	UniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( GaGuiShaderUniformBlockData ) ) );
	auto UniformBlock = MaterialComponent_->findUniformBlock( "GuiUniformBlock" );
	BcAssert( UniformBlock != BcErrorCode );
	MaterialComponent_->setUniformBlock( UniformBlock, UniformBuffer_ );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaGuiComponent::onDetach( ScnEntityWeakRef Parent )
{

	RsCore::pImpl()->destroyResource( UniformBuffer_ );
	Super::onDetach( Parent );

}
