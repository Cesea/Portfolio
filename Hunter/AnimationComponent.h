#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

struct AnimationComponent : public Component
{
	AnimationComponent();
	virtual ~AnimationComponent();

	ID3DXAllocateHierarchy *_allocateHierachy;
	ID3DXSkinInfo *_skinInfo;
	ID3DXMesh *_mesh;
};
#endif