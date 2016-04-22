#pragma once
#include <BRFImporterStructs.h>
#include <FileData.h>
#include <Fetch.h>
#include <MeshData.h>
#include <MaterialData.h>
#include <SkeletonData.h>

#include "../MeshManager/MeshManager.h"
#include "../ShaderManager/ShaderManager.h"
#include "../../ObjectNode/Mesh/Mesh.h"
#include "../../../Structs/DataTypes.h"

class BRFImporterHandler
{
private:
	BRFImporterLib::FileData* currentFile;
	MeshManager* meshManager;
public:
	BRFImporterHandler();
	~BRFImporterHandler();

	void LoadFile(std::string fileName, bool mesh, bool material, bool skeleton);

	void Initialize(MeshManager* meshManager);
	void Release();
};

