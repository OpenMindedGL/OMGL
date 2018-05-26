#include "Object.h"
#include <stdio.h>
#include <cstring>

#define _CRT_SECURE_NO_WARNIGNS 1

Object::Object() : Model()
{}

Object::Object(std::string pathObj, std::string pathMtl, bool reverse): Model()
{
	LoadObject(pathObj, pathMtl, reverse);
}

bool Object::LoadObject(std::string pathObj, std::string pathMtl, bool reverse)
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;
	
	FILE * file = fopen(pathObj.c_str(), "r");
	int it = -1;

	if (file == NULL) {
		printf("Impossible top open file, please check the path");
		getchar();
		return false;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		
		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			if (reverse) v.push_back(glm::vec3(vertex.x, vertex.z, vertex.y));
			else v.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 vertex;
			fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
			vt.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			if(reverse)	vn.push_back(glm::vec3(vertex.x, vertex.z, vertex.y));
			else vn.push_back(vertex);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			int r = 0;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			if (vt.size() > 0) {
				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
					&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				
				for (int i = 0; i < 3; i++) {
					r = contains(v[vertexIndex[i]-1], vt[uvIndex[i]-1], vn[normalIndex[i]-1]);
					if (r == -1) {
						m_Vertices.push_back(Vertexun(
							v[vertexIndex[i]-1], vt[uvIndex[i]-1], vn[normalIndex[i]-1]
						));
						m_Indices.push_back(++it);
					}
					else {
						m_Indices.push_back(r);
					}
				}
			}
			else{
				fscanf(file, "%d//%d %d//%d %d//%d\n",
					&vertexIndex[0], &normalIndex[0],
					&vertexIndex[1], &normalIndex[1],
					&vertexIndex[2], &normalIndex[2]);

				for (int i = 0; i < 3; i++) {
					r = contains(v[vertexIndex[i]-1], glm::vec2(1), vn[normalIndex[i]-1]);
					if (r == -1) {
						m_Vertices.push_back(Vertexun(
							v[vertexIndex[i]-1], glm::vec2(1), vn[normalIndex[i]-1]
						));
						m_Indices.push_back(++it);
					}
					else {
						m_Indices.push_back(r);
					}
				}
			}
		}
		else if (strcmp(lineHeader, "usemtl") == 0) {
			char nameMat[128];
			fscanf(file, "%s\n", nameMat);
			m_ListMat.insert(std::pair<unsigned int, std::string>(m_Vertices.empty() ? 0 : m_Vertices.size()-1, nameMat));
		}
	}
	
	if (pathMtl.size() > 0) InitMaterials(pathMtl, m_Materials);

	fclose(file);
	v.clear();
	vt.clear();
	vn.clear();
	return true;
}

int Object::contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn)
{
	int i = 0;
	while (i < m_Vertices.size()) {
		if (m_Vertices[i].pos == v &&
			m_Vertices[i].uv == vt &&
			m_Vertices[i].normal == vn)
			return i;
		i++;
	}
	return -1;
}

void Object::InitMaterials(std::string path, std::vector<Material>& listMat)
{
	FILE * file = fopen(path.c_str(), "r");
	int it = -1;

	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (file == NULL) {
			printf("Impossible top open file, please check the path");
			getchar();
			return;
		}

		if (strcmp(lineHeader, "newmtl") == 0) {
			char name[128];
			fscanf(file, "%s\n", name);
			listMat.push_back(Material(name));
			it++;
		}

		else if (strcmp(lineHeader, "Ns") == 0) {
			float ns;
			fscanf(file, "%f\n", &ns);
			listMat[it].setNs(ns);
		}

		else if (strcmp(lineHeader, "Ka") == 0) {
			glm::vec3 ka;
			fscanf(file, "%f %f %f\n", &ka.x, &ka.y, &ka.z);
			listMat[it].setKa(ka);
		}

		else if (strcmp(lineHeader, "Kd") == 0) {
			glm::vec3 kd;
			fscanf(file, "%f %f %f\n", &kd.x, &kd.y, &kd.z);
			listMat[it].setKd(kd);
		}

		else if (strcmp(lineHeader, "Ks") == 0) {
			glm::vec3 ks;
			fscanf(file, "%f %f %f\n", &ks.x, &ks.y, &ks.z);
			listMat[it].setKs(ks);
		}

		else if (strcmp(lineHeader, "Ke") == 0) {
			glm::vec3 ke;
			fscanf(file, "%f %f %f\n", &ke.x, &ke.y, &ke.z);
			listMat[it].setKe(ke);
		}

		else if (strcmp(lineHeader, "Ni") == 0) {
			float ni;
			fscanf(file, "%f\n", &ni);
			listMat[it].setNi(ni);
		}

		else if (strcmp(lineHeader, "d") == 0) {
			float d;
			fscanf(file, "%f\n", &d);
			listMat[it].setD(d);
		}

		else if (strcmp(lineHeader, "illum") == 0) {
			unsigned int illum;
			fscanf(file, "%d\n", &illum);
			listMat[it].setD(illum);
		}
	}
}