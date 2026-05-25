#include "particle.h"


namespace P6
{
	void P6::Particle::UpdatePosition(float deltaTime)
	{
		// P2 = P1 + Vit + (At^2/2)
		this->Position = this->Velocity + (this->Velocity * deltaTime) + 
			(1.0f / 2.0f) * (this->Acceleration * deltaTime * deltaTime);


	}

	void P6::Particle::UpdateVelocity(float deltaTime)
	{
		// Vf = Vi + At
		this->Velocity = this->Velocity + (this->Acceleration * deltaTime);
	}

	void Particle::Update(float time)
	{
		this->UpdatePosition(time);
		this->UpdateVelocity(time);
	}

	Particle::Particle()
	{
		this->Position = glm::vec3(0, 0, 0);
		this->Velocity = glm::vec3(0, 0, 0);
		this->Acceleration = glm::vec3(0, 0, 0);
	}



}

