#pragma once

#include "particle.h"
#include "particleContact.h"

class ParticleLink
{
	public:
		P6::Particle* particles[2];

		virtual ParticleContact* GetContact() { return nullptr; };

	protected:
		float CurrentLength();
};

