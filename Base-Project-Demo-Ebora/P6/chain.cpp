#include "chain.h"

ParticleContact* Chain::GetContact()
{
	float currLen = CurrentLength();

	if (currLen < +maxLength)
		return nullptr;

	if (currLen == 0.f)
		return nullptr;

	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = particles[0];
	ret->particles[1] = particles[1];

	glm::vec3 dir = particles[1]->Position - particles[0]->Position;
	dir = glm::normalize(dir);

	ret->contactNormal = dir;
	ret->depth = currLen - maxLength;

	return ret;
}