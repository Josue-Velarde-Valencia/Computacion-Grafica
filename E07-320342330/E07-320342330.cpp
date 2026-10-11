/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
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
Model Avion_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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
	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
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
	AgaveTexture.LoadTextureA();;
	dadoTexture = Texture("Textures/LogosStarWars.jpeg");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/LogosStarWars.jpeg");
	holocronTexture.LoadTextureA();

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
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion_mordecai.obj");

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


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
	
		//Avion
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion_M.RenderModel();

		//Rover con jerarquía
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 6.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		roverbase.RenderModel();
		//Base Brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazobase.RenderModel();
		//Tubo 1
		model = glm::translate(model, glm::vec3(0.8f, 1.1f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazouno.RenderModel();
		//Tubo 2
		model = glm::translate(model, glm::vec3(2.5f, 2.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazodos.RenderModel();
		//Pinza
		model = glm::translate(model, glm::vec3(-2.5f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pinza.RenderModel();
		//Llanta frontal +Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, 3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frontalmasz.RenderModel();
		//Llanta frontal -Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, -3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frontalmenosz.RenderModel();
		//Llanta media +Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.2f, 0.5f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mediomasz.RenderModel();
		//Llanta media -Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, -3.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mediomenosz.RenderModel();
		//Llanta trasera +Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, 0.5f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traseramasz.RenderModel();
		//Llanta trasera -Z
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, 0.5f, -3.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traseramenosz.RenderModel();

		//Cubo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 7.0f, 6.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();


		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
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
