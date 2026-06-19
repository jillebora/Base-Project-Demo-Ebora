#include "particleContact.h"

void ParticleContact::Resolve(float time)
{
	ResolveVelocity(time);

	ResolveInterPenetration(time);

}

float ParticleContact::GetSeparatingSpeed()
{
	glm::vec3 velocity = particles[0]->Velocity;
	if (particles[1])
		velocity -= particles[1]->Velocity;


	return glm::dot(velocity, contactNormal);
}

void ParticleContact::ResolveInterPenetration(float time)
{
	if (depth <= 0.f)
		return;

	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1])
		totalMass += (float)1 / particles[1]->mass;

	if (totalMass <= 0)
		return;

	float totalMoveByMass = depth / totalMass;

	glm::vec3 moveByMass = contactNormal * totalMoveByMass;

	glm::vec3 P_a = moveByMass * ((float)1 / particles[0]->mass);

	particles[0]->Position += P_a;

	if (particles[1])
	{
		glm::vec3 P_b = moveByMass * ((float)-1 / particles[1]->mass);
		particles[1]->Position += P_b;
	}

	depth = 0;

}

void ParticleContact::ResolveVelocity(float time)
{
	float separatingSpeed = GetSeparatingSpeed();

	if (separatingSpeed > 0)
		return;

	float newSS = -restitiution * separatingSpeed;
	float deltaSpeed = newSS - separatingSpeed;

	float totalMass = (float)1 / particles[0]->mass;

	if (particles[1])
		totalMass += (float)1 / particles[1]->mass;

	if (totalMass <= 0)
		return;

	float impulse_mag = deltaSpeed / totalMass;

	// Get impulse vector using contact normal
	glm::vec3 Impulse = contactNormal * impulse_mag;

	// V = I/m
	glm::vec3 V_a = Impulse * ((float)1 / particles[0]->mass);

	// Vf = Vi + I/m
	particles[0]->Velocity = particles[0]->Velocity + V_a;

	if (particles[1])
	{
		// Apply impulse in the opp dir for B
		glm::vec3 V_b = Impulse * ((float)-1 / particles[1]->mass);

		// Vf = Vi + I/m
		particles[1]->Velocity = particles[1]->Velocity + V_b;
	}

}
