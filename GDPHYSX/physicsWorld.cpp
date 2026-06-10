#include "physicsWorld.h"

void P6::PhysicsWorld::AddParticle(Particle* toAdd)
{
	Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}

void P6::PhysicsWorld::Update(float time)
{
	//Update List before doing everything else
	UpdateParticleList();

	forceRegistry.UpdateForces(time);
	for (std::list<P6::Particle*>::iterator p = Particles.begin(); p != Particles.end(); p++)
	{
		(*p)->Update(time);
	}
}

std::list<P6::Particle*>& P6::PhysicsWorld::GetParticles()
{
	return Particles;
}

void P6::PhysicsWorld::UpdateParticleList()
{
	Particles.remove_if([](P6::Particle* p) {return p->IsDestroyed(); });
}
