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
#include "System/Debug/DsCore.h"
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
		new ReField( "Velocity_", &GaSwarmElementComponent::Velocity_ , DsCore::DsCoreSerialised),
		new ReField( "StagedVelocity_", &GaSwarmElementComponent::StagedVelocity_ ),
		new ReField( "VelocityDriven_", &GaSwarmElementComponent::VelocityDriven_ ),
	};
		
	ReRegisterClass< GaSwarmElementComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 1 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmElementComponent::initialise( const Json::Value& Object )
{
	initialise( );
	if (Object["unitmask"] != Json::ValueType::nullValue)
	{
		UnitMask_ = (BcU8)Object["unitmask"].asUInt();
	}
	if (Object["velocitydriven"] != Json::ValueType::nullValue)
	{
		VelocityDriven_ = Object["velocitydriven"].asBool();
	}
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaSwarmElementComponent::initialise( )
{
	UnitMask_ = 0;
	Acceleration_ = MaVec2d( 0, 0 );
	Velocity_ = MaVec2d( 0, 0 );
	StagedAcceleration_ = MaVec2d( 0, 0 );
	StagedVelocity_ = MaVec2d( 0, 0 );
	VelocityDriven_ = false;
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSwarmElementComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	if ( VelocityDriven_ )
	{
		MaVec2d position = getPosition();
		position = position + Velocity_;
	
		MaVec3d realPos(position, 0.0f);
		getParentEntity()->setLocalPosition(realPos);
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSwarmElementComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Manager_ = Parent->getComponentAnyParentByType< GaSwarmElementComponent >();

	// TODO: Register with manager or something.
	BcAssert( Manager_ == nullptr );
	Manager_->registerElement(this);
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaSwarmElementComponent::onDetach(ScnEntityWeakRef Parent)
{
	Super::onDetach(Parent);

	// TODO: Unregister with manager or something.
	Manager_->deregisterElement(this);
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