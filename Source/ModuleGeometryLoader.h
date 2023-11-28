#pragma once
#include "Module.h"


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

	bool CleanUp();
};


