/**************************************************************************
*
* File:		GaFoodComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaFoodComponent_H__
#define __GaFoodComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaFoodComponent > GaFoodComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaFoodComponent
class GaFoodComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaFoodComponent, ScnComponent );

	void initialise();
	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

	BcF32 tryEat( BcF32 Amount );

private:
	BcF32 Size_;
	BcF32 Life_;
	BcU32 Sprite_;

	std::vector< MaVec2d > SpriteSizes_;
	std::vector< ReObjectRef< class ScnSpriteComponent > > Sprites_;
};

#endif

