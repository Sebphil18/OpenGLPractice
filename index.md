# OpenGlPractice Repo - Pages

This little repository should document the process of learning OpenGL with GLFW (C++). The result of this learning process will be a tiny api for drawing scenes to a window with less effort.
The implementation of a new feature is outsourced to a new branch, which is merged with the main branch after the implementation was completed.

## Model structure
This repo contains several objects for storing, drawing and loading models.
### Model
A model is a drawable object which consists of multiple meshes. It contains informations about the transformations (e.g. position, size, rotation) and a map, which stores loaded textures. To create a empty model the default constructor should be called. To add vertex-data to it a new Mesh has to be added with `model.addMesh(const Mesh& mesh)` (for a already existing one) or `model.pushbackNewMesh()`. Meshes are stored in a vector. To get a reference to a specific mesh in that vector `model.getMesh(size_t index)` can be used. For receiving the last mesh `model.getLastMesh()` should be used. Now the mesh can be filled with new vertex data. To add a texture to the model use `model.addTexture2D(const char* filePath, TextureType textureType, std::size_t meshIndex)`. The following code snipped should demonstrate how to create and draw a model: <br>
```c++
Model model;
model.pushbackNewMesh();

Mesh& lastMesh = model.getLastMesh();
mesh.setData(vertices, indices);

model.addTexture2D("rec/textures/brickwall.jpg", TextureType::diffuse, 0);
model.draw(shaderProgram);
```
### ModelLoader
With a ModelLoader a model-file (e.g. \*.obj, \*.fbx) can be loaded into a model object. This class inherits the Model class. The following code should demonstrate how to load a simple \*.obj file:
```C++
ModelLoader loader("rec/shapes/sphere/sphere.obj");
loader.draw(shaderProgram);
```
### Mesh
The Mesh class represents the smallest unit which can be drawn. It contains a list of vertices, indices and already loaded textures. For example, a mesh is not responsible for loading a texture from a file. As so it is recommended to use the Model class, which organizes the meshes used by a object. The following code demonstrates the use of a mesh:
```C++
Mesh mesh();
mesh.setData(vertices, indices, textures);
mesh.draw(shaderProgram);
```
#### Vertex
A Vertex is a structure which contains informations about a vertex of a triangle.
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
        // front
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
## ShaderProgram
A ShaderProgram represents a collection of vertex-, geometry- (optional) and fragmentshader. It is used to quickly create and use shaders. The following code snippet demonsrtates the usage of such a shader-program:
```C++
ShaderProgram program(
        "src/shader/vertex/VertexPhong.glsl", 
        "src/shader/geometry/GeoPhong.glsl", 
        "src/shader/fragment/FragmentPhong.glsl");
program.setUniformVec3("viewPos", glm::vec3(1, 1, 0));
model.draw(program); // draws a model using the shaders contained in 'program'
```
