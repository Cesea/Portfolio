#ifndef WORLD_EDITOR_H
#define WORLD_EDTIOR_H

class IScene;
class Entity;

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

class WorldEditor
{
public :

	void Edit(IScene *pScene);

private :
	
	//std::vector<EntityHandle> _selectedEntities;

	//Gizmo _gizmo;
};


#endif