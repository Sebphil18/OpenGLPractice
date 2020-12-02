# OpenGlPractice Repo - Pages

This little repository should document the process of learning OpenGL with GLFW (C++). The result of this learning process will be a tiny api for drawing scenes to a window with less effort.
The implementation of a new feature is outsourced to a new branch, which is merged with the main branch after the implementation was completed.

## Model structure
This repo contains several objects for storing, drawing and loading models.
### Model
A model is a drawable object which consists of multiple meshes. It contains informations about the transformations (e.g. position, size, rotation) and a map, which stores loaded textures. To create a empty model the default constructor should be called. To add vertex-data to it a new Mesh has to be added with `model.addMesh(const Mesh& mesh)` (for a already existing one) or `model.pushbackNewMesh()`. Meshes are stored in a vector. To get a reference to a specific mesh in that vector `model.getMesh(std::size_t index)` can be used. For receiving the last mesh `model.getLastMesh()` should be used. Now the mesh can be filled with new vertex data. To add a texture to the model use `model.addTexture2D(const char* filePath, TextureType textureType, std::size_t meshIndex)`. The following code snippet should demonstrate how to create and draw a model: <br>
```C++
Model model;
model.pushbackNewMesh();

Mesh& lastMesh = model.getLastMesh();
lastMesh.setData(vertices, indices);

model.addTexture2D("rec/textures/brickwall.jpg", TextureType::diffuse, 0);
model.draw(shaderProgram);
```
### ModelLoader
With the ModelLoader a model-file (e.g. \*.obj, \*.fbx) can be loaded into a model object. This class inherits the Model class. The following code should demonstrate how to load a simple \*.obj file:
```C++
ModelLoader loader("rec/shapes/sphere/sphere.obj");
loader.draw(shaderProgram);
```
### Mesh
The Mesh class represents the smallest unit which can be drawn. It contains a list of vertices, indices and already loaded textures. For example, a mesh is not responsible for loading a texture from a file. As so it is recommended to use the Model class, which organizes the meshes used by an object. The following code demonstrates the use of a mesh:
```C++
Mesh mesh();
mesh.setData(vertices, indices, textures);
mesh.draw(shaderProgram);
```
#### Vertex
A Vertex is a structure which contains informations about one vertex of a triangle.
The following code describes a Vertex:
```C++
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};
```
##### VertexConverter
This class can be used for converting an array of floats to a list of vertices. This converter expects the following format for the array: <br>
```C++
float cube_vertices[] = {
        // Position         Normal    TexCoord  Tangent     Bitangent
        -1.0, -1.0,  1.0,   0, 0, 0,    1, 0,   0, 0, 0,    0, 0, 0,
         1.0, -1.0,  1.0,   0, 0, 0,    0, 1,   0, 0, 0,    0, 0, 0,
         1.0,  1.0,  1.0,   0, 0, 0,    -1, 0,  0, 0, 0,    0, 0, 0,
        -1.0,  1.0,  1.0,   0, 0, 0,    0, -1,  0, 0, 0,    0, 0, 0,

        -1.0, -1.0, -1.0,   0, 0, 0,    1, 0,   0, 0, 0,    0, 0, 0,
         1.0, -1.0, -1.0,   0, 0, 0,    0, 1,   0, 0, 0,    0, 0, 0,
         1.0,  1.0, -1.0,   0, 0, 0,    -1, 0,  0, 0, 0,    0, 0, 0,
        -1.0,  1.0, -1.0,   0, 0, 0,    0, -1,  0, 0, 0,    0, 0, 0
};
```
This array can be converted into a list of vertices:
```C++
std::vector<Vertex> vertices = VertexConverter::floatArrToVertexVec(cube_vertices, sizeof(cube_vertices));
```
#### TextureContainer
The TextureContainer is a structure that can be described as:
```C++
struct TextureContainer {
	unsigned int id;
	TextureType type;
};
```
'id' is the OpenGL handle for that texture. 'type' is the type that specifies the usage of that texture.
##### TextureType
A TextureType is used to describe for what a specific texture is used for. Available types are: `TextureType::diffuse, TextureType::specular, TextureType::ambient, TextureType::normal`. For example, a texture that will be used as a normal map has to be TextureType::normal.
#### Material
Every Mesh contains one material. This material can be described as follows:
```C++
struct Material {
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	glm::vec4 ambientColor;
	float shininess;
};
```
The material can be set with `mesh.setMaterial(material)`
## ShaderProgram
A ShaderProgram represents a collection of vertex-, geometry- (optional) and fragmentshader. It is used to quickly create and use shader. The following code snippet demonstrates the usage of such a shader-program:
```C++
ShaderProgram program(
        "src/shader/vertex/VertexPhong.glsl", 
        "src/shader/geometry/GeoPhong.glsl", 
        "src/shader/fragment/FragmentPhong.glsl");
program.setUniformVec3("viewPos", glm::vec3(1, 1, 0));
model.draw(program); // draws a model using the shaders contained in 'program'
```
Every drawable object (e.g. Mesh, Model & Modelloader) has a draw function which expects a ShaderProgram, which will be used to render that specific object.
This class also provides basic functions for setting a uniform of a shader. For example, to set a uniform of type 'vec3' use `program.setUniformVec3f(const std::string& name, glm::vec3 vec)`
## UniformBuffer
This class represents a UniformBufferObject from OpenGL. To create a UniformBuffer you have to specify the amount of uniforms you want to store as well as the total length the uniformbuffer will have. The constructor `UniformBuffer uniformBuffer(unsigned int length, std::size_t size)` should be used. To set the data of an UniformBuffer, UniformBufferElements are used. A UniformBufferElement can be described with the following code:
```C++
struct UniformBufferElement {
	unsigned int size;
	unsigned int offset;
	const void* data;
};
```
Before you should set the data, use `uniformBuffer.setElementType(std::size_t elementIndex, UniformType type)`. A UniformType specifies the datatype for that element so the right amount of memory can be reserved for it. Valid UniformTypes are: `UniformType::integer, UniformType::decimal, UniformType::vec2D, UniformType::vec3D, UniformType::vec4D, UniformType::matrix4`. After the type was set, the element can be filled with data using `ubo.setElementData(std::size_t elementIndex, const void* data)`. For every operation involving setting data of an element the UniformBuffer has to be bound with `uniformBuffer.bind()`. To bind an UniformBuffer to a ShaderProgram use `program.bindUniformBuffer(unsigned int slot, const char* nameOfUniform)`. To determine the slot of an UniformBuffer use `uniformBuffer.getSlot()`. The following example demonstrates the full usage of an UniformBuffer: <br>
```C++
UniformBuffer ubo(4, 4 * sizeof(glm::mat4));
ubo.setElementType(0, UniformType::matrix4);
ubo.setElementType(1, UniformType::matrix4);
ubo.setElementType(2, UniformType::matrix4);
ubo.setElementType(3, UniformType::matrix4);

program.bindUniformBuffer(ubo.getSlot(), "matrices");
...
while(windowIsOpened) {
	...
	ubo.bind();
        ubo.setElementData(0, glm::value_ptr(cam.getProjectionMatrix()));
        ubo.setElementData(1, glm::value_ptr(cam.getViewMatrix()));
        ubo.unbind();
	...
	for(Model* model : models) {
	    ubo.bind();
            ubo.setElementData(2, glm::value_ptr(model->getWorldMat()));
            ubo.setElementData(3, glm::value_ptr(model->getNormalMat()));
            ubo.unbind();
            model->draw(program);
	}
	...
}
...
```
