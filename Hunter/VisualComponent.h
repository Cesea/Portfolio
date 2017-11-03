#ifndef VISUAL_COMPONENT_H
#define VISUAL_COMPONENT_H

#include "Component.h"

constexpr ComponentID BUFFER_VISUAL_COMPONENT_ID = 1;
constexpr ComponentID MESH_VISUAL_COMPONENT_ID = 1;


class VisualComponent : public Component
{
public :
	VisualComponent() {}
	virtual ~VisualComponent() {}

	virtual ComponentFamilyID GetFamilyID() const override { return VISUAL_FAMILY_ID; }

	virtual void Render() const = 0;
};

class BufferVisualComponent : public VisualComponent
{
public :
	BufferVisualComponent() {}
	virtual ~BufferVisualComponent() {}

	virtual void Render() const override;
	virtual ComponentID GetComponentID() const override { return BUFFER_VISUAL_COMPONENT_ID; }

private :

};

class MeshVisualComponent : public VisualComponent
{
public :
	MeshVisualComponent() {}
	virtual ~MeshVisualComponent() {}

	virtual void Render() const override;
	virtual ComponentID GetComponentID() const override { return MESH_VISUAL_COMPONENT_ID; }

private :

};


#endif