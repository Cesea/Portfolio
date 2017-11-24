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
	case Editor::eObjectEdit:
	{
		_editing = true;
		gEditorOn = true;
	} break;
	}
	_currentMode = mode;
}

void Editor::InTerrainEditMode()
{
	if (ImguiCollapse("TerrainEditor", nullptr, _editing))
	{
		ChangeEditState(EditMode::eNone);
	}

	ImguiIndent();

	ImguiSlider("Num Object To Paint", (float *)&_terrainEditor._numObjectToPaint, 0.0f, 5.0f, 1.0f);
	ImguiSlider("Object Spreadness", (float *)&_terrainEditor._spreadness, 0.0f, 5.0f, 0.2f);

	if (!_terrainEditor._currentStaticHandle.IsValid())
	{
		ImguiLabel("No Item Selected");
	}
	else
	{
		ImguiLabel("Paint Item!!!");
		if (ImguiButton("Reset Selection"))
		{
			_terrainEditor._currentStaticHandle.MakeInvalid();
		}
	}

	//에디터에서 하나의 타입을 에디팅 하고 있을때 다른 옵션들은 꺼진다
	if (false == _terrainEditor._editingRock)
	{
		if (ImguiCollapse("Rocks", nullptr, _terrainEditor._editingRock))
		{
			_terrainEditor._editingRock = !_terrainEditor._editingRock;
			_terrainEditor._editingGrass = false;
			_terrainEditor._editingTree = false;
		}
	}
	else
	{
		if (ImguiCollapse("Rocks", nullptr, _terrainEditor._editingRock))
		{
			_terrainEditor._editingRock = !_terrainEditor._editingRock;
		}

		ImguiIndent();
		if (ImguiButton("Rock01"))
		{
			_terrainEditor._currentStaticHandle = VIDEO->GetStaticXMesh("Rock01");
			//_channel.Broadcast<GameObjectFactory::CreateObjectEvent>(
			//	GameObjectFactory::CreateObjectEvent(ArcheType::eRock, Vector3(0.0f, 0.0f, 0.0f)));
		}
		if (ImguiButton("Rock02"))
		{
			_terrainEditor._currentStaticHandle = VIDEO->GetStaticXMesh("Rock02");
		}
		ImguiUnindent();
	}

	if (false == _terrainEditor._editingTree)
	{
		if (ImguiCollapse("Trees", nullptr, _terrainEditor._editingTree))
		{
			_terrainEditor._editingTree = !_terrainEditor._editingTree;
			_terrainEditor._editingRock = false;
			_terrainEditor._editingGrass = false;
		}
	}
	else
	{
		if (ImguiCollapse("Trees", nullptr, _terrainEditor._editingTree))
		{
			_terrainEditor._editingTree = !_terrainEditor._editingTree;
		}
		ImguiIndent();
		if (ImguiButton("Tree01"))
		{
			_terrainEditor._currentStaticHandle = VIDEO->GetStaticXMesh("Tree01");
		}
		if (ImguiButton("Tree01"))
		{
			_terrainEditor._currentStaticHandle = VIDEO->GetStaticXMesh("Tree02");
		}
		ImguiUnindent();
	}

	if (false == _terrainEditor._editingGrass)
	{
		if (ImguiCollapse("Grass", nullptr, _terrainEditor._editingGrass))
		{
			_terrainEditor._editingGrass = !_terrainEditor._editingGrass;
			_terrainEditor._editingRock = false;
			_terrainEditor._editingTree = false;
		}
	}
	else
	{
		if (ImguiCollapse("Grass", nullptr, _terrainEditor._editingGrass))
		{
			_terrainEditor._editingGrass = !_terrainEditor._editingGrass;
		}
		ImguiIndent();
		if (ImguiButton("Grass"))
		{
			_terrainEditor._currentStaticHandle = VIDEO->GetStaticXMesh("Grass01");
		}
		if (ImguiButton("Grass"))
		{
			_terrainEditor._currentStaticHandle = VIDEO->GetStaticXMesh("Grass02");
		}
		ImguiUnindent();
	}

	if (_terrainEditor._currentStaticHandle.IsValid() &&
		_leftButtonPressed &&
		!(_mx > 0 && _mx < EDITORX + EDITORSIZEX && _my >= 0 && _my < EDITORY + EDITORSIZEY))
	{
		ArcheType objectType;
		if (_terrainEditor._editingGrass)
		{
			objectType = ArcheType::eGrass;
		}
		else if (_terrainEditor._editingRock)
		{
			objectType = ArcheType::eRock;
		}
		else if (_terrainEditor._editingTree)
		{
			objectType = ArcheType::eTree;
		}

		ResourceHandle resourceHandle;
		resourceHandle.count = _terrainEditor._currentStaticHandle.count;
		resourceHandle.index = _terrainEditor._currentStaticHandle.index;

		_channel.Broadcast<GameObjectFactory::CreateObjectOnClickEvent>(
			GameObjectFactory::CreateObjectOnClickEvent(objectType, resourceHandle, Vector2(_mx, _my)));
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
	_mb = input.mouse.IsDown(MOUSE_BUTTON_LEFT);

	_leftButtonPressed = input.mouse.IsPressed(MOUSE_BUTTON_LEFT);
	_shiftDown = input.keyboard.GetShiftDown();
	//Console::Log("%d\n", input.keyboard.GetVKCode());
	_key = MapVirtualKey(input.keyboard.GetVKCode(), MAPVK_VK_TO_CHAR);
}

void Editor::Init()
{
	//strcpy(_name, "TerrainEditor");
	//strcpy(_button, "Button");
	//strcpy(_collapse, "Collapse");
	
	//this->RegisterEvents();
}

void Editor::Shutdown()
{
	//if (nullptr != _pEdittingObject)
	//{
	//	_pEdittingObject = nullptr;
	//}
}

void Editor::Edit(RefVariant &object, const InputManager &input)
{
	UpdateInput(input);
	ImguiBeginFrame(_mx, _my, _mb, _scroll, _key, _shiftDown);

	switch (_currentMode)
	{
	case Editor::eNone:
	{
		if (ImguiCollapse("Terrain Editor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eTerrainEdit);
			
		}
		if (ImguiCollapse("Object Editor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eObjectEdit);
			_editing = true;
			gEditorOn = true;
		}

	} break;
	case Editor::eTerrainEdit:
	{
		ImguiBeginScrollArea("TerrainEditor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InTerrainEditMode();
		ImguiEndScrollArea();

	} break;

	case Editor::eObjectEdit:
	{
		ImguiBeginScrollArea("ObjectEditor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
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

void Editor::Render(video::RenderView * renderView)
{
}


void Gizmo::Init()
{
}

void Brush::Init()
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
