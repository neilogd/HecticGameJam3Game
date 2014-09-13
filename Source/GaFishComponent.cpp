/**************************************************************************
*
* File:		GaFishComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaFishComponent.h"
#include "GaFoodComponent.h"
#include "GaSwarmManagerComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcProfiler.h"
 
//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaFishComponent );

void GaFishComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Size_", &GaFishComponent::Size_, DsCore::DsCoreSerialised ),
		new ReField( "SizeIncreaseMultiplier_", &GaFishComponent::SizeIncreaseMultiplier_, DsCore::DsCoreSerialised ),
		new ReField( "EatDistance_", &GaFishComponent::EatDistance_, DsCore::DsCoreSerialised ),
		new ReField( "EatSpeed_", &GaFishComponent::EatSpeed_, DsCore::DsCoreSerialised ),
		new ReField( "SwarmManager_", &GaFishComponent::SwarmManager_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised )
	};
	
	ReRegisterClass< GaFishComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFishComponent::initialise()
{
	Size_ = 0.0f;
	SizeIncreaseMultiplier_ = 1.0f;
	EatDistance_ = 0.0f;
	SwarmManager_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFishComponent::initialise( const Json::Value& Object )
{
	initialise();

	Size_ = BcF32( Object[ "size" ].asDouble() );
	SizeIncreaseMultiplier_ = BcF32( Object[ "sizeincreasemultiplier" ].asDouble() );
	EatDistance_ = BcF32( Object[ "eatdistance" ].asDouble() );
	EatSpeed_ = BcF32( Object[ "eatspeed" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaFishComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	if( SwarmManager_ != nullptr )
	{
		auto Foods = SwarmManager_->getNearbyUnits( 
			MaVec2d( 
				getParentEntity()->getWorldPosition().x(),
				getParentEntity()->getWorldPosition().y() ), 1, GaSwarmManagerComponent::FOOD );

		if( Foods.size() > 0 )
		{
			auto FoodSwarmElement = Foods[ 0 ];
			auto Distance = 
				( FoodSwarmElement->getParentEntity()->getWorldPosition() - getParentEntity()->getWorldPosition() ).magnitude();

			if( Distance < EatDistance_ )
			{
				auto Food = FoodSwarmElement->getParentEntity()->getComponentByType< GaFoodComponent >();
				auto AmountAte = Food->tryEat( Tick * EatSpeed_ );

				Size_ += AmountAte * SizeIncreaseMultiplier_;

				// Size change, set RS matrix.
				MaMat4d Scale;
				Scale.scale( MaVec4d( Size_, Size_, 0.0f, 0.0f ) );
				//getParentEntity()->setLocalMatrixRS( Scale );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaFishComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaFishComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// getFishSize
BcF32 GaFishComponent::getFishSize() const
{
	return Size_;
}

//////////////////////////////////////////////////////////////////////////
// getFishSize
void GaFishComponent::updateSwarmManagerRef( GaSwarmManagerComponent* SwarmManager )
{
	if( SwarmManager != nullptr )
	{
		SwarmManager_ = SwarmManager;
	}
	else
	{
		SwarmManager_ = getParentEntity()->getComponentAnyParentByType< GaSwarmManagerComponent >();
	}
}
