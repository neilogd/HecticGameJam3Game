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
	void spawnFood( BcU32 Noof );
	void magicReset();

	void registerEnemy(ScnEntityRef entity);

	MaVec2d getCentralPosition();
	void receiveFish();
public:
	ReObjectRef< class ScnCanvasComponent > Canvas_;
	ReObjectRef< class ScnMaterialComponent > SeaweedMaterial_;
	std::string SeaweedMaterialName_;

	BcBool HasCannon_;
	MaVec2d Dimensions_;
	MaVec2d CentralPosition_;
	BcU32 NoofFish_;

	BcF32 SpawnRateMin_;
	BcF32 SpawnRateMax_;
	BcF32 SpawnTimer_;

	MaVec3d CannonPosition_;

	BcF32 AnimationTimer_;
	std::vector< MaVec2d > SeaweedPositions_;
	std::vector< BcU32 > SeaweedSprites_;

	std::vector<ScnEntityRef> Children;
	std::vector<std::string> Messages_;
};

#endif

