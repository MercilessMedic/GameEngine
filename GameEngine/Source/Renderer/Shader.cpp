#include "Shader.h"



Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::ifstream vertexFile(vertexPath);
	std::ifstream fragmentFile(fragmentPath);


    if( !vertexFile )
    {
        std::cerr << "[ERROR] [SHADER] Failed to read Vertex file: " << vertexPath << std::endl;

    }

    if( !fragmentFile )
    {
        std::cerr << "[ERROR] [SHADER] Failed to read Fragment file:  " << fragmentPath << std::endl;
    }

    if( vertexFile && fragmentFile )
    {
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        std::string vertexCode;
        std::string fragmentCode;

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        //Create vertex Shader
        unsigned int vertex;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, Vertex);

        //Create fragment shader
        unsigned int fragment;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, Fragment);

        //Create the shader program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        checkCompileErrors(id, Program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
}

void Shader::use()
{
	glUseProgram( id );
}

unsigned int Shader::getId()
{
    return id;
};


void Shader::setBool(const std::string& name, bool value) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform1i( location, (int)value );
    }
}

void Shader::setInt(const std::string& name, int value) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform1i( location, value );
    }
}

void Shader::setFloat(const std::string& name, float value) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform1f( location, value );
    }
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform2fv(location, 1, &value[0]);
    }
}

void Shader::setVec2(const std::string& name, float x, float y) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform2f( location, x, y );
    }
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) 
{
    int location = getUniformLocation( name );
    if( location != -1 )
    {
        glUniform3fv( location, 1, &value[0] );
    }
}

void Shader::setVec3(const std::string& name, float x, float y, float z) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform3f(location, x, y, z);
    }
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform4fv( location, 1, &value[0] );
    }
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniform4f(location, x, y, z, w);
    }
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniformMatrix2fv( location , 1, GL_FALSE, &mat[0][0] );
    }
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) 
{
    int location = getUniformLocation(name);
    if( location != -1 )
    {
        glUniformMatrix3fv( location , 1, GL_FALSE, &mat[0][0]);
    }
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) 
{
   
    int location = getUniformLocation( name );
    if( location != -1 ) 
    {
        glUniformMatrix4fv( location, 1, GL_FALSE, &mat[0][0] );
    }
}

int Shader::getUniformLocation(const std::string& name )
{
    if( uniformLocationCache.find(name) != uniformLocationCache.end() )
    {
        return uniformLocationCache[name];
    }

    GLint location = glGetUniformLocation( id, name.c_str() );
    uniformLocationCache[name] = location;

    if( location == -1 )
    {
        std::cout << "[WARNING] [SHADER] Could not find uniform by name:" << name << "\n" << "In shader by ID:" << id << "\n";
    }
    

    return location;
}

void Shader::checkCompileErrors(unsigned int shader, ShaderType type)
{
    int success;
    char infoLog[1024];
    std::string shaderTypeName;
    if(type != Program )
    {
        if( type == Vertex ){ shaderTypeName = "Vertex"; }
        else { shaderTypeName = "Fragment"; }
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if( !success )
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "[ERROR] [SHADER] compilation error of type: " << shaderTypeName << "\n" << infoLog << "\n";
        }
    }
    else
    {
        shaderTypeName = "Program";
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if( !success )
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "[ERROR] [SHADER] linking error of type: " << shaderTypeName << "\n" << infoLog << "\n";
        }
    }
}