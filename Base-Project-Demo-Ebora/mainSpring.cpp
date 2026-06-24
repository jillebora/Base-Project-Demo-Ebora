
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


#include "P6/anchoredSpring.h"
#include "P6/particleSpring.h"
#include "p6/particleContact.h"
#include "physicsWorld.h"
#include "P6/rod.h"
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
	// ========= PARTICLES ==========
	// ==============================

	// RED PARTICLE
	P6::Particle p1;
	p1.setName("Red");
	p1.Position = glm::vec3(0, 0, 0);
	//p1.Velocity = //P6::Particle::makeVec(0.f, p1.Position);	
		//P6::Particle::makeVec(80.f, p1.Position);
	//p1.Acceleration = //P6::Particle::makeVec(0.f, p1.Position); 
		//P6::Particle::makeVec(14.5f, p1.Position);
	//P6::Particle::makeVec(14.5f, p1.Position);
	///p1.damping = 0.9f;
	p1.mass = 50;
	p1.radius = 50;
	pWorld.AddParticle(&p1);
	p1.AddForce(glm::vec3(0, 0.1f, 0) * 500000.f);

	// RED PARTICLE
	P6::Particle p2;
	p2.setName("Blue");
	p2.Position = glm::vec3(50, 0, 0);
	//p2.Velocity = //P6::Particle::makeVec(0.f, p1.Position);	
		//P6::Particle::makeVec(80.f, p1.Position);
	//p2.Acceleration = //P6::Particle::makeVec(0.f, p1.Position); 
		//P6::Particle::makeVec(14.5f, p1.Position);
	//P6::Particle::makeVec(14.5f, p1.Position);
	//p2.damping = 0.9f;
	p2.mass = 100;
	pWorld.AddParticle(&p2);
	
	// ROD
	Rod* r = new Rod();
	r->particles[0] = &p1;
	r->particles[1] = &p2;
	r->length = 200;

	pWorld.Links.push_back(r);


	// DRAG
	DragForceGenerator drag = DragForceGenerator(0.14, 0.1);
	pWorld.forceRegistry.Add(&p1, &drag);

	// RENDER PARTICLE 
	RenderParticle rp1(&p1, sphere.getRenderObject(), glm::vec3(0.4f, 0.f, 0.f));
	RenderParticle rp2(&p2, sphere.getRenderObject(), glm::vec3(0.f, 0.f, 0.4f));

	rp1.Scale = glm::vec3(p1.radius, p1.radius, p1.radius);
	rp2.Scale = glm::vec3(50.f);

	// MOVEMENT VECTOR

	vector<P6::Particle*> particles;
	particles.push_back(&p1);
	particles.push_back(&p2);

	// LIST

	list<RenderParticle*> RenderParticles;
	RenderParticles.push_back(&rp1);
	RenderParticles.push_back(&rp2);

	// GRAVITY
	GravityForceGenerator Gravity = GravityForceGenerator(glm::vec3(0.f));

	// CONTACT

	/*ParticleContact contact = ParticleContact();
	contact.particles[0] = &p1;
	contact.particles[1] = &p2;

	contact.contactNormal =  p1.Position - p2.Position;
	contact.contactNormal = glm::normalize(contact.contactNormal);
	contact.restitiution = 1;*/

	p1.Velocity = glm::vec3(-30.f, 0.f, 0.f);
	p2.Velocity = glm::vec3(30.f, 0.f, 0.f);

	// SPRING

	//ParticleSpring pS = ParticleSpring(&p1, 5, 1);
	//// Force will only pe applied to P2 since anchor is P1
	//pWorld.forceRegistry.Add(&p2, &pS);

	//ParticleSpring pS2 = ParticleSpring(&p2, 5, 1);
	//// Force will only pe applied to P1 since anchor is P2
	//pWorld.forceRegistry.Add(&p1, &pS2);

	//glm::vec3 springPos = glm::vec3(0, 200, 0);
	//AnchoredSpring aSpring = AnchoredSpring(springPos, 5, 0.5);
	//pWorld.forceRegistry.Add(&p1, &aSpring);

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

			//contact.Resolve(timestep_sec);

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
