#include "Object.h"
#include <stdio.h>
#include <cstring>
#define _CRT_SECURE_NO_WARNIGNS 1

Object::Object() : Model()
{}

Object::Object(std::string path): Model()
{
	LoadObject(path);
}

bool Object::LoadObject(std::string path)
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;
	
	FILE * file = fopen(path.c_str(), "r");
	int it = -1;

	if (file == NULL) {
		printf("Impossible top open file, please check the path");
		getchar();
		return false;
	}

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		
		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			v.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 vertex;
			fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
			vt.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vn.push_back(vertex);
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
	}
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
