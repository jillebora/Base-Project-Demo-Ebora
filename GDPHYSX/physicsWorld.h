#pragma once

#include <glm/glm.hpp>
#include<list>

#include "P6/particle.h"
#include "forceRegistry.h"
#include "gravityForceGenerator.h"

namespace P6
{
	class PhysicsWorld
	{
	public:
		std::list<P6::Particle*> Particles;
		ForceRegistry forceRegistry;

		void AddParticle(Particle* toAdd);
		void Update(float time);

		std::list<P6::Particle*>& GetParticles();

	private:
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(glm::vec3(0.0f, -9.8f, 0.0f));

	};
}