#pragma once
#include "P6/particle.h"

// Base class that computes the force
// needed to be applied to the target particle

class ForceGenerator
{
	public:
		// Override later
		virtual void UpdateForce(P6::Particle* p, float time)
		{
			// Base Definition in case
			p->AddForce(glm::vec3(0, 0, 0));
		}

		
};