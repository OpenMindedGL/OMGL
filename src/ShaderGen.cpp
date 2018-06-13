#include "ShaderGen.h"
#include <sstream>
#include <regex>
#include <fstream>
#include <iostream>


ShaderGenerator::ShaderGenerator(std::vector<bool> dynamicUniform, std::string shadersPath, std::string genShaderPath)
	: m_DynamicUniform(dynamicUniform), m_FilePath(shadersPath), m_GenShaderPath(genShaderPath)
{
	GenerateName();
	SeekFile();
	LoadGenFile();
	if (!m_Duplicate) CreateShader();
}


void ShaderGenerator::addLine(std::ofstream &file, const std::string & line)
{
	file << line << std::endl;
}

void ShaderGenerator::addLine(std::ofstream &file, const std::vector<std::string> & lines)
{
	for (int i = 0; i < lines.size(); i++) {
		file << lines[i] << std::endl;
	}
}

void ShaderGenerator::GenerateName()
{
	int nameNum = 0; 
	if (m_DynamicUniform[0]) nameNum += KD;
	if (m_DynamicUniform[1]) nameNum += KS;
	if (m_DynamicUniform[2]) nameNum += KA;
	if (m_DynamicUniform[3])  nameNum += D;
	m_Name = std::to_string(nameNum)+".shader";
}

void ShaderGenerator::SeekFile()
{
	std::string name = m_FilePath + '/' + m_Name;
	std::ifstream file(name, std::ios::in);
	if (file) m_Duplicate = true;
	file.close();
}

void ShaderGenerator::CreateShader()
{
	std::string name = m_FilePath + '/' + m_Name;
	std::ofstream file(name.c_str(), std::ios::out | std::ios::trunc);
	createVertexShader(file);
	createFragmentShader(file);
	file.close();
}

void ShaderGenerator::LoadGenFile()
{
	std::ifstream file(m_GenShaderPath.c_str(), std::ios::in);
	if (!file)
	{
		printf("Impossible to open file, please check the path\n");
		return;
	}
	std::string str;
	std::string line; 
	while (std::getline(file, line)) {
		line;
		LoadStaticValues(file, line);
	}
	file.close();
}

void ShaderGenerator::LoadStaticValues(std::ifstream & file, const std::string& input)
{
	std::regex rgx1("^(<)([a-zA-Z]+)(>)([A-Za-z0-9 _#=\(\);,.\n\t\\*+\/-]+)(<\/>)$"); //Regex pour code en ligne
	std::regex rgx2("^(<)([a-zA-Z]+)(>)( )*$"); //Regex pour un bloc de code
	std::regex rgx3("^(<\/>)( )*$"); //Regex pour fin d'un bloc
	std::smatch match;	
	
	if (std::regex_search(input.begin(), input.end(), match, rgx1)) {
		//match[2] = type | match[4] = value
		if (match[2] == "type") {
			if(match[4] == "#shader vertex")
				m_ActualType = VERTEX;
			else m_ActualType = FRAGMENT;
			m_Type[m_ActualType] = match[4];
		}
		if (match[2] == "version")
			m_Version[m_ActualType] = match[4];
		if (match[2] == "layout")
			m_Layout[m_ActualType].push_back(match[4]);
		if (match[2] == "out")
			m_Out[m_ActualType].push_back(match[4]);
		if (match[2] == "in")
			m_In[m_ActualType].push_back(match[4]);
		if (match[2] == "uniform")
			m_Uniform[m_ActualType].push_back(match[4]);
		if (match[2] == "main")
			m_MainContent[m_ActualType].push_back(match[4]);
		if (match[2] == "lightColor")
			m_lightColor = match[4];
		if (match[2] == "calcColor")
			m_calcColor = match[4];
	}
	else if (std::regex_search(input.begin(), input.end(), match, rgx2)) {
		std::string token = match[2];
		std::string value;
		std::getline(file, value);
		std::string const cvalue = value;
		initValue(token);
		while (!std::regex_search(cvalue.begin(), cvalue.end(), match, rgx3)) {
			if (token == "version")
				m_Version[m_ActualType] += value;
			if (token == "layout")
				m_Layout[m_ActualType].at(m_Layout[m_ActualType].size()-1) += value;
			if (token == "out")
				m_Out[m_ActualType].at(m_Out[m_ActualType].size() - 1) += value;
			if (token == "in")
				m_In[m_ActualType].at(m_In[m_ActualType].size() - 1) += value;
			if (token == "uniform")
				m_Uniform[m_ActualType].at(m_Uniform[m_ActualType].size() - 1) += value;
			if (token == "main")
				m_MainContent[m_ActualType].at(m_MainContent[m_ActualType].size() - 1) += value;
			if (token == "lightColor")
				m_lightColor += value;
			if (token == "calcColor")
				m_calcColor += value;
			std::getline(file, value);
			const_cast<std::string&>(cvalue) = value;
			value += "\n";
		}
	}
}

void ShaderGenerator::createVertexShader(std::ofstream &file) 
{
	addLine(file, m_Type[VERTEX]);
	addLine(file, m_Version[VERTEX]);
	addLine(file, m_Layout[VERTEX]);
	addLine(file, m_Out[VERTEX]);
	addLine(file, m_In[VERTEX]);
	addLine(file, m_Uniform[VERTEX]);
	addLine(file, "void main(){");
	addLine(file, m_MainContent[VERTEX]);
	addLine(file, "}");

}

void ShaderGenerator::createFragmentShader(std::ofstream &file)
{
	addLine(file, m_Type[FRAGMENT]);
	addLine(file, m_Version[FRAGMENT]);
	addLine(file, m_Layout[FRAGMENT]);
	addLine(file, m_Out[FRAGMENT]);
	addLine(file, m_In[FRAGMENT]);
	addLine(file, m_Uniform[FRAGMENT]);
	addDynamicsUniform(file);
	addLine(file, "void main(){");
	addLine(file, m_MainContent[FRAGMENT]);
	addLine(file, m_lightColor);
	declarationUniform(file);
	addLine(file, m_calcColor);
	addLine(file, "}");
}

void ShaderGenerator::initValue(std::string token)
{
	if (token == "version") m_Type[m_ActualType] = "";
	if (token == "layout") m_Layout[m_ActualType].push_back("");
	if (token == "out") m_Out[m_ActualType].push_back("");
	if (token == "in") m_In[m_ActualType].push_back("");
	if (token == "uniform") m_Uniform[m_ActualType].push_back("");
	if (token == "main") m_MainContent[m_ActualType].push_back("");
	if (token == "lightColor") m_lightColor = "";
	if (token == "calcColor") m_calcColor = "";
}

void ShaderGenerator::addDynamicsUniform(std::ofstream &file)
{
	for (int i = 0; i < m_UniformsList.size(); i++) {
		if (m_DynamicUniform[i])
			addLine(file, "uniform sampler2D " + m_UniformsTexturesList[i]+";");
		else 
			addLine(file, "uniform " + m_UniformsTypeList[i] + " " + m_UniformsList[i]+";");
	}
}

void ShaderGenerator::declarationUniform(std::ofstream &file)
{
	for (int i = 0; i < m_DynamicUniform.size(); i++) {
		if (m_DynamicUniform[i])
			addLine(file, "\tvec3 " + m_UniformsList[i] + " = texture( " + m_UniformsTexturesList[i] + ", uv).rgb;");
	}
}