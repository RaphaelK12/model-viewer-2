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

int Texture::GlobalTextureIndex = 0;

int main()
{
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    Display display = CreateDisplay(WIDTH, HEIGHT, "Model Viewer");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(display.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Load shader from file
    Shader shader = LoadShadersFromFiles("res/shaders/textured/textured.vert", "res/shaders/textured/textured.frag");
    glUseProgram(shader.ID);

    MeshIndexed mesh = LoadMeshIndexedFromOBJ("res/models/dragon.obj");
    Entity entity = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
    Texture texture = LoadTextureFromFile("res/textures/dragon_texture_color.png");
    glUniform1i(shader.uniformLocations["diffuse"], texture.index);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(shader.uniformLocations["view"], 1, GL_FALSE, &view[0][0]);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv(shader.uniformLocations["projection"], 1, GL_FALSE, &projection[0][0]);

    while(!glfwWindowShouldClose(display.window))
    {
        DeltaTimeCalc(display);

        glm::mat4 model(1.0f);
        model = glm::translate(model, entity.position);
        model = glm::rotate(model, glm::radians(entity.rotation.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.z), {0.0f, 0.0f, 1.0f});
        model = glm::scale(model, entity.scale);
        glUniformMatrix4fv(shader.uniformLocations["model"], 1, GL_FALSE, &model[0][0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.numVertices, GL_UNSIGNED_INT, nullptr);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Main Controls");
        ImGui::SliderFloat3("Translation", &entity.position.x, -1.0f, 1.0f);
        ImGui::SliderFloat3("Rotation", &entity.rotation.x, -360.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &entity.scale.x, 0.0f, 5.0f);
        ImGui::End();
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