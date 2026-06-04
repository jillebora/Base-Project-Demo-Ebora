#include "dragForceGenerator.h"

void DragForceGenerator::UpdateForce(P6::Particle* particle, float time)
{
	glm::vec3 force = glm::vec3(0, 0, 0);

	glm::vec3 currV = particle->Velocity;

	// use glm magnitude
	float mag = glm::length(currV);

	if (mag <= 0) return;

	float dragF = (k1 * mag) + (k2 * mag);

	// use glm normalize
	glm::vec3 dir = glm::normalize(currV);

	particle->AddForce(dir * -dragF);

}
