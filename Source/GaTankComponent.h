/**************************************************************************
*
* File:		GaTankComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTankComponent_H__
#define __GaTankComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTankComponent > GaTankComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTankComponent
class GaTankComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaTankComponent, ScnComponent );

	void initialise();
	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

	const MaVec2d& getDimensions() const;
	void spawnFood();
	void magicReset();

	void registerEnemy(ScnEntityRef entity);
private:
	ReObjectRef< class ScnCanvasComponent > Canvas_;
	ReObjectRef< class ScnMaterialComponent > WaterMaterial_;
	ReObjectRef< class ScnMaterialComponent > GlassMaterial_;
	std::string WaterMaterialName_;
	std::string GlassMaterialName_;

	MaVec2d Dimensions_;
	BcU32 NoofFish_;

	BcF32 SpawnRateMin_;
	BcF32 SpawnRateMax_;
	BcF32 SpawnTimer_;

	std::vector<ScnEntity*> Children;

};

#endif

