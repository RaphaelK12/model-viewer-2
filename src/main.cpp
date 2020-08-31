#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "AssetManagement/asset_loader.h"
#include "Display/display.h"
#include "Camera/camera.h"
#include <glm/gtc/matrix_transform.hpp>

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
    Shader shader = LoadShadersFromFiles("res/shaders/lighting/lighting.vert", "res/shaders/lighting/lighting.frag");
    glUseProgram(shader.ID);

    // Load mesh and its texture
    MeshIndexed mesh = LoadMeshIndexedFromOBJ("res/models/dragon.obj");
    Entity entity = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
    Texture texture = LoadTextureFromFile("res/textures/dragon_texture_color.png");
    glUniform1i(shader.uniformLocations["diffuse"], texture.index);

    // Load lightcube mesh
    Mesh lightMesh = GenerateCube();
    Shader lightShader = LoadShadersFromFiles("res/shaders/lightcube/lightcube.vert", "res/shaders/lightcube/lightcube.frag");

    // Used for debug axes view
    Mesh debugAxes = GenerateAxes();
    Shader debugShader = LoadShadersFromFiles("res/shaders/debug/debug.vert", "res/shaders/debug/debug.frag");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv(shader.uniformLocations["projection"], 1, GL_FALSE, &projection[0][0]);

    // Camera info
    Camera camera = { {0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -3.0f}, {0.0f, 1.0f, 0.0f}, 0.0f, 0.0f, true };
    bool rotating = false;
    bool shouldReset = false;
    bool axes = false;

    // Point light info
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    while(!glfwWindowShouldClose(display.window))
    {
        DeltaTimeCalc(display);
        if(!ImGui::GetIO().WantCaptureMouse)
            ProcessInput(display, camera, rotating, shouldReset);

        glUseProgram(shader.ID);
        glUniform3fv(shader.uniformLocations["pointLightPos"], 1, &lightPos.x);
        glUniform3fv(shader.uniformLocations["cameraPos"], 1, &camera.position.x);

        // Transform matrix for mesh
        glm::mat4 model(1.0f);
        model = glm::translate(model, entity.position);
        model = glm::rotate(model, glm::radians(entity.rotation.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.z), {0.0f, 0.0f, 1.0f});
        model = glm::scale(model, entity.scale);
        glUniformMatrix4fv(shader.uniformLocations["model"], 1, GL_FALSE, &model[0][0]);

        // Transform matrix for camera
        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
        glUniformMatrix4fv(shader.uniformLocations["view"], 1, GL_FALSE, &view[0][0]);

        // Render the mesh
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.numVertices, GL_UNSIGNED_INT, nullptr);

        // Switch to light shader for lightcube rendering
        glUseProgram(lightShader.ID);
        model = glm::mat4(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));
        glUniformMatrix4fv(lightShader.uniformLocations["model"], 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(lightShader.uniformLocations["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(lightShader.uniformLocations["projection"], 1, GL_FALSE, &projection[0][0]);

        // Render light mesh
        glBindVertexArray(lightMesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, lightMesh.numVertices);

        if(axes)
        {
            glDisable(GL_DEPTH_TEST);

            glm::mat4 debugModel(1.0f);
            debugModel = glm::scale(debugModel, { 10.0f, 10.0f, 10.0f });
            glm::mat4 MVP = projection * view * debugModel;
            glUseProgram(debugShader.ID);
            glUniformMatrix4fv(debugShader.uniformLocations["MVP"], 1, GL_FALSE, &MVP[0][0]);
            glBindVertexArray(debugAxes.VAO);
            glDrawArrays(GL_LINES, 0, debugAxes.numVertices);

            glEnable(GL_DEPTH_TEST);
        }

        // Start ImGui frame and render the window
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Main Controls");
        ImGui::Checkbox("Show Debug Axes?", &axes);
        ImGui::Text("Model transform");
        ImGui::SliderFloat3("Model Translation", &entity.position.x, -1.0f, 1.0f);
        ImGui::SliderFloat3("Model Rotation", &entity.rotation.x, -360.0f, 360.0f);
        ImGui::SliderFloat3("Model Scale", &entity.scale.x, 0.0f, 5.0f);
        if(ImGui::Button("Reset model transform"))
        {
            entity.position = entity.rotation = glm::vec3(0.0f);
            entity.scale = glm::vec3(1.0);
        }
        ImGui::Text("Camera transform");
        ImGui::Checkbox("Rotate camera with mouse?", &rotating);
        ImGui::Text("Point light");
        ImGui::SliderFloat3("Light Position", &lightPos.x, -5.0f, 5.0f);
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