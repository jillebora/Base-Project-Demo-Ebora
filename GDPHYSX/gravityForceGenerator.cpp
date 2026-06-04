#include "gravityForceGenerator.h"

void GravityForceGenerator::UpdateForce(P6::Particle* particle, float time)
{
	// Do nothing if mass is below 0
	if (particle->mass <= 0.f) return;

	// F = A * m
	glm::vec3 force = Gravity * particle->mass;
	particle->AddForce(force);
}
