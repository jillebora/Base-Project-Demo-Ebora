#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace std;

// Encapsulates OpenGL shader program
class Shader
{
	private:
		// Reads a shader source file and returns
		string loadFile(const char* path) const;

		// Validates shader compilation and program
		void checkCompileErrors(GLuint shader, const string& type) const;

	public:
		// OpenGL shader program identifier.
		GLuint ID;

		// Creates an empty shader object.
		Shader();

		// Loads, compiles, and links the specified vertex and fragment shaders
		Shader(const char* vertexPath, const char* fragmentPath);

		// Destructor
		~Shader();

		// Activates this shader program for rendering.
		void use() const;

		// Returns the OpenGL shader program ID.
		GLuint getID() const { return ID; }

		// Releases the OpenGL shader program.
		void destroy();

		// Uniform setters
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, const glm::vec3& v) const;
		void setMat4(const std::string& name, const glm::mat4& m) const;

};