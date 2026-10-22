/*
Práctica 8: Iluminación 2
*/
//Imagenes
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//Importador
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//Iluminacion
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;
Texture holocronTexture;
Texture FuegoAzulTexture;

Model Blackhawk_M;
Model roverbase;
Model brazobase;
Model brazouno;
Model brazodos;
Model pinza;
Model frontalmasz;
Model frontalmenosz;
Model mediomasz;
Model mediomenosz;
Model traseramasz;
Model traseramenosz;
Model llantafrontalmasz;
Model llantafrontalmenosz;
Model llantamediomasz;
Model llantamediomenosz;
Model llantatraseramasz;
Model llantatraseramenosz;
Model Avion_M;
Model Nave_M;


Skybox skybox;

//Materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//Luz direccional
DirectionalLight mainLight;
//Luces puntuales
static_assert(MAX_POINT_LIGHTS >= 5, "CommonValues.h debe permitir al menos 5 luces puntuales");
static_assert(MAX_SPOT_LIGHTS >= 5, "CommonValues.h debe permitir al menos 5 spotlights");
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLightsActivas[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Vertex shader
static const char* vShader = "shaders/shader_light.vert";

//Fragment shader
static const char* fShader = "shaders/shader_light.frag";


//Normales
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// back
		8, 9, 10,
		10, 11, 8,
		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
		// right
		4, 5, 6,
		6, 7, 4,
	};
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.05f,  0.76f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.24f,	0.76f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.24f,	0.98f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.05f,	0.98f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.02f,	0.02f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.24f,	0.02f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.24f,	0.26f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.02f,  0.26f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.28f,  0.26f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.48f,	0.26f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.48f,	0.5f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.28f,	0.5f,		0.0f,	0.0f,	1.0f,
		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.3f,  0.76f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.47f,	0.76f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.47f,	0.97f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.3f,	0.97f,		1.0f,	0.0f,	0.0f,
		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.04f,  0.27f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.22f,	0.27f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.22f,	0.49f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.04f,	0.49f,		0.0f,	1.0f,	0.0f,
		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.14f,  0.51f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.31f,	0.51f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.31f,	0.72f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.14f,	0.72f,		0.0f,	-1.0f,	0.0f,
	};
	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);
}

