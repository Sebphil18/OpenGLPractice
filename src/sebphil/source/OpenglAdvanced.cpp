#include <iostream>
#include <memory>
#include <vector>
#include <random>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/noise.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "camera/Camera.h"
#include "globjects/UniformBuffer.h"
#include "modelstructure/ModelLoader.h"
#include "terrainmodel/TerrainModel.h"
#include "window/Application.h"
#include "window/Window.h"
#include "scene/Scene.h"
#include "shader/DefaultShaders.h"
#include "imgui/ImGuiInterface.h"
#include "windowcontroller/TerrainSettingsWindow.h"
#include "windowcontroller/HydraulicErosionWindow.h"
#include "heightmap/SpecialEffect.h"
#include "erosion/HydraulicErosion.h"

static Camera cam(1200, 800);
static bool isFirstMouseMove = true;
static float lastX = 0, lastY = 0;
static double frameTime = 0;

static std::vector<Window> windows;

static void setWindowCallbacks(Window& window);
static void startRenderLoop(int* width, int* height, GLFWwindow* window);

static void setUpMatrixUbo(UniformBuffer& matrixUbo);
static std::shared_ptr<TerrainModel> createTerrain();
static std::shared_ptr<ModelLoader> createPreviewSphere();
static void setSkyboxTextures(SkyBox& skybox);
static void setUpLights(Scene& scene);

static void update(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo);
static void draw(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo, int* width, int* height);
static void drawModels(std::vector<std::shared_ptr<Model>>& models, UniformBuffer& matrixUbo, ShaderProgram& program);
static void printFrameData(double& timeAtLastFrame);

static void resizeFrameBuffer(GLFWwindow* window, int width, int height);
static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
static void processInput(GLFWwindow* window);
static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

// TODO: refactor TerrainModel
int main() {
    Application app;
    Window win("Terrain-Generation Demo", 1200, 800);

    setWindowCallbacks(win);
    windows.push_back(win);

    int width = win.getWidth();
    int height = win.getHeight();
    startRenderLoop(&width, &height, win.getGlfwWindow());

    return 0;
}

void setWindowCallbacks(Window& window) {
    window.setFramebufferSizeCallback(resizeFrameBuffer);
    window.setCursorPosCallback(onMouseMove);
    window.setKeyCallback(onKeyPressed);
}

// TODO: too long!
void startRenderLoop(int* width, int* height, GLFWwindow* window) {

    Scene scene;
    DefaultShaders shaders;
    
    ImGuiInterface imgui(window);

    UniformBuffer matrixUbo(4, 4 * sizeof(glm::mat4));
    setUpMatrixUbo(matrixUbo);
    
    ShaderProgram landscapeProgram(
        "src/sebphil/shader/vertex/VertexLandscape.glsl",
        "src/sebphil/shader/geometry/GeoLandscape.glsl",
        "src/sebphil/shader/fragment/FragmentLandscape.glsl");
    shaders.standardProgram = landscapeProgram;

    ShaderProgram& program = shaders.standardProgram;
    ShaderProgram& skyBoxProgram = shaders.skyboxProgram;

    program.bindUniformBuffer(matrixUbo.getSlot(), "matrices");
    skyBoxProgram.bindUniformBuffer(matrixUbo.getSlot(), "matrices");

    std::shared_ptr<ModelLoader> sphere = createPreviewSphere();
    std::shared_ptr<TerrainModel> terrain = createTerrain();

    std::vector<std::shared_ptr<Model>>& models = scene.models;
    models.push_back(sphere);
    models.push_back(terrain);

    setSkyboxTextures(scene.skyBox);
    setUpLights(scene);

    HydraulicErosionWindow erosionSettings(terrain);
    TerrainSettingsWindow terrainSettings(terrain);

    float parallaxStrength = 0.08;
    int minDepthLayers = 20;
    int maxDepthLayers = 40;

    float layerSlope1 = 1;
    float layerLevel1 = 0;
    float blend1 = 0.5;

    float textureScaling = 0.4;

    float testVec[4] = { 1, 1, 1, 1 };
    float minShadowBias = 0.005;

    // TODO: shaderwindow
    program.setUniform1f("parallaxStrength", parallaxStrength);

    double timeAtLastFrame = 0;
    windows[0].focus();

    while (!glfwWindowShouldClose(window)) {

        update(scene, shaders, matrixUbo);
        draw(scene, shaders, matrixUbo, width, height);

        imgui.newFrame();
        terrainSettings.draw();
        erosionSettings.draw();

        ImGui::Begin("shader-settings");

        if (ImGui::SliderFloat("parallax-strength", &parallaxStrength, 0, 1)) {
            program.setUniform1f("parallaxStrength", parallaxStrength);
        }

        if (ImGui::SliderInt("min depth-layers", &minDepthLayers, 0, 80)) {
            program.setUniform1i("minDepthLayers", minDepthLayers);
        }

        if (ImGui::SliderInt("max depth-layers", &maxDepthLayers, 0, 100)) {
            program.setUniform1i("maxDepthLayers", maxDepthLayers);
        }

        if (ImGui::SliderFloat("layerSlope1", &layerSlope1, 0.01, 20)) {
            program.setUniform1f("layers[0].slope", layerSlope1);
        }

        if (ImGui::SliderFloat("layerLevel1", &layerLevel1, -30, 50)) {
            program.setUniform1f("layers[0].maxLevel", layerLevel1);
        }

        if (ImGui::SliderFloat("blend1", &blend1, 0.01, 1)) {
            program.setUniform1f("layers[0].blend", blend1);
        }

        if (ImGui::SliderFloat("texture-scaling", &textureScaling, 0, 1)) {
            program.setUniform1f("textureScaling", textureScaling);
        }

        if (ImGui::SliderFloat("minimal shadow-bias", &minShadowBias, 0, 0.1)) {
            program.setUniform1f("minShadowBias", minShadowBias);
        }

        ImGui::End();

        imgui.render();

        glfwGetWindowSize(window, width, height);
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        printFrameData(timeAtLastFrame);
    }

}

