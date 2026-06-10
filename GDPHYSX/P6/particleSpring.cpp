#include "particleSpring.h"

void ParticleSpring::UpdateForce(P6::Particle* particle, float time)
{
	// Get the current position of the particle 
	glm::vec3 pos = particle->Position;

	//Create a vector Pos -> Anchor Point
	glm::vec3 force = pos - otherParticle->Position;

	// get the magnitudde of the vector above
	float mag = glm::length(force);

	// f = -k * (|lcurr - lrest|)
	float springForce = -springConstant * abs(mag - restLength);

	// get the direction form the particle to the anchor point
	force = (mag > 0.0f) ? glm::normalize(force) : glm::vec3(0.0f);

	// Apply the force using the direction above
	force = force * springForce;

	// Add force
	particle->AddForce(force);
}
