#ifndef GUI_H
#define GUI_H
#pragma once

#include <vector>

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
};

extern Gui gui;
#endif
