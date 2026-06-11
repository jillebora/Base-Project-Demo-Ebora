#pragma once
#include "P6/particle.h"
#include <vector>

class ParticleContact
{
	public:
		P6::Particle* particles[2];
		
		float restitiution;

		float depth;

		glm::vec3 contactNormal;

		void Resolve(float time);
		float GetSeparatingSpeed();

	protected:
		void ResolveInterPenetration(float time);	

		void ResolveVelocity(float time);
};

