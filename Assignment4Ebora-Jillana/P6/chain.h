#pragma once
#include "particleLink.h"
class Chain : public ParticleLink
{
	public:
		float maxLength = 1;

		// Chain links don't bounce - default to 0
		float restitution = 0.f;

		ParticleContact* GetContact() override;

};

