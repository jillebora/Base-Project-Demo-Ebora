#pragma once

#include <glm//glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6
{
	class Particle
	{
		protected:
			void UpdatePosition(float deltaTime);
			void UpdateVelocity(float deltaTime);

		public:
			glm::vec3 Position;
			glm::vec3 Velocity;
			glm::vec3 Acceleration;

			void Update(float deltaTime);

			Particle();

	};

}
