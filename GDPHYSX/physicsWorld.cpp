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

	GenerateContacts();

	if (Contacts.size() > 0)
	{
		contactResolver.ResolveContacts(Contacts, time);
	}
}

std::list<P6::Particle*>& P6::PhysicsWorld::GetParticles()
{
	return Particles;
}

void P6::PhysicsWorld::GenerateContacts()
{
	for (ParticleContact* c : Contacts)
		delete c;

	Contacts.clear();

	for (std::list<ParticleLink*>::iterator i = Links.begin(); i != Links.end(); i++)
	{
		ParticleContact* contact = (*i)->GetContact();

		if (contact != nullptr)
		{
			Contacts.push_back(contact);
		}
	}

}

void P6::PhysicsWorld::UpdateParticleList()
{
	// Removes ALL particles in list
	// where is Dest.is true
	Particles.remove_if(
		[](Particle* p)
		{
			return p->IsDestroyed();
		}

	);
	// It resizes the list after as well
}

