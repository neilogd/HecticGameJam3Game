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

#include "GaSwarmElementComponent.h"
#include "GaSpeechBubbleComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaTankComponent );

void GaTankComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Canvas_", &GaTankComponent::Canvas_, bcRFF_TRANSIENT ),
		new ReField( "SeaweedMaterial_", &GaTankComponent::SeaweedMaterial_, bcRFF_TRANSIENT ),
		new ReField( "SeaweedMaterialName_", &GaTankComponent::SeaweedMaterialName_ ),
		new ReField( "Dimensions_", &GaTankComponent::Dimensions_ ),
		new ReField( "NoofFish_", &GaTankComponent::NoofFish_ ),
		new ReField( "SpawnRateMin_", &GaTankComponent::SpawnRateMin_ ),
		new ReField( "SpawnRateMax_", &GaTankComponent::SpawnRateMax_ ),
		new ReField( "SpawnTimer_", &GaTankComponent::SpawnTimer_, bcRFF_TRANSIENT ),
		new ReField( "CannonPosition_", &GaTankComponent::CannonPosition_ ),
		new ReField( "AnimationTimer_", &GaTankComponent::AnimationTimer_, bcRFF_TRANSIENT ),
		new ReField( "SeaweedPositions_", &GaTankComponent::SeaweedPositions_, bcRFF_TRANSIENT ),
		new ReField( "Messages_", &GaTankComponent::Messages_, bcRFF_TRANSIENT ),
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
	AnimationTimer_ = 0.0f;
	Messages_.push_back("We don't/like yer kind/round here");
	Messages_.push_back("GERT OURT");
	Messages_.push_back("Get the new/comer!");
	Messages_.push_back("When did/you make/that choice?");
	Messages_.push_back("Someone needs/a cutting!");
	Messages_.push_back("You're askin'/for it");
	Messages_.push_back("Let's be/friends");
	Messages_.push_back(" / /We accept you");
	Messages_.push_back("You made/some choices/be happy");
	Messages_.push_back("Welcome...");




}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTankComponent::initialise( const Json::Value& Object )
{
	initialise();
	SeaweedMaterialName_ = Object[ "seaweedmaterial" ].asCString();
	Dimensions_ = Object[ "dimensions" ].asCString();
	NoofFish_ = Object[ "nooffish" ].asUInt();
	SpawnRateMin_ = BcF32( Object[ "spawnratemin" ].asDouble() );
	SpawnRateMax_ = BcF32( Object[ "spawnratemax" ].asDouble() );
	SpawnTimer_ = SpawnRateMin_;
	CannonPosition_ = Object[ "cannonposition" ].asCString();
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

	Canvas_->setMaterialComponent( SeaweedMaterial_ );

	auto Texture = SeaweedMaterial_->getTexture( SeaweedMaterial_->findTextureSlot( "aDiffuseTex" ) );

	ScnCanvasComponentVertex* FirstVertex = Canvas_->allocVertices( 16 * SeaweedPositions_.size() );
	ScnCanvasComponentVertex* Vertex = FirstVertex;
	MaVec2d HalfDimensions( MaVec2d( 256.0f, 768.0f ) * 0.85f );
	for( BcU32 Idx = 0; Idx < SeaweedPositions_.size(); ++Idx )
	{
		auto SeaweedPosition = SeaweedPositions_[ Idx ];
		auto TextureRect = Texture->getRect( SeaweedSprites_[ Idx ] );
		ScnCanvasComponentVertex* PrimitiveVertex = Vertex;

		// 0
		Vertex[ 0 ].X_ = -HalfDimensions.x() + SeaweedPosition.x();
		Vertex[ 0 ].Y_ = 0.0f + SeaweedPosition.y();
		Vertex[ 0 ].Z_ = 0.0f;
		Vertex[ 0 ].ABGR_ = 0xffffffff;
		Vertex[ 0 ].U_ = TextureRect.X_;
		Vertex[ 0 ].V_ = TextureRect.Y_ + TextureRect.H_;
		
		// 1	   
		Vertex[ 1 ].X_ = HalfDimensions.x() + SeaweedPosition.x();
		Vertex[ 1 ].Y_ = 0.0f + SeaweedPosition.y();
		Vertex[ 1 ].Z_ = 0.0f;
		Vertex[ 1 ].ABGR_ = 0xffffffff;
		Vertex[ 1 ].U_ = TextureRect.X_ + TextureRect.W_;
		Vertex[ 1 ].V_ = TextureRect.Y_ + TextureRect.H_;
		
		// 14	   
		Vertex[ 14 ].X_ = -HalfDimensions.x() + SeaweedPosition.x();
		Vertex[ 14 ].Y_ = HalfDimensions.y() + SeaweedPosition.y();
		Vertex[ 14 ].Z_ = 0.0f;
		Vertex[ 14 ].ABGR_ = 0xffffffff;
		Vertex[ 14 ].U_ = TextureRect.X_;
		Vertex[ 14 ].V_ = TextureRect.Y_ + ( TextureRect.H_ * 0.4f );
		
		// 15
		Vertex[ 15 ].X_ = HalfDimensions.x() + SeaweedPosition.x();
		Vertex[ 15 ].Y_ = HalfDimensions.y() + SeaweedPosition.y();
		Vertex[ 15 ].Z_ = 0.0f;
		Vertex[ 15 ].ABGR_ = 0xffffffff;
		Vertex[ 15 ].U_ = TextureRect.X_ + TextureRect.W_;
		Vertex[ 15 ].V_ = TextureRect.Y_ + ( TextureRect.H_ * 0.4f );

		for( BcU32 VertIdx = 2; VertIdx < 16; VertIdx  += 2 )
		{
			BcF32 LerpVal = BcF32( VertIdx ) / 16.0f;
			BcF32 XOffset = BcSin( LerpVal * BcPIMUL2 + ( AnimationTimer_ + BcF32( Idx ) ) ) * 16.0f;

			if( VertIdx < 4 )
			{
				XOffset = 0.0f;
			}

			Vertex[ VertIdx ].X_ = BcLerp( Vertex[ 0 ].X_, Vertex[ 14 ].X_, LerpVal ) + XOffset;
			Vertex[ VertIdx ].Y_ = BcLerp( Vertex[ 0 ].Y_, Vertex[ 14 ].Y_, LerpVal );
			Vertex[ VertIdx ].Z_ = 0.0f;
			Vertex[ VertIdx ].ABGR_ = 0xffffffff;
			Vertex[ VertIdx ].U_ = BcLerp( Vertex[ 0 ].U_, Vertex[ 14 ].U_, LerpVal );
			Vertex[ VertIdx ].V_ = BcLerp( Vertex[ 0 ].V_, Vertex[ 14 ].V_, LerpVal );

			// 1	   
			Vertex[ VertIdx + 1 ].X_ = BcLerp( Vertex[ 1 ].X_, Vertex[ 15 ].X_, LerpVal ) + XOffset;
			Vertex[ VertIdx + 1 ].Y_ = BcLerp( Vertex[ 1 ].Y_, Vertex[ 15 ].Y_, LerpVal );
			Vertex[ VertIdx + 1 ].Z_ = 0.0f;
			Vertex[ VertIdx + 1 ].ABGR_ = 0xffffffff;
			Vertex[ VertIdx + 1 ].U_ = BcLerp( Vertex[ 1 ].U_, Vertex[ 15 ].U_, LerpVal );
			Vertex[ VertIdx + 1 ].V_ = BcLerp( Vertex[ 1 ].V_, Vertex[ 15 ].V_, LerpVal );

		}

		Canvas_->addPrimitive( RsTopologyType::TRIANGLE_STRIP, PrimitiveVertex, 16, ( Idx % 3 ) * 10 + 3, BcTrue );
		Vertex += 16;
	}

	AnimationTimer_ += Tick;

	Canvas_->popMatrix();

	// Food spawn logic.
	if( SpawnTimer_ < 0.0f )
	{
		// Update timer.
		BcF32 SpawnRange = BcRandom::Global.randRealRange( SpawnRateMin_, SpawnRateMax_ );
		SpawnTimer_ += SpawnRange;

		// Spawn the food.
		spawnFood( 8 );

	}
	SpawnTimer_ -= Tick;

	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTankComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Find a canvas to use for rendering (someone in ours, or our parent's hierarchy).
	Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >( 0 );
	BcAssertMsg( Canvas_.isValid(), "Sprite component needs to be attached to an entity with a canvas component in any parent!" );

	// Find a canvas to use for rendering (someone in ours, or our parent's hierarchy).
	SeaweedMaterial_ = Parent->getComponentAnyParentByType< ScnMaterialComponent >( SeaweedMaterialName_ );

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
	BcChar buffer[256];
	BcMemZero(buffer, 256);
	BcSPrintf(buffer, "%s_%s", (*getParentEntity()->getName()).c_str(), "Spawn_Food");
	DsCore::pImpl()->registerFunction( buffer, std::bind( &GaTankComponent::spawnFood, this, 1 ) );
	BcSPrintf(buffer, "%s_%s", (*getParentEntity()->getName()).c_str(), "Reset_Position");
	DsCore::pImpl()->registerFunction( buffer, std::bind( &GaTankComponent::magicReset, this ) );

	for( BcU32 Idx = 0; Idx < 8; ++Idx )
	{
		MaVec2d Position;
		Position.x( BcRandom::Global.randRealRange( 150.0f, Dimensions_.x() - 150.0f ) );
		Position.y( BcRandom::Global.randRealRange( 32, 100 ) );
		SeaweedPositions_.push_back( Position );
		SeaweedSprites_.push_back( BcRandom::Global.randRange( 1, 15 ) );
	}

	ScnEntitySpawnParams CannonEntityParams =
	{
		"cannon", "CannonEntity", "CannonEntity_0",
		MaMat4d(),
		Parent,
		nullptr
	};

	CannonEntityParams.Transform_.translation(
		CannonPosition_ );

	ScnCore::pImpl()->spawnEntity( CannonEntityParams );

	Super::onAttach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTankComponent::onDetach( ScnEntityWeakRef Parent )
{
	
	BcChar buffer[256];
	BcSPrintf(buffer, "%s_%s", (*getParentEntity()->getName()).c_str(), "Spawn_Food");
	DsCore::pImpl()->deregisterFunction( buffer );
	BcSPrintf(buffer, "%s_%s", (*getParentEntity()->getName()).c_str(), "Reset_Position");
	DsCore::pImpl()->deregisterFunction( buffer );

	Super::onDetach( Parent );	
}

//////////////////////////////////////////////////////////////////////////
// getDimensions
const MaVec2d& GaTankComponent::getDimensions() const
{
	return Dimensions_;
}

//////////////////////////////////////////////////////////////////////////
// spawnFood
void GaTankComponent::spawnFood( BcU32 Noof )
{
	ScnEntitySpawnParams EnemyEntityParams =
	{
		"food", "FoodEntity", BcName( "FoodEntity" ).getUnique(),
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

	ScnCore::pImpl()->spawnEntity( EnemyEntityParams ) ;
}

void GaTankComponent::magicReset()
{
	auto TankDimensions = getDimensions();
	auto CentralPosition = MaVec3d( 
		TankDimensions.x() * 0.5f,
		TankDimensions.y() * 0.5f,
		0.0f ) + getParentEntity()->getWorldPosition();
	BcF32 Rot = 0.0f;
	BcF32 RotAdv = BcPIMUL2 / BcF32( Children.size() );
	BcF32 Radius = 128.0f;
	for( BcU32 FishIdx = 0; FishIdx < Children.size(); ++FishIdx )
	{
		Children[FishIdx]->setWorldPosition( CentralPosition +
			MaVec3d( BcCos( Rot ), -BcSin( Rot ), 0.0f ) * Radius );

		Rot += RotAdv;
	}
}

void GaTankComponent::registerEnemy(ScnEntityRef entity)
{
	Children.push_back( entity );
}


MaVec2d GaTankComponent::getCentralPosition()
{
	return MaVec2d( 
		getDimensions().x() * 0.5f + getParentEntity()->getWorldPosition().x(),
		getDimensions().y() * 0.5f + getParentEntity()->getWorldPosition().y());
}

void GaTankComponent::receiveFish()
{
	int message = BcRandom::Global.rand() % Messages_.size();
	ScnEntityRef first;
	for ( BcU32 Idx = 0; Idx < Children.size(); ++Idx )
	{
		if ( Children[ Idx ].isValid() )
		{
			first = Children[ Idx ];
			break;
		}
	}
	if (first.isValid())
	{
		GaSpeechBubbleComponentRef bubble = getParentEntity()->getComponentAnyParentByType<GaSpeechBubbleComponent>();
		bubble->setTarget(first);
		bubble->setText(Messages_[message]);
		bubble->show();
	}
}