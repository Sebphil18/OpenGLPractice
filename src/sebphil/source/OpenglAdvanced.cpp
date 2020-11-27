#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/noise.hpp"

#include "globjects/VertexBuffer.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/VertexArray.h"
#include "camera/Camera.h"
#include "shader/ShaderProgram.h"
#include "globjects/IndexBuffer.h"
#include "globjects/UniformBuffer.h"
#include "globjects/FrameBuffer.h"
#include "globjects/RenderBuffer.h"
#include "structure/Mesh.h"
#include "structure/Model.h"
#include "structure/ModelLoader.h"
#include "light/DirectionLight.h"
#include "light/PointLight.h"
#include "light/LightBundle.h"
#include "skybox/SkyBox.h"
#include "light/ShadowDirLight.h"
#include "light/ShadowLightBundle.h"
#include "light/ShadowPointLight.h"
#include "structure/VertexConverter.h"

#include "stbi/stb_Image.h"

#include <vector>

#define DEBUG_MODE 1

void setUpWindow();
void setUpListener(GLFWwindow* window);
bool loadGlad();
void setUpOpenGL();
void startRenderLoop(int* width, int* height, GLFWwindow* window);
GLFWwindow* createWindow(unsigned int width, unsigned int height, const char* title);
bool isWindowValid(GLFWwindow* window);

void printFrameData(double& timeAtLastFrame);

void resizeFrameBuffer(GLFWwindow* window, int width, int height);
void processMouse(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

Camera cam(800, 800);
bool isFirstMouseMove = true;
float lastX = 0, lastY = 0;
double frameTime = 0;

float dynamicVar = 0;

Model terrain;
std::vector<unsigned int> terrIndices;


    // TODO[2020-11-13]: Mesh (hide vertices and indices)
    //    -> add Buffer Class (first implement functions in mesh directly!)
    //        -> add function for reallocating buffer (e.g. add vertice)
    //        -> add function for changing data (subData)
    //        -> add function for removing data (e.g. remove vertice)
    //    -> use Buffer Class in Mesh
    // TODO[2020-11-13]: Add Support for Normal-Maps
    // TODO: Container for setting matrices (with uniformBuffer)
    // TODO: Container for Cameras; switch between multiple cameras
    // TODO: Container for Models & Meshes (ShapeBundle)


int main() {

    int width, height;

    if (!glfwInit())
        std::cout << "ERROR::GLFW::Could not be initialized!" << std::endl;

    setUpWindow();

    GLFWwindow* window = createWindow(800, 800, "OpenGL Advanced");
    
    if (window == nullptr) {
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    setUpListener(window);

    bool gladIsLoaded = loadGlad();
    if (!gladIsLoaded) {
        glfwTerminate();
        return 3;
    }

    setUpOpenGL();

    startRenderLoop(&width, &height, window);

    glfwTerminate();

    return 1;
}

/*
* Sets up all WindowHints for a OpenGL Context.
* This Function sets the OpenGL version (4.6) and enables Multisampling (Samples: 4).
*/
void setUpWindow() {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

}

/*
 Generates a new GLFWwindow* and checks if it was generated successfully.
 This function returns null, when the window was not created successfully and
 makes the current window the current context.
*/
GLFWwindow* createWindow(unsigned int width, unsigned int height, const char* title) {

    GLFWwindow* window = glfwCreateWindow(800, 800, title, NULL, NULL);

    if (!isWindowValid(window))
        return nullptr;

    return window;
}

/*
 Validates a window if it was created successfully.
 This function returns true when the widow was created successfully and
 false if the window could not be created.
*/
bool isWindowValid(GLFWwindow* window) {

    if (window == NULL) {
        std::cout << "ERROR::GLFW::Window could not be created." << std::endl;
        return false;
    }

    return true;
}

/*
 This function calculates and prints the frameTime and frames per seconds to the console.
*/
void printFrameData(double& timeAtLastFrame) {

    double currentTime = glfwGetTime();
    frameTime = currentTime - timeAtLastFrame;
    timeAtLastFrame = currentTime;
    float fps = 1 / frameTime;

    std::cout << "INFO::FRAME::frameTime: " << frameTime << std::endl;
    std::cout << "INFO::FRAME::framerate: " << (int) fps << std::endl;
    std::cout << std::endl;

}

/*
 This function sets the input-mode for the mouse and the callbacks for mouse movement 
 and resizing the window.
*/
void setUpListener(GLFWwindow* window) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, processMouse);
    glfwSetFramebufferSizeCallback(window, resizeFrameBuffer);

}

/*
 This function loads all necessary OpenGL bindings using glad.
 This function returns true when Glad could successfully load and false if not.
*/
bool loadGlad() {

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR::GLAD::Glad could not be initilized." << std::endl;
        return false;
    }

    return true;
}

/*
 This function makes all necessary OpenGL calls to enable features and to set
 functions. (example: BlendFunction)
*/
void setUpOpenGL() {

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);

}

