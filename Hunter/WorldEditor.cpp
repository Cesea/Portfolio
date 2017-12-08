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
	case Editor::eSystemEdit :
	{
		_editing = true;
		gEditorOn = true;
	}
	}
	_currentMode = mode;
}

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
			_terrainEditor._saveSceneInfo = false;
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

			_pCurrentScene->_world.SaveEntitiesInWorld("../resources/tmp01.ed");
			_pCurrentScene->ReleaseAllGameObjects();
			_pCurrentScene->_world.Clear();
			_pCurrentScene->AddSystemToWorld();

			TERRAIN->RebuildTerrain(_terrainEditor._terrainConfig);

			_pCurrentScene->CreateLightsAndCameras();
			_pCurrentScene->CreateObjectFromFile("../resources/tmp01.ed");
			_pCurrentScene->_camera.SetTargetObject((Player *)GAMEOBJECTFACTORY->GetPlayerObject());
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
			_terrainEditor._saveSceneInfo = false;
		}
	}
	if (_terrainEditor._editingHeight)
	{
		ImguiIndent();

		ImguiSlider("Brush Inner Radius", &_terrainEditor._heightBrush._innerRadius, 0.0f, 5.0f, 0.1f);
		_terrainEditor._heightBrush.SetInnerRadius(_terrainEditor._heightBrush._innerRadius);
		ImguiSlider("Brush Outter Radius", &_terrainEditor._heightBrush._outterRadius, 0.0f, 5.0f, 0.1f);
		_terrainEditor._heightBrush.SetOutterRadius(_terrainEditor._heightBrush._outterRadius);
		ImguiSlider("Brush Intensity", &_terrainEditor._heightBrush._intensity, 0.0f, 1.0f, 0.05f);

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

		if (_mouseLeftDown && 
			!(_mx > 0  && _mx < EDITORSIZEX && _my > 0 && _my < EDITORSIZEX))
		{
			if (_terrainEditor._grow)
			{
				TERRAIN->AddHeightOnCursorPos(Vector2((float)_mx, (float)_my), 
					_terrainEditor._heightBrush._innerRadius,
					_terrainEditor._heightBrush._outterRadius,
					_terrainEditor._heightBrush._intensity);
			}
			else if (_terrainEditor._dig)
			{
				TERRAIN->AddHeightOnCursorPos(Vector2((float)_mx, (float)_my), 
					_terrainEditor._heightBrush._innerRadius,
					_terrainEditor._heightBrush._outterRadius,
					-1.0f -_terrainEditor._heightBrush._intensity);
			}
			else if (_terrainEditor._smooth)
			{
				TERRAIN->SmoothOnCursorPos(Vector2((float)_mx, (float)_my), 
					_terrainEditor._heightBrush._outterRadius);
			}
			else if (_terrainEditor._flat)
			{
			}
		}

		if (_leftButtonReleased)
		{
			TERRAIN->ReCreateQuadTree();
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
			_terrainEditor._saveSceneInfo = false;
		}
	}
	if (_terrainEditor._editingTexture)
	{
		ImguiIndent();
		ImguiSlider("Brush Inner Radius", &_terrainEditor._textureBrush._innerRadius, 0.0f, 5.0f, 0.1f);
		_terrainEditor._textureBrush.SetInnerRadius(_terrainEditor._textureBrush._innerRadius);
		ImguiSlider("Brush Outter Radius", &_terrainEditor._textureBrush._outterRadius, 0.0f, 5.0f, 0.1f);
		_terrainEditor._textureBrush.SetOutterRadius(_terrainEditor._textureBrush._outterRadius);

#pragma region Texture Select
		ImguiLabel("Select Texture");
		{
			ImguiIndent();
			if (ImguiCheck("Layer1", _terrainEditor._layer1))
			{
				_terrainEditor._layer1 = !_terrainEditor._layer1;
				if (_terrainEditor._layer1)
				{
					_terrainEditor._layer = 0;
					_terrainEditor._layer2 = false;
				}
			}
			if (ImguiCheck("Layer2", _terrainEditor._layer2))
			{
				_terrainEditor._layer2 = !_terrainEditor._layer2;
				if (_terrainEditor._layer2)
				{
					_terrainEditor._layer = 1;
					_terrainEditor._layer1 = false;
				}
			}
			ImguiUnindent();
		}
#pragma endregion
		
#pragma region Texture Load
		//Tile Texture 0/////////////////////////////////////////////////////
		ImguiLabel("Base Texture");
		{
			ImguiIndent();

			if (TERRAIN->_tile0Handle.IsValid())
			{
				ImguiDrawTexture(0, 0, 64, 64, TERRAIN->_tile0Handle);
			}

			if (ImguiEdit(_terrainEditor._textureName00, 120))
			{
				_pCurrentScene->_editorInput = true;
			}

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
					strncpy(TERRAIN->_currentConfig._tile0FileName, _terrainEditor._textureName00, EDITOR_MAX_NAME);
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName00, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName00, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

		//Tile Texture 1/////////////////////////////////////////////////////
		ImguiLabel("Texture Layer1");
		{
			ImguiIndent();

			if (TERRAIN->_tile1Handle.IsValid())
			{
				ImguiDrawTexture(0, 0, 64, 64, TERRAIN->_tile1Handle);
			}

			if (ImguiEdit(_terrainEditor._textureName01, 120))
			{
				_pCurrentScene->_editorInput = true;
			}

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
					strncpy(TERRAIN->_currentConfig._tile1FileName, _terrainEditor._textureName01, EDITOR_MAX_NAME);
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName01, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName01, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

		//Tile Texture 2/////////////////////////////////////////////////////
		ImguiLabel("Texture Layer2");
		{
			ImguiIndent();

			if (TERRAIN->_tile2Handle.IsValid())
			{
				ImguiDrawTexture(0, 0, 64, 64, TERRAIN->_tile2Handle);
			}

			if (ImguiEdit(_terrainEditor._textureName02, 120))
			{
				_pCurrentScene->_editorInput = true;
			}

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
					strncpy(TERRAIN->_currentConfig._tile2FileName, _terrainEditor._textureName02, EDITOR_MAX_NAME);
				}
				else
				{
					ZeroMemory(_terrainEditor._textureName02, sizeof(char) * EDITOR_MAX_NAME);
					strncpy(_terrainEditor._textureName02, "Texture Not Found", EDITOR_MAX_NAME);
				}
			}
			ImguiUnindent();
		}

#pragma endregion

		//여기서 선택되어있는 텍스쳐를 그리는 작업을 실행하자...
		if (_mouseLeftDown && 
			!(_mx > 0  && _mx < EDITORSIZEX && _my > 0 && _my < EDITORSIZEY))
		{
			TERRAIN->DrawAlphaTextureOnCursorPos(Vector2((float)_mx, (float)_my),
				_terrainEditor._textureBrush._innerRadius, _terrainEditor._textureBrush._outterRadius,
				_terrainEditor._layer, _shiftDown ? true : false);
		}
		ImguiUnindent();
	}

	if (ImguiCollapse("Save Terrain", nullptr, _terrainEditor._saveSceneInfo))
	{
		_terrainEditor._saveSceneInfo = !_terrainEditor._saveSceneInfo;
		if (_terrainEditor._editingTexture)
		{
			_terrainEditor._editingExtent = false;
			_terrainEditor._editingHeight = false;
			_terrainEditor._editingTexture = false;
		}
	}

	if (_terrainEditor._saveSceneInfo)
	{
		ImguiLabel("File Name");
		ImguiIndent();

		if (ImguiEdit(_terrainEditor._fileName, 120))
		{
			_pCurrentScene->_editorInput = true;
		}

		if (ImguiButton("Save Terrain"))
		{
			TERRAIN->SaveTerrain(_terrainEditor._fileName);

			std::string fileNameCopy = _terrainEditor._fileName;
			std::string path;
			std::string name;
			std::string extension;

			SplitFilePathToNamePathExtension(fileNameCopy, name, path, extension);

			TERRAIN->_pCurrentScene->_world.SaveEntitiesInWorld(path + name + ".ed");

		}

		if (ImguiButton("Load Terrain"))
		{
			TERRAIN->LoadTerrain(_terrainEditor._fileName, true);
		}

		ImguiUnindent();
	}
	ImguiUnindent();
}

