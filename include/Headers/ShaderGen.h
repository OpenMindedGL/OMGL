#define VERTEX 0
#define FRAGMENT 1
#define KD 1
#define KS 10
#define KA 100
#define D 1000
#define NS 10000

#include <vector>
#include <string>

class ShaderGenerator
{
private : 
	std::string m_Type[2];
	std::string m_Version[2];
	std::vector<std::string> m_Layout[2];
	std::vector<std::string> m_Out[2];
	std::vector<std::string> m_In[2];
	std::vector<std::string> m_Uniform[2];
	std::vector<std::string> m_MainContent[2];
	std::string m_calcColor;
	std::string m_lightColor;
	
	std::vector<bool> m_DynamicUniform;
	std::vector<std::string> m_UniformsList = { "u_Kd", "u_Ks", "u_Ka", "u_D", "u_Ns" };
	std::vector<std::string> m_UniformsTexturesList = { "u_KdMap", "u_KsMap", "u_KaMap", "u_DMap", "u_NsMap" };
	std::vector<std::string> m_UniformsTypeList = { "vec3", "vec3", "vec3", "float", "float" };

	std::string m_Name;
	std::string m_FilePath; 
	std::string m_GenShaderPath;
	
	int m_ActualType;
	bool m_Duplicate;

	void addLine(std::ofstream &file, const std::string &line);
	void addLine(std::ofstream &file, const std::vector<std::string> &lines);
	void LoadGenFile();
	void LoadStaticValues(std::ifstream & file,  const std::string& input);
	void createVertexShader(std::ofstream &file);
	void createFragmentShader(std::ofstream &file);
	void initValue(std::string token);
	void addDynamicsUniform(std::ofstream &file);
	void declarationUniform(std::ofstream &file);	

public : 
	ShaderGenerator(std::vector<bool> dynamicUniform, std::string shadersPath, std::string genShaderPath);

	void GenerateName();
	void SeekFile();
	void CreateShader();

	inline std::string getName() { return m_Name; }

};