void setUpMatrixUbo(UniformBuffer& matrixUbo) {
    matrixUbo.setElementType(0, UniformType::matrix4);
    matrixUbo.setElementType(1, UniformType::matrix4);
    matrixUbo.setElementType(2, UniformType::matrix4);
    matrixUbo.setElementType(3, UniformType::matrix4);
}

std::shared_ptr<ModelLoader> createPreviewSphere() {
    std::shared_ptr<ModelLoader> sphere = std::make_shared<ModelLoader>("rec/shapes/sphere/sphereobj.obj");
    sphere->setPosition(glm::vec3(0, 2, 0));
    sphere->getLastMesh().setMaterial({ glm::vec4(1, 1, 0, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0.1, 0.1, 0, 1), 3 });
    /*sphere->addTexture2D("rec/textures/BrickDiff2.jpg", TextureType::diffuse, 0);
    sphere->addTexture2D("rec/textures/BrickOcc.jpg", TextureType::specular, 0);
    sphere->addTexture2D("rec/textures/BrickOcc.jpg", TextureType::ambient, 0);
    sphere->addTexture2D("rec/textures/BrickNormal2.jpg", TextureType::normal, 0);
    sphere->addTexture2D("rec/textures/BrickDisplacement.jpg", TextureType::depth, 0);*/

    sphere->addTexture2D("rec/textures/rock2/RockDiff.png", TextureType::diffuse, 0);
    sphere->addTexture2D("rec/textures/rock2/RockRough.png", TextureType::specular, 0);
    sphere->addTexture2D("rec/textures/rock2/RockOcc.png", TextureType::ambient, 0);
    sphere->addTexture2D("rec/textures/rock2/RockNormal.png", TextureType::normal, 0);
    sphere->addTexture2D("rec/textures/rock2/RockHeight.png", TextureType::depth, 0);

    /*sphere->addTexture2D("rec/textures/sand2/SandDiff2.png", TextureType::diffuse, 0);
    sphere->addTexture2D("rec/textures/sand2/SandRough2.png", TextureType::specular, 0);
    sphere->addTexture2D("rec/textures/sand2/SandOcc2.png", TextureType::ambient, 0);
    sphere->addTexture2D("rec/textures/sand2/SandNormal2.png", TextureType::normal, 0);
    sphere->addTexture2D("rec/textures/sand2/SandHeight2.png", TextureType::depth, 0);*/

    /*sphere->addTexture2D("rec/textures/sand/SandDiff2.png", TextureType::diffuse, 0);
    sphere->addTexture2D("rec/textures/rock2/RockDiff.png", TextureType::diffuse, 0);
    sphere->addTexture2D("rec/textures/sand/SandRough2.png", TextureType::specular, 0);
    sphere->addTexture2D("rec/textures/sand/SandOcc2.png", TextureType::ambient, 0);
    sphere->addTexture2D("rec/textures/sand/SandNormal2.png", TextureType::normal, 0);
    sphere->addTexture2D("rec/textures/rock2/RockNormal.png", TextureType::normal, 0);
    sphere->addTexture2D("rec/textures/sand/SandHeight2.png", TextureType::depth, 0);
    sphere->addTexture2D("rec/textures/rock2/RockHeight.png", TextureType::depth, 0);*/

    return sphere;
}

