#pragma once
#include "../../../Structs/RenderInfo.h"
#include "../../../Source/Constants.h"
#include "../../../Source/LibIncluder.h"
#include "../../ObjectNode/Mesh/Mesh.h"
#include "../../../Enumerations/Enumerations.h"
#include "../../ObjectNode/QuadTree/QuadTree.h"


class MeshManager
{

private:
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;

	//This vector will be indexed with the enum "Meshes"
	std::vector<Mesh>* gameMeshes		= nullptr;
	std::vector<Mesh>* sceneMeshes		= nullptr;
	Mesh placeHolder;
	Mesh placeHolderPlane;
	Mesh fullScreenQuad;
	Mesh killCountQuad;
	Mesh waveCountQuad;
	Mesh waveCompleteQuad;
	
	
private:
	void CreatePlaceHolder();
	void CreatePlaceHolderPlane();
	void CreateFullScreenQuad();
	void CreateKillCountQuad();
	void CreateWaveCountQuad();
	void CreateWaveCompleteQuad();
public:
	QuadTree* quadTree;
	MeshManager();
	~MeshManager();
	
	void Initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	void Release();

	void AddMesh(
		bool hasSkeleton,
		unsigned int skeletonID,
		int materialID,
		unsigned int vertexCount,
		UINT indexCount,
		std::vector<Vertex> vertices,
		std::vector<AnimVert> aniVertices,
		std::vector<UINT> indices,
		bool isScene
		);
	//UVoffset offset;
	std::vector<Mesh>* GetMeshes() { return this->gameMeshes; }
	std::vector<Mesh>* GetSceneMeshes() { return this->sceneMeshes; }
	void CreateQuadTree();
	void GetMeshRenderInfo(MeshEnum* meshEnum, RenderInstructions* toRender);

	void GetPlaceHolderMeshInfo(RenderInstructions* toRender);
	void GetPlaceHolderPlaneInfo(RenderInstructions* toRender);
	void GetFullScreenQuadInfo(RenderInstructions* toRender);
	void GetFullScreenQuadInfoUI(UITextures* uiEnum, RenderInstructions* toRender);
	
};

