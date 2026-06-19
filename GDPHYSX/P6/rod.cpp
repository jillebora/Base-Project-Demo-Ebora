#include "rod.h"
#include "rod.h"

ParticleContact* Rod::GetContact()
{
	float currLen = CurrentLength();

	if (currLen == length)
		return nullptr;

	if (currLen == 0.f)
		return nullptr;
	
	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = particles[0];
	ret->particles[1] = particles[1];

	glm::vec3 dir = particles[1]->Position - particles[0]->Position;
	dir = glm::normalize(dir);

	if (currLen > length)
	{
		ret->contactNormal = dir;
		ret->depth = currLen - length;
	}
	else
	{
		ret->contactNormal = dir * -1.f;
		ret->depth = length - currLen;
	}

	ret->restitiution = restitution;

	return ret;

}
