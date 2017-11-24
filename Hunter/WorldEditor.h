#ifndef WORLD_EDITOR_H
#define WORLD_EDTIOR_H

class IScene;
class Entity;
class Terrain;

#define EDITOR_MAX_NAME 64

#define EDITOR_MAX_NAME_VAR 32

struct TransformComponent;
struct RenderComponent;
struct ScriptComponent;
struct CollisionComponent;
struct ActionComponent;

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

	int32 _numObjectToPaint{};
	float _spreadness{ 1.0f };

	std::vector<std::string> _rockNames{};
	std::vector<std::string> _treeNames{};
	std::vector<std::string> _grassNames{};
};

struct ObjectEditor
{
	void Reset()
	{
		_selectingEntity.GetID().Clear();

		_pTransform = nullptr;
		_pRender = nullptr;
		_pScript = nullptr;
		_pCollision = nullptr;
		_pAction = nullptr;
	}

	void OnNewSelection(Entity entity);

	TransformComponent *_pTransform{};
	RenderComponent *_pRender{};
	ScriptComponent *_pScript{};
	CollisionComponent *_pCollision{};
	ActionComponent *_pAction{};

	Entity _selectingEntity{ };
};


class Editor
{
public :
	Editor();
	virtual ~Editor();

	virtual void Init();
	virtual void Shutdown();

	struct GetObjectFromSceneEvent
	{
		GetObjectFromSceneEvent(const Vector2 &cursorPos)
			:_cursorPos(cursorPos)
		{}
		Vector2 _cursorPos;
	};

	virtual void Edit(RefVariant &pObject, const InputManager &input);

	void SetEdittingEntity(Entity &entity);

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
	
	Brush _brush;
	Gizmo _gizmo;
	bool32 _editing{ false };

	int32 _mx;
	int32 _my;
	int8 _mb;
	bool32 _leftButtonPressed{};
	int32 _scroll;
	bool32 _shiftDown;
	uint32 _key;

	void UpdateInput(const InputManager &input);

	TerrainEditor _terrainEditor;
	ObjectEditor _objectEditor;

	EventChannel _channel;
};

#endif