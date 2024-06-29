#include "gameMenu.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <iostream>
#include "stb_image.h"
#include <GLFW/glfw3.h>
#define GL_CLAMP_TO_EDGE 0x812F

int choosenMaze;
int choosenLight;
bool quitTheGame = false;


// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 1;
    int image_height = 1;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

int window_width, window_height;
bool isMenuOk = false;
int my_image_width = 1;
int my_image_height = 1;
GLuint my_image_texture = 1;


menuData* renderGameMenu(int gameState) {
    bool closed = false;

    // Initialize GLFW
    if (!glfwInit()) {
        return NULL;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 300, "Maze Game Menu", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop
    while (!isMenuOk) {
        if (glfwWindowShouldClose(window)) {
            closed = true;
            goto endMainLoop;
        }
        mainLoops(window, gameState);
    }

    endMainLoop:

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    isMenuOk = false;

    // Prepare the data to return
    menuData* data = new menuData;
    // Populate data based on the selections made in the menu
    data->closed = closed;
    data->selectedMaze = choosenMaze;
    data->selectedLight = choosenLight;
    data->quitTheGame = quitTheGame;
    data->everyLightEnabled = false;

    return data;
}

void mainLoops(GLFWwindow* window, int gameState) {
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set up window size and position
    glfwGetWindowSize(window, &window_width, &window_height);
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    // Start a new ImGui window
    ImGui::Begin("Maze Game Menu");
    ImGui::SetWindowFontScale(1.3);
	ImGui::Spacing(); ImGui::Spacing(); // Adds extra space 
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();


    // Determine spacing for checkboxes based on window width
    float checkboxWidth = 100.0f;
    float totalCheckboxWidth = checkboxWidth * 3;
    float spacing = (window_width - totalCheckboxWidth) / 4;

    // Maze selection radio buttons
    static int selectedMaze = -1; // -1 for none selected initially
    ImGui::SetCursorPosX(spacing);
    ImGui::RadioButton("Maze 1", &selectedMaze, 0); ImGui::SameLine();
    ImGui::SetCursorPosX(spacing * 2 + checkboxWidth);
    ImGui::RadioButton("Maze 2", &selectedMaze, 1); ImGui::SameLine();
    ImGui::SetCursorPosX(spacing * 3 + checkboxWidth * 2);
    ImGui::RadioButton("Maze 3", &selectedMaze, 2);
    choosenMaze = selectedMaze;

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();


    // Light selection radio buttons
    static int selectedLight = -1; // -1 for none selected initially
    float lightSpacing = (window_width - (checkboxWidth * 4)) / 5;
    ImGui::SetCursorPosX(lightSpacing);
    ImGui::RadioButton("Sun Light", &selectedLight, 0); ImGui::SameLine();
    ImGui::SetCursorPosX(lightSpacing * 2 + checkboxWidth);
    ImGui::RadioButton("Spot Light", &selectedLight, 1); ImGui::SameLine();
    ImGui::SetCursorPosX(lightSpacing * 3 + checkboxWidth * 2);
    ImGui::RadioButton("Point Light", &selectedLight, 2); ImGui::SameLine();
    ImGui::SetCursorPosX(lightSpacing * 4 + checkboxWidth * 3);
    ImGui::RadioButton("Every Light", &selectedLight, 3);
    choosenLight = selectedLight;

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing(); 
    ImGui::Spacing(); 
    ImGui::Spacing(); 
    ImGui::Spacing(); // Adds extra space


    // OK button centered horizontally
    float okButtonWidth = 200.0f; // Width of the OK button
    float okButtonHeight = 50.0f; // Height of the OK button
    float centerPosX = (window_width - okButtonWidth) * 0.5f; // Center position for the OK button
    ImGui::SetCursorPosX(centerPosX);
    if (gameState == 0) {
        if (ImGui::Button("Start", ImVec2(okButtonWidth, okButtonHeight))) {
			isMenuOk = true;
		}
	}

    else if (gameState == 3) {
        // Assuming you want some space between the buttons
        float spacing = 10.0f; // Adjust the spacing to your liking
        float totalWidth = okButtonWidth * 2 + spacing;
        float windowWidth = ImGui::GetWindowWidth();
        float offset = (windowWidth - totalWidth) * 0.5f;

        // Set the cursor to the calculated offset to center the buttons
        ImGui::SetCursorPosX(offset);

        // Restart button
        if (ImGui::Button("Restart", ImVec2(okButtonWidth, okButtonHeight))) {
            isMenuOk = true;
            // Insert logic for restarting the game here
        }

        // Same line, with spacing
        ImGui::SameLine(0, spacing);

        // Quit button
        if (ImGui::Button("Quit", ImVec2(okButtonWidth, okButtonHeight))) {
            quitTheGame = true;
        }
	}   

    // End the ImGui window
    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, window_width, window_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
}