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

	bool CleanUp();

private:

	std::vector<Model>Models;
	
};


