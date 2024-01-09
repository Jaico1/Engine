#pragma once
#include "Module.h"



class ModuleTexture :
    public Module
{
public:
	 ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	unsigned LoadTexture(const char* filePath);
	unsigned LoadTexturePNG(const char* filePath);
	unsigned LoadCubeMap(const char* fileName);
	size_t textureWidth ;
	size_t textureHeight ;

private:
	
	unsigned texture;
	
};


