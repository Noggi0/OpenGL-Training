#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>
#include <stdexcept>
#include <iostream>

class Texture
{
public:
	unsigned int ID;
	Texture(const std::string &path, bool flipped = false);
	~Texture() = default;
};

#endif // !TEXTURE_HPP