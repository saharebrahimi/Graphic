#include "CubeObject.h"
#include <stb_image.h>
float scale_x = 1;
float scale_y = 1;
float scale_z = 1;


CubeObject::CubeObject(GraphicDevice* device) : GameObject(device)
{
	float v[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int ii[] = {
		0, 1, 2,
		3, 4, 5,
		
		6, 7, 8,
		9, 10, 11,
		
		12, 13, 14,
		15, 16, 17,
		
		18, 19, 20,
		21, 22, 23,
		
		24, 25, 26,
		27, 28, 29,
		
		30, 31, 32,
		33, 34, 35
	};

	verticesSize = 180;
	indicesSize = 36;
	vertices = new float[verticesSize];
	indices = new unsigned int[indicesSize];
	for (size_t i = 0; i < verticesSize; i++)
	{
		vertices[i] = v[i];
	}
	for (size_t i = 0; i < indicesSize; i++)
	{
		indices[i] = ii[i];
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load("color.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void CubeObject::Update(GLFWwindow* window)
{
	glm::quat angle = GetQuaternion();

		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			angle = glm::rotate(angle, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			angle = glm::rotate(angle, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {

			angle = glm::rotate(angle, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			angle = glm::rotate(angle, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			angle = glm::rotate(angle, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			angle = glm::rotate(angle, glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		SetQuaternion(angle);

		auto scale = GetScale();
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			scale = scale+glm::vec3(0.001f, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			if (GetScale().x >= 0)
			scale = scale+glm::vec3(-0.001f, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {

			scale = scale+ glm::vec3(0.0f, 0.001f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (GetScale().y >= 0)
			scale =scale+ glm::vec3(0.0f, -0.001f, 0.0f);
		}
		
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			scale = scale+glm::vec3(0.0f, 0.0f, 0.001f);		
		}
	
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{
			if(GetScale().z >= 0)
			scale =scale+ glm::vec3(0.0f, 0.0f,-0.001f);
		}
		SetScale(scale);
		
		
	
}


void CubeObject::rotation(GLFWwindow* window,int obj)
{

	

}

//setup cube object here
void CubeObject::Setup()
{
//	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	auto angle = GetQuaternion();
	SetQuaternion(glm::rotate(angle, glm::radians(00.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	SetScale(glm::vec3(0.25f, 0.25f, 0.25f));

}
