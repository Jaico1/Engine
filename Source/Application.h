#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleProgram;
class ModuleDebugDraw;
class ModuleRender;
class ModuleCamera;
class ModuleGeometryLoader;


class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleEditor* GetEditor() { return editor; }
    ModuleProgram* GetProgram() { return program; }
    ModuleDebugDraw* GetDebug() { return debug; }
    ModuleRender* GetRender() { return renderer; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleGeometryLoader* GetMesh() { return mesh; }


private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debug = nullptr; 
    ModuleProgram* program = nullptr;
    ModuleRender* renderer = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleGeometryLoader* mesh= nullptr;
    std::list<Module*> modules;

};

extern Application* App;
