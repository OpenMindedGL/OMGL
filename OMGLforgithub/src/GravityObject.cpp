#include "GravityObject.h"

GravityObject::GravityObject(glm::vec3 foot) {
  m_Foot = foot;
};

void GravityObject::ApplyGravity(float foot_height,double tps, double& lastTime, double& currentTime){

  float v;
  float v0 = 300;
  float fall_coef;

  if (GetFoot().y > foot_height){

    currentTime = glfwGetTime();

    v = v0 * glm::tanh((currentTime- tps)/(v0/9.81f));
    fall_coef = (-1)*v*(currentTime-lastTime);

    SetFoot(glm::vec3(GetFoot().x, GetFoot().y + fall_coef , GetFoot().z));     
    Translate(glm::vec3(0.0f, fall_coef, 0.0f));
    
    lastTime = glfwGetTime();
  }

  else if (GetFoot().y < foot_height){

    SetFoot(glm::vec3(GetFoot().x, foot_height, GetFoot().z)); 
    Translate(glm::vec3(0.0f, foot_height - GetFoot().y,0.0f));
  }
};
