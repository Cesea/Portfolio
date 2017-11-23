#include "stdafx.h"
#include "WorldEditor.h"

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

void Editor::UpdateInput(const InputManager & input)
{
	_mx = input.mouse.GetCurrentPoint().x;
	_my = input.mouse.GetCurrentPoint().y;
	_mb = input.mouse.IsDown(MOUSE_BUTTON_LEFT);

	_shiftDown = input.keyboard.GetShiftDown();
	//Console::Log("%d\n", input.keyboard.GetVKCode());
	_key = MapVirtualKey(input.keyboard.GetVKCode(), MAPVK_VK_TO_CHAR);
}

void Editor::Init()
{
	//strcpy(_name, "TerrainEditor");
	//strcpy(_button, "Button");
	//strcpy(_collapse, "Collapse");
	
	this->RegisterEvents();
}

void Editor::Shutdown()
{
	if (nullptr != _pEdittingObject)
	{
		_pEdittingObject = nullptr;
	}
}

void Editor::Edit(RefVariant &object, const InputManager &input)
{
	UpdateInput(input);
	ImguiBeginFrame(_mx, _my, _mb, _scroll, _key, _shiftDown);

	switch (_currentMode)
	{
	case Editor::eNone:
	{
		if (ImguiCollapse("TerrainEditor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eTerrainEdit);
			
		}
		if (ImguiCollapse("ObjectEditor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eObjectEdit);
			_editing = true;
			gEditorOn = true;
		}

	} break;
	case Editor::eTerrainEdit:
	{
		ImguiBeginScrollArea("TerrainEditor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		if (ImguiCollapse("TerrainEditor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eNone);
		}

		ImguiIndent();

		if (false == _terrainEditor._editingRock)
		{
			if (ImguiCollapse("Rocks", nullptr, _terrainEditor._editingRock))
			{
				_terrainEditor._editingRock = !_terrainEditor._editingRock;
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
				_channel.Broadcast<BaseScene::SpawnEvent>(SpawnEvent());
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

		ImguiEndScrollArea();

	} break;

	case Editor::eObjectEdit:
	{
		ImguiBeginScrollArea("ObjectEditor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		if (ImguiCollapse("ObjectEditor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eNone);
		}

		if (nullptr == _selectedEntity)
		{
			ImguiLabel("No Object Selected");
			if (ImguiButton("Pick Object!"))
			{
				//_pickingObject = !_pickingObject;
			}
		}
		else
		{
			if (ImguiButton("Object Selected"))
			{
				//_pickingObject = !_pickingObject;
			}
		}
		//ImguiCollapse(_collapse, nullptr, false);

		//ImguiSlider(_slider, &_val, -10, 10, 1.0f);

		imguiEdit("Editor", 90);

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

