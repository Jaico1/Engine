#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleEditor:public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void RenderMainMenu();
	void RenderPerformance()const;
	void RenderProperties() const;
	bool CleanUp();
private:
	bool showAboutWindow = false;  
	bool QUIT = false;
	bool showEditorWindows = true;
	bool showGitHubWindow = false;
};

