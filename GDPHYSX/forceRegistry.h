#pragma once

#include <list>
#include "P6/particle.h"
#include "forceGenerator.h"

class ForceRegistry
{
	protected:
		struct ParticleForceRegistry
		{
			P6::Particle* particle;
			ForceGenerator* generator;
		};

		std::list<ParticleForceRegistry> Registry;

	public:
		void Add(P6::Particle* particle, ForceGenerator* generator);
		
		void Remove(P6::Particle* particle, ForceGenerator* generator);

		void Clear();

		void UpdateForces(float time);

};