/*
 This function starts the rendering-loop and blocks until this loop is exited.
 It also processes Keyboard-Input with each iteration.
*/
void startRenderLoop(int* width, int* height, GLFWwindow* window) {

    float cube_vertices[] = {
        // front
        // Position         Normal    TexCoord
        -1.0, -1.0,  1.0,   0, 0, 0,    1, 0,   0, 0, 0,    0, 0, 0,
         1.0, -1.0,  1.0,   0, 0, 0,    0, 1,   0, 0, 0,    0, 0, 0,
         1.0,  1.0,  1.0,   0, 0, 0,    -1, 0,   0, 0, 0,    0, 0, 0,
        -1.0,  1.0,  1.0,   0, 0, 0,    0, -1,   0, 0, 0,    0, 0, 0,

        // back
        -1.0, -1.0, -1.0,   0, 0, 0,    1, 0,   0, 0, 0,    0, 0, 0,
         1.0, -1.0, -1.0,   0, 0, 0,    0, 1,   0, 0, 0,    0, 0, 0,
         1.0,  1.0, -1.0,   0, 0, 0,    -1, 0,   0, 0, 0,    0, 0, 0,
        -1.0,  1.0, -1.0,   0, 0, 0,    0, -1,   0, 0, 0,    0, 0, 0
    };

    unsigned int cube_elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,

        // right
        1, 5, 6,
        6, 2, 1,

        // back
        7, 6, 5,
        5, 4, 7,

        // left
        4, 0, 3,
        3, 7, 4,

        // bottom
        4, 5, 1,
        1, 0, 4,

        // top
        3, 2, 6,
        6, 7, 3
    };

    float screenQuadVertices[] = {
        // positions  texCoords
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    ShaderProgram program(
        "src/sebphil/shader/vertex/VertexPhong.glsl", 
        "src/sebphil/shader/geometry/GeoPhong.glsl", 
        "src/sebphil/shader/fragment/FragmentPhong.glsl");

    UniformBuffer ubo(4, 4 * sizeof(glm::mat4));
    ubo.setElementType(0, UniformType::matrix4);
    ubo.setElementType(1, UniformType::matrix4);
    ubo.setElementType(2, UniformType::matrix4);
    ubo.setElementType(3, UniformType::matrix4);

    program.bindUniformBuffer(ubo.getSlot(), "matrices");

    std::vector<Vertex>  vertices = VertexConverter::floatArrToVertexVec(cube_vertices, sizeof(cube_vertices));
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < sizeof(cube_elements) / sizeof(unsigned int); i++) {
        indices.push_back(cube_elements[i]);
    }

    ModelLoader modelLoader("rec/shapes/sphere/sphereobj.obj");
    modelLoader.getLastMesh().setMaterial({glm::vec4(1, 1, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(0.1, 0.1, 0, 1), 20});
    modelLoader.addTexture2D("rec/textures/BrickText.jpg", TextureType::diffuse, 0);
    modelLoader.addTexture2D("rec/textures/BrickOcc.jpg", TextureType::specular, 0);
    modelLoader.addTexture2D("rec/textures/BrickNormal.jpg", TextureType::normal, 0);
    modelLoader.setPosition(glm::vec3(-5, 0, -5));

    modelLoader.getLastMesh().setData(vertices, indices);

    ModelLoader modelLoader2("rec/shapes/teapot/Teapot.obj");
    modelLoader2.setPosition(glm::vec3(10, 0, 0));
    modelLoader2.getMesh(0).setMaterial({glm::vec4(0.8, 0.8, 0, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0.2, 0.2, 0, 1), 40});

    ModelLoader modelLoader3("rec/shapes/plane/plane.obj");
    modelLoader3.getMesh(0).setMaterial({glm::vec4(0.9, 0.9, 0.9, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0.1, 0.1, 0.1, 1), 32});
    modelLoader3.setPosition(glm::vec3(0, -2, 0));
    modelLoader3.setSize(glm::vec3(3, 3, 3));
    modelLoader3.addTexture2D("rec/textures/StonesText.jpg", TextureType::diffuse, 0);
    modelLoader3.addTexture2D("rec/textures/StonesOcc.jpg", TextureType::specular, 0);
    modelLoader3.addTexture2D("rec/textures/StonesNormal.jpg", TextureType::normal, 0);

    std::vector<Model*> models;
    models.push_back(&modelLoader);
    models.push_back(&modelLoader2);
    models.push_back(&modelLoader3);

    ShaderProgram skyBoxProgram("src/sebphil/shader/vertex/VertexSkyBox.glsl", "src/sebphil/shader/fragment/FragmentSkyBox.glsl");
    skyBoxProgram.bindUniformBuffer(ubo.getSlot(), "matrices");
    skyBoxProgram.setUniform1i("skybox", 1);

    std::string paths[] = {
        "rec/skyboxes/right.jpg",
        "rec/skyboxes/left.jpg",
        "rec/skyboxes/top.jpg",
        "rec/skyboxes/bottom.jpg",
        "rec/skyboxes/front.jpg",
        "rec/skyboxes/back.jpg",
    };

    SkyBox box(paths);

    ShaderProgram reflectionProgram("src/sebphil/shader/vertex/VertexReflection.glsl", "src/sebphil/shader/fragment/FragmentReflection.glsl");
    reflectionProgram.bindUniformBuffer(ubo.getSlot(), "matrices");
    reflectionProgram.setUniform1i("skybox", 1);

    ShaderProgram refractionProgram("src/sebphil/shader/vertex/VertexRefraction.glsl", "src/sebphil/shader/fragment/FragmentRefraction.glsl");
    refractionProgram.bindUniformBuffer(ubo.getSlot(), "matrices");
    refractionProgram.setUniform1f("n1", 1);
    refractionProgram.setUniform1f("n2", 1.33);
    refractionProgram.setUniform1i("skybox", 1);
    
    ShaderProgram shadowProgram("src/sebphil/shader/vertex/VertexShadow.glsl", "src/sebphil/shader/fragment/FragmentShadow.glsl");

    LightBundle lightBundle;
    ShadowLightBundle shadowLightBundle;
    shadowLightBundle.enablePointLight(program);
    shadowLightBundle.enableDirLight(program);
    shadowLightBundle.pointLight.setPosition(glm::vec3(0, 8, 0));

    ShaderProgram pointShadowProgram(
        "src/sebphil/shader/vertex/VertexPointShadow.glsl", 
        "src/sebphil/shader/geometry/GeoPointShadow.glsl", 
        "src/sebphil/shader/fragment/FragmentPointShadow.glsl");

    terrain.pushbackNewMesh();
    Mesh& terrMesh = terrain.getLastMesh();

    uint32_t length = 20;
    for (uint32_t x = 0; x < length - 1; x++) {
        for (uint32_t z = 0; z < length - 1; z++) {
            terrIndices.push_back(z * length + x);
            terrIndices.push_back(z * length + x + 1);
            terrIndices.push_back((z + 1) * length + x);

            terrIndices.push_back(z * length + x + 1);
            terrIndices.push_back((z + 1) * length + x + 1);
            terrIndices.push_back((z + 1) * length + x);
        }
    }

    std::vector<Vertex> terrVertices;
    for (uint32_t x = 0; x < length; x++) {
        for (uint32_t z = 0; z < length; z++) {
            float y = glm::simplex(glm::vec3(x, 0, z));
            terrVertices.push_back({glm::vec3(x, y, z), glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) });
        }
    }
    terrMesh.setData(terrVertices, terrIndices);
    models.push_back(&terrain);

    unsigned long frame = 0;
    double timeAtLastFrame = 0;
    double frameTime = 0;

    //lightBundle.update(program);

    while (!glfwWindowShouldClose(window)) {

        printFrameData(timeAtLastFrame);

        glClearColor(0.01, 0.01, 0.01, 1.0);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        //DO RENDERING HERE
        //------------------------------------------------------------------

        ubo.bind();
        ubo.setElementData(0, glm::value_ptr(cam.getProjectionMatrix()));
        ubo.setElementData(1, glm::value_ptr(cam.getViewMatrix()));
        ubo.unbind();

        program.setUniformVec3f("viewPos", cam.getPosition());
        reflectionProgram.setUniformVec3f("viewPosition", cam.getPosition());
        refractionProgram.setUniformVec3f("viewPosition", cam.getPosition());

        float xDirection = frame * 0.01;
        //shadowLightBundle.pointLight.setPosition(glm::vec3(std::cos(xDirection) * 4, 6, std::sin(xDirection) * 4));

        lightBundle.update(program);
        shadowLightBundle.update(models, shadowProgram, pointShadowProgram, program);

        // draw - main framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, *width, *height);

        for (size_t i = 0; i < models.size(); i++) {

            ubo.bind();
            ubo.setElementData(2, glm::value_ptr(models[i]->getWorldMat()));
            ubo.setElementData(3, glm::value_ptr(models[i]->getNormalMat()));
            ubo.unbind();

            models[i]->draw(program);
        }

        box.draw(skyBoxProgram);

        //------------------------------------------------------------------

        glfwGetWindowSize(window, width, height);
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        frame++;
    }

}

void resizeFrameBuffer(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    cam.setWidth(width);
    cam.setHeight(height);
}

/*
 Processes Keyboard-Input (example: for Camera).
*/
void processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

        glfwSetWindowShouldClose(window, GLFW_TRUE);

    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        cam.moveForward(frameTime);

    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        cam.moveBackwards(frameTime);

    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        cam.moveLeft(frameTime);

    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        cam.moveRight(frameTime);
        
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        dynamicVar += 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        dynamicVar -= 0.1;
    } 
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {

        dynamicVar += 0.01;

        Mesh& terrMesh = terrain.getLastMesh();
        std::vector<Vertex> terrVertices;
        for (uint32_t x = 0; x < 20; x++) {
            for (uint32_t z = 0; z < 20; z++) {
                float offX = (x + dynamicVar * 2) * 0.4;
                float offZ = (z + dynamicVar * 2) * 0.4;
                float offY = dynamicVar;
                float y = glm::simplex(glm::vec3(offX, offY, offZ)) * 0.7;
                terrVertices.push_back({ glm::vec3(x, y, z), glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) });
            }
        }
        terrMesh.setData(terrVertices, terrIndices);

    }

}

/*
 Processes Mouse-Movement.
*/
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