void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		// Frente
		0, 2, 1,
		2, 0, 3,
		// Lateral derecho
		4, 5, 6,
		6, 7, 4,
		// Parte posterior
		8, 9, 10,
		10, 11, 8,
		// Lateral izquierdo
		12, 13, 14,
		14, 15, 12,
		// Base inferior
		16, 18, 17,
		18, 16, 19,
		// Base superior
		20, 21, 22,
		22, 23, 20,
		// Esquinas superiores
		24, 25, 26,
		27, 28, 29,
		// Esquinas inferiores
		30, 31, 32,
		33, 34, 35,
		// Esquinas posteriores superiores
		36, 37, 38,
		39, 40, 41,
		// Esquinas posteriores inferiores
		42, 43, 44,
		45, 46, 47,
	};
	GLfloat holocron_vertices[] = {
		// Frente
		// X     Y     Z       S     T       NX    NY    NZ
		-0.5f, 0.0f, 0.5f,   0.06f, 0.76f,  0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, 0.5f,    0.06f, 0.96f,  0.0f, 0.0f, -1.0f,
		0.5f, 0.0f, 0.5f,    0.24f, 0.96f,  0.0f, 0.0f, -1.0f,
		0.0f, -0.5f, 0.5f,   0.24f, 0.76f,  0.0f, 0.0f, -1.0f,
		// Derecha
		0.5f, 0.0f, -0.5f,  0.3f, 0.76f,  -1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,   0.3f, 0.96f,  -1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f,   0.46f, 0.96f,  -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  0.46f, 0.76f,  -1.0f, 0.0f, 0.0f,
		// Atrás
		-0.5f, 0.0f, -0.5f, 0.54f, 0.76f,  0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, -0.5f,  0.54f, 0.96f,  0.0f, 0.0f, 1.0f,
		0.5f, 0.0f, -0.5f,  0.7f, 0.96f,  0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, -0.5f, 0.7f, 0.76f,  0.0f, 0.0f, 1.0f,
		// Izquierda
		-0.5f, 0.0f, 0.5f,  0.78f, 0.76f,  1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,  0.78f, 0.96f,  1.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f, 0.94f, 0.96f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.94f, 0.76f,  1.0f, 0.0f, 0.0f,
		// Inferior
		0.5f, -0.5f, 0.0f,  0.14f, 0.52f,  0.0f, 1.0f, 0.0f,
		0.0f, -0.5f, -0.5f, 0.14f, 0.72f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.3f, 0.72f,  0.0f, 1.0f, 0.0f,
		0.0f, -0.5f, 0.5f,  0.3f, 0.52f,  0.0f, 1.0f, 0.0f,
		// Superior
		0.5f, 0.5f, 0.0f,   0.53f, 0.04f,  0.0f, -1.0f, 0.0f,
		0.0f, 0.5f, -0.5f,  0.53f, 0.24f,  0.0f, -1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,  0.7f, 0.24f,  0.0f, -1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,   0.7f, 0.04f,  0.0f, -1.0f, 0.0f,
		// Frente superior derecho
		0.5f, 0.0f, 0.5f,   0.56f, 0.5f,  -0.577f, -0.577f, -0.577f,
		0.5f, 0.5f, 0.0f,   0.4f, 0.71f,  -0.577f, -0.577f, -0.577f,
		0.0f, 0.5f, 0.5f,   0.7f, 0.71f,  -0.577f, -0.577f, -0.577f,
		// Frente superior izquierdo
		-0.5f, 0.5f, 0.0f,  0.13f, 0.2f,  0.577f, -0.577f, -0.577f,
		-0.5f, 0.0f, 0.5f,  0.0f, 0.47f,  0.577f, -0.577f, -0.577f,
		0.0f, 0.5f, 0.5f,   0.25f, 0.47f,  0.577f, -0.577f, -0.577f,
		// Frente inferior derecho
		0.5f, -0.5f, 0.0f,  0.64f, 0.72f,  -0.577f, 0.577f, -0.577f,
		0.5f, 0.0f, 0.5f,   0.8f, 0.5f,  -0.577f, 0.577f, -0.577f,
		0.0f, -0.5f, 0.5f,  0.92f, 0.72f,  -0.577f, 0.577f, -0.577f,
		// Frente inferior izquierdo
		-0.5f, 0.0f, 0.5f,  0.38f, 0.22f,  0.577f, 0.577f, -0.577f,
		-0.5f, -0.5f, 0.0f, 0.25f, 0.47f,  0.577f, 0.577f, -0.577f,
		0.0f, -0.5f, 0.5f,  0.5f, 0.47f,  0.577f, 0.577f, -0.577f,
		// Atrás superior derecho
		0.5f, 0.5f, 0.0f,   0.49f, 0.46f,  -0.577f, -0.577f, 0.577f,
		0.5f, 0.0f, -0.5f,  0.61f, 0.26f,  -0.577f, -0.577f, 0.577f,
		0.0f, 0.5f, -0.5f,  0.71f, 0.46f,  -0.577f, -0.577f, 0.577f,
		// Atrás superior izquierdo
		-0.5f, 0.0f, -0.5f, 0.0f, 0.22f,  0.577f, -0.577f, 0.577f,
		-0.5f, 0.5f, 0.0f,  0.12f, 0.0f,  0.577f, -0.577f, 0.577f,
		0.0f, 0.5f, -0.5f,  0.25f, 0.22f,  0.577f, -0.577f, 0.577f,
		// Atrás inferior derecho
		0.5f, 0.0f, -0.5f,  0.38f, 0.29f,  -0.577f, 0.577f, 0.577f,
		0.5f, -0.5f, 0.0f,  0.51f, 0.03f,  -0.577f, 0.577f, 0.577f,
		0.0f, -0.5f, -0.5f, 0.24f, 0.03f,  -0.577f, 0.577f, 0.577f,
		// Atrás inferior izquierdo
		-0.5f, -0.5f, 0.0f, 0.72f, 0.28f,  0.577f, 0.577f, 0.577f,
		-0.5f, 0.0f, -0.5f, 0.84f, 0.54f,  0.577f, 0.577f, 0.577f,
		0.0f, -0.5f, -0.5f, 0.98f, 0.28f,  0.577f, 0.577f, 0.577f,
	};
	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
	meshListModel.push_back(holocron);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();
	//Shader del plasma
	Shader shaderPlasma;
	shaderPlasma.CreateFromString(
		R"GLSL(#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec2 coordenadaUV;
out vec2 UV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    UV = coordenadaUV;
    gl_Position = projection * view * model * vec4(posicion, 1.0);
}
)GLSL",
R"GLSL(#version 330 core
in vec2 UV;
out vec4 colorFinal;
uniform sampler2D texturaFuego;
void main()
{
    colorFinal = texture(texturaFuego, UV);
}
)GLSL");
	shaderPlasma.UseShader();
	GLint programaPlasma = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &programaPlasma);
	GLint estadoPlasma = GL_FALSE;
	if (programaPlasma != 0) glGetProgramiv(programaPlasma, GL_LINK_STATUS, &estadoPlasma);
	bool plasmaDisponible = (programaPlasma != 0 && estadoPlasma == GL_TRUE);
	GLint plasmaModel = plasmaDisponible ? (GLint)shaderPlasma.GetModelLocation() : -1;
	GLint plasmaView = plasmaDisponible ? (GLint)shaderPlasma.GetViewLocation() : -1;
	GLint plasmaProjection = plasmaDisponible ? (GLint)shaderPlasma.GetProjectionLocation() : -1;
	GLint plasmaTextura = plasmaDisponible ? glGetUniformLocation(programaPlasma, "texturaFuego") : -1;

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/LogosStarWars.jpeg");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/LogosStarWars.jpeg");
	holocronTexture.LoadTextureA();
	//Textura del plasma
	const char* rutaFuego = "Textures/fuegozul.tga";
	FILE* archivoFuego = nullptr;
	fopen_s(&archivoFuego, rutaFuego, "rb");
	if (!archivoFuego)
	{
		rutaFuego = "Textures/fuegoazul.tga";
		fopen_s(&archivoFuego, rutaFuego, "rb");
	}
	bool tieneTexturaFuego = (archivoFuego != nullptr);
	if (archivoFuego)
	{
		fclose(archivoFuego);
		FuegoAzulTexture = Texture(rutaFuego);
		FuegoAzulTexture.LoadTextureA();
	}

	roverbase = Model();
	roverbase.LoadModel("Models/roverbase.obj");
	brazobase = Model();
	brazobase.LoadModel("Models/brazobase.obj");
	brazouno = Model();
	brazouno.LoadModel("Models/brazouno.obj");
	brazodos = Model();
	brazodos.LoadModel("Models/brazodos.obj");
	pinza = Model();
	pinza.LoadModel("Models/pinza.obj");
	frontalmasz = Model();
	frontalmasz.LoadModel("Models/frontal+z.obj");
	frontalmenosz = Model();
	frontalmenosz.LoadModel("Models/frontal-z.obj");
	mediomasz = Model();
	mediomasz.LoadModel("Models/medio+z.obj");
	mediomenosz = Model();
	mediomenosz.LoadModel("Models/medio-z.obj");
	traseramasz = Model();
	traseramasz.LoadModel("Models/trasera+z.obj");
	traseramenosz = Model();
	traseramenosz.LoadModel("Models/trasera-z.obj");
	llantafrontalmasz = Model();
	llantafrontalmasz.LoadModel("Models/llantafrontal+z.obj");
	llantafrontalmenosz = Model();
	llantafrontalmenosz.LoadModel("Models/llantafrontal-z.obj");
	llantamediomasz = Model();
	llantamediomasz.LoadModel("Models/llantamedio+z.obj");
	llantamediomenosz = Model();
	llantamediomenosz.LoadModel("Models/llantamedio-z.obj");
	llantatraseramasz = Model();
	llantatraseramasz.LoadModel("Models/llantatrasera+z.obj");
	llantatraseramenosz = Model();
	llantatraseramenosz.LoadModel("Models/llantatrasera-z.obj");
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion_mordecai.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/naveespacial.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Luz direccional
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//Luces puntuales
	//Luz del Holocron
	const glm::vec3 HOLOCRON_POS = glm::vec3(-7.0f, 7.0f, 6.0f);
	const glm::vec3 LUZ_HOLOCRON_OFFSET = glm::vec3(0.0f, 1.3f, 0.0f);
	auto crearLuzHolocron = [](const glm::vec3& pos)
		{
			return PointLight(0.35f, 0.75f, 1.0f,
				0.0f, 3.0f,
				pos.x, pos.y, pos.z,
				1.0f, 0.7f, 1.8f);
		};
	pointLights[0] = crearLuzHolocron(HOLOCRON_POS + LUZ_HOLOCRON_OFFSET);

	//Luz de la pinza
	const glm::vec3 LUZ_PINZA_OFFSET = glm::vec3(0.0f, 1.0f, 0.0f);
	auto crearLuzPinza = [](const glm::vec3& pos)
		{
			return PointLight(0.0f, 1.0f, 0.0f,
				0.0f, 1.5f,
				pos.x, pos.y, pos.z,
				1.0f, 0.22f, 0.20f);
		};
	pointLights[1] = crearLuzPinza(glm::vec3(0.0f, 0.0f, 0.0f));

	//Luces de la nave
	auto crearLuzNave = [](const glm::vec3& pos)
		{
			return PointLight(0.85f, 0.95f, 1.0f,
				0.0f, 2.5f,
				pos.x, pos.y, pos.z,
				1.0f, 0.14f, 0.07f);
		};
	auto crearLuzMotor = [](const glm::vec3& pos)
		{
			return PointLight(0.12f, 0.50f, 1.0f,
				0.0f, 4.0f,
				pos.x, pos.y, pos.z,
				1.0f, 0.22f, 0.20f);
		};
	pointLights[2] = crearLuzNave(glm::vec3(0.0f));
	pointLights[3] = crearLuzMotor(glm::vec3(0.0f));
	pointLights[4] = crearLuzMotor(glm::vec3(0.0f));

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.01f, 0.01f,
		15.0f);
	spotLightCount++;


	//Luz del Rover
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -0.8f, 0.0f,
		1.0f, 0.09f, 0.032f,
		25.0f);
	spotLightCount++;

	//Faro inferior del avion
	const glm::vec3 COLORES_FARO[6] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f)
	};
	auto crearFaroBajoAvion = [](const glm::vec3& c)
		{
			return SpotLight(c.r, c.g, c.b,
				0.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.09f, 0.032f,
				25.0f);
		};
	spotLights[2] = crearFaroBajoAvion(glm::vec3(1.0f, 1.0f, 1.0f));
	spotLightCount++;

	//Luz frente avion
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.09f, 0.032f,
		25.0f);
	spotLightCount++;

	//Faro de la nave
	spotLights[4] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.09f, 0.032f,
		25.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//Ciclo principal

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	int indiceColorFaro = -1;
	bool luzHolocronEncendida = true;
	bool luzPinzaEncendida = true;
	bool teclaZAnterior = false;
	bool teclaXAnterior = false;
	bool teclaCAnterior = false;
	bool luzNaveEncendida = true;
	bool motor1Encendido = true;
	bool motor2Encendido = true;
	bool teclaVAnterior = false;
	bool teclaBAnterior = false;
	bool teclaNAnterior = false;

	//Posiciones de la nave
	const glm::vec3 NAVE_POS = glm::vec3(3.0f, 6.0f, -9.0f);
	const glm::vec3 NAVE_ESCALA = glm::vec3(0.3f, 0.3f, 0.3f);
	const GLfloat NAVE_GIRO_Y = 0.0f;
	//Luces de la nave
	const glm::vec3 LUZ_NAVE_OFFSET = glm::vec3(0.0f, 2.4f, 10.0f);
	const glm::vec3 FARO_NAVE_OFFSET = glm::vec3(0.0f, -2.0f, 0.0f);
	const glm::vec3 FARO_NAVE_DIRECCION = glm::vec3(0.0f, -1.0f, 0.0f);
	//Motores
	const glm::vec3 MOTOR1_OFFSET = glm::vec3(5.0f, 2.0f, -17.0f);
	const glm::vec3 MOTOR2_OFFSET = glm::vec3(-3.0f, 2.0f, -17.0f);
	//Fuego de los motores
	const glm::vec3 FUEGO1_OFFSET = glm::vec3(-0.85f, 0.0f, 0.0f);
	const glm::vec3 FUEGO2_OFFSET = glm::vec3(-0.85f, 0.0f, 0.0f);
	const glm::vec3 FUEGO_ESCALA = glm::vec3(2.5f, 4.0f, 2.5f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Entradas
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Controles de luces
		GLFWwindow* ventana = glfwGetCurrentContext();
		bool teclaZ = glfwGetKey(ventana, GLFW_KEY_Z) == GLFW_PRESS;
		bool teclaX = glfwGetKey(ventana, GLFW_KEY_X) == GLFW_PRESS;
		bool teclaC = glfwGetKey(ventana, GLFW_KEY_C) == GLFW_PRESS;
		bool teclaV = glfwGetKey(ventana, GLFW_KEY_V) == GLFW_PRESS;
		bool teclaB = glfwGetKey(ventana, GLFW_KEY_B) == GLFW_PRESS;
		bool teclaN = glfwGetKey(ventana, GLFW_KEY_N) == GLFW_PRESS;
		if (teclaZ && !teclaZAnterior)
		{
			indiceColorFaro = (indiceColorFaro + 1) % 6;
			spotLights[2] = crearFaroBajoAvion(COLORES_FARO[indiceColorFaro]);
		}
		if (teclaX && !teclaXAnterior)
		{
			luzHolocronEncendida = !luzHolocronEncendida;
		}
		if (teclaC && !teclaCAnterior)
		{
			luzPinzaEncendida = !luzPinzaEncendida;
		}
		if (teclaV && !teclaVAnterior)
		{
			luzNaveEncendida = !luzNaveEncendida;
		}
		if (teclaB && !teclaBAnterior)
		{
			motor1Encendido = !motor1Encendido;
		}
		if (teclaN && !teclaNAnterior)
		{
			motor2Encendido = !motor2Encendido;
		}
		teclaZAnterior = teclaZ;
		teclaXAnterior = teclaX;
		teclaCAnterior = teclaC;
		teclaVAnterior = teclaV;
		teclaBAnterior = teclaB;
		teclaNAnterior = teclaN;

		//Limpiar ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//Materiales
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//Linterna de la camara
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Movimiento Rover
		GLfloat avanceRover = 0.05f * (mainWindow.getarticulacion10() - mainWindow.getarticulacion11());
		glm::mat4 matrizRover = glm::mat4(1.0f);
		matrizRover = glm::translate(matrizRover, glm::vec3(1.0f - avanceRover, 0.0f, 6.0f));
		matrizRover = glm::scale(matrizRover, glm::vec3(0.4f, 0.4f, 0.4f));
		matrizRover = glm::translate(matrizRover, glm::vec3(0.0f, 1.0f, 0.0f));
		// Luz frente Rover
		glm::vec3 posicionFaroRover = glm::vec3(matrizRover * glm::vec4(-5.0f, 1.0f, 0.0f, 1.0f));
		glm::vec3 direccionFaroRover = glm::normalize(glm::vec3(matrizRover * glm::vec4(-1.0f, -0.5f, 0.0f, 0.0f)));
		spotLights[1].SetFlash(posicionFaroRover, direccionFaroRover);

		// Movimiento del Avion
		GLfloat alturaAvion = 0.05f * (mainWindow.getarticulacion12() - mainWindow.getarticulacion13());
		GLfloat avanceAvion = 0.05f * (mainWindow.getarticulacion14() - mainWindow.getarticulacion15());
		glm::mat4 giroAvion = glm::rotate(glm::mat4(1.0f), -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 frenteLocalAvion = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 frenteAvion = glm::normalize(glm::vec3(giroAvion * glm::vec4(frenteLocalAvion, 0.0f)));
		glm::mat4 matrizAvion = glm::mat4(1.0f);
		matrizAvion = glm::translate(matrizAvion, glm::vec3(0.0f, 5.0f + alturaAvion, 6.0f) + frenteAvion * avanceAvion);
		matrizAvion = glm::scale(matrizAvion, glm::vec3(0.3f, 0.3f, 0.3f));
		matrizAvion = glm::rotate(matrizAvion, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Luz bajo Avion
		glm::vec3 posicionFaroInferior = glm::vec3(matrizAvion * glm::vec4(0.0f, -2.0f, 0.0f, 1.0f));
		glm::vec3 direccionFaroInferior = glm::normalize(glm::vec3(matrizAvion * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		spotLights[2].SetFlash(posicionFaroInferior, direccionFaroInferior);
		// Luz frente Avion
		glm::vec3 posicionFaroFrontal = glm::vec3(matrizAvion * glm::vec4(0.0f, 0.0f, -3.0f, 1.0f));
		glm::vec3 direccionFaroFrontal = glm::normalize(glm::vec3(matrizAvion * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
		spotLights[3].SetFlash(posicionFaroFrontal, direccionFaroFrontal);

		//Jerarquia de la pinza
		glm::mat4 matrizBaseBrazo = matrizRover;
		matrizBaseBrazo = glm::translate(matrizBaseBrazo, glm::vec3(-2.5f, 1.5f, 1.0f));
		glm::mat4 matrizBrazoUno = matrizBaseBrazo;
		matrizBrazoUno = glm::translate(matrizBrazoUno, glm::vec3(0.8f, 1.1f, 0.0f));
		matrizBrazoUno = glm::rotate(matrizBrazoUno, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrizBrazoDos = matrizBrazoUno;
		matrizBrazoDos = glm::translate(matrizBrazoDos, glm::vec3(2.5f, 2.5f, 0.0f));
		matrizBrazoDos = glm::rotate(matrizBrazoDos, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrizGarra = matrizBrazoDos;
		matrizGarra = glm::translate(matrizGarra, glm::vec3(-2.5f, 3.0f, 0.0f));
		matrizGarra = glm::rotate(matrizGarra, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 posicionLuzPinza = glm::vec3(matrizGarra * glm::vec4(LUZ_PINZA_OFFSET, 1.0f));
		pointLights[1] = crearLuzPinza(posicionLuzPinza);

		//Luz del Holocron
		glm::mat4 matrizHolocron = glm::mat4(1.0f);
		matrizHolocron = glm::translate(matrizHolocron, HOLOCRON_POS);
		matrizHolocron = glm::scale(matrizHolocron, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec3 posicionLuzHolocron = glm::vec3(matrizHolocron * glm::vec4(LUZ_HOLOCRON_OFFSET, 1.0f));
		pointLights[0] = crearLuzHolocron(posicionLuzHolocron);

		//Jerarquia de la nave
		//Nave
		glm::mat4 matrizNave = glm::mat4(1.0f);
		matrizNave = glm::translate(matrizNave, NAVE_POS);
		matrizNave = glm::rotate(matrizNave, glm::radians(NAVE_GIRO_Y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrizNave = glm::scale(matrizNave, NAVE_ESCALA);

		//Luces de la nave
		glm::vec3 posicionLuzNave = glm::vec3(matrizNave * glm::vec4(LUZ_NAVE_OFFSET, 1.0f));
		glm::vec3 posicionFaroNave = glm::vec3(matrizNave * glm::vec4(FARO_NAVE_OFFSET, 1.0f));
		glm::vec3 direccionFaroNave = glm::normalize(glm::vec3(matrizNave * glm::vec4(FARO_NAVE_DIRECCION, 0.0f)));
		pointLights[2] = crearLuzNave(posicionLuzNave);
		spotLights[4] = SpotLight(
			1.0f, 0.0f, 0.0f,
			0.0f, luzNaveEncendida ? 3.0f : 0.0f,
			posicionFaroNave.x, posicionFaroNave.y, posicionFaroNave.z,
			direccionFaroNave.x, direccionFaroNave.y, direccionFaroNave.z,
			1.0f, 0.09f, 0.032f,
			25.0f);
		spotLights[4].SetFlash(posicionFaroNave, direccionFaroNave);

		glm::mat4 matrizMotor1 = matrizNave;
		matrizMotor1 = glm::translate(matrizMotor1, MOTOR1_OFFSET);
		glm::mat4 matrizMotor2 = matrizNave;
		matrizMotor2 = glm::translate(matrizMotor2, MOTOR2_OFFSET);

		glm::mat4 matrizFuego1 = matrizMotor1;
		matrizFuego1 = glm::translate(matrizFuego1, FUEGO1_OFFSET);
		glm::mat4 matrizFuego2 = matrizMotor2;
		matrizFuego2 = glm::translate(matrizFuego2, FUEGO2_OFFSET);
		glm::vec3 posicionLuzMotor1 = glm::vec3(matrizFuego1 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 posicionLuzMotor2 = glm::vec3(matrizFuego2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		pointLights[3] = crearLuzMotor(posicionLuzMotor1);
		pointLights[4] = crearLuzMotor(posicionLuzMotor2);

		//Luces activas
		unsigned int pointLightCountActivas = 0;
		if (luzHolocronEncendida) pointLightsActivas[pointLightCountActivas++] = pointLights[0];
		if (luzPinzaEncendida) pointLightsActivas[pointLightCountActivas++] = pointLights[1];
		if (luzNaveEncendida) pointLightsActivas[pointLightCountActivas++] = pointLights[2];
		if (motor1Encendido) pointLightsActivas[pointLightCountActivas++] = pointLights[3];
		if (motor2Encendido) pointLightsActivas[pointLightCountActivas++] = pointLights[4];

		//Iluminacion
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLightsActivas, pointLightCountActivas);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();

		//Avion
		model = matrizAvion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion_M.RenderModel();

		//Rover con jerarquía
		model = matrizRover;
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		roverbase.RenderModel();
		//Base Brazo
		model = matrizBaseBrazo;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazobase.RenderModel();
		//Tubo 1
		model = matrizBrazoUno;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazouno.RenderModel();
		//Tubo 2
		model = matrizBrazoDos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazodos.RenderModel();
		//Pinza
		model = matrizGarra;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pinza.RenderModel();
		//Llanta frontal +Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, 3.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frontalmasz.RenderModel();
		model = glm::translate(model, glm::vec3(-3.6f, -2.7f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantafrontalmasz.RenderModel();
		//Llanta frontal -Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, -3.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frontalmenosz.RenderModel();
		model = glm::translate(model, glm::vec3(-3.6f, -2.7f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantafrontalmenosz.RenderModel();
		//Llanta media +Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.2f, 0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mediomasz.RenderModel();
		model = glm::translate(model, glm::vec3(-1.5f, -2.1f, 1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantamediomasz.RenderModel();
		//Llanta media -Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, -3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mediomenosz.RenderModel();
		model = glm::translate(model, glm::vec3(-1.5f, -2.1f, -1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantamediomenosz.RenderModel();
		//Llanta trasera +Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, 0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traseramasz.RenderModel();
		model = glm::translate(model, glm::vec3(1.5f, -2.1f, 1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantatraseramasz.RenderModel();
		//Llanta trasera -Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.2f, 0.5f, -3.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traseramenosz.RenderModel();
		model = glm::translate(model, glm::vec3(1.5f, -2.1f, -1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantatraseramenosz.RenderModel();

		//Cubo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		model = matrizHolocron;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();

		//Nave y plasma
		//Nave
		model = matrizNave;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_M.RenderModel();

		//Plasma de los motores
		if (plasmaDisponible && tieneTexturaFuego && (motor1Encendido || motor2Encendido))
		{
			GLboolean cullingAnterior = glIsEnabled(GL_CULL_FACE);
			GLboolean blendAnterior = glIsEnabled(GL_BLEND);
			GLboolean profundidadAnterior;
			glGetBooleanv(GL_DEPTH_WRITEMASK, &profundidadAnterior);
			glDisable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDepthMask(GL_FALSE);
			shaderPlasma.UseShader();
			glUniformMatrix4fv(plasmaProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 vistaPlasma = camera.calculateViewMatrix();
			glUniformMatrix4fv(plasmaView, 1, GL_FALSE, glm::value_ptr(vistaPlasma));
			glUniform1i(plasmaTextura, 0);
			glActiveTexture(GL_TEXTURE0);
			FuegoAzulTexture.UseTexture();
			if (motor1Encendido)
			{
				model = matrizFuego1;
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, FUEGO_ESCALA);
				glUniformMatrix4fv(plasmaModel, 1, GL_FALSE, glm::value_ptr(model));
				meshListModel[3]->RenderMeshModel();
			}
			if (motor2Encendido)
			{
				model = matrizFuego2;
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, FUEGO_ESCALA);
				glUniformMatrix4fv(plasmaModel, 1, GL_FALSE, glm::value_ptr(model));
				meshListModel[3]->RenderMeshModel();
			}
			glDepthMask(profundidadAnterior);
			if (!blendAnterior) glDisable(GL_BLEND);
			if (cullingAnterior) glEnable(GL_CULL_FACE);
			shaderList[0].UseShader();
		}

		//Agave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//Transparencia
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}