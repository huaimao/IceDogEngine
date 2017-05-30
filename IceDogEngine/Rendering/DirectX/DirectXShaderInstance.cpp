#include "DirectXShaderInstance.h"

using namespace IceDogRendering;

DirectXShaderInstance::DirectXShaderInstance(std::string url, ShaderType st):ShaderInstance(url, st)
{
	r_deviceShaderPtr = nullptr;
	r_cbPerFrame = nullptr;
	r_cbPerObject = nullptr;
}

DirectXShaderInstance::~DirectXShaderInstance()
{
	ReleaseCOM(r_deviceShaderPtr);
	ReleaseCOM(r_cbPerFrame);
	ReleaseCOM(r_cbPerObject);
}

void DirectXShaderInstance::Init(PlatformDependenceRenderResource pdrr, std::string x86_compiler, std::string x64_compiler)
{
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	r_pdrr = pdrr;
	// compile the shader
	// construct the compile command
	std::cout << "Compile Shader:" << c_url << std::endl;
	std::string compile_cmd;
	std::string compile_mode = "";
	std::string compile_type = "";
	std::string compile_tag = c_url;
	compile_tag = compile_tag.replace(compile_tag.find(".hlsl"), std::string(".hlsl").size(), ".cso");
#ifdef _WIN32
	compile_cmd = x86_compiler + " ";
#endif
#ifdef _WIN64
	compile_cmd = x64_compiler + " ";
#endif 
#ifdef _DEBUG
	compile_mode = "/Od /Zi ";
#endif
	std::cout << "Using Compiler:" << compile_cmd << std::endl;
	
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		compile_type = "ps_5_0 ";
		break;
	case IceDogRendering::ShaderType::VS:
		compile_type = "vs_5_0 ";
		break;
	case IceDogRendering::ShaderType::CS:
		compile_type = "cs_5_0 ";
		break;
	case IceDogRendering::ShaderType::GS:
		compile_type = "gs_5_0 ";
		break;
	}

	compile_cmd += compile_mode;
	compile_cmd += "/T ";
	compile_cmd += compile_type;
	compile_cmd += "/Fo ";
	compile_cmd += compile_tag + " ";
	compile_cmd += c_url;

	// compile the shader
	if (system(compile_cmd.c_str()))
	{
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "Compile Failed!" << std::endl;
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
		return;
	}
	std::cout << std::endl;

	// read the shader
	r_shaderCode = IceDogUtils::ReadAllBytes(compile_tag.c_str());

	bool failed_flag = false;
	// create the shader
	switch (c_shaderType)
	{
	case IceDogRendering::ShaderType::PS:
		ID3D11PixelShader* ps_ptr;
		if (ISFAILED(pdrr.r_device->CreatePixelShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &ps_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Pixel Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = ps_ptr;
		break;
	case IceDogRendering::ShaderType::VS:
		ID3D11VertexShader* vs_ptr;
		if (ISFAILED(pdrr.r_device->CreateVertexShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &vs_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Vertex Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = vs_ptr;
		break;
	case IceDogRendering::ShaderType::CS:
		ID3D11ComputeShader* cs_ptr;
		if (ISFAILED(pdrr.r_device->CreateComputeShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &cs_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Compute Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = cs_ptr;
		break;
	case IceDogRendering::ShaderType::GS:
		ID3D11GeometryShader* gs_ptr;
		if (ISFAILED(pdrr.r_device->CreateGeometryShader(r_shaderCode.data(), r_shaderCode.size(), NULL, &gs_ptr)))
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "Create Geometry Shader Failed!" << std::endl;
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
			failed_flag = true;
		}
		r_deviceShaderPtr = gs_ptr;
		break;
	}
	if (failed_flag)
	{
		throw "Create Shader Failed!";
	}
}

void IceDogRendering::DirectXShaderInstance::Close()
{
	ReleaseCOM(r_deviceShaderPtr);
	ReleaseCOM(r_cbPerFrame);
	ReleaseCOM(r_cbPerObject);
	ShaderInstance::Close();
}

void* IceDogRendering::DirectXShaderInstance::GetRawShaderPtr()
{
	return r_deviceShaderPtr;
}

void IceDogRendering::DirectXShaderInstance::UpdateBuffer(ContinuousMode cm)
{
	switch (cm)
	{
	case IceDogRendering::ContinuousMode::PerObject:
		if (r_cbPerObject)
			UpdateCB(cm);
		else
			CreateBuffer(cm);
		break;
	case IceDogRendering::ContinuousMode::PerFrame:
		if (r_cbPerFrame)
			UpdateCB(cm);
		else
			CreateBuffer(cm);
		break;
	}
}

void IceDogRendering::DirectXShaderInstance::CreateBuffer(ContinuousMode cm)
{
	size_t bufferSize = 0;
	switch (cm)
	{
	case IceDogRendering::ContinuousMode::PerObject:
		for (auto i : r_aliasSizeMap_po)
			bufferSize += i.second;
		break;
	case IceDogRendering::ContinuousMode::PerFrame:
		for (auto i : r_aliasSizeMap_pf)
			bufferSize += i.second;
		break;
	}

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
}

void IceDogRendering::DirectXShaderInstance::UpdateCB(ContinuousMode cm)
{

}