#ifndef WORLD_EDITOR_H
#define WORLD_EDTIOR_H

class IScene;
class Entity;
class Terrain;

#define EDITOR_MAX_NAME 64

#define EDITOR_MAX_NAME_VAR 32

class Gizmo
{
public :
	enum Type
	{
		eTranslation = 0,
		eRotation,
		eScaling,
	};
	void Init();

//	void MakeTransformCenter(std::vector<EntityHandle> entities);
//	void Edit(std::vector<EntityHandle> entities);
//
//private :
//	void ChangeTranslation(std::vector<EntityHandle> entities);
//	void ChangeRotation(std::vector<EntityHandle> entities);
//	void ChangeScaling(std::vector<EntityHandle> entities);
//
//	TransformComponent transform;

	void ChangeMode(Gizmo::Type type);

	video::StaticXMeshHandle _x;
	video::StaticXMeshHandle _t;
	video::StaticXMeshHandle _z;
};

class Brush
{
public :
	void Init();

	Vector3 _position;

	video::VertexBufferHandle _vHandle;
	video::MaterialHandle _mHandle;
};

struct TerrainEditor
{
	void Reset()
	{
		_editingRock = false;
		_editingTree = false;
		_editingGrass = false;
		_currentStaticHandle.MakeInvalid();;

	}
	bool32 _editingRock{};
	bool32 _editingTree{};
	bool32 _editingGrass{};

	video::StaticXMeshHandle _currentStaticHandle;
};

struct ObjectEditor
{

};


class Editor
{
public :
	Editor();
	virtual ~Editor();

	virtual void Init();
	virtual void Shutdown();

	virtual void Edit(RefVariant &pObject, const InputManager &input);
	void Render(video::RenderView *renderView);
public:
	void RegisterEvents();
public :
	Entity *_selectedEntity{};

	enum EditMode
	{
		eNone,
		eTerrainEdit,
		eObjectEdit
	};
	void ChangeEditState(EditMode mode);

	void InNoneMode(RefVariant object);
	void InTerrainEditMode();
	void InObjectEditMode();

	EditMode _currentMode;
	EventChannel _channel;
	

	Brush _brush;
	Gizmo _gizmo;

	bool32 _editing{ false };
	
	int32 _mx;
	int32 _my;
	int8 _mb;
	int32 _scroll;
	bool32 _shiftDown;
	uint32 _key;

	void *_pEdittingObject{};

	void UpdateInput(const InputManager &input);

	TerrainEditor _terrainEditor;
};

#endif