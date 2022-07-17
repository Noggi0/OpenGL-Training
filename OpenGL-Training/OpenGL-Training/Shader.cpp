#include "Shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	try {
		std::ifstream vShaderFile(vertexPath, std::ios::binary);
		std::ifstream fShaderFile(fragmentPath, std::ios::binary);

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "Error : Failed to load shader !" << std::endl;
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		throw std::runtime_error("Failed to compile vertex shader: " + vertexPath + "\nerror: " + std::string(infoLog));
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		throw std::runtime_error("Failed to compile fragment shader: " + fragmentPath + "\nerror: " + std::string(infoLog));
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(this->ID, 512, nullptr, infoLog);
		throw std::runtime_error("Failed to link shader program" + std::string(infoLog));
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
};

void Shader::use()
{
	glUseProgram(this->ID);
};

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
};

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setTransform(const std::string& name, const glm::mat4 &transform) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}
