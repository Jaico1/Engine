#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleGeometryLoader.h"
#include "ModuleTexture.h"





using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(input = new ModuleInput());
	modules.push_back(debug = new ModuleDebugDraw());
	modules.push_back(mesh = new ModuleGeometryLoader());
	modules.push_back(editor = new ModuleEditor());
	
	
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::SetFileDropCallback(FileDropCallback callback) {
	fileDropCallback = callback;
}

void Application::HandleFileDrop(const char* filePath) {
	// Call the registered file drop callback
	if (fileDropCallback) {
		fileDropCallback(filePath);
	}
}
