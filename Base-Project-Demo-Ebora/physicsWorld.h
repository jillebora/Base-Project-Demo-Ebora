#pragma once

#include <glm/glm.hpp>
#include<list>

#include "P6/particle.h"
#include "P6/particleLink.h"
#include "P6/contactResolver.h"
#include "forceRegistry.h"
#include "gravityForceGenerator.h"

namespace P6
{
	class PhysicsWorld
	{
		public:
			std::list<P6::Particle*> Particles;
			ForceRegistry forceRegistry;
			std::vector<ParticleContact*> Contacts;
			std::list<ParticleLink*> Links;

			void AddParticle(Particle* toAdd);
			void Update(float time);

			std::list<P6::Particle*>& GetParticles();

			void AddContact(P6::Particle* p1, P6::Particle* p2, float restitiution, glm::vec3 contactNormal, float depth);

		protected:
			void GenerateContacts();

			void GetOverlaps();

		private:
			void UpdateParticleList();

			GravityForceGenerator Gravity = GravityForceGenerator(glm::vec3(0.0f, 10.f, 0.0f));
			ContactResolver contactResolver = ContactResolver(20);

	};
}
