#pragma once

#include <vector>
#include <map>
#include <string>
#include "Material.h"
#include "Mesh.h"

class Object {


private : 

	glm::mat4 m_ModelMatrix;

	std::string m_TextureDirectory;

	glm::mat4 m_TranslationMat;
	glm::mat4 m_RotationMat;
    glm::mat4 m_ScaleMat;
    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

    void UpdateScaleMatrix();
    void UpdateTranslationMatrix();
    void UpdateRotationMatrix();

	std::string GetFileName(FILE * f);
protected : 

	Mesh<Vertexun> * m_Mesh;
	std::vector<Material*> m_Materials;
	std::map<unsigned int, unsigned int> * m_ListMat;

public :
	Object();
	Object(Mesh<Vertexun> * m);
	Object(Mesh<Vertexun> * m, Material * mat);
	Object(Mesh<Vertexun> * m, std::vector<Material*> mat);
	Object(Mesh<Vertexun> * m, std::string shaderPath);
	Object(Mesh<Vertexun> * m, float * color);
	Object(std::string pathObj, std::string pathMtl, bool reverse = false, int renderType = GL_TRIANGLES); //FICHIER OBJ ET MTL
	Object(std::string pathObj, float * color, bool reverse = false, int renderType = GL_TRIANGLES); //FICHIER OBJ ET CODE COULEUR RBG POUR MODEL
	Object(std::string pathObj, std::string pathMtl, std::string texturesFile, bool reverse = false, int renderType = GL_TRIANGLES);
	Object(std::string pathObj, bool reverse = false);

	void LoadObject(std::string pathObj, bool reverse);
	void LoadMaterials(std::string path, std::vector<Material*> & materials);
	void LoadTexturesMap();
	void LoadTexturesMap(std::string path);
	int contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn);
	bool hasMaterial(Material* mat);
	unsigned int GetMaterialId(std::string nameMat);

	void GenerateShaders(std::string shadersPath, std::string genShaderPath);
	void Init(unsigned int renderType, std::string shaderPath);
	void Bind();
	void Unbind();
	
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 axis);
	void RotationRad(float x, float y, float z);
    void RotationRad(glm::vec3 angles);
	void RotationDeg(float x, float y, float z);
	void RotationDeg(glm::vec3 angles);
	void Scale(float x, float y, float z);
	void Scale(glm::vec3 axis);
	
	void UpdateModelMatrix();

	//getters
	inline std::vector<Material*> GetMaterials() { return m_Materials; }
	inline Mesh<Vertexun> * GetMesh() { return m_Mesh; }
	inline glm::mat4 GetModelMatrix() { return m_ModelMatrix; }  // Computing it each time you need it, could do better
	inline std::map<unsigned int, unsigned int> * GetListMat() { return m_ListMat; }
	inline std::string GetTextureDirectory() const { return m_TextureDirectory; }

	inline void SetTextureDirectory(std::string path) { m_TextureDirectory = path; }

	inline glm::vec3 GetPosition(){ return m_Position; }
	inline glm::vec3 GetScale(){ return m_Scale; }
	inline glm::vec3 GetRotation(){ return m_Rotation; }
	
	// setters
	inline void SetPosition(glm::vec3& p){ m_Position = p; UpdateTranslationMatrix();}
	inline void SetRotation(glm::vec3& r){ m_Rotation = r; UpdateRotationMatrix();}
	inline void SetScale(glm::vec3& s){ m_Scale = s; UpdateScaleMatrix();}
};
