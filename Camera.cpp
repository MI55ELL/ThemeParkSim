#include"Camera.h"


Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null ma
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}


void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	
	//p pressed to enter first person view
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		 PPressed = true;
	}
	// p is now true so it enters first person mode
	if (PPressed)
	{
		// where if the user tries to move up and down the speed is set to 0 meaning they cannot move up and down from their sparw point 
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS))
		{
			speed = 0;
			std::cout << "   x= " << Position.x << "       z= " << Position.z << std::endl;
		}

		//		x			z
		// (-18.0122, -24.3686)
		// ( 16.1871, -24.7544)
		// ( 16.0207,  9.24621)
		// (-18.5137,  9.29644)
		
		if ((Position.x < -18) || (Position.x > 16))
		{
			speed = 0.2;
			Position += speed * -Up;
			
		}
		
		if ((Position.z < -24) || (Position.z > 9)) 
		{
			speed = 0.2;
			Position += speed * -Up;
		}
		if ((Position.y < -3) || (Position.y > 5))
		{
			speed = 0.2;
			Position += speed * -Up;
		}
		////		x			z
		// (-8.567934, -2.71143)
		// (-8.62208, -13.4118)
		// ( 9.12965, -13.2812)
		// ( 8.17749,  -2.50703)
		//if (((Position.x > -8) && (Position.x < 8)) && ((Position.z > -13) && (Position.z < -2)))
		//{
			
		//}
	

		// TODO: - implement jump
		// TODO: - implement collisions with floor 
		// if o is pressed the camera will leave first person mode and instead use free camera mode
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			PPressed = false;
		}
		auto newOrientation = Orientation;
		newOrientation.y = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += speed * newOrientation;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += speed * -glm::normalize(glm::cross(newOrientation, Up));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position += speed * -newOrientation;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += speed * glm::normalize(glm::cross(newOrientation, Up));
		}

	}
	else 
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += speed * Orientation;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += speed * -glm::normalize(glm::cross(Orientation, Up));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position += speed * -Orientation;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += speed * glm::normalize(glm::cross(Orientation, Up));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Position += speed * Up;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			Position += speed * -Up;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			speed = 0.4f;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		{
			speed = 0.2f;
		}
	}



	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
	

}