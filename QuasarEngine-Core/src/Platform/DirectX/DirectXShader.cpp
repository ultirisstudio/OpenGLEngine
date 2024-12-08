#include "qepch.h"
#include "DirectXShader.h"

#include <fstream>
#include <d3d11.h>

namespace QuasarEngine
{
	/*static HRESULT D3DX11CompileFromFile(
		LPCTSTR pSrcFile,
		D3D10_SHADER_MACRO* pDefines,
		LPD3D10INCLUDE pInclude,
		LPCSTR pFunctionName,
		LPCSTR pProfile,
		UINT Flags1,
		UINT Flags2,
		ID3D10Blob** ppShader,
		ID3D10Blob** ppErrorMsgs,
		HRESULT* pHResult)
	{

	}*/

	std::string DirectXShader::readFile(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.is_open())
			return "";

		std::ostringstream stream;
		stream << file.rdbuf();

		std::string str = stream.str();

		file.close();

		return str;
	}

	DirectXShader::DirectXShader(ShaderFile files)
	{
		ShaderSource sources;

		if (!files.vertexShaderFile.empty())
		{
			sources.vertexShaderSource = readFile(files.vertexShaderFile);
		}

		if (!files.fragmentShaderFile.empty())
		{
			sources.fragmentShaderSource = readFile(files.fragmentShaderFile);
		}

		if (!files.geometryShaderFile.empty())
		{
			sources.geometryShaderSource = readFile(files.geometryShaderFile);
		}

		if (!files.computeShaderFile.empty())
		{
			sources.computeShaderSource = readFile(files.computeShaderFile);
		}

		if (!files.tessControlShaderFile.empty())
		{
			sources.tessControlShaderSource = readFile(files.tessControlShaderFile);
		}

		if (!files.tessEvaluationShaderFile.empty())
		{
			sources.tessEvaluationShaderSource = readFile(files.tessEvaluationShaderFile);
		}

		createShader(sources);
	}

	DirectXShader::DirectXShader(ShaderSource sources)
	{
		createShader(sources);
	}

	DirectXShader::~DirectXShader()
	{
		
	}

	void DirectXShader::createShader(ShaderSource sources)
	{
			
	}

	void DirectXShader::setUniform(const std::string& name, bool value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, uint32_t value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, int value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, float value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, double value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const glm::vec3& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const glm::mat3& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const glm::mat4& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<bool>& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<unsigned int>& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<int>& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<float>& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<double>& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<glm::vec3>& value) const
	{
		
	}

	void DirectXShader::setUniform(const std::string& name, const std::vector<glm::mat4>& value) const
	{
		
	}

	void DirectXShader::use() const
	{
		
	}

	void DirectXShader::unuse() const
	{
		
	}
}