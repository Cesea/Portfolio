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

//class Gizmo
//{
//public :
//	enum Type
//	{
//		eTranslation = 0,
//		eRotation,
//		eScaling,
//	};
//	void Init();
//	void ChangeMode(Gizmo::Type type);
//
//	video::StaticXMeshHandle _x;
//	video::StaticXMeshHandle _t;
//	video::StaticXMeshHandle _z;
//};
//
//class Brush
//{
//public :
//	void Init();
//
//	Vector3 _position;
//
//	video::VertexBufferHandle _vHandle;
//	video::MaterialHandle _mHandle;
//};

struct TerrainEditor
{
	void Reset();

	bool32 _editingExtent;
	bool32 _editingHeight;
	bool32 _editingTexture;

	bool32 _grow{true};
	bool32 _dig{false};
	bool32 _smooth{false};
	bool32 _flat{false};

	float _brushRadius{2.0f};
	float _brushIntensity{3.0f};

	float _countX{};
	float _countZ{};
	Terrain::TerrainConfig _terrainConfig{};

	char _textureName01[EDITOR_MAX_NAME]{0, };
	char _textureName02[EDITOR_MAX_NAME]{0, };
	char _textureName03[EDITOR_MAX_NAME]{0, };
	char _textureName04[EDITOR_MAX_NAME]{0, };

	char _fileName[EDITOR_MAX_NAME];
};

struct ObjectLocator
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

	void Render();
public:
	void RegisterEvents();
public :

	Entity *_selectedEntity{};

	enum EditMode
	{
		eNone,
		eTerrainEdit,
		eObjectLocate,
		eObjectEdit
	};
	void ChangeEditState(EditMode mode);

	void InTerrainEditMode();
	void InObjectLocateMode();
	void InObjectEditMode();

	EditMode _currentMode;
	
	bool32 _editing{ false };

	int32 _mx;
	int32 _my;

	uint8 _mouseLeftDown;
	uint8 _mouseRightDown;

	bool32 _leftButtonPressed{};
	int32 _scroll;
	bool32 _shiftDown;
	uint32 _key;

	void UpdateInput(const InputManager &input);

	TerrainEditor _terrainEditor;
	ObjectLocator _objectLocator;
	ObjectEditor _objectEditor;

	EventChannel _channel;
};

#endif