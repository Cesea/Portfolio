#include "stdafx.h"
#include "WorldEditor.h"

//#include "BaseScene.h"

bool32 gEditorOn = false;

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::RegisterEvents()
{
}

void Editor::ChangeEditState(EditMode mode)
{
	switch (mode)
	{
	case Editor::eNone:
	{
		_editing = false;
		gEditorOn = false;
	} break;
	case Editor::eTerrainEdit:
	{
		_editing = true;
		gEditorOn = true;
	} break;
	case Editor::eObjectLocate:
	{
		_editing = true;
		gEditorOn = true;
	} break;
	case Editor::eObjectEdit:
	{
		_editing = true;
		gEditorOn = true;
	} break;
	}
	_currentMode = mode;
}

//struct TerrainConfig
//
//	{
//		bool32 _createFromHeightMap;
//		bool32 _createFromBuffer;
//
//		int32 _xResolution;
//		int32 _zResolution;
//
//		//만약 CreateFromHeightMap이 true라면 heightmap을 읽어서 로드한다...
//		//CreateFromBuffer가 true라면 버텍스 버퍼와 인덱스 버퍼를 읽어서 로드 한다.
//		std::string _heightFileName;
//		std::string _tile0FileName;
//		std::string _tile1FileName;
//		std::string _tile2FileName;
//		std::string _tile3FileName;
//		std::string _splatFileName;
//
//		float _heightScale;
//		//uv가 얼마나 반복 될 것인지...
//		float _textureMult;
//	};

