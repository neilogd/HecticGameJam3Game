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
		new ReField( "FishSize_", &GaFishComponent::FishSize_, DsCore::DsCoreSerialised ),
		new ReField( "SwarmManager_", &GaFishComponent::SwarmManager_, bcRFF_TRANSIENT | DsCore::DsCoreSerialised )
	};
	
	ReRegisterClass< GaFishComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFishComponent::initialise()
{
	FishSize_ = 0.0f;
	SwarmManager_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFishComponent::initialise( const Json::Value& Object )
{
	initialise();

	FishSize_ = BcF32( Object[ "fishsize" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaFishComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	if( SwarmManager_ != nullptr )
	{
		auto Food = SwarmManager_->getNearbyUnits( 
			MaVec2d( 
				getParentEntity()->getWorldPosition().x(),
				getParentEntity()->getWorldPosition().y() ), 1, GaSwarmManagerComponent::FOOD );
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
	return FishSize_;
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
