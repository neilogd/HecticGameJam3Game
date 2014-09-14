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
#include "System/Scene/Rendering/ScnSpriteComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"
#include "System/Os/OsEvents.h"

#include "System/Debug/DsCore.h"
//////////////////////////////////////////////////////////////////////////
// Define resource GaCustomisationComponent.
DEFINE_RESOURCE( GaCustomisationComponent );

void GaCustomisationComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Image_", &GaCustomisationComponent::Image_, bcRFF_TRANSIENT ),
		new ReField( "CreditImage_", &GaCustomisationComponent::CreditImage_, DsCore::DsCoreSerialised ),
		new ReField( "ShowCredits_", &GaCustomisationComponent::ShowCredits_, DsCore::DsCoreSerialised ),
		new ReField( "InGame_", &GaCustomisationComponent::InGame_, DsCore::DsCoreSerialised ),
	};

	ReRegisterClass< GaCustomisationComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaCustomisationComponent::initialise( const Json::Value& Object )
{
	ShowCredits_ = false;
	InGame_ = false;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaCustomisationComponent::initialise( )
{
	ShowCredits_ = false;
	InGame_ = false;
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaCustomisationComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	if (InGame_)
	{
		Image_->setColour( RsColour( 1.0f, 1.0f, 1.0f, ShowCredits_ ? 0.0f : 0.0f ) );
		CreditImage_->setColour( RsColour( 1.0f, 1.0f, 1.0f, ShowCredits_ ? 0.0f : 0.0f ) );
	}
	BcF32 width = OsCore::pImpl()->getClient( 0 )->getWidth();;
	BcF32 height = OsCore::pImpl()->getClient( 0 )->getHeight();;
	MaVec2d size( width, -height );
	Image_->setSize( size );
	CreditImage_->setSize( size );

	Image_->setColour( RsColour( 1.0f, 1.0f, 1.0f, ShowCredits_ ? 0.0f : 1.0f ) );
	CreditImage_->setColour( RsColour( 1.0f, 1.0f, 1.0f, ShowCredits_ ? 1.0f : 0.0f ) );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaCustomisationComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
	Image_ = getParentEntity()->getComponentByType<ScnSpriteComponent>( "CustomiseImage_0" );
	CreditImage_ = getParentEntity()->getComponentByType<ScnSpriteComponent>( "CreditImage_0" );


	OsEventInputMouse::Delegate OnMouseDown = OsEventInputMouse::Delegate::bind< GaCustomisationComponent, &GaCustomisationComponent::onMouseDown >( this );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, OnMouseDown );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaCustomisationComponent::onDetach( ScnEntityWeakRef Parent )
{

	OsCore::pImpl()->unsubscribeAll( this );
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// onMouseDown
eEvtReturn GaCustomisationComponent::onMouseDown( EvtID ID, const OsEventInputMouse& Event )
{
	if (ShowCredits_ )
	{
		ShowCredits_ = false;
		return evtRET_PASS;
	}
	if (InGame_)
		return evtRET_PASS;
	// Offset mouse postion.
	MaVec2d MousePosition( MaVec2d( BcF32( Event.MouseX_ ), BcF32( Event.MouseY_ ) ) );
	
	auto Client = OsCore::pImpl()->getClient( 0 );
	BcF32 wid = BcF32( Client->getWidth() );
	BcF32 hei = BcF32( Client->getHeight() );
	//MousePosition -= MaVec2d(  ) ) * 0.5f;
	//MousePosition = MaVec2d( MousePosition.x(), -MousePosition.y() );
	float ux = 2816.f * MousePosition.x() / wid;
	float uy = 2048 * MousePosition.y() / hei;

	if ( ( 350.f <= ux ) && ( ux <= 2560.f ) && 
		( 1550.f <= uy ) && ( uy <= 1695.f ) )
	{
		Image_->setColour(RsColour::RED);
		InGame_ = true;
		spawnGame();
		ScnCore::pImpl()->removeEntity( getParentEntity() );
	}
	else
	{
		Image_->setColour(RsColour::GREEN);
		
		ShowCredits_ = true;
	}
	return evtRET_PASS;
}


void GaCustomisationComponent::spawnGame()
{
	ScnEntitySpawnParams GameEntityParams =
	{
		"default", "GameEntity", "GameEntity_0",
		MaMat4d(),
		getParentEntity()->getParentEntity(),
	};

	GameEntityParams.OnSpawn_ = []( ScnEntity* ParentEntity )
	{
		MaVec3d Position( 0.0f, 0.0f, 0.0f );
		MaVec3d PositionIncrement( 2200.0f, 0.0f, 0.0f );

		const std::string Tanks[] =
		{
			"SpawnTankEntity",
			"TankEntity_0",
			"TankEntity_1",
			"TankEntity_1",
			"TankEntity_1",
			"WinnerTankEntity",
		};

		for( BcU32 Idx = 0; Idx < 5; ++Idx )
		{
			ScnEntitySpawnParams TankEntityParams =
			{
				"tank", Tanks[ Idx ], BcName( "TankEntity", Idx ),
				MaMat4d(),
				ParentEntity,
			};

			TankEntityParams.Transform_.translation( Position );
			Position += PositionIncrement;

			ScnCore::pImpl()->spawnEntity( TankEntityParams );
		}

		ScnEntitySpawnParams PlayerEntityParams =
		{
			"player", "PlayerEntity", "PlayerEntity_0",
			MaMat4d(),
			ParentEntity,
		};

		PlayerEntityParams.OnSpawn_ = []( ScnEntity* ParentEntity )
		{
			ScnEntitySpawnParams GuiEntityParams =
			{
				"gui", "GuiEntity", "GuiEntity_0",
				MaMat4d(),
				ParentEntity,
				nullptr
			};

			ScnCore::pImpl()->spawnEntity( GuiEntityParams );
		};

		ScnCore::pImpl()->spawnEntity( PlayerEntityParams );
	};

	ScnCore::pImpl()->spawnEntity( GameEntityParams );
}
