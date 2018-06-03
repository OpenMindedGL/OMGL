#ifndef GravityObject_H
#define GravityObject_H
#pragma once

#include "Model.h"
#include "Vertex.h"

class GravityObject : public Model<Vertexun>{

private : 
        glm::vec3 m_Foot;



public: 
	GravityObject(glm::vec3 foot);
        void ApplyGravity(float foot_height, double tps, double& lastTime, double& currentTime);
        inline glm::vec3 GetFoot(){return m_Foot;};
        inline void SetFoot(glm::vec3 a){m_Foot=a;};
};

#endif
