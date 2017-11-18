#ifndef WORLD_EDITOR_H
#define WORLD_EDTIOR_H

class IScene;
class Entity;
class Terrain;

#define EDITOR_MAX_NAME 64

class Gizmo
{
public :
	enum EditMode
	{
		Translation = 0,
		Rotation,
		Scaling,
	};

//	void MakeTransformCenter(std::vector<EntityHandle> entities);
//	void Edit(std::vector<EntityHandle> entities);
//
//private :
//	void ChangeTranslation(std::vector<EntityHandle> entities);
//	void ChangeRotation(std::vector<EntityHandle> entities);
//	void ChangeScaling(std::vector<EntityHandle> entities);
//
//	TransformComponent transform;
};

class Editor
{
public :
	Editor();
	virtual ~Editor();

	virtual void Init();
	virtual void Shutdown();

	virtual void Edit(void *pObject);

public:
	void RegisterEvents();

	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::MouseReleasedEvent &event);
	void Handle(const InputManager::MouseMoveEvent &event);
	void Handle(const InputManager::MouseWheelEvent &event);

	void Handle(const InputManager::KeyPressedEvent &event);
public :
	
	int32 _mx;
	int32 _my;
	int8 _mb;
	int32 _scroll;
	uint32 _key;

	void *_pEdittingObject{};

	char _name[EDITOR_MAX_NAME] = {0, };
};

class WorldEditor
{
public :
	void Edit(IScene *pScene);
private :
	//std::vector<EntityHandle> _selectedEntities;
	//Gizmo _gizmo;
};


#endif