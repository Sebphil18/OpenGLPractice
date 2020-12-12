#include <iostream>
#include <memory>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/noise.hpp"

#include "camera/Camera.h"
#include "globjects/UniformBuffer.h"
#include "modelstructure/ModelLoader.h"
#include "light/LightBundle.h"
#include "light/ShadowPointLight.h"
#include "window/Application.h"
#include "window/Window.h"
#include "scene/Scene.h"
#include "shader/DefaultShaders.h"

#define SEB_DEBUGMODE 1

#ifndef SEB_APP
#define SEB_APP 1

static Camera cam(800, 800);
static bool isFirstMouseMove = true;
static float lastX = 0, lastY = 0;
static double frameTime = 0;
static std::vector<Window> windows;

void startRenderLoop(int* width, int* height, GLFWwindow* window);
void setUpMatrixUbo(UniformBuffer& matrixUbo);
void setSkyboxTextures(SkyBox& skybox);

void update(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo);
void draw(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo, int* width, int* height);
void drawModels(std::vector<std::shared_ptr<Model>>& models, UniformBuffer& matrixUbo, ShaderProgram& program);

void printFrameData(double& timeAtLastFrame);
void resizeFrameBuffer(GLFWwindow* window, int width, int height);
void processMouse(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

int main() {

    Application app;
    Window win("Test-Window", 800, 800);

    int width = win.getWidth();
    int height = win.getHeight();

    win.setFramebufferSizeCallback(resizeFrameBuffer);
    win.setCursorPosCallback(processMouse);

    windows.push_back(win);

    startRenderLoop(&width, &height, win.getGlfwWindow());

    return 0;
}

void startRenderLoop(int* width, int* height, GLFWwindow* window) {

    Scene scene;
    DefaultShaders shaders;

    UniformBuffer matrixUbo(4, 4 * sizeof(glm::mat4));
    setUpMatrixUbo(matrixUbo);

    ShaderProgram& program = shaders.phongProgram;
    ShaderProgram& skyBoxProgram = shaders.skyboxProgram;

    program.bindUniformBuffer(matrixUbo.getSlot(), "matrices");
    skyBoxProgram.bindUniformBuffer(matrixUbo.getSlot(), "matrices");

    std::shared_ptr<ModelLoader> modelLoader = std::make_shared<ModelLoader>("rec/shapes/sphere/sphereobj.obj");
    modelLoader->getLastMesh().setMaterial({glm::vec4(1, 1, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(0.1, 0.1, 0, 1), 20});
    modelLoader->addTexture2D("rec/textures/BrickText.jpg", TextureType::diffuse, 0);
    modelLoader->addTexture2D("rec/textures/BrickOcc.jpg", TextureType::specular, 0);
    modelLoader->addTexture2D("rec/textures/BrickNormal.jpg", TextureType::normal, 0);
    modelLoader->setPosition(glm::vec3(-5, 0, -5));

    std::shared_ptr<ModelLoader> modelLoader2 = std::make_shared<ModelLoader>("rec/shapes/teapot/Teapot.obj");
    modelLoader2->setPosition(glm::vec3(10, 0, 0));
    modelLoader2->getMesh(0).setMaterial({glm::vec4(0.8, 0.8, 0, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0.2, 0.2, 0, 1), 40});

    std::shared_ptr<ModelLoader> modelLoader3 = std::make_shared<ModelLoader>("rec/shapes/plane/plane.obj");
    modelLoader3->getMesh(0).setMaterial({glm::vec4(0.9, 0.9, 0.9, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0.1, 0.1, 0.1, 1), 32});
    modelLoader3->setPosition(glm::vec3(0, -2, 0));
    modelLoader3->setSize(glm::vec3(3, 3, 3));

    std::vector<std::shared_ptr<Model>>& models = scene.models;
    models.push_back(modelLoader);
    models.push_back(modelLoader2);
    models.push_back(modelLoader3);
    
    setSkyboxTextures(scene.skyBox);

    ShadowLightBundle& shadowLights = scene.shadowLights;    
    shadowLights.enablePointLight(program);
    shadowLights.enableDirLight(program);
    shadowLights.pointLight.setPosition(glm::vec3(0, 8, 0));

    unsigned long frame = 0;
    double timeAtLastFrame = 0;

    windows[0].focus();
    while (!glfwWindowShouldClose(window)) {

        update(scene, shaders, matrixUbo);
        draw(scene, shaders, matrixUbo, width, height);

        glfwGetWindowSize(window, width, height);
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        printFrameData(timeAtLastFrame);
        frame++;
    }

}

void setUpMatrixUbo(UniformBuffer& matrixUbo) {
    matrixUbo.setElementType(0, UniformType::matrix4);
    matrixUbo.setElementType(1, UniformType::matrix4);
    matrixUbo.setElementType(2, UniformType::matrix4);
    matrixUbo.setElementType(3, UniformType::matrix4);
}

void setSkyboxTextures(SkyBox& skybox) {
    std::string paths[] = {
        "rec/skyboxes/right.jpg",
        "rec/skyboxes/left.jpg",
        "rec/skyboxes/top.jpg",
        "rec/skyboxes/bottom.jpg",
        "rec/skyboxes/front.jpg",
        "rec/skyboxes/back.jpg",
    };
    skybox.loadTextures(paths);
}

void update(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo) {
    matrixUbo.bind();
    matrixUbo.setElementData(0, glm::value_ptr(cam.getProjectionMatrix()));
    matrixUbo.setElementData(1, glm::value_ptr(cam.getViewMatrix()));
    matrixUbo.unbind();

    shaders.phongProgram.setUniformVec3f("viewPos", cam.getPosition());

    scene.lights.update(shaders.phongProgram);
    scene.shadowLights.update(scene.models, shaders.dirShadowProgram, shaders.pointShadowProgram, shaders.phongProgram);
}

void draw(Scene& scene, DefaultShaders& shaders, UniformBuffer& matrixUbo, int* width, int* height) {
    const std::vector<std::shared_ptr<Model>>& models = scene.models;

    glClearColor(0.01, 0.01, 0.01, 1.0);
    windows[0].clear();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, *width, *height);
    glEnable(GL_CULL_FACE);

    drawModels(scene.models, matrixUbo, shaders.phongProgram);

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

void printFrameData(double& timeAtLastFrame) {
    double currentTime = glfwGetTime();
    frameTime = currentTime - timeAtLastFrame;
    timeAtLastFrame = currentTime;
    float fps = 1 / frameTime;

    std::cout << "INFO::FRAME::frameTime: " << frameTime << std::endl;
    std::cout << "INFO::FRAME::framerate: " << (int)fps << std::endl;
    std::cout << std::endl;
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

void processMouse(GLFWwindow* window, double xpos, double ypos) {

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
#endif