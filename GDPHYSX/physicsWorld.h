#pragma once

#include<list>

#include "P6/particle.h"


namespace P6
{
	class PhysicsWorld
	{
		public:
			std::list<P6::Particle*> Particles;

			void AddParticle(Particle* toAdd);
			void Update(float time);

			std::list<P6::Particle*>& GetParticles();

		private:
			void UpdateParticleList();

	};
}
