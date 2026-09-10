//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";
static const char* fShaderAmarillo = "shaders/shaderamarillo.frag";
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shaderverde.frag";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shadercafe.frag";
static const char* vShaderMagenta = "shaders/shadermagenta.vert";
static const char* fShaderMagenta = "shaders/shadermagenta.frag";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shaderazul.frag";
static const char* vShaderNaranja = "shaders/shadernaranja.vert";
static const char* fShaderNaranja = "shaders/shadernaranja.frag";

//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
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

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letraJ[] = {
		//X		Y	 Z		R	 G		B
		//Letra J
		-1.0f, 0.6f,0.0f, 1.0f,	0.0f,	0.0f,
		-1.0f, 0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.3f, 0.6f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.3f, 0.6f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.3f, 0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-1.0f, 0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.6f, 0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f, 0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.6f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f, 0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.6f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.6f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f,-0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f,-0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.7f,-0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.5f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-1.0f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-1.0f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
		-1.0f,-0.2f,0.0f, 1.0f,	0.0f,	0.0f,
		-1.0f,-0.2f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.2f,0.0f, 1.0f,	0.0f,	0.0f,
		-0.9f,-0.4f,0.0f, 1.0f,	0.0f,	0.0f,
	};
	MeshColor* letraJ = new MeshColor();
	letraJ->CreateMeshColor(vertices_letraJ, 180);
	meshColorList.push_back(letraJ);

	GLfloat vertices_letraO[] = {
		//X		Y	 Z		R	 G		B
		//Letra O
		-0.1f,0.6f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.6f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.6f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,0.6f,0.0f, 1.0f,	0.0f,	0.5f,
		0.3f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.3f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		0.3f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,0.6f,0.0f, 1.0f,	0.0f,	0.5f,
		0.3f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.2f,0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.2f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.2f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,-0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		-0.1f,-0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f,
		0.2f,-0.5f,0.0f, 1.0f,	0.0f,	0.5f,
		0.3f,-0.4f,0.0f, 1.0f,	0.0f,	0.5f
	};
	MeshColor* letraO = new MeshColor();
	letraO->CreateMeshColor(vertices_letraO, 216);
	meshColorList.push_back(letraO);

	GLfloat vertices_letraS[] = {
		//X		Y	 Z		R	 G		B
		//Letra S
		0.5f,0.6f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,0.6f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,0.6f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.6f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.5f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.1f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,0.0f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.9f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,-0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,-0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,-0.5f,0.0f, 1.0f,	1.0f,	0.0f,
		0.8f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,-0.4f,0.0f, 1.0f,	1.0f,	0.0f,
		0.4f,-0.5f,0.0f, 1.0f,	1.0f,	0.0f
	};
	MeshColor* letraS = new MeshColor();
	letraS->CreateMeshColor(vertices_letraS, 252);
	meshColorList.push_back(letraS);
}


void CreateShaders()
{
	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
	Shader* shader3 = new Shader();
	shader3->CreateFromFiles(vShaderAmarillo, fShaderAmarillo);
	shaderList.push_back(*shader3);
	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shader4);
	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shader5);
	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shader6);
	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vShaderMagenta, fShaderMagenta);
	shaderList.push_back(*shader7);
	Shader* shader8 = new Shader();
	shader8->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shader8);
	Shader* shader9 = new Shader();
	shader9->CreateFromFiles(vShaderNaranja, fShaderNaranja);
	shaderList.push_back(*shader9);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,0.5f,0.7f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//Para el cubo y las pirámides se usa el primer set de shaders con índice 0 en ShaderList
		//Shader Azul
		shaderList[7].useShader();
		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.05f, 0.015f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		//Shader Amarillo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.775f, 0.3f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.16f, 0.12f, -3.0f));
		model = glm::scale(model, glm::vec3(0.21f, 0.21f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[0]->RenderMesh();

		//Shader Rojo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.775f, 0.1f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.06f, 0.12f, -3.0f));
		model = glm::scale(model, glm::vec3(0.21f, 0.21f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.8f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[0]->RenderMesh();

		//Shader Verde
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.775f, -0.1f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.06f, -0.092f, -3.0f));
		model = glm::scale(model, glm::vec3(0.215f, 0.215f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[0]->RenderMesh();

		//Shader Cafe
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9f, 0.175f, -3.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.76f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.175f, -3.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.76f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.05f, 0.015f, -2.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[1]->RenderMesh();

		//Shader Magenta
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.25f, -3.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.1f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Shader Naranja
		shaderList[8].useShader();
		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.158f, -0.092f, -3.0f));
		model = glm::scale(model, glm::vec3(0.215f, 0.215f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, 0.25f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[0]->RenderMesh();

		/*
		// Shader para las letras
		shaderList[1].useShader();

		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Dibujar J
		meshColorList[0]->RenderMeshColor();

		// Dibujar O
		meshColorList[1]->RenderMeshColor();

		// Dibujar S
		meshColorList[2]->RenderMeshColor();
		*/
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/