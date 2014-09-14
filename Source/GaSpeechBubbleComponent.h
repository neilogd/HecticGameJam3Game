/**************************************************************************
*
* File:		GaSpeechBubbleComponent.h
* Author:	Daniel de Zwart
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaSpeechBubbleComponent_H__
#define __GaSpeechBubbleComponent_H__

#include "Psybrus.h"
#include "System/Scene/Rendering/ScnSpriteComponent.h"
#include "System/Scene/Rendering/ScnFont.h"
//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaSpeechBubbleComponent > GaSpeechBubbleComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaSpeechBubbleComponent
class GaSpeechBubbleComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaSpeechBubbleComponent, ScnComponent );

	void initialise();
	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

	BcF32 getRequiredSize() const;

private:
	BcF32 RequiredSize_;
	BcBool Visible_;

	ScnSpriteComponentRef SpeechBubble_;
	
	ScnFontComponentRef FontComponent_;
	ScnCanvasComponentRef Canvas_;
	std::vector<std::string> Text_;
};

#endif

