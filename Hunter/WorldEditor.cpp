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

	//�����Ϳ��� �ϳ��� Ÿ���� ������ �ϰ� ������ �ٸ� �ɼǵ��� ������
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

		_channel.Broadcast<GameObjectFactory::CreateObjectOnClickEvent>(
			GameObjectFactory::CreateObjectOnClickEvent(objectType, Vector2(_mx, _my)));
	}
}

void Editor::InObjectEditMode()
{
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

void Editor::Render(video::RenderView * renderView)
{
}


void Gizmo::Init()
{
}

void Brush::Init()
{
}

