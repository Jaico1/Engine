#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleGeometryLoader.h"
#include "ModuleTexture.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_sdl2.h"
#include "ImGUI\imgui_impl_opengl3.h"
#include "GL\glew.h"


ModuleEditor::ModuleEditor() {

}

ModuleEditor::~ModuleEditor() {

}

size_t GetMemoryUsage() {
    
    return 0;
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
    

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->GetSDLwindow(),App->GetOpenGL()->context);
    ImGui::SetNextWindowSize(ImVec2(800, 600));

    ImGui_ImplOpenGL3_Init("#version 460");
    
   
    
    return true;
}

update_status ModuleEditor::PreUpdate() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->GetSDLwindow());
    ImGui::NewFrame();
    return UPDATE_CONTINUE;
}


update_status ModuleEditor::Update() {
    bool show_demo_window = true;
    //ImGui::ShowDemoWindow(&show_demo_window);

    RenderMainMenu();
    if(showEditorWindows==true){
    RenderPerformance();
    RenderProperties();
    
    ImGui::Render();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (QUIT)
    {
        return UPDATE_STOP;
    }

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {

    //ImGui_ImplSDL2_ProcessEvent(App->GetInput()->); 
    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(App->GetWindow()->GetSDLwindow(), App->GetOpenGL()->context);
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

void ModuleEditor::RenderMainMenu() {

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Add menu items under "File"
            if (ImGui::MenuItem("Quit", "Alt+F4")) {
                QUIT = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
           
            ImGui::MenuItem("GitHub Page", nullptr, &showGitHubWindow);
           

            ImGui::MenuItem("About", nullptr, &showAboutWindow);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            // Add menu items under "View"
            ImGui::MenuItem("Toggle Editor Windows", nullptr, &showEditorWindows);
                
            ImGui::EndMenu();
        }

        // Add more menus as needed...
      
        ImGui::EndMainMenuBar();
        
        
    }

    // Display About window if the flag is set
    if (showAboutWindow) {
        ImGui::Begin("About", &showAboutWindow);
        ImGui::Text("Version:0.0.1");
        // Add more information or widgets as needed
        ImGui::End();
    }

    if (showGitHubWindow) {
        ImGui::Begin("GitHub", &showGitHubWindow);
        ImGui::Text("https://github.com/Jaico1/Engine"); 
        // Add more information or widgets as needed
        ImGui::End();
    }

    

}

void ModuleEditor::RenderProperties() const {

    ImGui::Begin("Properties");
    
    if (ImGui::CollapsingHeader("Geometry", ImGuiTreeNodeFlags_DefaultOpen)) // ImGuiTreeNodeFlags_DefaultOpen will make it initially open
    {
        ImGui::Text("Total Triangles: %lu ", App->GetMesh()->GetTotalTriangleCount());
        
    }

    // Section 2
    if (ImGui::CollapsingHeader("Texture"))
    {
        ImGui::Text("Texture Height: %lu ", App->GetTexture()->textureHeight);
       
        ImGui::Text("Texture Width: %lu ", App->GetTexture()->textureWidth);
    }


    


    ImGui::End();

}

void ModuleEditor::RenderPerformance() const{
    //ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
    
    ImGui::Begin("Performance");

    static float fpsValues[1000] = { 0 };
    static int fpsValuesIndex = 0;

    float currentFps = ImGui::GetIO().Framerate;
    fpsValues[fpsValuesIndex] = currentFps;
    fpsValuesIndex = (fpsValuesIndex + 1) % IM_ARRAYSIZE(fpsValues);

    ImGui::PlotLines("FPS", fpsValues, IM_ARRAYSIZE(fpsValues), fpsValuesIndex, "FPS", 0.0f, 60.0f, ImVec2(0, 80));
    ImGui::Text("Current FPS: %.2f", currentFps);

    size_t memoryUsage = GetMemoryUsage();
    ImGui::Text("Memory Consumption: %lu bytes", memoryUsage);

    // Display hardware information
    ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
    ImGui::Text("GPU Renderer: %s", glGetString(GL_RENDERER));

    // Display software versions
    ImGui::Text("SDL Version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
    ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));
    

    ImGui::End();

   
}


