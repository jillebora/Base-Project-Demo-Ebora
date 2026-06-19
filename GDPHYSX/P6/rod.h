#pragma once
#include "particleLink.h"
#include "particle.h"

class Rod : public ParticleLink
{
	public:
		float length = 1;

		// Set to  0 since rod has no bounciness
		float restitution = 0;

		ParticleContact* GetContact() override;

};

