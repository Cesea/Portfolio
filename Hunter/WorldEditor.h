#ifndef WORLD_EDITOR_H
#define WORLD_EDITOR_H

class IScene;
class Entity;
class Terrain;

#define EDITOR_MAX_NAME 128
#define EDITOR_MAX_NAME_VAR 32

#define BRUSH_INNER_COLOR (0xffff0000)
#define BRUSH_OUTTER_COLOR (0xff00ff00)

#define BRUSH_CIRCLE_RES (37)

struct TransformComponent;
struct RenderComponent;
struct ScriptComponent;
struct CollisionComponent;
struct ActionComponent;

struct Brush
{
	struct BrushVertex
	{
		Vector3 _position;
		uint32 _color;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
	};

	bool Init();

	void SetInnerRadius(float radius);
	void SetOutterRadius(float radius);
	void Render();

	float _innerRadius;
	float _outterRadius;

	float _intensity;

	Vector3 _centerPos;

	BrushVertex _innerVertices[BRUSH_CIRCLE_RES];
	BrushVertex _outterVertices[BRUSH_CIRCLE_RES];
};

struct TerrainEditor
{
	void Reset();

	bool32 _editingExtent;
	bool32 _editingHeight;
	bool32 _editingTexture;
	bool32 _saveSceneInfo;

	bool32 _grow{true};
	bool32 _dig{false};
	bool32 _smooth{false};
	bool32 _flat{false};

	Brush _heightBrush;
	Brush _textureBrush;
	//float _brushInnerRadius{2.0f};
	//float _brushOutterRadius{4.0f};
	//float _brushIntensity{1.0f};

	int32 _layer{ 0 };
	bool32 _layer1{ 0 };
	bool32 _layer2{ 0 };

	float _countX{};
	float _countZ{};
	Terrain::TerrainConfig _terrainConfig{};

	char _textureName00[EDITOR_MAX_NAME]{0, };
	char _textureName01[EDITOR_MAX_NAME]{0, };
	char _textureName02[EDITOR_MAX_NAME]{0, };

	char _fileName[EDITOR_MAX_NAME];
};

struct ObjectLocator
{
	void Reset()
	{
		_locateRock = false;
		_locateTree = false;
		_locateTrunk = false;
		_locateGrass = false;
		_locateMushroom = false;
		_locateMonster = false;
		_currentStaticHandle.MakeInvalid();
		_currentSkinnedHandle.MakeInvalid();
		_typeToLocate = ARCHE_NONE;
	}
	
	bool32 _locateRock{};
	bool32 _locateTree{};
	bool32 _locateTrunk{};
	bool32 _locateGrass{};
	bool32 _locateMushroom{};
	bool32 _locateMonster{};

	float _scaleMin{0.9f};
	float _scaleMax{1.1f};
	float _rotationMin{-0.9f};
	float _rotationMax{0.9f};

	video::StaticXMeshHandle _currentStaticHandle;
	video::SkinnedXMeshHandle _currentSkinnedHandle;

	int32 _numObjectToPaint{};
	Brush _objectPaintBrush{};
	ARCHE_TYPE _typeToLocate{};
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

		QuaternionIdentity(&_xRotation);
		QuaternionIdentity(&_yRotation);
		QuaternionIdentity(&_zRotation);
	}

	void OnNewSelection(Entity entity);

	TransformComponent *_pTransform{};
	RenderComponent *_pRender{};
	ScriptComponent *_pScript{};
	CollisionComponent *_pCollision{};
	ActionComponent *_pAction{};

	Entity _selectingEntity{ };

	Quaternion _xRotation;
	Quaternion _yRotation;
	Quaternion _zRotation;
};

struct SystemEditor
{
	void Reset()
	{
		_editScrptSystem = false;
		_editRenderSystem = false;
		_editActionSystem = false;
		_editCollisionSystem = false;
	}
	bool32 _editScrptSystem;
	bool32 _editCollisionSystem;
	bool32 _editRenderSystem;
	bool32 _editActionSystem;

	bool32 _scriptRunning{ true };
	bool32 _renderShadow{true};
};

//일단은 StatusWindow는 Player만 사용할 수 있게끔 한다..
struct StatusWindow
{
	void Reset()
	{
		_selectingType = ARCHE_NONE;
		_selectingState = 0;

		ZeroMemory(_worldPosStr, sizeof(char) *EDITOR_MAX_NAME);
		ZeroMemory(_chunkPosStr, sizeof(char) *EDITOR_MAX_NAME);
		ZeroMemory(_tilePosStr, sizeof(char) *EDITOR_MAX_NAME);
		ZeroMemory(_relPosStr, sizeof(char) *EDITOR_MAX_NAME);

		ZeroMemory(_chunkInfoStr, sizeof(char) *EDITOR_MAX_NAME);
		ZeroMemory(_tileInfoStr, sizeof(char) *EDITOR_MAX_NAME);

	}

	ARCHE_TYPE _selectingType{ARCHE_NONE};
	int32 _selectingState{0};

	char _worldTerrainInfoStr[EDITOR_MAX_NAME];
	char _worldObjectInfoStr[EDITOR_MAX_NAME];

	char _worldPosStr[EDITOR_MAX_NAME];
	char _chunkPosStr[EDITOR_MAX_NAME];
	char _tilePosStr[EDITOR_MAX_NAME];
	char _relPosStr[EDITOR_MAX_NAME];

	char _chunkInfoStr[EDITOR_MAX_NAME];
	char _tileInfoStr[EDITOR_MAX_NAME];

	char _cursorWorldPosStr[EDITOR_MAX_NAME];
	char _cursorChunkInfoStr[EDITOR_MAX_NAME];
	char _cursorTileInfoStr[EDITOR_MAX_NAME];
};

//Editor에서는 Scene의 포인터를 가지고 있어서 몬스터들의 스크립트나, 액션 시스템을 비 활성화 할 수 있다.
class Editor
{
public :
	Editor();
	~Editor();

	void Init(IScene *pScene);
	void Shutdown();

	struct GetObjectFromSceneEvent
	{
		GetObjectFromSceneEvent(const Vector2 &cursorPos)
			:_cursorPos(cursorPos)
		{}
		Vector2 _cursorPos;
	};

	void Edit(RefVariant &pObject, const InputManager &input);

	void SetEdittingEntity(Entity &entity);

	void SetShowStatusObject(BaseGameObject *pObject) { _pSelectedObject = pObject; }

	void Render();
public:
	void RegisterEvents();
public :

	Entity *_selectedEntity{};
	BaseGameObject *_pSelectedObject{};

	enum EditMode
	{
		eNone,
		eTerrainEdit,
		eObjectLocate,
		eObjectEdit,
		eSystemEdit,
	};
	void ChangeEditState(EditMode mode);

	void InTerrainEditMode();
	void InObjectLocateMode();
	void InObjectEditMode();
	void InSystemEditMode();

	void ShowStatusWindow();

	EditMode _currentMode;
	
	bool32 _editing{ false };

	bool32 _showStatus{ false };

	int32 _mx;
	int32 _my;

	uint8 _mouseLeftDown;
	uint8 _mouseRightDown;

	bool32 _leftButtonPressed{};
	bool32 _leftButtonReleased{};
	int32 _scroll;
	bool32 _shiftDown;
	uint32 _key;

	void UpdateInput(const InputManager &input);

	TerrainEditor _terrainEditor;
	ObjectLocator _objectLocator;
	ObjectEditor _objectEditor;

	SystemEditor _systemEditor;

	StatusWindow _statusWindow;

	IScene *_pCurrentScene{};

	EventChannel _channel;
};

#endif