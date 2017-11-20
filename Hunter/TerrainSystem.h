#ifndef TERRAINSYSTEM_H
#define TERRAINSYSTEM_H


//class Terrain
//{
//public:
//	struct LocalVertex
//	{
//		cVector2 xyPosition;
//		cVector2 localUV;
//	};
//
//	struct SectorVertex
//	{
//		float	height;
//		cVector3 normal;
//	};
//
//	struct ElevationData
//	{
//		float	minElevation;
//		float	maxElevation;
//		float	minNormalZ;
//		float	maxNormalZ;
//		float	strength;
//	};
//
//	struct TerrainTextureData
//	{
//		cImage* pImage;
//		float	uvScale;
//		elevationData elevation;
//	};
//
//	// Public Data...
//
//	// Creators...
//
//	Terrain();
//	virtual ~Terrain();
//
//	// Operators...
//
//
//	// Mutators...
//	virtual bool Create(cSceneNode* pRootNode, cTexture* heightMap, const cRect3d& worldExtents, uint8 shift = 3);
//	virtual void Destroy();
//
//	virtual void SubmitSection(
//		cTerrainSection* pSection)const;
//
//	virtual void RenderSection(
//		cTerrainSection* pSection,
//		u32Flags activationFlags,
//		const cRenderEntry* entry)const;
//
//	void ReadUserInput();
//
//	float ReadWorldHeight(uint16 mapX, uint16 mapY)const;
//	float ReadWorldHeight(uint16 mapIndex)const;
//	const cVector3& readWorldNormal(uint16 mapX, uint16 mapY)const;
//
//	float CalcWorldHeight(float worldX, float worldY)const;
//	void CalcWorldNormal(cVector3& normal, float worldX, float worldY)const;
//
//	float CalcMapHeight(float mapX, float mapY)const;
//	void CalcMapNormal(cVector3& normal, float mapX, float mapY)const;
//
//	void CreateBlendTexture();
//
//	void GenerateTerrainImage(
//		cImage* pTextureMap,
//		terrainTextureData* pTextureList,
//		int	textureCount);
//
//	void GenerateBlendImage(
//		cImage* pBlendImage,
//		elevationData* pElevationData,
//		int	elevationDataCount);
//
//	void SetRenderMethod(cRenderMethod* pRenderMethod);
//
//	uint16 TableWidth()const { return m_tableWidth; }
//	uint16 TableHeight()const { return m_tableHeight; }
//	uint16 TableIndex(uint16 mapX, uint16 mapY)const;
//
//	const cVector3& mapScale()const { return m_mapScale; }
//	const cRect3d& worldExtents()const { return m_worldExtents; }
//
//	float computeErrorMetricOfGrid(
//		uint16 xVerts,	// width of grid
//		uint16 yVerts,	// height of grid
//		uint16 xStep,	// horz vertex count per cell
//		uint16 yStep,	// vert vertex count per cell
//		uint16 xOffset,	// starting index X
//		uint16 yOffset);// starting index Y
//
//	void setTessellationParameters(float vScale, float vLimit);
//	float lodErrorScale()const { return m_vScale; }
//	float lodRatioLimit()const { return m_vLimit; }
//
//	uint8 sectorShift()const { return m_sectorShift; }
//
//protected:
//
//	// Private Data...
//	cSceneNode* m_pRootNode;
//	cRect3d m_worldExtents;
//	cVector3 m_worldSize;
//	cVector3 m_mapScale;
//	uint16	m_sectorCountX;
//	uint16	m_sectorCountY;
//	cTerrainSection* m_pSectorArray;
//	cVertexBuffer* m_pVertexGrid;
//	cIndexBuffer* m_pTriangles;
//	cRenderMethod* m_pRenderMethod;
//
//	cVector2 m_sectorSize;
//
//	cCamera* m_activeCamera;
//
//	uint8 m_sectorShift;
//	uint8 m_sectorUnits;
//	uint8 m_sectorVerts;
//
//	uint16 m_tableWidth;
//	uint16 m_tableHeight;
//	float* m_heightTable;
//	cVector3* m_normalTable;
//
//	float m_vScale;
//	float m_vLimit;
//
//	// Private Functions...
//	virtual bool allocateSectors();
//	virtual bool buildVertexBuffer();
//	virtual bool setVertexDescription();
//	virtual bool buildIndexBuffer();
//	void buildHeightAndNormalTables(cTexture* pTexture);
//
//	// Invalid Functions...
//	cTerrain(const cTerrain& Src);
//	cTerrain& operator=(const cTerrain& Src);
//
//
//};
//
////- Inline Functions --------------------------------------------
//
////. Creators ....................................................
//
///*	cTerrain
//-----------------------------------------------------------------
//
//	Default Constructor
//
//-----------------------------------------------------------------
//*/
//inline cTerrain::cTerrain()
//	:m_pRootNode(0)
//	, m_pSectorArray(0)
//	, m_pVertexGrid(0)
//	, m_pTriangles(0)
//	, m_heightTable(0)
//	, m_normalTable(0)
//	, m_pRenderMethod(0)
//	, m_activeCamera(0)
//	, m_vScale(1.0f)
//	, m_vLimit(1.0f)
//{
//}
//
//
///*	~cTerrain
//-----------------------------------------------------------------
//
//	Default Destructor
//
//-----------------------------------------------------------------
//*/
//inline Terrain::~Terrain()
//{
//	Destroy();
//}


#endif  
