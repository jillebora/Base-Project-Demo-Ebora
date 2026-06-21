#include "particle.h"


namespace P6
{
	void P6::Particle::AddForce(glm::vec3 force)
	{
		this->accumulatedForce += force;
	}

	void P6::Particle::UpdatePosition(float deltaTime)
	{
		// P2 = P1 + Vit + (At^2/2)

		glm::vec3 oldPos = this->Position;

		this->Position = this->Position + (this->Velocity * deltaTime) + 
			(1.0f / 2.0f) * (this->Acceleration * deltaTime * deltaTime);

	}

	void P6::Particle::ResetForce()
	{
		float d_mass = glm::max(std::numeric_limits<float>::min(), mass);

		this->Acceleration -= accumulatedForce * (1 / d_mass);
		this->accumulatedForce = glm::vec3(0, 0, 0);
	}

	void P6::Particle::UpdateVelocity(float deltaTime)
	{
		// lowest positive float != 0, prevents division by zero
		float d_mass = glm::max(std::numeric_limits<float>::min(), mass);
		
		// A = F / m 
		this->Acceleration += accumulatedForce * (1 / d_mass);

		// Vf = Vi + At
		this->Velocity = this->Velocity + (this->Acceleration * deltaTime);

		// Vfd = Vf * damping^t
		this->Velocity = this->Velocity * powf(damping, deltaTime);
	}

	void Particle::Destroy()
	{
		isDestroyed = true;
	}

	bool Particle::IsDestroyed()
	{
		return isDestroyed;
	}

	void Particle::Update(float time)
	{
		this->UpdatePosition(time);
		this->UpdateVelocity(time);

		//Reset the forces calculations
		this->ResetForce();
	}

	Particle::Particle()
	{
		this->Position = glm::vec3(0, 0, 0);
		this->Velocity = glm::vec3(0, 0, 0);
		this->Acceleration = glm::vec3(0, 0, 0);
		
		isDestroyed = false;

	}



}

