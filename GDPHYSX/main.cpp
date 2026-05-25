
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <list>
#include <chrono>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#include "P6/particle.h"
#include "renderParticle.h"

using namespace std;
using namespace std::chrono_literals;

int main()
{

	// Initialize GLFW
	if (!glfwInit())
	{
		return -1;
	}

	float windowWidth = 800;
	float windowHeight = 800;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Jillana Ebora", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// SHADERS

	std::fstream vertSrc("Shaders/sphere.vert");
	std::stringstream vertBuff;
	//add the file stream to the string stream
	vertBuff << vertSrc.rdbuf();

	std::string vertS = vertBuff.str();
	const char* v = vertS.c_str();

	std::fstream fragSrc("Shaders/sphere.frag");
	std::stringstream fragBuff;

	fragBuff << fragSrc.rdbuf();

	std::string fragS = fragBuff.str();
	const char* f = fragS.c_str();

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glEnable(GL_DEPTH_TEST);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &v, NULL);

	glCompileShader(vertexShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShader, 1, &f, NULL);

	glCompileShader(fragShader);

	GLuint shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragShader);

	glLinkProgram(shaderProg);

	GLfloat vertices[]{
		0.f, 0.5f, 0.f,	  //0
		-1.f, -0.5f, 0.f, //1
		0.5f, -0.5f, 0.f  //2
	};

	GLuint indices[]{
		0,1,2
	};

	// LOAD OBJ

	string path = "3D/sphere.obj";
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> material;
	string error;
	string warning;


	tinyobj::attrib_t attributes;

	bool success_obj = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		path.c_str()
	);


	vector<GLuint> mesh_indices;
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
	{
		mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	// OPENGL BUFFERS

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * attributes.vertices.size(),
		&attributes.vertices[0],
		GL_STATIC_DRAW);


	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// CAMERA

	glm::mat4 proj = glm::ortho(
		-10.f, 10.f,   // left, right
		-10.f, 10.f,   // bottom, top
		-1.f, 100.f   // near, far
	);

	glm::vec3 cameraPos =
		glm::vec3(0.f, 0.f, 5.f);

	glm::mat4 view = glm::lookAt(
		cameraPos,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f)
	);

	// PARTICLE

	P6::Particle particle;

	particle.Position = glm::vec3(0.f, 0.f, 0.f);
	particle.Velocity = glm::vec3(3.f, 0.f, 0.f);
	particle.Acceleration = glm::vec3(-10.f, 0.f, 0.f);

	// RENDER OBJECT

	RenderObject sphereObj;

	sphereObj.shaderProg = &shaderProg;

	sphereObj.VAO = &VAO;

	sphereObj.projection = &proj;

	sphereObj.view = &view;

	sphereObj.indexCount =
		mesh_indices.size();


	// RENDER PARTICLE

	RenderParticle rp(
		&particle,
		&sphereObj,
		glm::vec3(1.f, 0.f, 0.f)
	);

	rp.Scale = glm::vec3(5.f);

	// LIST

	list<RenderParticle*> RenderParticles;

	RenderParticles.push_back(&rp);

	// TIME

	using clock = std::chrono::high_resolution_clock;

	constexpr std::chrono::nanoseconds timestep(16ms);

	auto curr_time = clock::now();
	auto prev_time = curr_time;

	std::chrono::nanoseconds curr_ns(0);

	// LOOP

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		curr_time = clock::now();

		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(
			curr_time - prev_time
		);

		prev_time = curr_time;

		curr_ns += dur;

		if (curr_ns >= timestep)
		{
			constexpr float timestep_sec =
				timestep.count() / (float)1E09;

			curr_ns -= timestep;

			particle.Update(timestep_sec);
		}

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DRAW LIST

		for (
			list<RenderParticle*>::iterator i =
			RenderParticles.begin();

			i != RenderParticles.end();

			i++
			)
		{
			(*i)->Draw();
		}

		glfwSwapBuffers(window);
	}

	//clean
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
