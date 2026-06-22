
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


//#include "P6/anchoredSpring.h"
#include "physicsWorld.h"
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

	float windowWidth = 700;
	float windowHeight = 700;

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
	sphere.setScale(glm::vec3(10.f));

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
	// ========= PARTICLES ==========
	// ==============================

	// RED PARTICLE
	P6::Particle p1;
	p1.setName("Red");
	p1.Position = glm::vec3(-400, 200, 0);
	p1.Velocity = P6::Particle::makeVec(80.f, p1.Position);
	p1.Acceleration = P6::Particle::makeVec(14.5f, p1.Position);
	p1.damping = 0.9f;
	p1.mass = 50;
	pWorld.AddParticle(&p1);

	// YELLOW PARTICLE

	P6::Particle p2;
	p2.setName("Yellow");
	p2.Position = glm::vec3(-400.f, 0.f, 0);
	p2.Velocity = P6::Particle::makeVec(110.f, p2.Position);
	p2.Acceleration = P6::Particle::makeVec(3.f, p2.Position);
	pWorld.AddParticle(&p2);

	// GREEN PARTICLE

	P6::Particle p3;
	p3.setName("Green");
	p3.Position = glm::vec3(-400.f, -200.f, 0);
	p3.Velocity = P6::Particle::makeVec(90.f, p3.Position);
	p3.Acceleration = P6::Particle::makeVec(8.f, p3.Position);
	pWorld.AddParticle(&p3);

	// DRAG
	DragForceGenerator drag = DragForceGenerator(0.14, 0.1);
	pWorld.forceRegistry.Add(&p1, &drag);

	// RENDER PARTICLE 
	RenderParticle rp1(&p1, sphere.getRenderObject(), glm::vec3(1.f, 0.f, 0.f));
	RenderParticle rp2(&p2, sphere.getRenderObject(), glm::vec3(1.f, 1.f, 0.f));
	RenderParticle rp3(&p3, sphere.getRenderObject(), glm::vec3(0.f, 1.f, 0.f));

	rp1.Scale = glm::vec3(5.f);
	rp2.Scale = glm::vec3(5.f);
	rp3.Scale = glm::vec3(5.f);

	// MOVEMENT VECTOR

	vector<P6::Particle*> particles;
	particles.push_back(&p1);
	particles.push_back(&p2);
	particles.push_back(&p3);

	// LIST

	list<RenderParticle*> RenderParticles;
	RenderParticles.push_back(&rp1);
	RenderParticles.push_back(&rp2);
	RenderParticles.push_back(&rp3);

	// TIME

	using clock = std::chrono::high_resolution_clock;

	constexpr std::chrono::nanoseconds timestep(16ms);

	auto curr_time = clock::now();
	auto prev_time = curr_time;

	std::chrono::nanoseconds curr_ns(0);

	int currentRank = 1;
	bool printedResults = false;

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

			for (auto* p : pWorld.GetParticles())
			{
				// when Particle reaches center
				if (glm::length(p->Position) <= 1.f)
				{ 
					if (p == &p1)
					{
						p1.Destroy();
					}
				}
			}
		}


		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DRAW LIST

		for (auto* rp : RenderParticles)
			rp->Draw();

		glfwSwapBuffers(window);
	}

	sphere.destroy();
	shader.destroy();

	glfwTerminate();
	return 0;
}
