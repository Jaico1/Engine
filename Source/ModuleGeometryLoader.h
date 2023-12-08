#pragma once
#include "Module.h"
#include <vector>

using namespace std;

class Model;
class Mesh;

class ModuleGeometryLoader :
    public Module
{
public:

	ModuleGeometryLoader();
	~ModuleGeometryLoader();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	int GetTotalTriangleCount();
	
	void LoadGeometry();

	using FileDropCallback = void (*)(const char*);

	// Function to handle file drop events
	void OnFileDrop(const char* filePath);

	// Set the file drop callback
	void SetFileDropCallback(FileDropCallback callback);

	bool CleanUp();

private:
	FileDropCallback fileDropCallback;
	std::vector<Model>Models;
	
};


