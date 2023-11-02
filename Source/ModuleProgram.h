#pragma once
#include "Module.h"
class ModuleProgram : public Module  
{
public:
	ModuleProgram();
	~ModuleProgram();
	bool Init();


public:
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
};

