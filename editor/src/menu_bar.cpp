#include "menu_bar.h"

#include "imgui.h"

bool DrawMenuBar()
{
    bool exitRequested = false;

    if (!ImGui::BeginMainMenuBar())
    {
        return false;
    }

    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("New");
        ImGui::MenuItem("Open", "Ctrl+O");
        ImGui::MenuItem("Save", "Ctrl+S");

        ImGui::Separator();

        if (ImGui::MenuItem("Exit"))
        {
            exitRequested = true;
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::MenuItem("Undo", "Ctrl+Z", false, false);
        ImGui::MenuItem("Redo", "Ctrl+Y", false, false);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View"))
    {
        ImGui::MenuItem("Project");
        ImGui::MenuItem("Inspector");
        ImGui::MenuItem("Grid");

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
        ImGui::MenuItem("About");

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    return exitRequested;
}