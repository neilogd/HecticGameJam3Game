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

#include <functional>

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"
#include "Base/BcRandom.h"
#include "System/Debug/DsCore.h"
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
		new ReField( "Dimensions_", &GaTankComponent::Dimensions_ ),
		new ReField( "NoofFish_", &GaTankComponent::NoofFish_ ),
		new ReField( "SpawnRateMin_", &GaTankComponent::SpawnRateMin_ ),
		new ReField( "SpawnRateMax_", &GaTankComponent::SpawnRateMax_ ),
		new ReField( "SpawnTimer_", &GaTankComponent::SpawnTimer_, bcRFF_TRANSIENT )
	};

	ReRegisterClass< GaTankComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTankComponent::initialise()
{
	Dimensions_ = MaVec2d( 0.0f, 0.0f );
	NoofFish_ = 0;
	SpawnRateMin_ = 1.0f;
	SpawnRateMax_ = 2.0f;
	SpawnTimer_ = SpawnRateMin_;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTankComponent::initialise( const Json::Value& Object )
{
	initialise();
	WaterMaterialName_ = Object[ "watermaterial" ].asCString();
	GlassMaterialName_ = Object[ "glassmaterial" ].asCString();
	Dimensions_ = Object[ "dimensions" ].asCString();
	NoofFish_ = Object[ "nooffish" ].asUInt();
	SpawnRateMin_ = BcF32( Object[ "spawnratemin" ].asDouble() );
	SpawnRateMax_ = BcF32( Object[ "spawnratemax" ].asDouble() );
	SpawnTimer_ = SpawnRateMin_;
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

	// Food spawn logic.
	/*if( SpawnTimer_ < 0.0f )
	{
		// Update timer.
		BcF32 SpawnRange = BcRandom::Global.randRealRange( SpawnRateMin_, SpawnRateMax_ );
		SpawnTimer_ += SpawnRange;

		// Spawn the food.
		auto TankDimensions = getDimensions();
		auto CentralPosition = MaVec3d(
			BcRandom::Global.randRealRange( 0.0f, TankDimensions.x() ),
			TankDimensions.y(),
			0.0f );

		ScnEntitySpawnParams EnemyEntityParams =
		{
			"food", "FoodEntity", BcName( "FoodEntity" ).getUnique(),
			MaMat4d(),
			getParentEntity(),
			nullptr
		};

		EnemyEntityParams.Transform_.translation(
			CentralPosition );

		ScnCore::pImpl()->spawnEntity( EnemyEntityParams );

	}/**/
	SpawnTimer_ -= Tick;

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

	// Spawn fish.
	auto TankDimensions = getDimensions();
	auto CentralPosition = MaVec3d( 
		TankDimensions.x() * 0.5f,
		TankDimensions.y() * 0.5f,
		0.0f );

	BcF32 Rot = 0.0f;
	BcF32 RotAdv = BcPIMUL2 / BcF32( NoofFish_ );
	BcF32 Radius = 128.0f;
	for( BcU32 FishIdx = 0; FishIdx < NoofFish_; ++FishIdx )
	{
		ScnEntitySpawnParams EnemyEntityParams =
		{
			"enemies", "EnemyEntity", BcName( "EnemyEntity", FishIdx ),
			MaMat4d(),
			Parent,
			nullptr
		};

		EnemyEntityParams.Transform_.translation(
			CentralPosition +
			MaVec3d( BcCos( Rot ), -BcSin( Rot ), 0.0f ) * Radius );

		ScnCore::pImpl()->spawnEntity( EnemyEntityParams );

		Rot += RotAdv;
	}
	DsCore::pImpl()->registerFunction( "SpawnFood", std::bind( &GaTankComponent::spawnFood, this ) );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTankComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
	DsCore::pImpl()->deregisterFunction( "SpawnFood" );

}

//////////////////////////////////////////////////////////////////////////
// getDimensions
const MaVec2d& GaTankComponent::getDimensions() const
{
	return Dimensions_;
}


void GaTankComponent::spawnFood()
{
	ScnEntitySpawnParams EnemyEntityParams =
	{
		"default", "FoodEntity", BcName( "FoodEntity" ).getUnique(),
		MaMat4d(),
		getParentEntity(),
		nullptr
	};
	auto TankDimensions = getDimensions();
	auto CentralPosition = MaVec3d(
		BcRandom::Global.randRealRange( 0.0f, TankDimensions.x() ),
		TankDimensions.y(),
		0.0f );

	EnemyEntityParams.Transform_.translation(
		CentralPosition );

	ScnCore::pImpl()->spawnEntity( EnemyEntityParams );

}
