#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "Texture.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL-Training", nullptr, nullptr);

	if (window == nullptr)
		throw std::runtime_error("Failed to create GLFW window");
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		throw std::runtime_error("Failed to initialize GLAD");
	
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void drawTriangle(std::vector<Texture> textures)
{
	for (const Texture& texture : textures) {
		glActiveTexture(GL_TEXTURE0 + texture.ID - 1);
		glBindTexture(GL_TEXTURE_2D, texture.ID);
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void createBuffers()
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	constexpr float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	constexpr unsigned int indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void mainLoop(GLFWwindow *window)
{
	Shader triangleShader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
	std::vector<Texture> textures;
	Texture triangleTexture("textures/wall.jpg");
	Texture faceTexture("textures/awesomeface.png", true);
	textures.push_back(triangleTexture);
	textures.push_back(faceTexture);
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		triangleShader.use();
		//triangleShader.setFloat("time", 0 /*glfwGetTime()*/);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glm::radians(glfwGetTime() * 125.0f) , glm::vec3(0.0f, 0.0f, 1.0f));
		triangleShader.setInt("ourTexture", 0);
		triangleShader.setInt("secondTexture", 1);
		triangleShader.setTransform("transform", trans);
		drawTriangle(textures);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main(int argc, char** argv)
{
	GLFWwindow *window = createWindow(800, 600);
	createBuffers();
	mainLoop(window);

	glfwTerminate();
	
	return 0;
}