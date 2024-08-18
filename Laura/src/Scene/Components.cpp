#include "Components.h"


namespace Laura 
{
	namespace TransformHandler
	{
		void Rotate(TransformComponent& transform, glm::vec3& angles) 
		{
			glm::mat4 rotation{1.0f};
			
			rotation = glm::rotate(rotation, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
			rotation = glm::rotate(rotation, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
			rotation = glm::rotate(rotation, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));

			transform.transform = rotation;
		}

		void Translate(TransformComponent& transform, const glm::vec3& translation)
		{
			transform = glm::translate(glm::mat4(1.0f), translation);
		}

		void Scale(TransformComponent& transform, const glm::vec3& scale)
		{
			transform = glm::scale(glm::mat4(1.0f), scale);
		}
	}

}