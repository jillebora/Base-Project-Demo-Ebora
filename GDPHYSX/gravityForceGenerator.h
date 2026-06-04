#pragma once
#include "forceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
	private:
		glm::vec3 Gravity = glm::vec3(0, -9.8f, 0);

	public:
		// COnstructor for the gracity force generator
		GravityForceGenerator(const glm::vec3& gravity) : Gravity(gravity) {}

		// Override the UpdateForce function of the base class
		void UpdateForce(P6::Particle* particle, float time) override;
};