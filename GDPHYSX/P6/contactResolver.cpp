#include "contactResolver.h"
#include "contactResolver.h"

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
{
	current_iterations = 0;

	while (current_iterations < max_iterations)
	{
		float min_separating_speed = 0.0f;
		int contactIndex = -1;

		for (int i = 0; i < contacts.size(); i++)
		{
			float separating_speed = contacts[i]->GetSeparatingSpeed();

			if ((separating_speed < min_separating_speed) && (separating_speed < 0.0f || contacts[i]->depth > 0.0f))
			{
				min_separating_speed = separating_speed;
				contactIndex = i;
			}
		}

		if (contactIndex == -1)
			break;

		contacts[contactIndex]->Resolve(time);

		current_iterations++;

	}

}
