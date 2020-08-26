#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "AssetManagement/asset_loader.h"
#include "Display/display.h"
#include <glm/gtc/matrix_transform.hpp>

// #include <imconfig.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

int main()
{
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    Display display = CreateDisplay(WIDTH, HEIGHT, "Model Viewer");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(display.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Our state
    bool colorWindow = true;
    ImVec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

    // Load shader from file
    Shader shader = LoadShadersFromFiles("res/shaders/basic/basic.vert", "res/shaders/basic/basic.frag");
    glUseProgram(shader.ID);

    float vertices[]
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glm::mat4 model(1.0f);
    glUniformMatrix4fv(shader.uniformLocations["model"], 1, GL_FALSE, &model[0][0]);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(shader.uniformLocations["view"], 1, GL_FALSE, &view[0][0]);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv(shader.uniformLocations["projection"], 1, GL_FALSE, &projection[0][0]);

    while(!glfwWindowShouldClose(display.window))
    {
        DeltaTimeCalc(display);

        glUniform4fv(shader.uniformLocations["inColor"], 1, (float*)&color);

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if(colorWindow)
        {
            ImGui::Begin("Square color picker");
            ImGui::ColorEdit3("Square color", (float*)&color);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(display.window);
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}