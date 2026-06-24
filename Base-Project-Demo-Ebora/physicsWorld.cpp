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

void P6::PhysicsWorld::AddContact(P6::Particle* p1, P6::Particle* p2, float restitiution, glm::vec3 contactNormal, float depth)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;

	toAdd->restitiution = restitiution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	Contacts.push_back(toAdd);
}

void P6::PhysicsWorld::GenerateContacts()
{
	for (ParticleContact* c : Contacts)
		delete c;

	Contacts.clear();

	GetOverlaps();

	for (std::list<ParticleLink*>::iterator i = Links.begin(); i != Links.end(); i++)
	{
		ParticleContact* contact = (*i)->GetContact();

		if (contact != nullptr)
		{
			Contacts.push_back(contact);
		}
	}

}

void P6::PhysicsWorld::GetOverlaps()
{
	for (int i = 0; i < Particles.size() - 1; i++)
	{
		std::list<P6::Particle*>::iterator a = std::next(Particles.begin(), i);

		for (int h = i + 1; h < Particles.size(); h++)
		{
			std::list<P6::Particle*>::iterator b = std::next(Particles.begin(), h);

			glm::vec3 mag2Vector = (*a)->Position - (*b)->Position;

			float mag2 = glm::dot(mag2Vector, mag2Vector);

			float rad = (*a)->radius + (*b)->radius;

			float rad2 = rad * rad;


			if (mag2 <= rad2)
			{
				glm::vec3 dir = glm::normalize(mag2Vector);

				float r = rad2 - mag2;
				float depth = sqrt(r);

				float restitution = fmin((*a)->restitution, (*b)->restitution);

				AddContact(*a, *b, restitution, dir, depth);

			}

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