std::shared_ptr<TerrainModel> createTerrain() {
    std::shared_ptr<TerrainModel> terrain = std::make_shared<TerrainModel>();
    terrain->getLastMesh().setMaterial({ glm::vec4(0.8, 1, 0.2, 1), glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), 1 });

    /*terrain->addTexture2D("rec/textures/sand/SandDiff2.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/rock2/RockDiff.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/sand/SandRough2.png", TextureType::specular, 0);
    terrain->addTexture2D("rec/textures/sand/SandOcc2.png", TextureType::ambient, 0);
    terrain->addTexture2D("rec/textures/sand/SandNormal2.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/rock2/RockNormal.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/sand/SandHeight2.png", TextureType::depth, 0);
    terrain->addTexture2D("rec/textures/rock2/RockHeight.png", TextureType::depth, 0);*/

    /*terrain->addTexture2D("rec/textures/sand/SandDiff2.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/canyon/RockDiff.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/sand/SandRough2.png", TextureType::specular, 0);
    terrain->addTexture2D("rec/textures/sand/SandOcc2.png", TextureType::ambient, 0);
    terrain->addTexture2D("rec/textures/sand/SandNormal2.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/canyon/RockNormal.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/sand/SandHeight2.png", TextureType::depth, 0);
    terrain->addTexture2D("rec/textures/canyon/RockHeight.png", TextureType::depth, 0);*/

    /*terrain->addTexture2D("rec/textures/snow/SnowDiff.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/snow/SnowRockDiff.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/snow/SnowRough.png", TextureType::specular, 0);
    terrain->addTexture2D("rec/textures/snow/SnowRough.png", TextureType::ambient, 0);
    terrain->addTexture2D("rec/textures/snow/SnowNormal.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/snow/SnowRockNormal.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/snow/SnowHeight.png", TextureType::depth, 0);
    terrain->addTexture2D("rec/textures/snow/SnowRockHeight.png", TextureType::depth, 0);*/

    terrain->addTexture2D("rec/textures/moss/MossDiff.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/rock2/RockDiff.png", TextureType::diffuse, 0);
    terrain->addTexture2D("rec/textures/moss/MossRough.png", TextureType::specular, 0);
    terrain->addTexture2D("rec/textures/moss/MossOcc.png", TextureType::ambient, 0);
    terrain->addTexture2D("rec/textures/moss/MossNormal.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/rock2/RockNormal.png", TextureType::normal, 0);
    terrain->addTexture2D("rec/textures/moss/MossHeight.png", TextureType::depth, 0);
    terrain->addTexture2D("rec/textures/rock2/RockHeight.png", TextureType::depth, 0);

    return terrain;
}

void setSkyboxTextures(SkyBox& skybox) {
    std::string paths[] = {
        "rec/skyboxes/clouds/right.png",
        "rec/skyboxes/clouds/left.png",
        "rec/skyboxes/clouds/top.png",
        "rec/skyboxes/clouds/bottom.png",
        "rec/skyboxes/clouds/front.png",
        "rec/skyboxes/clouds/back.png",
    };
    skybox.loadTextures(paths);
}

void setUpLights(Scene& scene) {

    LightBundle& lights = scene.lights;
    DirectionLight dirLight;
    dirLight.setDirection(glm::vec3(0.4, -1, 0.3));
    lights.dirLights.push_back(dirLight);

}

void update(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo) {

    matrixUbo.bind();
    matrixUbo.setElementData(0, glm::value_ptr(cam.getProjectionMatrix()));
    matrixUbo.setElementData(1, glm::value_ptr(cam.getViewMatrix()));
    matrixUbo.unbind();

    shaders.standardProgram.setUniformVec3f("viewPos", cam.getPosition());

    scene.lights.update(shaders.standardProgram);

    scene.shadowLights.dirLight.setPosition(cam.getPosition());
    scene.shadowLights.update(scene.models, shaders.dirShadowProgram, shaders.pointShadowProgram, shaders.standardProgram);

}

void draw(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo, int* width, int* height) {
    const std::vector<std::shared_ptr<Model>>& models = scene.models;

    glClearColor(0.01, 0.01, 0.01, 1.0);
    windows[0].clear();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, *width, *height);

    glEnable(GL_CULL_FACE);
    drawModels(scene.models, matrixUbo, shaders.standardProgram);
    glDisable(GL_CULL_FACE);

    scene.skyBox.draw(shaders.skyboxProgram);
}

void drawModels(std::vector<std::shared_ptr<Model>>& models, UniformBuffer& matrixUbo, ShaderProgram& program) {
    for (size_t i = 0; i < models.size(); i++) {
        matrixUbo.bind();
        matrixUbo.setElementData(2, glm::value_ptr(models[i]->getWorldMat()));
        matrixUbo.setElementData(3, glm::value_ptr(models[i]->getNormalMat()));
        matrixUbo.unbind();

        models[i]->draw(program);
    }
}

// TODO: misleading name! this also sets the frameTime -> when left out the camera won't move because delta frametime becomes 0!
void printFrameData(double& timeAtLastFrame) {
    double currentTime = glfwGetTime();
    frameTime = currentTime - timeAtLastFrame;
    timeAtLastFrame = currentTime;
    float fps = 1 / frameTime;

    std::cout << "INFO::FRAME::frametime: " << frameTime << "s  \n";
    std::cout << "INFO::FRAME::framerate: " << (int)fps << "fps \n";
    std::cout << "\n";
}

void resizeFrameBuffer(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    cam.setWidth(width);
    cam.setHeight(height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.moveForward(frameTime);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.moveBackwards(frameTime);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.moveLeft(frameTime);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.moveRight(frameTime);
    }
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {

    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
        return;

    if (isFirstMouseMove) {
        isFirstMouseMove = false;
        lastX = xpos;
        lastY = ypos;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    cam.moveYawAndPitch(xoffset, yoffset);
}

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
        if (inputMode == GLFW_CURSOR_DISABLED)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        isFirstMouseMove = true;
    }

}