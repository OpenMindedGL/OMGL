#ifndef GUI_H
#define GUI_H
#pragma once

#include <map>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


class Gui {
  
  private:


    bool hasChanged;
    void DrawElements();

  public:
    static void Init(GLFWwindow* window);
    Gui();
    void Draw();
    void InitPointers();
    std::map<std::string,float> ptr;
    std::map<std::string,int> ptri;
    inline bool GetHasChanged(){ return hasChanged; };
};

extern Gui gui;
#endif
