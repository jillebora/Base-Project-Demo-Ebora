
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
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#include "physicsWorld.h"
#include "P6/chain.h"
#include "renderParticle.h"
#include "shader.h"
#include "model.h"
#include "dragForceGenerator.h"

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

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Base Demo - Jillana Ebora", NULL, NULL);

	P6::PhysicsWorld pWorld = P6::PhysicsWorld();


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	// SHADERS
	Shader shader("Shaders/sphere.vert", "Shaders/sphere.frag");

	// MODEL
	Model sphere;
	sphere.loadFromObj("3D/sphere.obj");
	sphere.setScale(glm::vec3(1.f));

	// CAMERA

	glm::mat4 proj = glm::ortho(-350.f, 350.f, -350.f, 350.f, -500.f, 1000.f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.f, 0.f, 700.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));

	// Wire shader + camera into the model's RenderObject
	sphere.setShader(&shader.ID);
	sphere.setCamera(&proj, &view);

	// ==============================
	// =========== CHAIN ============
	// ==============================

	const int numLinks = 10;
	const float linkLength = 35.f;

	const::glm::vec anchorPos = glm::vec3(0.f, 300.f, 0.f);

	vector<P6::Particle*> chainParticles;
	vector<RenderParticle*> chainRenderParticles;
	vector<Chain*> chainLinks;


	for (int i = 0; i < numLinks; i++)
	{
		P6::Particle* p = new P6::Particle();
		p->setName("ChainLink" + std::to_string(i));
		p->Position = anchorPos - glm::vec3(0.f, linkLength * (float)i, 0.f);
		p->mass = 5.f;
		p->damping = 0.96f;

		pWorld.AddParticle(p);
		chainParticles.push_back(p);

		RenderParticle* rp = new RenderParticle(p, sphere.getRenderObject(), glm::vec3(0.9f, 0.7f, 0.1f));
		rp->Scale = glm::vec3(4.f);
		chainRenderParticles.push_back(rp);

	}

	for (size_t i = 1; i < chainParticles.size(); i++)
	{
		chainParticles[i]->Velocity = glm::vec3(40.f, 0.f, 0.f);
	}

	for (int i = 0; i < numLinks - 1; i++)
	{
		Chain* c = new Chain();
		c->particles[0] = chainParticles[i];
		c->particles[1] = chainParticles[i + 1];
		c->maxLength = linkLength;
		c->restitution = 0.0f;

		pWorld.Links.push_back(c);
		chainLinks.push_back(c);
	}

	DragForceGenerator drag = DragForceGenerator(0.14f, 0.1f);
	for (P6::Particle* p : chainParticles)
	{
		pWorld.forceRegistry.Add(p, &drag);
	}

	// TIME

	using clock = std::chrono::high_resolution_clock;

	constexpr std::chrono::nanoseconds timestep(16ms);

	auto curr_time = clock::now();
	auto prev_time = curr_time;

	std::chrono::nanoseconds curr_ns(0);

	// LINE LOADING 

	GLuint lineVAO, lineVBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, (numLinks - 1) * 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

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
			constexpr float timestep_sec = timestep.count() / (float)1E09;

			curr_ns -= timestep;

			pWorld.Update(timestep_sec);

			// Pin first chain particle to anchor point so the chain
			// has something to hang from.
			chainParticles[0]->Position = anchorPos;
			chainParticles[0]->Velocity = glm::vec3(0.f);

		}


		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vector<float> lineVertices;
		lineVertices.reserve((numLinks - 1) * 6);
		for (Chain* c : chainLinks)
		{
			glm::vec3 p1 = c->particles[0]->Position;
			glm::vec3 p2 = c->particles[1]->Position;

			lineVertices.push_back(p1.x);
			lineVertices.push_back(p1.y);
			lineVertices.push_back(p1.z);

			lineVertices.push_back(p2.x);
			lineVertices.push_back(p2.y);
			lineVertices.push_back(p2.z);
		}

		shader.use();
		shader.setMat4("transform", glm::mat4(1.f));
		shader.setMat4("projection", proj);
		shader.setMat4("view", view);
		shader.setVec3("color", glm::vec3(0.9f, 0.7f, 0.1f));

		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(lineVAO);

		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_DYNAMIC_DRAW);

		glDrawArrays(GL_LINES, 0, (GLsizei)(lineVertices.size() / 3));

		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);

		// DRAW LIST

		for (auto* rp : chainRenderParticles)
			rp->Draw();

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &lineVAO);
	glDeleteBuffers(1, &lineVBO);

	for (auto* c : chainLinks)
		delete c;
	for (auto* rp : chainRenderParticles)
		delete rp;
	for (auto* p : chainParticles)
		delete p;

	sphere.destroy();
	shader.destroy();

	glfwTerminate();
	return 0;
}