void Editor::InTerrainEditMode()
{
	if (ImguiCollapse("Terrain Editor", nullptr, _editing))
	{
		ChangeEditState(EditMode::eNone);
	}
	ImguiIndent();

	//범위 조정
	if (ImguiCollapse("Edit Extent", nullptr, _terrainEditor._editingExtent))
	{
		_terrainEditor._editingExtent = !_terrainEditor._editingExtent;
		if (_terrainEditor._editingExtent)
		{
			_terrainEditor._editingHeight = false;
			_terrainEditor._editingTexture = false;
		}
	}
	if (_terrainEditor._editingExtent)
	{
		ImguiIndent();
		ImguiSlider("X Extent", (float *)&_terrainEditor._countX, 0.0f, 16.0f, 2.0f);
		ImguiSlider("Z Extent", (float *)&_terrainEditor._countZ, 0.0f, 16.0f, 2.0f);

		if (ImguiButton("Create Terrain!!!"))
		{
			_terrainEditor._terrainConfig._xChunkCount = (int32)_terrainEditor._countX;
			_terrainEditor._terrainConfig._zChunkCount = (int32)_terrainEditor._countZ;

			TERRAIN->RebuildTerrain(_terrainEditor._terrainConfig);
		}
		ImguiUnindent();
	}
	
	//높이 조정
	if (ImguiCollapse("Edit Height", nullptr, _terrainEditor._editingHeight))
	{
		_terrainEditor._editingHeight = !_terrainEditor._editingHeight;
		if (_terrainEditor._editingHeight)
		{
			_terrainEditor._editingExtent = false;
			_terrainEditor._editingTexture = false;
		}
	}
	if (_terrainEditor._editingHeight)
	{
		ImguiIndent();

		//TODO : 브러쉬가 Inner, Outter Radius의 영향을 제대로 받게끔 고치자
		ImguiSlider("Brush Inner Radius", &_terrainEditor._brushInnerRadius, 4.0f, 10.0f, 0.1f);
		ImguiSlider("Brush Outter Radius", &_terrainEditor._brushOutterRadius, 4.0f, 10.0f, 0.1f);
		ImguiSlider("Brush Intensity", &_terrainEditor._brushIntensity, 1.0f, 10.0f, 0.05f);

		if (ImguiCheck("Grow", _terrainEditor._grow))
		{
			_terrainEditor._grow = !_terrainEditor._grow;
			_terrainEditor._dig = false;
			_terrainEditor._smooth = false;
			_terrainEditor._flat = false;
		}
		if (ImguiCheck("Dig", _terrainEditor._dig))
		{
			_terrainEditor._dig = !_terrainEditor._dig;

			_terrainEditor._grow = false;
			_terrainEditor._smooth = false;
			_terrainEditor._flat = false;
		}
		if (ImguiCheck("Smooth", _terrainEditor._smooth))
		{
			_terrainEditor._smooth = !_terrainEditor._smooth;

			_terrainEditor._grow = false;
			_terrainEditor._dig = false;
			_terrainEditor._flat = false;
		}
		if (ImguiCheck("Flat", _terrainEditor._flat))
		{
			_terrainEditor._flat = !_terrainEditor._flat;

			_terrainEditor._grow = false;
			_terrainEditor._dig = false;
			_terrainEditor._smooth = false;
		}

		if (ImguiButton("Rebuild QuadTree"))
		{
			TERRAIN->ReCreateQuadTree();
		}

		if (_mouseLeftDown && 
			!(_mx > 0  && _mx < EDITORSIZEX && _my > 0 && _my < EDITORSIZEX))
		{
			if (_terrainEditor._grow)
			{
				TERRAIN->AddHeightOnCursorPos(Vector2((float)_mx, (float)_my), 
					_terrainEditor._brushInnerRadius, _terrainEditor._brushIntensity);
			}
			else if (_terrainEditor._dig)
			{
				TERRAIN->AddHeightOnCursorPos(Vector2((float)_mx, (float)_my), 
					_terrainEditor._brushInnerRadius, -_terrainEditor._brushIntensity);
			}
			else if (_terrainEditor._smooth)
			{
				TERRAIN->SmoothOnCursorPos(Vector2((float)_mx, (float)_my), _terrainEditor._brushInnerRadius);
			}
			else if (_terrainEditor._flat)
			{
			}
		}
		ImguiUnindent();
	}

	//텍스쳐 조정
	if (ImguiCollapse("Edit Texture", nullptr, _terrainEditor._editingTexture))
	{
		_terrainEditor._editingTexture = !_terrainEditor._editingTexture;
		if (_terrainEditor._editingTexture)
		{
			_terrainEditor._editingExtent = false;
			_terrainEditor._editingHeight = false;
		}
	}
	if (_terrainEditor._editingTexture)
	{
		ImguiIndent();

		ImguiLabel("Select Texture");
#pragma region Texture Select
		{
			ImguiIndent();
			if (ImguiCheck("Texture0", _terrainEditor._r))
			{
				_terrainEditor._r = !_terrainEditor._r;
				if (_terrainEditor._r)
				{
					_terrainEditor._channel = 0;
					_terrainEditor._g = false;
					_terrainEditor._b = false;
					_terrainEditor._a = false;
				}
			}
			if (ImguiCheck("Texture1", _terrainEditor._g))
			{
				_terrainEditor._g = !_terrainEditor._g;
				if (_terrainEditor._g)
				{
					_terrainEditor._channel = 1;
					_terrainEditor._r = false;
					_terrainEditor._b = false;
					_terrainEditor._a = false;
				}
			}
			if (ImguiCheck("Texture2", _terrainEditor._b))
			{
				_terrainEditor._b = !_terrainEditor._b;
				if (_terrainEditor._b)
				{
					_terrainEditor._channel = 2;
					_terrainEditor._r = false;
					_terrainEditor._g = false;
					_terrainEditor._a = false;
				}
			}
			if (ImguiCheck("Texture3", _terrainEditor._a))
			{
				_terrainEditor._a = !_terrainEditor._a;
				if (_terrainEditor._a)
				{
					_terrainEditor._channel = 3;
					_terrainEditor._r = false;
					_terrainEditor._g = false;
					_terrainEditor._b = false;
				}
			}
			ImguiUnindent();
		}
#pragma endregion
		
#pragma region Texture Load
		ImguiLabel("Texture00");
		{
			ImguiIndent();
			ImguiEdit(_terrainEditor._textureName00, 120);
			if (ImguiButton("LoadTexture"))
			{
				if (_terrainEditor._textureName00 == TERRAIN->_currentConfig._tile0FileName)
				{
					ZeroMemory(_terrainEditor._textureName00, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName00, "Texture is Same", EDITOR_MAX_NAME);
					return;
				}
				video::TextureHandle loadedTexture = VIDEO->CreateTexture(_terrainEditor._textureName00,
					_terrainEditor._textureName00);
				if (loadedTexture.IsValid())
				{
					VIDEO->DestroyTexture(TERRAIN->_tile0Handle);
					TERRAIN->_tile0Handle = loadedTexture;
					TERRAIN->_currentConfig._tile0FileName = _terrainEditor._textureName00;
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName00, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName00, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

		ImguiLabel("Texture01");
		{
			ImguiIndent();
			ImguiEdit(_terrainEditor._textureName01, 120);
			if (ImguiButton("LoadTexture"))
			{
				if (_terrainEditor._textureName01 == TERRAIN->_currentConfig._tile1FileName)
				{
					ZeroMemory(_terrainEditor._textureName01, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName01, "Texture is Same", EDITOR_MAX_NAME);
					return;
				}
				video::TextureHandle loadedTexture = VIDEO->CreateTexture(_terrainEditor._textureName01,
					_terrainEditor._textureName01);
				if (loadedTexture.IsValid())
				{
					VIDEO->DestroyTexture(TERRAIN->_tile1Handle);
					TERRAIN->_tile1Handle = loadedTexture;
					TERRAIN->_currentConfig._tile1FileName = _terrainEditor._textureName01;
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName01, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName01, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

		ImguiLabel("Texture02");
		{
			ImguiIndent();
			ImguiEdit(_terrainEditor._textureName02, 120);
			if (ImguiButton("LoadTexture"))
			{
				if (_terrainEditor._textureName02 == TERRAIN->_currentConfig._tile2FileName)
				{
					ZeroMemory(_terrainEditor._textureName02, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName02, "Texture is Same", EDITOR_MAX_NAME);
					return;
				}
				video::TextureHandle loadedTexture = VIDEO->CreateTexture(_terrainEditor._textureName02,
					_terrainEditor._textureName02);
				if (loadedTexture.IsValid())
				{
					VIDEO->DestroyTexture(TERRAIN->_tile2Handle);
					TERRAIN->_tile2Handle = loadedTexture;
					TERRAIN->_currentConfig._tile2FileName = _terrainEditor._textureName02;
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName02, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName02, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

		ImguiLabel("Texture03");
		{
			ImguiIndent();
			ImguiEdit(_terrainEditor._textureName03, 120);
			if (ImguiButton("LoadTexture"))
			{
				if (_terrainEditor._textureName03 == TERRAIN->_currentConfig._tile3FileName)
				{
					ZeroMemory(_terrainEditor._textureName03, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName03, "Texture is Same", EDITOR_MAX_NAME);
					return;
				}
				video::TextureHandle loadedTexture = VIDEO->CreateTexture(_terrainEditor._textureName03,
					_terrainEditor._textureName03);
				if (loadedTexture.IsValid())
				{
					VIDEO->DestroyTexture(TERRAIN->_tile3Handle);
					TERRAIN->_tile3Handle = loadedTexture;
					TERRAIN->_currentConfig._tile3FileName = _terrainEditor._textureName03;
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName03, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName03, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

#pragma endregion

		//여기서 선택되어있는 텍스쳐를 그리는 작업을 실행하자...
		if (_mouseLeftDown && 
			!(_mx > 0  && _mx < EDITORSIZEX && _my > 0 && _my < EDITORSIZEX))
		{
			TERRAIN->DrawAlphaTextureOnCursorPos(Vector2((float)_mx, (float)_my),
				_terrainEditor._brushInnerRadius, _terrainEditor._brushOutterRadius,
				_terrainEditor._brushIntensity, video::TextureHandle(), _terrainEditor._channel);
		}


		ImguiUnindent();
	}

	ImguiLabel("File Name");
	ImguiIndent();
	{
		ImguiEdit(_terrainEditor._fileName, 120);
		if (ImguiButton("Save Terrain"))
		{
			TERRAIN->SaveTerrain(_terrainEditor._fileName);
		}

		if (ImguiButton("Load Terrain"))
		{
			TERRAIN->LoadTerrain(_terrainEditor._fileName);
		}
	}
	ImguiUnindent();

	ImguiUnindent();
}

void Editor::InObjectLocateMode()
{
	if (ImguiCollapse("Object Locator", nullptr, _editing))
	{
		ChangeEditState(EditMode::eNone);
	}

	ImguiIndent();

	ImguiSlider("Num Object To Paint", (float *)&_objectLocator._numObjectToPaint, 0.0f, 5.0f, 1.0f);
	ImguiSlider("Object Spreadness", (float *)&_objectLocator._spreadness, 0.0f, 5.0f, 0.2f);

	if (!_objectLocator._currentStaticHandle.IsValid())
	{
		ImguiLabel("No Item Selected");
	}
	else
	{
		ImguiLabel("Paint Item!!!");
		if (ImguiButton("Reset Selection"))
		{
			_objectLocator._currentStaticHandle.MakeInvalid();
		}
	}

	//에디터에서 하나의 타입을 에디팅 하고 있을때 다른 옵션들은 꺼진다
	if (false == _objectLocator._editingRock)
	{
		if (ImguiCollapse("Rocks", nullptr, _objectLocator._editingRock))
		{
			_objectLocator._editingRock = !_objectLocator._editingRock;
			_objectLocator._editingGrass = false;
			_objectLocator._editingTree = false;
		}
	}
	else
	{
		if (ImguiCollapse("Rocks", nullptr, _objectLocator._editingRock))
		{
			_objectLocator._editingRock = !_objectLocator._editingRock;
		}

		ImguiIndent();
		if (ImguiButton("Rock01"))
		{
			_objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock01");
			//_channel.Broadcast<GameObjectFactory::CreateObjectEvent>(
			//	GameObjectFactory::CreateObjectEvent(ArcheType::eRock, Vector3(0.0f, 0.0f, 0.0f)));
		}
		if (ImguiButton("Rock02"))
		{
			_objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock02");
		}
		ImguiUnindent();
	}

	if (false == _objectLocator._editingTree)
	{
		if (ImguiCollapse("Trees", nullptr, _objectLocator._editingTree))
		{
			_objectLocator._editingTree = !_objectLocator._editingTree;
			_objectLocator._editingRock = false;
			_objectLocator._editingGrass = false;
		}
	}
	else
	{
		if (ImguiCollapse("Trees", nullptr, _objectLocator._editingTree))
		{
			_objectLocator._editingTree = !_objectLocator._editingTree;
		}
		ImguiIndent();
		if (ImguiButton("Tree01"))
		{
			_objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree01");
		}
		if (ImguiButton("Tree01"))
		{
			_objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree02");
		}
		ImguiUnindent();
	}

	if (false == _objectLocator._editingGrass)
	{
		if (ImguiCollapse("Grass", nullptr, _objectLocator._editingGrass))
		{
			_objectLocator._editingGrass = !_objectLocator._editingGrass;
			_objectLocator._editingRock = false;
			_objectLocator._editingTree = false;
		}
	}
	else
	{
		if (ImguiCollapse("Grass", nullptr, _objectLocator._editingGrass))
		{
			_objectLocator._editingGrass = !_objectLocator._editingGrass;
		}
		ImguiIndent();
		if (ImguiButton("Grass"))
		{
			_objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass01");
		}
		if (ImguiButton("Grass"))
		{
			_objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass02");
		}
		ImguiUnindent();
	}

	if (_objectLocator._currentStaticHandle.IsValid() &&
		_leftButtonPressed &&
		!(_mx > 0 && _mx < EDITORX + EDITORSIZEX && _my >= 0 && _my < EDITORY + EDITORSIZEY))
	{
		ARCHE_TYPE objectType;
		if (_objectLocator._editingGrass)
		{
			objectType = ARCHE_GRASS;
		}
		else if (_objectLocator._editingRock)
		{
			objectType = ARCHE_ROCK;
		}
		else if (_objectLocator._editingTree)
		{
			objectType = ARCHE_TREE;
		}

		ResourceHandle resourceHandle;
		resourceHandle.count = _objectLocator._currentStaticHandle.count;
		resourceHandle.index = _objectLocator._currentStaticHandle.index;

		_channel.Broadcast<GameObjectFactory::CreateObjectOnClickEvent>(
			GameObjectFactory::CreateObjectOnClickEvent(objectType, resourceHandle, Vector2((float)_mx, (float)_my)));
	}
}

void Editor::InObjectEditMode()
{
	if (ImguiCollapse("ObjectEditMode", nullptr, _editing))
	{
		_objectEditor.Reset();
		ChangeEditState(EditMode::eNone);
	}

	ImguiIndent();
	if (_objectEditor._selectingEntity.GetID().IsNull())
	{
		ImguiLabel("Select Object To Edit!!");
		if ( _leftButtonPressed &&
			!(_mx > 0 && _mx < EDITORX + EDITORSIZEX && _my >= 0 && _my < EDITORY + EDITORSIZEY))
		{
			_channel.Broadcast<Editor::GetObjectFromSceneEvent>( Editor::GetObjectFromSceneEvent(Vector2(_mx, _my)));
		}
	}
	else
	{
		ImguiLabel("Editing Object");

		if (ImguiButton("Delete Object"))
		{
			_objectEditor._selectingEntity.Kill();
			_objectEditor.Reset();
		}

		if (ImguiButton("Reset Seletion"))
		{
			_objectEditor.Reset();
		}

		if (_objectEditor._pTransform)
		{
			ImguiLabel("Transform Component");
			ImguiIndent();

			ImguiLabel("Position");
			{
				ImguiIndent();
				ImguiSlider("X", &_objectEditor._pTransform->_position.x, -100.0f, 100.0f, 0.1f);
				ImguiSlider("Y", &_objectEditor._pTransform->_position.y, -100.0f, 100.0f, 0.1f);
				ImguiSlider("Z", &_objectEditor._pTransform->_position.z, -100.0f, 100.0f, 0.1f);
				ImguiUnindent();
			}

			ImguiLabel("Scale");
			{
				ImguiIndent();
				ImguiSlider("X", &_objectEditor._pTransform->_scale.x, 0.0f, 100.0f, 0.1f);
				ImguiSlider("Y", &_objectEditor._pTransform->_scale.y, 0.0f, 100.0f, 0.1f);
				ImguiSlider("Z", &_objectEditor._pTransform->_scale.z, 0.0f, 100.0f, 0.1f);
				ImguiUnindent();
			}

			ImguiLabel("Orientation");
			{
				//지금 물체에 대한 모든 Transform정보를 가져올 수 있지만 함수가 비싸다...더 좋은 방법을 찾아보자.
				Quaternion  quaternion;
				Vector3 translation, scale;

				MatrixDecompose(&scale, &quaternion, &translation, &_objectEditor._pTransform->GetFinalMatrix());

				ImguiIndent();

				ImguiSlider("X", &quaternion.x, 0.0f, 1.0f, 0.1f);
				ImguiSlider("Y", &quaternion.y, 0.0f, 1.0f, 0.1f);
				ImguiSlider("Z", &quaternion.z, 0.0f, 1.0f, 0.1f);

				_objectEditor._pTransform->SetRotateWorld(quaternion);

				ImguiUnindent();
			}

			ImguiUnindent();
		}

		if (_objectEditor._pRender)
		{

		}

		if (_objectEditor._pScript)
		{

		}

		if (_objectEditor._pCollision)
		{

		}

		if (_objectEditor._pAction)
		{

		}
	}
}

void Editor::UpdateInput(const InputManager & input)
{
	_mx = input.mouse.GetCurrentPoint().x;
	_my = input.mouse.GetCurrentPoint().y;
	_mouseLeftDown = input.mouse.IsDown(MOUSE_BUTTON_LEFT);
	_mouseRightDown = input.mouse.IsDown(MOUSE_BUTTON_RIGHT);

	_leftButtonPressed = input.mouse.IsPressed(MOUSE_BUTTON_LEFT);
	_shiftDown = input.keyboard.GetShiftDown();
	//Console::Log("%d\n", input.keyboard.GetVKCode());
	_key = input.keyboard.GetVKCode();
}

void Editor::Init()
{
}

void Editor::Shutdown()
{
}

void Editor::Edit(RefVariant &object, const InputManager &input)
{
	UpdateInput(input);
	ImguiBeginFrame(_mx, _my, _mouseLeftDown, _scroll, _key, _shiftDown);

	switch (_currentMode)
	{
	case Editor::eNone:
	{
		if (ImguiCollapse("Terrain Editor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eTerrainEdit);
		}
		if (ImguiCollapse("Object Locator", nullptr, _editing))
		{
			ChangeEditState(EditMode::eObjectLocate);
		}
		if (ImguiCollapse("Object Editor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eObjectEdit);
		}
	} break;
	
	case Editor::eTerrainEdit :
	{
		ImguiBeginScrollArea("Terrain Editor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InTerrainEditMode();
		ImguiEndScrollArea();
	}break;

	case Editor::eObjectLocate:
	{
		ImguiBeginScrollArea("Object Locator", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InObjectLocateMode();
		ImguiEndScrollArea();

	} break;

	case Editor::eObjectEdit:
	{
		ImguiBeginScrollArea("ObjectE ditor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InObjectEditMode();
		ImguiEndScrollArea();
	} break;
	}

	ImguiEndFrame();

}

void Editor::SetEdittingEntity(Entity & entity)
{
	_objectEditor.OnNewSelection(entity);
	//_objectEditor._pSelectingEntity = &entity;
}

void Editor::Render()
{
}

void ObjectEditor::OnNewSelection(Entity entity)
{
	if (!entity.GetID().IsNull())
	{
		if (entity.HasComponent<TransformComponent>())
		{
			_pTransform = &entity.GetComponent<TransformComponent>();
		}
		if (entity.HasComponent<RenderComponent>())
		{
			_pRender = &entity.GetComponent<RenderComponent>();
		}
		if (entity.HasComponent<ScriptComponent>())
		{
			_pScript = &entity.GetComponent<ScriptComponent>();
		}
		if (entity.HasComponent<CollisionComponent>())
		{
			_pCollision = &entity.GetComponent<CollisionComponent>();
		}
		if (entity.HasComponent<ActionComponent>())
		{
			_pAction = &entity.GetComponent<ActionComponent>();
		}

		_selectingEntity = entity;
	}
}

void TerrainEditor::Reset()
{
}
