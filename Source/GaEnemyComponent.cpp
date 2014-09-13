/**************************************************************************
*
* File:		GaEnemyComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaEnemyComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

#include "GaTankComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaEnemyComponent );

void GaEnemyComponent::StaticRegisterClass()
{
	ReRegisterClass< GaEnemyComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaEnemyComponent::initialise( const Json::Value& Object )
{
	
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaEnemyComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaEnemyComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	GaTankComponentRef tank = getParentEntity()->getParentEntity()->getComponentByType< GaTankComponent >();

	if (tank.isValid())
	{
		tank->registerEnemy(getParentEntity());
	}
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaEnemyComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
