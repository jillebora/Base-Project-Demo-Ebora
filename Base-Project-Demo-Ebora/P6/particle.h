#pragma once

#include <glm//glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace P6
{
	class Particle
	{
		private:
			std::string Name;

		protected:
			glm::vec3 accumulatedForce = glm::vec3(0, 0, 0);

			bool isDestroyed = false;

			void UpdatePosition(float deltaTime);
			void UpdateVelocity(float deltaTime);

		public:

			glm::vec3 Position;
			glm::vec3 StartPosition;
			glm::vec3 Velocity;
			glm::vec3 Acceleration;

			void AddForce(glm::vec3 force);
			void ResetForce();

			float mass = 1.f;	//KG

			float radius = 1.f;

			// How bouncy the particle is
			float restitution = 1.f;

			// Default damping = or "Air Drag"
			float damping = 0.9f;

			void Destroy();

			bool IsDestroyed();

			void Update(float deltaTime);

			static glm::vec3 makeVec(float scalar, glm::vec3 start, glm::vec3 target = glm::vec3(0.f))
			{
				// move towards the center
				return scalar * glm::normalize(target - start);
			}

			void setName(std::string n) { Name = n; }
			std::string getName() { return Name; }
		
			Particle();

	};

}
