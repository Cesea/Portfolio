#ifndef VISUAL_COMPONENT_H
#define VISUAL_COMPONENT_H

#include "Component.h"

class VisualComponent : public Component
{
public :
	//VisualComponent() {}
	//virtual ~VisualComponent() {}
	//virtual ComponentFamilyID GetFamilyID() const override { return VISUAL_FAMILY_ID; }
	//virtual void Render() const = 0;
};

class BufferVisualComponent : public Component
{
public :
	BufferVisualComponent() {}
	~BufferVisualComponent() {}

	VertexBufferHandle vertexHandle;
	IndexBufferHandle indexHandle;

	EffectHandle effectHandle;
	//VertexDeclHandle declHandle;
	//SurfaceMaterialHandle materialHandle;

private :

};

class MeshVisualComponent : public Component
{
public :
	MeshVisualComponent() {}
	~MeshVisualComponent() {}

private :

};


#endif