#include "Gui.h"

void Gui::Init(GLFWwindow* window){
  ImGui::CreateContext();
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();
}

Gui::Gui(){
  hasChanged = false;

}

void Gui::Draw(){
  ImGui_ImplGlfwGL3_NewFrame();
  
  DrawElements();

  ImGui::Render();
  ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}


void Gui::DrawElements(){
  ImGui::Text("Hello, world %d", 123);
  if (ImGui::Button("Save"))
  {
  }
}
