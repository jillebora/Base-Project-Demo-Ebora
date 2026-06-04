#include "forceRegistry.h"

void ForceRegistry::Add(P6::Particle* particle, ForceGenerator* generator)
{
	// Create a struct to be Added
	ParticleForceRegistry toAdd;

	//Assign the pair
	toAdd.particle = particle;
	toAdd.generator = generator;

	Registry.push_back(toAdd);
}

void ForceRegistry::Remove(P6::Particle* particle, ForceGenerator* generator)
{
	Registry.remove_if([particle, generator](ParticleForceRegistry reg)
		{
			return reg.particle == particle && reg.generator == generator;
		}
	);
}

void ForceRegistry::Clear()
{
	Registry.clear();
}

void ForceRegistry::UpdateForces(float time)
{
	for (std::list<ParticleForceRegistry>::iterator i = Registry.begin();
		i != Registry.end(); i++)
	{
		i->generator->UpdateForce(i->particle, time);

	}

}
