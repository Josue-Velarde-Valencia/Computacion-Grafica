/*
Práctica 5: Optimización y Carga de Modelos
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
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
//Rover
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
//Holocron
Model baseholocron;
Model esquinageneral;
//Satelite
Model satelitebase;
Model panel;
Model parabolica;
Model centro;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


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

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	float anguloEsquinas = 0.0f;

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//Cargar modelos
	//Rover
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
	//Holocron
	baseholocron = Model();
	baseholocron.LoadModel("Models/baseholocron.obj");
	esquinageneral = Model();
	esquinageneral.LoadModel("Models/esquinageneral.obj");
	//Satelite
	satelitebase = Model();
	satelitebase.LoadModel("Models/satelitebase.obj");
	panel = Model();
	panel.LoadModel("Models/panel.obj");
	parabolica = Model();
	parabolica.LoadModel("Models/parabolica.obj");
	centro = Model();
	centro.LoadModel("Models/centro.obj");

	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		anguloEsquinas += 30.0f * deltaTime;

		if (anguloEsquinas >= 360.0f)
			anguloEsquinas -= 360.0f;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();
		
		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Modelo Inicial
		color = glm::vec3(0.0f, 0.8f, 1.0f); //modelo de color azul
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		roverbase.RenderModel();//modificar por el modelo de solo cuerpo del Rover, para que se pueda separar el brazo y las llantas
		//En sesión se separara una parte del modelo y se unirá por jeraquía al cuerpo
		
		//Base Brazo
		model = modelaux;
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-2.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazobase.RenderModel();
		//Tubo 1
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(0.8f, 0.8f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazouno.RenderModel();
		//Tubo 2
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(2.5f, 2.5f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazodos.RenderModel();
		//Pinza
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-2.5f, 3.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pinza.RenderModel();
		//LLanta frontal +Z
		model = modelaux;
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, 3.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frontalmasz.RenderModel();
		//Llanta frontal -Z
		model = modelaux;
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-2.2f, 1.2f, -3.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frontalmenosz.RenderModel();
		//Llanta media +Z
		model = modelaux;
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		model = glm::translate(model, glm::vec3(2.2f, 0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mediomasz.RenderModel();
		// Llanta media -Z
		model = modelaux;
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, -3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mediomenosz.RenderModel();
		//Llanta trasera derecha
		model = modelaux;
		color = glm::vec3(0.5f, 0.0f, 0.5f);
		model = glm::translate(model, glm::vec3(4.0f, 0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traseramasz.RenderModel();
		//	Llanta trasera izquierda
		model = modelaux;
		color = glm::vec3(1.0f, 0.5f, 0.8f);
		model = glm::translate(model, glm::vec3(4.0f, 0.5f, -3.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traseramenosz.RenderModel();


		// SATELITE
		// Base
		color = glm::vec3(0.5f, 0.0f, 0.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.5f, 20.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelitebase.RenderModel();
		// Antena Parabólica
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		color = glm::vec3(0.3f, 0.3f, 0.3f);
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parabolica.RenderModel();
		// Centro
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = modelaux2; 
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		color = glm::vec3(0.8f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		centro.RenderModel();
		// Panel Izquierdo
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.5f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		panel.RenderModel();
		// Panel Derecho
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.5f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion13()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1,	GL_FALSE, glm::value_ptr(model));
		panel.RenderModel();


		// HOLOCRON
		//Modelo Inicial
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::mat4(1.0);
		model = modelaux;
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.5f, -16.0f));
		modelaux2 = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baseholocron.RenderModel();
		// Esquina 1 Rojo
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.1f, 5.2f, 5.1f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::rotate(model, -43.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -54.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion14()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 2 Rosa
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-5.1f, 5.2f, -5.1f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -54.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion15()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 3 Amarilla
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-5.1f, -5.2f, 5.1f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = glm::rotate(model, 43.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -234.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion16()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 4 Naranja
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-5.1f, -5.2f, -5.1f));
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		model = glm::rotate(model, -45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -234.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion17()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 5 Verde
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-5.1f, 5.2f, 5.1f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::rotate(model, -133.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -54.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion18()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 6 Azul
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.2f, 5.1f, -5.2f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -55.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion19()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 7 Morado
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.2f, -5.1f, 5.2f));
		color = glm::vec3(0.5f, 0.0f, 0.5f);
		model = glm::rotate(model, -225.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -235.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion20()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();
		// Esquina 8 Cafe
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.1f, -5.2f, -5.1f));
		color = glm::vec3(0.8f, 0.5f, 0.2f);
		model = glm::rotate(model, -138.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -235.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion21()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		esquinageneral.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
