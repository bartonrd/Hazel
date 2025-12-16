#include "Shader.h"
#include "OpenGLLoader.h"
#include "../Log.h"
#include <vector>

namespace Hazel {

	static unsigned int CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> infoLog(length);
			glGetShaderInfoLog(shader, length, &length, infoLog.data());
			HZ_ERROR("Shader compilation failed: " + std::string(infoLog.data()));
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Compile shaders
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

		// Link shaders
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		int success;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			int length;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> infoLog(length);
			glGetProgramInfoLog(m_RendererID, length, &length, infoLog.data());
			HZ_ERROR("Shader linking failed: " + std::string(infoLog.data()));
		}

		// Clean up
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		HZ_INFO("Shader created successfully");
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

}
