/**************************************************************************
*
* File:		GaFoodComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaFoodComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaFoodComponent );

void GaFoodComponent::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Size_", &GaFoodComponent::Size_, DsCore::DsCoreSerialised ),
	};
	
	ReRegisterClass< GaFoodComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFoodComponent::initialise()
{
	Size_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaFoodComponent::initialise( const Json::Value& Object )
{
	initialise();

	Size_ = BcF32( Object[ "size" ].asDouble() );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaFoodComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaFoodComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaFoodComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// tryEat
BcF32 GaFoodComponent::tryEat( BcF32 Amount )
{
	if( Amount < Size_ )
	{
		Size_ -= Amount;
		return Amount;
	}

	ScnCore::pImpl()->removeEntity( getParentEntity() );

	return Size_;
}
