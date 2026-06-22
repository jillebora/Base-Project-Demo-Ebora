#pragma once

#include "../forceGenerator.h"

class AnchoredSpring : public ForceGenerator
{
	public:
		AnchoredSpring(glm::vec3 pos, float _springConst, float _restLen) : anchorPoint(pos), springConstant(_springConst), restLength(_restLen) {}

		void UpdateForce(P6::Particle* particle, float time) override;

	private:

		// point the other end of the spring is
		glm::vec3 anchorPoint;

		// How stiff the spring is
		float springConstant;
		
		// rest length of the spring
		float restLength;


};

