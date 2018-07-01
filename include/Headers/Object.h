#pragma once

#include <vector>
#include <map>
#include <string>
#include "Material.h"
#include "Mesh.h"


#include "ObjectDefine.h"


class Object {


private : 

	glm::mat4 m_ModelMatrix;
	glm::mat4 m_TranslationMat;
	glm::mat4 m_RotationMat;
    glm::mat4 m_ScaleMat;
    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

    void UpdateScaleMatrix();
    void UpdateTranslationMatrix();
    void UpdateRotationMatrix();

	std::string m_TextureDirectory;
	std::string GetFileName(FILE * f);

protected : 

	Mesh<Vertexun> * m_Mesh;
	std::map<unsigned int, unsigned int> * m_ListMat;

public :
	std::vector<Material*> m_Materials;
	Object(); //DEFAULT CONSTRUCTOR
	Object(Mesh<Vertexun> * m); //CONSTRUCTOR WITH A MESH
	Object(Mesh<Vertexun> * m, Material * mat); //CONSTRUCTOR WITH A MESH AND A MTL
	Object(Mesh<Vertexun> * m, std::vector<Material*> mat); //CONSTRUCTOR WITH A MESH AND A LIST OF MTL
	Object(Mesh<Vertexun> * m, std::string shaderPath); //CONSTRUCTOR WITH A MESH AND A SHADER
	
	Object(
		Mesh<Vertexun> * m, float * color, 
		std::string shadersPath = SHADERDEFAULTPATH, std::string genShaderPath = GENSHADERDEFAULTPATH, 
		int renderType = RENDERTYPEDEFAULT
	); //CONSTRUCTOR WITH A MESH AND A COLOR(RGB)
		
	Object(
		std::string pathObj, float * color, 
		bool reverse = true,
		std::string shadersPath = SHADERDEFAULTPATH, std::string genShaderPath = GENSHADERDEFAULTPATH, 
		int renderType = RENDERTYPEDEFAULT); //CONSTRUCTOR WITH A .OBJ AND A COLOR(RGB)

	//CONSTRUCTOR FOR OBJ FILE
	Object(
		std::string pathObj, //PATH OF THE .OBJ FILE
		std::string pathMtl = "", // PATH OF THE .MTL FILE
		bool reverse = true, //IF Y AND Z ARE INVERTED
		std::string shadersPath = SHADERDEFAULTPATH, //PATH OF THE SHADERS' DIRECTORY
		std::string genShaderPath = GENSHADERDEFAULTPATH, //PATH OF THE .GENSHADER FILE
		std::string pathTextures = "", // PATH OF THE TEXTURES' DIRECTORY
		int renderType = RENDERTYPEDEFAULT //RENDERING TYPE (TRIANGLES/STRIP/FAN...)
	);

	//Parse fichier .obj
	void LoadObject(std::string pathObj, bool reverse);

	//Parse fichier .mtl
	void LoadMaterials(std::string path, std::vector<Material*> & materials);

	//Charge les textures utilisées par le modele
	void LoadTexturesMap();
	void LoadTexturesMap(std::string path);

	//Verifie si un vertex est déjà existant dans la liste
	int contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn);

	//Verifie si l'object possède ce material
	bool hasMaterial(Material* mat);

	//Genere les shaders automatiquement
	void GenerateShaders(std::string shadersPath = SHADERDEFAULTPATH, std::string genShaderPath = GENSHADERDEFAULTPATH);
	
	void Init(unsigned int renderType, std::string shaderPath);
	void Bind();
	void Unbind();
	
	//Translation
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 axis);
	
	//Rotation radiale
	void RotationRad(float x, float y, float z);
    void RotationRad(glm::vec3 angles);
	
	//Rotation degrès
	void RotationDeg(float x, float y, float z);
	void RotationDeg(glm::vec3 angles);
	
	//Changement de taille
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
	
	unsigned int GetMaterialId(std::string nameMat);

	// setters
	inline void SetPosition(glm::vec3& p){ m_Position = p; UpdateTranslationMatrix();}
	inline void SetRotation(glm::vec3& r){ m_Rotation = r; UpdateRotationMatrix();}
	inline void SetScale(glm::vec3& s){ m_Scale = s; UpdateScaleMatrix();}
};
