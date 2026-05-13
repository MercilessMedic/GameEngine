#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

class Shader
{
public:
	//Used for checking compilation errors 
	enum ShaderType
	{
		Program,
		Vertex,
		Fragment
	};

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	unsigned int getId();

	//The methods below set the uniforms of a shader program
	//------------------------------------------------------
	void setBool(const std::string& name, bool value);					      

	void setInt(const std::string& name, int value);

	void setFloat(const std::string& name, float value);

	void setVec2(const std::string& name, const glm::vec2& value);

	void setVec2(const std::string& name, float x, float y);

	void setVec3(const std::string& name, const glm::vec3& value);

	void setVec3(const std::string& name, float x, float y, float z);

	void setVec4(const std::string& name, const glm::vec4& value);

	void setVec4(const std::string& name, float x, float y, float z, float w);

	void setMat2(const std::string& name, const glm::mat2& mat);				  

	void setMat3(const std::string& name, const glm::mat3& mat);				  

	void setMat4(const std::string& name, const glm::mat4& mat);				  


private:
	unsigned int id;
	void checkCompileErrors(unsigned int shader, ShaderType type);
	int getUniformLocation(const std::string& name);
	std::unordered_map<std::string, int> uniformLocationCache;
	
};

