#pragma once
#include "Module.h"
class ModuleProgram : public Module  
{
public:
	ModuleProgram();
	~ModuleProgram();
	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	

public:
	unsigned program;
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
};

