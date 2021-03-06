#include "Math/MaAABB.h"
#include "Math/MaCPUVec2d.h"
#include "Math/MaCPUVec3d.h"
#include "Math/MaCPUVec4d.h"
#include "Math/MaMat3d.h"
#include "Math/MaMat4d.h"
#include "Math/MaPlane.h"
#include "Math/MaQuat.h"
#include "System/Content/CsPackage.h"
#include "System/Content/CsPackageImporter.h"
#include "System/Content/CsResource.h"
#include "System/Content/CsResourceImporter.h"
#include "System/Content/CsTypes.h"
#include "System/File/FsTypes.h"
#include "System/Renderer/RsCore.h"
#include "System/Renderer/RsCoreImpl.h"
#include "System/Renderer/RsTypes.h"
#include "System/Scene/Animation/ScnAnimation.h"
#include "System/Scene/Animation/ScnAnimationComponent.h"
#include "System/Scene/Animation/ScnAnimationPose.h"
#include "System/Scene/Animation/ScnAnimationTreeBlendNode.h"
#include "System/Scene/Animation/ScnAnimationTreeNode.h"
#include "System/Scene/Animation/ScnAnimationTreeTrackNode.h"
#include "System/Scene/Import/ScnAnimationImport.h"
#include "System/Scene/Import/ScnComponentImport.h"
#include "System/Scene/Import/ScnEntityImport.h"
#include "System/Scene/Import/ScnFontImport.h"
#include "System/Scene/Import/ScnMaterialImport.h"
#include "System/Scene/Import/ScnModelImport.h"
#include "System/Scene/Import/ScnShaderImport.h"
#include "System/Scene/Import/ScnSoundImport.h"
#include "System/Scene/Import/ScnTextureImport.h"
#include "System/Scene/Physics/ScnPhysicsBoxCollisionShape.h"
#include "System/Scene/Physics/ScnPhysicsCollisionShape.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Scene/Rendering/ScnLightComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"
#include "System/Scene/Rendering/ScnModel.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"
#include "System/Scene/Rendering/ScnRenderPipeline.h"
#include "System/Scene/Rendering/ScnRenderTarget.h"
#include "System/Scene/Rendering/ScnShader.h"
#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnSpriteComponent.h"
#include "System/Scene/Rendering/ScnTexture.h"
#include "System/Scene/Rendering/ScnTextureAtlas.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Scene/ScnComponent.h"
#include "System/Scene/ScnEntity.h"
#include "System/Scene/ScnSpatialComponent.h"
#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"
#include "System/Scene/Sound/ScnSoundListenerComponent.h"
#include "System/Sound/SoLoud/SsCoreImplSoLoud.h"
#include "System/Sound/SsCore.h"
#include "System/SysKernel.h"
#include "System/SysSystem.h"
#include "GaCameraComponent.h"
#include "GaCannonComponent.h"
#include "GaCustomisationComponent.h"
#include "GaEnemyComponent.h"
#include "GaFishComponent.h"
#include "GaFoodComponent.h"
#include "GaGameComponent.h"
#include "GaGuiComponent.h"
#include "GaMenuComponent.h"
#include "GaPlayerComponent.h"
#include "GaSpeechBubbleComponent.h"
#include "GaSwarmElementComponent.h"
#include "GaSwarmManagerComponent.h"
#include "GaTankComponent.h"
void AutoGenRegisterReflection()
{
	MaAABB::StaticRegisterClass();
	MaCPUVec2d::StaticRegisterClass();
	MaCPUVec3d::StaticRegisterClass();
	MaCPUVec4d::StaticRegisterClass();
	MaMat3d::StaticRegisterClass();
	MaMat4d::StaticRegisterClass();
	MaPlane::StaticRegisterClass();
	MaQuat::StaticRegisterClass();
	CsPackage::StaticRegisterClass();
	CsPackageDependencies::StaticRegisterClass();
	CsResource::StaticRegisterClass();
	CsResourceImporterAttribute::StaticRegisterClass();
	CsResourceImporter::StaticRegisterClass();
	CsDependency::StaticRegisterClass();
	FsTimestamp::StaticRegisterClass();
	FsStats::StaticRegisterClass();
	RsCore::StaticRegisterClass();
	RsCoreImpl::StaticRegisterClass();
	RsColour::StaticRegisterClass();
	ScnAnimation::StaticRegisterClass();
	ScnAnimationComponent::StaticRegisterClass();
	ScnAnimationPose::StaticRegisterClass();
	ScnAnimationTreeBlendNode::StaticRegisterClass();
	ScnAnimationTreeNode::StaticRegisterClass();
	ScnAnimationTreeTrackNode::StaticRegisterClass();
	ScnAnimationImport::StaticRegisterClass();
	ScnComponentImport::StaticRegisterClass();
	ScnEntityImport::StaticRegisterClass();
	ScnFontImport::StaticRegisterClass();
	ScnMaterialImport::StaticRegisterClass();
	ScnModelImport::StaticRegisterClass();
	ScnShaderImport::StaticRegisterClass();
	ScnSoundImport::StaticRegisterClass();
	ScnTextureImport::StaticRegisterClass();
	ScnPhysicsBoxCollisionShape::StaticRegisterClass();
	ScnPhysicsCollisionShape::StaticRegisterClass();
	ScnPhysicsRigidBodyComponent::StaticRegisterClass();
	ScnPhysicsWorldComponent::StaticRegisterClass();
	ScnCanvasComponent::StaticRegisterClass();
	ScnDebugRenderComponent::StaticRegisterClass();
	ScnFont::StaticRegisterClass();
	ScnFontComponent::StaticRegisterClass();
	ScnLightComponent::StaticRegisterClass();
	ScnMaterial::StaticRegisterClass();
	ScnMaterialComponent::StaticRegisterClass();
	ScnModel::StaticRegisterClass();
	ScnModelComponent::StaticRegisterClass();
	ScnParticleSystemComponent::StaticRegisterClass();
	ScnRenderableComponent::StaticRegisterClass();
	ScnRenderPipeline::StaticRegisterClass();
	ScnRenderTarget::StaticRegisterClass();
	ScnShader::StaticRegisterClass();
	ScnShaderViewUniformBlockData::StaticRegisterClass();
	ScnShaderLightUniformBlockData::StaticRegisterClass();
	ScnShaderObjectUniformBlockData::StaticRegisterClass();
	ScnShaderBoneUniformBlockData::StaticRegisterClass();
	ScnShaderAlphaTestUniformBlockData::StaticRegisterClass();
	ScnSpriteComponent::StaticRegisterClass();
	ScnTexture::StaticRegisterClass();
	ScnTextureAtlas::StaticRegisterClass();
	ScnViewComponent::StaticRegisterClass();
	ScnComponentAttribute::StaticRegisterClass();
	ScnComponent::StaticRegisterClass();
	ScnEntity::StaticRegisterClass();
	ScnSpatialComponent::StaticRegisterClass();
	ScnSound::StaticRegisterClass();
	ScnSoundEmitterComponent::StaticRegisterClass();
	ScnSoundListenerComponent::StaticRegisterClass();
	SsCoreImplSoLoud::StaticRegisterClass();
	SsCore::StaticRegisterClass();
	SysKernel::StaticRegisterClass();
	SysSystem::StaticRegisterClass();
	GaCameraComponent::StaticRegisterClass();
	GaCannonComponent::StaticRegisterClass();
	GaCustomisationComponent::StaticRegisterClass();
	GaEnemyComponent::StaticRegisterClass();
	GaFishComponent::StaticRegisterClass();
	GaFoodComponent::StaticRegisterClass();
	GaGameComponent::StaticRegisterClass();
	GaGuiShaderUniformBlockData::StaticRegisterClass();
	GaGuiComponent::StaticRegisterClass();
	GaMenuComponent::StaticRegisterClass();
	GaPlayerComponent::StaticRegisterClass();
	GaSpeechBubbleComponent::StaticRegisterClass();
	GaSwarmElementComponent::StaticRegisterClass();
	GaSwarmManagerComponent::StaticRegisterClass();
	GaTankComponent::StaticRegisterClass();
}
