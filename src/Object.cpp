#include "Object.h"
#include <string.h>

Object::Object()
{
  m_Mesh = new Mesh<Vertexun>;
}

Object::Object(Mesh<Vertexun> * m) : m_Mesh(m)
{}

Object::Object(Mesh<Vertexun> * m, std::string shaderPath) : m_Mesh(m)
{
  //m_Mesh = new Mesh<Vertexun>(&m);
  m_Materials.push_back(new Material(new Shader(shaderPath)));
}

Object::Object(Mesh<Vertexun> * m, Material* mat) : m_Mesh(m)
{
  //On verifie si le mat existe déjà dans la liste des mat, on ajoute ou non dans la liste en fonction
  if (!hasMaterial(mat)) m_Materials.push_back(mat);
}

Object::Object(Mesh<Vertexun> * m, std::vector<Material*> mat) : m_Mesh(m)
{
  //On parcourt chaque element dans la liste de mat et on ajoute ceux qui n'existe pas encore
  for (int i = 0; i < mat.size(); i++) {
    if (!hasMaterial(mat[i])) m_Materials.push_back(mat[i]);
  }
}

Object::Object(std::string pathObj, std::string pathMtl, bool reverse)
{
  m_Mesh = new Mesh<Vertexun>;
  this->LoadObject(pathObj, pathMtl, reverse);
}

void Object::LoadObject(std::string pathObj, std::string pathMtl, bool reverse)
{
  if (pathMtl.size() > 0) InitMaterials(pathMtl, m_Materials);

  std::vector<glm::vec3> v;
  std::vector<glm::vec2> vt;
  std::vector<glm::vec3> vn;

  FILE * file = fopen(pathObj.c_str(), "r");
  int it = -1;

  if (file == NULL) {
    printf("Impossible top open file, please check the path");
    getchar();
    return;
  }

  while (1) {
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break;

    if (strcmp(lineHeader, "v") == 0) {
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
      if (reverse)	vn.push_back(glm::vec3(vertex.x, vertex.z, vertex.y));
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
          r = contains(v[vertexIndex[i] - 1], vt[uvIndex[i] - 1], vn[normalIndex[i] - 1]);
          if (r == -1) {
            this->m_Mesh->GetVertices()->push_back(Vertexun(
                  v[vertexIndex[i] - 1], vt[uvIndex[i] - 1], vn[normalIndex[i] - 1]
                  ));
            m_Mesh->GetIndices()->push_back(++it);

          }
          else {
            m_Mesh->GetIndices()->push_back(r);
          }
        }
      }
      else {
        fscanf(file, "%d//%d %d//%d %d//%d\n",
            &vertexIndex[0], &normalIndex[0],
            &vertexIndex[1], &normalIndex[1],
            &vertexIndex[2], &normalIndex[2]);

        for (int i = 0; i < 3; i++) {
          r = contains(v[vertexIndex[i] - 1], glm::vec2(1), vn[normalIndex[i] - 1]);
          if (r == -1) {
            m_Mesh->GetVertices()->push_back(Vertexun(
                  v[vertexIndex[i] - 1], glm::vec2(1), vn[normalIndex[i] - 1]
                  ));
            m_Mesh->GetIndices()->push_back(++it);
          }
          else {
            m_Mesh->GetIndices()->push_back(r);
          }
        }
      }
    }
    else if (strcmp(lineHeader, "usemtl") == 0) {
      char nameMat[128];
      fscanf(file, "%s\n", nameMat);
      m_ListMat.insert(std::pair<unsigned int, std::string>(m_Mesh->GetVertices()->empty() ? 0 : m_Mesh->GetVertices()->size() - 1, nameMat));
    }
  }

  if (pathMtl.size() > 0) InitMaterials(pathMtl, m_Materials);

  fclose(file);
  v.clear();
  vt.clear();
  vn.clear();
}

void Object::InitMaterials(std::string path, std::vector<Material*>& materials)
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
      materials.push_back(new Material(name));
      it++;
    }

    else if (strcmp(lineHeader, "Ns") == 0) {
      float ns;
      fscanf(file, "%f\n", &ns);
      materials[it]->SetNs(ns);
    }

    else if (strcmp(lineHeader, "Ka") == 0) {
      glm::vec3 ka;
      fscanf(file, "%f %f %f\n", &ka.x, &ka.y, &ka.z);
      materials[it]->SetKa(ka);
    }

    else if (strcmp(lineHeader, "Kd") == 0) {
      glm::vec3 kd;
      fscanf(file, "%f %f %f\n", &kd.x, &kd.y, &kd.z);
      materials[it]->SetKd(kd);
    }

    else if (strcmp(lineHeader, "Ks") == 0) {
      glm::vec3 ks;
      fscanf(file, "%f %f %f\n", &ks.x, &ks.y, &ks.z);
      materials[it]->SetKs(ks);
    }

    else if (strcmp(lineHeader, "Ke") == 0) {
      glm::vec3 ke;
      fscanf(file, "%f %f %f\n", &ke.x, &ke.y, &ke.z);
      materials[it]->SetKe(ke);
    }

    else if (strcmp(lineHeader, "Ni") == 0) {
      float ni;
      fscanf(file, "%f\n", &ni);
      materials[it]->SetNi(ni);
    }

    else if (strcmp(lineHeader, "d") == 0) {
      float d;
      fscanf(file, "%f\n", &d);
      materials[it]->SetD(d);
    }

    else if (strcmp(lineHeader, "illum") == 0) {
      unsigned int illum;
      fscanf(file, "%d\n", &illum);
      materials[it]->SetIllum(illum);
    }
  }
}

int Object::contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn)
{
  int i = 0;
  while (i < m_Mesh->GetVertices()->size()) {
    if (m_Mesh->GetVertices()->at(i).pos == v &&
        m_Mesh->GetVertices()->at(i).uv == vt &&
        m_Mesh->GetVertices()->at(i).normal == vn)
      return i;
    i++;
  }
  return -1;
}

bool Object::hasMaterial(Material * mat)
{
  for (int i = 0; i < m_Materials.size(); i++) {
    if (m_Materials[i]->GetId() == mat->GetId()) return true;
  }
  return false;
}

void Object::Init(unsigned int renderType, std::string shaderPath) {
  m_Mesh->Init(renderType);
  for (int i = 0; i < m_Materials.size(); i++) {
    m_Materials[i]->Init(shaderPath);
  }
}

void Object::Bind()
{
  m_Mesh->Bind();
  for (int i = 0; i < m_Materials.size(); i++) {
    m_Materials[i]->Bind();
  }
}

void Object::Unbind()
{
  m_Mesh->Unbind();
  for (int i = 0; i < m_Materials.size(); i++) {
    m_Materials[i]->Unbind();
  }
}

void Object::Translate(glm::vec3 position)
{
  m_ModelMatrix = glm::translate(m_ModelMatrix, position);
}

void Object::Rotation(float angle, glm::vec3 axis)
{
  m_ModelMatrix = glm::rotate(m_ModelMatrix, angle, axis);
}

void Object::Scale(glm::vec3 scale)
{
  m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
}
