#include"ModelMesh.h"
#include"mesh.h"
#include"Model.h"
#include"Duck.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


// sets an initial window width and height, this can and will be changes if the user changes the size of their screen 
const unsigned int width = 1000;
const unsigned int height = 1000;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void screenResize(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};



Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

float skyBoxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyBoxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "Hook A Duck", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, screenResize);

	// Load GLAD so it configures OpenGL
	gladLoadGL();


	Texture textures[]
	{
		Texture("resources/textures/grass.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("resources/textures/grassSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");
	Shader skyBoxShader("resources/shaders/skyBox.vert", "resources/shaders/skyBox.frag");
	Shader modelShader("resources/shaders/modelShader.vert", "resources/shaders/modelShader.frag");
	
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);
	



	// Shader for light cube
	Shader lightShader("resources/shaders/light.vert", "resources/shaders/light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	Mesh light(lightVerts, lightInd, tex);



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	skyBoxShader.Activate();
	glUniform1i(glGetUniformLocation(skyBoxShader.ID, "skyBox"), 0);

	modelShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));

	// model loading 
	stbi_set_flip_vertically_on_load(true);
	Model duck("resources/models/duck/lowPolyDuck/lowPolyDuck.obj");
	Model pool("resources/models/duckPool/scene.obj");

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creating a camera object and settign its intital start position, width of screen and height of screen 
	Camera camera(width, height, glm::vec3(0.0f, 5.0f, 0.0f));

	unsigned int skyBoxVAO, skyBoxVBO, skyBoxEBO;
	// passing number of generated vertex arrays and passing where we want it to be stored 
	// vertex array object used to manage vertex buffer objects and element buffer objects
	// holds pointers to the EBO and VBO 
	glGenVertexArrays(1, &skyBoxVAO);
	// generates buffers to to hold VBO and EBO data in memory
	// vetex buffer object - holds vertices
	glGenBuffers(1, &skyBoxVBO);
	// element buffer object - holds indices
	glGenBuffers(1, &skyBoxEBO);
	// setting the VAO array to be the current object puttin it into global context so everything can modify it 
	glBindVertexArray(skyBoxVAO);
	// giving the buffer object a type of gl_array_buffer, so it knows how to handel the skyBox vertices
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	// binds data of the VBO (vertices) with the VAO so makes the array of the VAO hold the pointer to the vertex buffer object (the indices)
	// giving the VAO array the amount of the data the pointer is pointing to, reference to the first value in the pointer and telling opengl how to draw it (do not chnge vbertices)
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), &skyBoxVertices, GL_STATIC_DRAW);
	// giving the buffer object a type of element_array_buffer so opengl knows how to handel the skyBox indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyBoxEBO);
	// giving the VAO array the type of buffer we would like to store reference to, the amount of data that reference is pointing to, the refernce pointer to the first value in EBO and telling opengl not to change indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyBoxIndices), &skyBoxIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// array of 6 strings holding paths to 6 images 
	std::string facesCubeMap[6] =
	{
		"resources/textures/skybox/right.jpg",
		"resources/textures/skybox/left.jpg", //
		"resources/textures/skybox/top.jpg", //
		"resources/textures/skybox/bottom.jpg", //
		"resources/textures/skybox/front.jpg", //
		"resources/textures/skybox/back.jpg" //
	};

	//creating a cube map texture 
	unsigned int cubeMapTexture;
	glGenTextures(1, &cubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubeMap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "failed to load texture: " << facesCubeMap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	// Initialise before loop
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Duck idolDuck{ glm::vec3(50.0f, 30.0f, -9.0f), 2.0f, -40.0f}; 
	Duck Duck1{ glm::vec3(-5.0f, -2.0f, -14.0f), 0.4f, 0.0f};
	Duck Duck2{ glm::vec3(5.0f, -2.0f, -14.0f), 0.4f, 0.0f};
	Duck Duck3{ glm::vec3(0.0f, -2.0f, -14.0f), 0.4f, 0.0f};

	struct {
		glm::vec3 position;
		glm::float32 radius;
		glm::float32 angle;
		uint count;
	} TeaCup;


	// Main while loop
	while (!glfwWindowShouldClose(window))

	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		// Draws different meshes
		// floor.Draw(shaderProgram, camera);
		//light.Draw(lightShader, camera);
		modelShader.Activate();
		camera.Matrix(modelShader, "camMatrix");
		// drawing objects 
		pool.Draw(modelShader);
		idolDuck.Matrix(modelShader, "model");
		duck.Draw(modelShader);
		Duck1.Matrix(modelShader, "model");
		duck.Draw(modelShader);
		Duck2.Matrix(modelShader, "model");
		duck.Draw(modelShader);
		Duck3.Matrix(modelShader, "model");
		duck.Draw(modelShader);

		Duck1.RaceDuck();
		Duck2.RaceDuck();
		Duck3.RaceDuck();

		glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));


		glDepthFunc(GL_LEQUAL);
		skyBoxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(105.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyBoxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyBoxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyBoxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DragFloat3("Position", glm::value_ptr(Duck3.position));
		ImGui::InputFloat("scale", &Duck3.scale);
		ImGui::InputFloat("angle", &Duck3.angle);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	skyBoxShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}