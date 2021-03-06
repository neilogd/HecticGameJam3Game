/**************************************************************************
*
* File:		GaPlayerComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaPlayerComponent_H__
#define __GaPlayerComponent_H__

#include "Psybrus.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaPlayerComponent > GaPlayerComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaPlayerComponent
class GaPlayerComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaPlayerComponent, ScnComponent );

	void initialise();
	void initialise( const Json::Value& Object );

	virtual void preUpdate( BcF32 Tick );
	virtual void update( BcF32 Tick );
	virtual void postUpdate( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

	eEvtReturn onMouseDown( EvtID ID, const struct OsEventInputMouse& Event );

	void jumpTank( BcU32 TankIndex, BcBool Force = BcFalse );

	const MaVec2d& getTargetPosition() const;

	ScnEntityRef getCannon();
	MaVec2d getPosition() const;
	MaVec2d getCannonPosition() const;

private:
	ReObjectRef< class ScnCanvasComponent > Canvas_;
	MaMat4d ViewMatrix_;
	MaVec2d TargetPosition_;

	BcF32 OriginalSize_;
	
	BcF32 JumpHeight_;
	BcF32 JumpTimer_;
	BcF32 JumpSpeed_;
	MaVec2d JumpStart_;
	MaVec2d JumpEnd_;

	BcF32 CannonTimer_;
	BcF32 CannonSpeed_;
	BcF32 CannonSuckDistance_;
	MaVec2d CannonStart_;
	MaVec2d CannonEnd_;

	BcU32 TankIndex_;
	ScnEntityRef Tank_;
	ScnEntityRef Cannon_;
	bool FirstUpdate_;
	enum class PlayerState
	{
		IDLE,
		CANNON_SUCK,
		CANNON_LOAD,
		JUMP,
	};

	PlayerState PlayerState_;
	BcBool ShownNextMessage_;
	BcF32 TimeSinceStart_;
};

#endif