void Editor::InObjectLocateMode()
{
	if (ImguiCollapse("Object Locator", nullptr, _editing))
	{
		ChangeEditState(EditMode::eNone);
	}

	ImguiIndent();

	ImguiSlider("Object Scale Min Factor", &_objectLocator._scaleMin, 0.1f, 2.0f, 0.1f);
	ImguiSlider("Object Scale Max Factor", &_objectLocator._scaleMax, 0.1f, 2.0f, 0.1f);
	if (_objectLocator._scaleMin >= _objectLocator._scaleMax)
	{
		_objectLocator._scaleMin = _objectLocator._scaleMax - 0.1f;
	}
	if (_objectLocator._scaleMax <= _objectLocator._scaleMin)
	{
		_objectLocator._scaleMax = _objectLocator._scaleMin + 0.1f;
	}

	ImguiSlider("Object Rotation Min Factor", &_objectLocator._rotationMin, -0.9f, 0.9f, 0.05f);
	ImguiSlider("Object Rotation Max Factor", &_objectLocator._rotationMax, -0.9f, 0.9f, 0.05f);
	if (_objectLocator._rotationMin >= _objectLocator._rotationMax)
	{
		_objectLocator._rotationMin = _objectLocator._rotationMax - 0.1f;
	}
	if (_objectLocator._rotationMax <= _objectLocator._rotationMin)
	{
		_objectLocator._rotationMax = _objectLocator._rotationMin + 0.1f;
	}

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

	//NOTE : 돌 생성하는 부분
	if (false == _objectLocator._locateRock)
	{
		if (ImguiCollapse("Rocks", nullptr, _objectLocator._locateRock))
		{
			_objectLocator.Reset();
			_objectLocator._locateRock = true;
		}
	}
	else
	{
		if (ImguiCollapse("Rocks", nullptr, _objectLocator._locateRock))
		{
			_objectLocator.Reset();
		}

		ImguiIndent();
		if (ImguiButton("Rock01")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock01"); }
		if (ImguiButton("Rock02")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock02"); }
		if (ImguiButton("Rock03")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock03"); }
		if (ImguiButton("Rock04")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock04"); }
		if (ImguiButton("Rock05")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock05"); }
		if (ImguiButton("Rock06")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock06"); }
		if (ImguiButton("Rock07")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock07"); }
		if (ImguiButton("Rock08")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock08"); }
		if (ImguiButton("Rock09")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock09"); }
		if (ImguiButton("Rock10")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Rock10"); }

		_objectLocator._typeToLocate = (_objectLocator._currentStaticHandle.IsValid()) ? ARCHE_ROCK : ARCHE_NONE;

		ImguiUnindent();
	}

	//NOTE : 나무 생성하는 부분
	if (false == _objectLocator._locateTree)
	{
		if (ImguiCollapse("Trees", nullptr, _objectLocator._locateTree))
		{
			_objectLocator.Reset();
			_objectLocator._locateTree = true;
		}
	}
	else
	{
		if (ImguiCollapse("Trees", nullptr, _objectLocator._locateTree))
		{
			_objectLocator.Reset();
		}
		ImguiIndent();
		if (ImguiButton("Tree01")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree01"); }
		if (ImguiButton("Tree02")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree02"); }
		if (ImguiButton("Tree03")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree03"); }
		if (ImguiButton("Tree04")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree04"); }
		if (ImguiButton("Tree05")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree05"); }
		if (ImguiButton("Tree06")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree06"); }
		if (ImguiButton("Tree07")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree07"); }
		if (ImguiButton("Tree08")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree08"); }
		if (ImguiButton("Tree09")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree09"); }
		if (ImguiButton("Tree10")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Tree10"); }
		_objectLocator._typeToLocate = (_objectLocator._currentStaticHandle.IsValid()) ? ARCHE_TREE : ARCHE_NONE;
		ImguiUnindent();
	}

	//NOTE : 나무기둥 생성하는 부분
	if (false == _objectLocator._locateTrunk)
	{
		if (ImguiCollapse("Trunk", nullptr, _objectLocator._locateTrunk))
		{
			_objectLocator.Reset();
			_objectLocator._locateTrunk = true;
		}
	}
	else
	{
		if (ImguiCollapse("Trunk", nullptr, _objectLocator._locateTrunk))
		{
			_objectLocator.Reset();
		}
		ImguiIndent();
		if (ImguiButton("Trunk01")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("TreeTrunk01"); }
		if (ImguiButton("Trunk02")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("TreeTrunk02"); }
		if (ImguiButton("Trunk03")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("TreeTrunk03"); }
		_objectLocator._typeToLocate = (_objectLocator._currentStaticHandle.IsValid()) ? ARCHE_TREETRUNK : ARCHE_NONE;
		ImguiUnindent();
	}

	//NOTE : 풀 생성하는 부분
	if (false == _objectLocator._locateGrass)
	{
		if (ImguiCollapse("Grass", nullptr, _objectLocator._locateGrass))
		{
			_objectLocator.Reset();
			_objectLocator._locateGrass = true;
		}
	}
	else
	{
		if (ImguiCollapse("Grass", nullptr, _objectLocator._locateGrass))
		{
			_objectLocator.Reset();
		}
		ImguiIndent();
		if (ImguiButton("Grass01")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass01"); }
		if (ImguiButton("Grass02")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass02"); }
		if (ImguiButton("Grass03")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass03"); }
		if (ImguiButton("Grass04")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass04"); }
		if (ImguiButton("Grass05")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass05"); }
		if (ImguiButton("Grass06")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass06"); }
		if (ImguiButton("Grass07")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass07"); }
		if (ImguiButton("Grass08")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass08"); }
		if (ImguiButton("Grass09")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass09"); }
		if (ImguiButton("Grass10")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Grass10"); }
		_objectLocator._typeToLocate = (_objectLocator._currentStaticHandle.IsValid()) ? ARCHE_GRASS : ARCHE_NONE;
		ImguiUnindent();
	}
	//NOTE : 버섯 생성하는 부분
	if (false == _objectLocator._locateMushroom)
	{
		if (ImguiCollapse("Mushroom", nullptr, _objectLocator._locateMushroom))
		{
			_objectLocator.Reset();
			_objectLocator._locateMushroom = true;
		}
	}
	else
	{
		if (ImguiCollapse("Mushroom", nullptr, _objectLocator._locateMushroom))
		{
			_objectLocator.Reset();
		}
		ImguiIndent();
		if (ImguiButton("Mushroom01")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Mushroom01"); }
		if (ImguiButton("Mushroom02")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Mushroom02"); }
		if (ImguiButton("Mushroom03")) { _objectLocator._currentStaticHandle = VIDEO->GetStaticXMesh("Mushroom03"); }
		_objectLocator._typeToLocate = (_objectLocator._currentStaticHandle.IsValid()) ? ARCHE_MUSHROOM : ARCHE_NONE;
		ImguiUnindent();
	}

	//NOTE : 몬스터 생성하는 부분
	if (false == _objectLocator._locateMonster)
	{
		if (ImguiCollapse("Monsters", nullptr, _objectLocator._locateMonster))
		{
			_objectLocator.Reset();
			_objectLocator._locateMonster = true;
		}
	}
	else
	{
		if (ImguiCollapse("Monsters", nullptr, _objectLocator._locateMonster))
		{
			_objectLocator.Reset();
		}
		ImguiIndent();
		if (ImguiButton("Bat"))
		{
			_objectLocator._typeToLocate = ARCHE_BAT;
		}
		if (ImguiButton("Cat"))
		{
			_objectLocator._typeToLocate = ARCHE_CAT;
		}
		if (ImguiButton("Hydra"))
		{
			_objectLocator._typeToLocate = ARCHE_HYDRA;
		}
		if (ImguiButton("Lizard"))
		{
			_objectLocator._typeToLocate = ARCHE_LIZARD;
		}
		if (ImguiButton("Snake"))
		{
			_objectLocator._typeToLocate = ARCHE_SNAKE;
		}
		if (ImguiButton("Turtle"))
		{
			_objectLocator._typeToLocate = ARCHE_TURTLE;
		}
		//if (ImguiButton("Dragon"))
		//{
		//	_objectLocator._currentSkinnedHandle = VIDEO->GetSkinnedXMesh("Dragon");
		//}
		ImguiUnindent();
	}

	if ((_objectLocator._currentStaticHandle.IsValid() || _objectLocator._typeToLocate != ARCHE_NONE) &&
		_leftButtonPressed &&
		!(_mx > 0 && _mx < EDITORX + EDITORSIZEX && _my >= 0 && _my < EDITORY + EDITORSIZEY))
	{
		ResourceHandle resourceHandle;
		if (_objectLocator._currentStaticHandle.IsValid())
		{
			resourceHandle.count = _objectLocator._currentStaticHandle.count;
			resourceHandle.index = _objectLocator._currentStaticHandle.index;
		}
		else if (_objectLocator._typeToLocate != ARCHE_NONE)
		{
			resourceHandle.count = _objectLocator._currentSkinnedHandle.count;
			resourceHandle.index = _objectLocator._currentSkinnedHandle.index;
		}

		_channel.Broadcast<GameObjectFactory::CreateObjectOnClickEvent>(
			GameObjectFactory::CreateObjectOnClickEvent(_objectLocator._typeToLocate, 
				resourceHandle, 
				Vector2((float)_mx, (float)_my),
			RandFloat(_objectLocator._scaleMin, _objectLocator._scaleMax),
			RandFloat(_objectLocator._rotationMin, _objectLocator._rotationMax)));
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
				Quaternion quat;
				Matrix rotation = _objectEditor._pTransform->_matFinal;
				rotation._41 = 0;
				rotation._42 = 0;
				rotation._43 = 0;

				QuaternionRotationMatrix(&quat, &rotation);

				ImguiIndent();

				ImguiSlider("X", &quat.x, 0.0f, 1.0f, 0.01f);
				ImguiSlider("Y", &quat.y, -0.9f, 0.9f, 0.01f);
				ImguiSlider("Z", &quat.z, 0.0f, 1.0f, 0.01f);

				QuaternionNormalize(&quat, &quat);

				_objectEditor._pTransform->SetRotateWorld(quat);

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

void Editor::InSystemEditMode()
{
	if (ImguiCollapse("System Edit Mode", nullptr, _editing))
	{
		_systemEditor.Reset();
		ChangeEditState(EditMode::eNone);
	}

	ImguiIndent();
	{
		//Edit Render System ///////////////////////////////////////////////////////////////////
		if (false == _systemEditor._editRenderSystem)
		{
			if (ImguiCollapse("Render System", nullptr, _systemEditor._editRenderSystem))
			{
				_systemEditor.Reset();
				_systemEditor._editRenderSystem = true;
			}
		}
		else
		{
			if (ImguiCollapse("Render System", nullptr, _systemEditor._editRenderSystem))
			{
				_systemEditor.Reset();
			}
			ImguiIndent();
			{
				if (ImguiCheck("Render Shadow", _systemEditor._renderShadow))
				{
					_systemEditor._renderShadow = !_systemEditor._renderShadow;
				}
			}
			ImguiUnindent();
		}

		//Edit Action System ///////////////////////////////////////////////////////////////////
		if (false == _systemEditor._editActionSystem)
		{
			if (ImguiCollapse("Action System", nullptr, _systemEditor._editActionSystem))
			{
				_systemEditor.Reset();
				_systemEditor._editActionSystem = true;
			}
		}
		else
		{
			if (ImguiCollapse("Action System", nullptr, _systemEditor._editScrptSystem))
			{
				_systemEditor.Reset();
			}
			ImguiIndent();
			{
				if (ImguiCheck("Action Running", _pCurrentScene->_actionSystem.GetRunning()))
				{
					bool32 running = _pCurrentScene->_actionSystem.GetRunning();
					_pCurrentScene->_actionSystem.SetRunning(!running);
					_pCurrentScene->_actionSystem.ClearAllComponentsQueue();
				}
			}
			ImguiUnindent();
		}

		//Edit Script System ///////////////////////////////////////////////////////////////////
		if (false == _systemEditor._editScrptSystem)
		{
			if (ImguiCollapse("Script System", nullptr, _systemEditor._editScrptSystem))
			{
				_systemEditor.Reset();
				_systemEditor._editScrptSystem = true;
			}
		}
		else
		{
			if (ImguiCollapse("Script System", nullptr, _systemEditor._editScrptSystem))
			{
				_systemEditor.Reset();
			}
			ImguiIndent();
			{
				if (ImguiCheck("Script Running", _pCurrentScene->_scriptSystem.GetRunning()))
				{
					bool32 isRunning = _pCurrentScene->_scriptSystem.GetRunning();
					_pCurrentScene->_scriptSystem.SetRunning(!isRunning);

					Player * pPlayer = (Player *)GAMEOBJECTFACTORY->GetPlayerObject();
					if (isRunning &&
						nullptr != pPlayer)
					{
						pPlayer->UnRegisterEvents();
					}
					else
					{
						pPlayer->RegisterEvents();
					}
				}
			}
			ImguiUnindent();
		}
	}
	ImguiUnindent();
}

void Editor::ShowStatusWindow()
{
	if (false == _showStatus)
	{
		if (ImguiFreeCollapse("Show Status", nullptr, _showStatus, WINSIZEX - 400, 0))
		{
			_showStatus = !_showStatus;
		}
	}
	else
	{
		ImguiBeginScrollArea("Status", WINSIZEX - 400, 0, 400, 600, &_scroll);
		if (ImguiCollapse("Status", nullptr, _showStatus))
		{
			_showStatus = !_showStatus;
		}

		ImguiLabel("World State");
		{
			ImguiIndent();
			sprintf(_statusWindow._worldTerrainInfoStr, "XChunkCount : %d, ZChunkCount : %d", 
				TERRAIN->GetXChunkCount(), TERRAIN->GetZChunkCount());
			//sprintf(_statusWindow._worldObjectInfoStr, "");

			ImguiLabel(_statusWindow._worldTerrainInfoStr);
			//ImguiLabel(_statusWindow._worldObjectInfoStr);

			ImguiUnindent();
		}


		if (nullptr != _pSelectedObject)
		{
			TerrainTilePos tilePos = _pSelectedObject->GetTilePos();
			ImguiLabel("Terrain Chunk State");
			{
				ImguiIndent();
				ImguiLabel(_statusWindow._chunkPosStr);

				const Terrain::TerrainChunk &refChunk = TERRAIN->GetChunkAt(tilePos._chunkX, tilePos._chunkZ);

				sprintf(_statusWindow._chunkInfoStr, "Total Num Entities : %d", refChunk._numTotalEntity);
				sprintf(_statusWindow._tileInfoStr, "Tile Num Entity : %d", 
					refChunk._tiles[Index2D(tilePos._tileX, tilePos._tileZ, TERRAIN_TILE_DIM)]._entities.size());

				ImguiLabel(_statusWindow._chunkInfoStr);
				ImguiLabel(_statusWindow._tileInfoStr);

				ImguiUnindent();
			}

			ImguiSeparatorLine();

			ImguiLabel("Player Pos");
			{
				ImguiIndent();
				sprintf(_statusWindow._chunkPosStr, "ChunkX : %d, ChunkZ : %d", tilePos._chunkX, tilePos._chunkZ);
				sprintf(_statusWindow._tilePosStr, "TileX : %d, TileZ : %d", tilePos._tileX, tilePos._tileZ);
				sprintf(_statusWindow._relPosStr, "RelX : %f, RelZ : %f", tilePos._relX, tilePos._relZ);

				ImguiLabel(_statusWindow._chunkPosStr);
				ImguiLabel(_statusWindow._tilePosStr);
				ImguiLabel(_statusWindow._relPosStr);
				ImguiUnindent();
			}

			ImguiLabel("Cursor Pos");
			{
				ImguiIndent();
				Ray ray;
				_pCurrentScene->_camera.ComputeRay(Vector2((float)_mx, (float)_my), &ray);
				Vector3 rayHitPos;
				if (TERRAIN->IsIntersectRay(ray, &rayHitPos))
				{
					TerrainTilePos cursorTilePos;
					TERRAIN->ConvertWorldPostoTilePos(rayHitPos, &cursorTilePos);

					const Terrain::TerrainChunk &refCursorChunk = 
						TERRAIN->GetChunkAt(cursorTilePos._chunkX, cursorTilePos._chunkZ);

					sprintf(_statusWindow._cursorWorldPosStr, "Cursor World X : %f, Y : %f, Z : %f",
						rayHitPos.x, rayHitPos.y, rayHitPos.z);
					sprintf(_statusWindow._cursorChunkInfoStr, "Chunk Total Num Entities : %d",
						refCursorChunk._numTotalEntity);
					sprintf(_statusWindow._cursorTileInfoStr, "Tile Num Entity : %d",
						refCursorChunk.
						_tiles[Index2D(cursorTilePos._tileX, cursorTilePos._tileZ, TERRAIN_TILE_DIM)].
						_entities.size());

					ImguiLabel(_statusWindow._cursorWorldPosStr);
					ImguiLabel(_statusWindow._cursorChunkInfoStr);
					ImguiLabel(_statusWindow._cursorTileInfoStr);
				}
				ImguiUnindent();
			}
		}

		ImguiEndScrollArea();
	}
}


void Editor::UpdateInput(const InputManager & input)
{
	_mx = input.mouse.GetCurrentPoint().x;
	_my = input.mouse.GetCurrentPoint().y;
	_mouseLeftDown = input.mouse.IsDown(MOUSE_BUTTON_LEFT);
	_mouseRightDown = input.mouse.IsDown(MOUSE_BUTTON_RIGHT);

	_leftButtonPressed = input.mouse.IsPressed(MOUSE_BUTTON_LEFT);
	_leftButtonReleased = input.mouse.IsReleased(MOUSE_BUTTON_LEFT);
	_shiftDown = input.keyboard.GetShiftDown();
	_key = input.keyboard.GetVKCode();
	_scroll = -(int32)((float)input.mouse.GetWheelDelta() / 120.0f) * 6;
}

void Editor::Init(IScene *pScene)
{
	strncpy(_terrainEditor._textureName00, TERRAIN->_currentConfig._tile0FileName, EDITOR_MAX_NAME);
	strncpy(_terrainEditor._textureName01, TERRAIN->_currentConfig._tile1FileName, EDITOR_MAX_NAME);
	strncpy(_terrainEditor._textureName02, TERRAIN->_currentConfig._tile2FileName, EDITOR_MAX_NAME);

	_terrainEditor._terrainConfig._textureMult = TERRAIN->_currentConfig._textureMult;
	_terrainEditor._heightBrush.Init();
	_terrainEditor._textureBrush.Init();

	_objectLocator._objectPaintBrush.Init();

	strncpy(_terrainEditor._terrainConfig._tile0FileName, TERRAIN->_currentConfig._tile0FileName, EDITOR_MAX_NAME);
	strncpy(_terrainEditor._terrainConfig._tile1FileName, TERRAIN->_currentConfig._tile1FileName, EDITOR_MAX_NAME);
	strncpy(_terrainEditor._terrainConfig._tile2FileName, TERRAIN->_currentConfig._tile2FileName, EDITOR_MAX_NAME);

	_pCurrentScene = pScene;
}

void Editor::Shutdown()
{
	_pCurrentScene = nullptr;
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
		if (ImguiCollapse("System Editor", nullptr, _editing))
		{
			ChangeEditState(EditMode::eSystemEdit);
		}
	} break;
	
	case Editor::eTerrainEdit :
	{
		Vector3 cursorWorldPos;
		Ray ray;
		TERRAIN->_pCurrentScene->_camera.ComputeRay(
			Vector2(input.mouse.GetCurrentPoint().x, input.mouse.GetCurrentPoint().y), &ray);
		if (TERRAIN->IsIntersectRay(ray, &cursorWorldPos))
		{
			_terrainEditor._heightBrush._centerPos = cursorWorldPos;
			_terrainEditor._textureBrush._centerPos = cursorWorldPos;
		}
		ImguiBeginScrollArea("Terrain Editor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InTerrainEditMode();
		ImguiEndScrollArea();
	}break;

	case Editor::eObjectLocate:
	{
		static int32 scroll = 0;
		ImguiBeginScrollArea("Object Locator", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &scroll);
		InObjectLocateMode();
		ImguiEndScrollArea();
	} break;

	case Editor::eObjectEdit:
	{
		ImguiBeginScrollArea("ObjectE Editor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InObjectEditMode();
		ImguiEndScrollArea();
	} break;
	case Editor ::eSystemEdit :
	{
		ImguiBeginScrollArea("System Editor", EDITORX, EDITORY, EDITORSIZEX, EDITORSIZEY, &_scroll);
		InSystemEditMode();
		ImguiEndScrollArea();
	}break;
	}

	ShowStatusWindow();


	ImguiEndFrame();
}

void Editor::SetEdittingEntity(Entity & entity)
{
	_objectEditor.OnNewSelection(entity);
}

void Editor::Render()
{
	switch (_currentMode)
	{
	case Editor::eTerrainEdit:
	{
		if (_terrainEditor._editingHeight)
		{
			_terrainEditor._heightBrush.Render();
		}
		else if (_terrainEditor._editingTexture)
		{
			_terrainEditor._textureBrush.Render();
		}
	} break;
	case Editor::eObjectLocate:
	{
		_objectLocator._objectPaintBrush.Render();

	} break;
	case Editor::eObjectEdit:
	{

	} break;
	}
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
	_layer = 0;
	_layer1 = false;
	_layer2 = false;
}

bool Brush::Init()
{
	_innerRadius = 1.0f;
	_outterRadius = 2.0f;

	_intensity = 1.0f;

	_centerPos = Vector3(0.0f, 0.0f, 0.0f);

	float deltaAngle = 2 * D3DX_PI / (BRUSH_CIRCLE_RES - 1);

	for (int32 i = 0; i < BRUSH_CIRCLE_RES; ++i)
	{
		_innerVertices[i]._position.x = cosf(deltaAngle * i) * _innerRadius + _centerPos.x;
		_innerVertices[i]._position.x = sinf(deltaAngle * i) * _innerRadius + _centerPos.z;
		_innerVertices[i]._color = BRUSH_INNER_COLOR;
	}

	for (int32 i = 0; i < BRUSH_CIRCLE_RES; ++i)
	{
		_outterVertices[i]._position.x = cosf(deltaAngle * i) * _outterRadius + _centerPos.x;
		_outterVertices[i]._position.x = sinf(deltaAngle * i) * _outterRadius + _centerPos.z;
		_outterVertices[i]._color = BRUSH_OUTTER_COLOR;
	}

	return true;
}

void Brush::SetInnerRadius(float radius)
{
	_innerRadius = radius;

	float deltaAngle = (2 * D3DX_PI) / (BRUSH_CIRCLE_RES - 1);

	for (uint32 i = 0; i < BRUSH_CIRCLE_RES ; ++i)
	{
		_innerVertices[i]._position.x = cosf(i * deltaAngle) * _innerRadius + _centerPos.x;
		_innerVertices[i]._position.z = sinf(i * deltaAngle) * _innerRadius + _centerPos.z;
	}
	for (uint32 i = 0; i < BRUSH_CIRCLE_RES; ++i)
	{
		_innerVertices[i]._position.y = TERRAIN->GetHeight(
			_innerVertices[i]._position.x, _innerVertices[i]._position.z) + 0.1f;
	}

	if (_outterRadius < _innerRadius)
	{
		SetOutterRadius(radius + 0.1f);
	}
}

void Brush::SetOutterRadius(float radius)
{
	_outterRadius = radius;

	float deltaAngle = (2 * D3DX_PI) / (BRUSH_CIRCLE_RES - 1);

	for (uint32 i = 0; i < BRUSH_CIRCLE_RES ; ++i)
	{
		_outterVertices[i]._position.x = cosf(i * deltaAngle) * _outterRadius + _centerPos.x;
		_outterVertices[i]._position.z = sinf(i * deltaAngle) * _outterRadius + _centerPos.z;
	}

	for (uint32 i = 0; i < BRUSH_CIRCLE_RES; ++i)
	{
		_outterVertices[i]._position.y = TERRAIN->GetHeight(
			_outterVertices[i]._position.x, _outterVertices[i]._position.z) + 0.1f;
	}

	if (_innerRadius >= _outterRadius)
	{
		SetInnerRadius(radius - 0.1f);
	}
}

void Brush::Render()
{
	Matrix iden;
	MatrixIdentity(&iden);
	gpDevice->SetTransform(D3DTS_WORLD, &iden);
	gpDevice->SetFVF(BrushVertex::FVF);
	gpDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, BRUSH_CIRCLE_RES - 1, _innerVertices, sizeof(BrushVertex));
	gpDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, BRUSH_CIRCLE_RES - 1, _outterVertices, sizeof(BrushVertex));

}