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
// initialise
void GaGuiComponent::initialise()
{

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
		Player_ = ParentEntity_->getParentEntity()->getComponentByType<GaPlayerComponent>();
	if (Player_.isValid())
	{
		BcF32 health = Player_->getHealth();
		BcF32 maxHealth = Player_->getMaxHealth();
		float ratio = health / maxHealth;
		float width = 512.f * ratio;
		ScnSpriteComponentRef sprite = ParentEntity_->getComponentByType<ScnSpriteComponent>(BcName("HealthBar",0));
		if (sprite.isValid())
		{
			sprite->setSize(MaVec2d(width, 64.0f));
		}
	}
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
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaGuiComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
