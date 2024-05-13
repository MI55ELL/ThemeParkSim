#ifndef DUCK_H
#define DUCK_H

 #include <glm/glm.hpp>

class Duck
{
	public:
		glm::vec3 position;
		glm::float32 scale;
		glm::float32 angle;
		glm::float32 direction = 1;

		Duck(glm::vec3 Position = glm::vec3(0.0f), glm::float32 Scale = 1, glm::float32 Angle = 0) 
		{
			position = Position;
			scale = Scale;
			angle = Angle;
		}

		void Matrix(Shader& shader, const char* uniform) {
			glm::mat4 model = glm::translate(glm::mat4(1), position); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(scale));    // it's a bit too big for our scene, so scale it down
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(model));
		}

		void RaceDuck()
		{
			position.z += 0.02f*direction;
			if (position.z >= -1.5f)
			{
				angle = 180.0f; 
				direction = -1;
			}
			if (position.z == -14.000f) 
			{
				angle = 180.0f;
				position.z = -14.0f;
			}
		}
};
#endif