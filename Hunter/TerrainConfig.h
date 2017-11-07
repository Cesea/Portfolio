#ifndef TERRAIN_CONFIG_H
#define TERRAIN_CONFIG_H

class TerrainConfig
{
public :
	TerrainConfig();

	void ReadFromFile(const std::string &fileName);
	void WriteToFile(const std::string fileName);

	//int32 SetMorphingArea(int32 lod)
	//{
	//	(int32)((_scaleXZ / TerrainQuadtree::GetRootNodes()) / pow(2, lod));
	//}

	//void SetLodRange(int32 index, int32 lodRange)
	//{
	//}

private :
	float _scaleXZ;
	float _scaleY;

	int32 _lodRange[8];
	int32 _lodMorphingArea[8];
};


#endif