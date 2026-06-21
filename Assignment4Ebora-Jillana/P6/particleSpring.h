#pragma once

#include "../forceGenerator.h"

class ParticleSpring : public ForceGenerator
{
	public:
		// Constructor for the spring
		ParticleSpring(P6::Particle* particle, float _springConst, float _restLen) : 
			otherParticle(particle), springConstant(_springConst),restLength(_restLen) {}

		void UpdateForce(P6::Particle* particle, float time) override;
	private:
		//the other particle on the other end of the spring
		P6::Particle* otherParticle;

		// How stiff the spring is
		float springConstant;

		// rest length of the spring
		float restLength;

};
