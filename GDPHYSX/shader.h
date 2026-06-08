#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace std;

class Shader
{
	private:
		string loadFile(const char* path) const;
		void checkCompileErrors(GLuint shader, const string& type) const;

	public:
		GLuint ID;

		Shader();

		Shader(const char* vertexPath, const char* fragmentPath);

		~Shader();

		void use() const;
		GLuint getID() const { return ID; }

		void destroy();

		// Uniform setters
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, const glm::vec3& v) const;
		void setMat4(const std::string& name, const glm::mat4& m) const;

};