/**************************************************************************
*
* File:		GaSwarmElementComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaSwarmElementComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

#include "GaSwarmManagerComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaSwarmElementComponent );

void GaSwarmElementComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Manager_", &GaSwarmElementComponent::Manager_, bcRFF_TRANSIENT ),
		new ReField( "UnitMask_", &GaSwarmElementComponent::UnitMask_ ),
		new ReField( "Acceleration_", &GaSwarmElementComponent::Acceleration_ ),
		new ReField( "StagedAcceleration_", &GaSwarmElementComponent::StagedAcceleration_ ),
		new ReField( "Velocity_", &GaSwarmElementComponent::Velocity_ ),
		new ReField( "StagedVelocity_", &GaSwarmElementComponent::StagedVelocity_ ),
	};

	ReRegisterClass< GaSwarmElementComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 1 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmElementComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSwarmElementComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSwarmElementComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Manager_ = Parent->getComponentAnyParentByType< GaSwarmElementComponent >();

	// TODO: Register with manager or something.
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaSwarmElementComponent::onDetach(ScnEntityWeakRef Parent)
{
	Super::onDetach(Parent);

	// TODO: Unregister with manager or something.
}


//////////////////////////////////////////////////////////////////////////
// getUnitMask
//virtual
BcU8 GaSwarmElementComponent::getUnitMask()
{
	return UnitMask_;
}

MaVec2d	GaSwarmElementComponent::getAcceleration()
{
	return Acceleration_;
}
void GaSwarmElementComponent::stageAcceleration( MaVec2d acceleration )
{
	StagedAcceleration_ = acceleration;
}
MaVec2d GaSwarmElementComponent::getVelocity()
{
	return Velocity_;
}
void GaSwarmElementComponent::stageVelocity( MaVec2d acceleration )
{
	StagedVelocity_ = acceleration;
}
void GaSwarmElementComponent::commitChanges()
{
	Acceleration_ = StagedAcceleration_;
	Velocity_ = StagedVelocity_;
}

MaVec2d GaSwarmElementComponent::getPosition()
{
	MaVec3d pos = getParentEntity()->getLocalPosition();
	return MaVec2d(pos.x(), pos.y());
}