#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    UseShader(shader);

    std::vector<Model> models;
    models.push_back
    ({
        LoadMeshFromOBJ("res/models/Lantern_01.obj"),
        LoadTextureFromFile("res/textures/lantern-diffuse.png"),
        LoadTextureFromFile("res/textures/lantern-normal.png"),
        LoadTextureFromFile("res/textures/lantern-occ-rough-metal.png"),
    });
    models.push_back
    ({
        LoadMeshFromOBJ("res/models/sofa_02.obj"),
        LoadTextureFromFile("res/textures/sofa-diffuse.png"),
        LoadTextureFromFile("res/textures/sofa-normal.png"),
        LoadTextureFromFile("res/textures/sofa-occ-rough-metal.png"),
    });
    std::vector<const char*> modelNames;
    for(auto& m : models)
        modelNames.push_back(m.mesh.name);

    Entity entity = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };

    // Load lightcube mesh
    Mesh lightMesh = GenerateCube();
    Shader lightShader = LoadShadersFromFiles("res/shaders/lightcube/lightcube.vert", "res/shaders/lightcube/lightcube.frag");

    // Used for debug axes view
    Mesh debugAxes = GenerateAxes();
    Shader debugShader = LoadShadersFromFiles("res/shaders/debug/debug.vert", "res/shaders/debug/debug.frag");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);
    UniformMat4(shader, "projection", projection);

    // Camera info
    bool shouldReset = false;
    Camera camera = { {0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -3.0f}, {0.0f, 1.0f, 0.0f}, 0.0f, 0.0f, true, 3.0f };

    // ImGui state
    bool rotating = false;
    bool axes = false;
    int currentModel = 0;

    // Point light info
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    while(!glfwWindowShouldClose(display.window))
    {
        DeltaTimeCalc(display);
        if(!ImGui::GetIO().WantCaptureMouse)
            ProcessInput(display, camera, rotating, shouldReset);

        UseShader(shader);
        UniformVec3(shader, "pointLightPos", lightPos);
        UniformVec3(shader, "cameraPos", camera.position);

        // Transform matrix for mesh
        glm::mat4 model(1.0f);
        model = glm::translate(model, entity.position);
        model = glm::rotate(model, glm::radians(entity.rotation.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.z), {0.0f, 0.0f, 1.0f});
        model = glm::scale(model, entity.scale);
        UniformMat4(shader, "model", model);

        // Transform matrix for camera
        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
        UniformMat4(shader, "view", view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the mesh
        UniformInt(shader, "diffuseMap", models[currentModel].diffuse.index);
        UniformInt(shader, "normalMap", models[currentModel].normal.index);
        UniformInt(shader, "specularMap", models[currentModel].specular.index);
        Draw(models[currentModel].mesh);

        // Switch to light shader for lightcube rendering
        UseShader(lightShader);
        model = glm::mat4(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        UniformMat4(lightShader, "model", model);
        UniformMat4(lightShader, "view", view);
        UniformMat4(lightShader, "projection", projection);
        Draw(lightMesh);

        if(axes)
        {
            glDisable(GL_DEPTH_TEST);
            UseShader(debugShader);
            
            glm::mat4 debugModel(1.0f);
            debugModel = glm::scale(debugModel, { 10.0f, 10.0f, 10.0f });

            glm::mat4 MVP = projection * view * debugModel;
            UniformMat4(debugShader, "MVP", MVP);

            DrawLines(debugAxes);

            glEnable(GL_DEPTH_TEST);
        }

        // Start ImGui frame and render the window
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Main Controls");
        ImGui::Combo("Select Model", &currentModel, modelNames.data(), (int)modelNames.size());
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
        ImGui::Text("Camera Controls");
        if(ImGui::SliderFloat("Camera distance", &camera.cameraDistance, 1.0f, 10.0f))
        {
            camera.forward = glm::normalize(camera.forward) * camera.cameraDistance;
            camera.position = -camera.forward;
        }
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