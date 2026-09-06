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
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
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
	Mesh *piramidetriangular = new Mesh();
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
		2,3,0

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
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
	};
	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_cuadradomorado[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.4f,	0.0f,	0.4f,
		0.5f,	-0.5f,		0.5f,			0.4f,	0.0f,	0.4f,
		0.5f,	0.5f,		0.5f,			0.4f,	0.0f,	0.4f,
		-0.5f,	-0.5f,		0.5f,			0.4f,	0.0f,	0.4f,
		0.5f,	0.5f,		0.5f,			0.4f,	0.0f,	0.4f,
		-0.5f,	0.5f,		0.5f,			0.4f,	0.0f,	0.4f,
	};
	MeshColor* cuadradomorado = new MeshColor();
	cuadradomorado->CreateMeshColor(vertices_cuadradomorado, 36);
	meshColorList.push_back(cuadradomorado);

	GLfloat vertices_triangulonaranja[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.5f,	0.0f,
	};
	MeshColor* triangulonaranja = new MeshColor();
	triangulonaranja->CreateMeshColor(vertices_triangulonaranja, 18);
	meshColorList.push_back(triangulonaranja);

	GLfloat vertices_triangulonaranja2[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.5f,	0.0f,
		-1.0f,	1.0f,		0.5f,			1.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.5f,	0.0f,
	};
	MeshColor* triangulonaranja2 = new MeshColor();
	triangulonaranja2->CreateMeshColor(vertices_triangulonaranja2, 18);
	meshColorList.push_back(triangulonaranja2);

	GLfloat vertices_trianguloverde2[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
	};
	MeshColor* trianguloverde2 = new MeshColor();
	trianguloverde2->CreateMeshColor(vertices_trianguloverde2, 18);
	meshColorList.push_back(trianguloverde2);

	GLfloat vertices_trianguloamarillo2[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
	};
	MeshColor* trianguloamarillo2 = new MeshColor();
	trianguloamarillo2->CreateMeshColor(vertices_trianguloamarillo2, 18);
	meshColorList.push_back(trianguloamarillo2);

	GLfloat vertices_triangulorojo2[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	};
	MeshColor* triangulorojo2 = new MeshColor();
	triangulorojo2->CreateMeshColor(vertices_triangulorojo2, 18);
	meshColorList.push_back(triangulorojo2);

	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.8f,	0.8f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.8f,	0.8f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.8f,	0.8f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.8f,	0.8f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.8f,	0.8f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.8f,	0.8f,
	};
	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
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
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
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

		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//Triangulo Amarillo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, 0.3f, -3.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Triangulo Rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, 0.1f, -3.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[1]->RenderMeshColor();

		//Triangulo Verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.1f, -3.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[2]->RenderMeshColor();

		//Cuadrado Cafe 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.82f, 0.18f, -3.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.76f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[3]->RenderMeshColor();

		//Cuadrado Cafe 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.58f, 0.18f, -3.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.76f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[3]->RenderMeshColor();

		//Cuadrado Morado
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.25f, -3.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.1f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[4]->RenderMeshColor();
		
		//Triangulo Naranja Fig 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[6]->RenderMeshColor();

		//Triangulo Verde Fig 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.05f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[7]->RenderMeshColor();

		//Triangulo Amarillo Fig 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, 0.25f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[8]->RenderMeshColor();

		//Triangulo Rojo Fig 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.05f, 0.25f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[9]->RenderMeshColor();

		//Cuadrado Azul Fig 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.10f, 0.10f, -3.0f));
		model = glm::scale(model, glm::vec3(0.425f, 0.425f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[10]->RenderMeshColor();

		//Cuadrado Cafe Fig 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.10f, 0.10f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 4.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[3]->RenderMeshColor();

		//Triangulo Amarillo Fig 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Triangulo Rojo Fig 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[1]->RenderMeshColor();

		//Triangulo Verde Fig 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, -0.05f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[2]->RenderMeshColor();

		//Triangulo Azul Fig 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, 0.25f, -3.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[5]->RenderMeshColor();

		
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

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