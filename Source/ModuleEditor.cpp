#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_sdl2.h"
#include "ImGUI\imgui_impl_opengl3.h"


ModuleEditor::ModuleEditor() {

}

ModuleEditor::~ModuleEditor() {

}

bool ModuleEditor::Init() {

	ImGui::CreateContext();
    
    
    bool show_another_window = false;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window,App->GetOpenGL()->context);
    ImGui_ImplOpenGL3_Init("#version 460");
    
   
    
    return true;
}

update_status ModuleEditor::PreUpdate() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
    ImGui::NewFrame();
    return UPDATE_CONTINUE;
}


update_status ModuleEditor::Update() {
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {

    //ImGui_ImplSDL2_ProcessEvent(App->GetInput()->); 
    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(App->GetWindow()->window, App->GetOpenGL()->context);
    }
    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}


