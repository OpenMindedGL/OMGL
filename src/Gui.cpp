#include "Gui.h"

void Gui::Init(GLFWwindow* window){
  ImGui::CreateContext();
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();
}

void Gui::InitPointers(){
  ptri = {
    {"nboctave", 7},
    {"neg", 0},
    {"seed", 69},
    {"choose", 2}
  };
  ptr = {
    {"zoom", 2.0f},
    {"lacunarity", 2.0f},
    {"persistence", 2.0f},
    {"hash freq", 2.0f},
    {"f", 2.2f},
    {"s", 0.5f},
    {"a", 0.0f},
    {"b", 0.5f},
    {"sc", 0.01f},
  };
}

Gui::Gui(){
  hasChanged = false;
  InitPointers();

}

void Gui::Draw(){
  ImGui_ImplGlfwGL3_NewFrame();
  
  DrawElements();

  ImGui::Render();
  ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}


void Gui::DrawElements(){
  hasChanged = false;
  ImGui::SliderInt("nbOctave", &ptri["nboctave"], 1, 10);
  ImGui::SliderFloat("zoom", &ptr["zoom"], 0.0f, 10.0f);
  ImGui::SliderInt("0 is <0, 1 is >0", &ptri["neg"], 0, 1);
  ImGui::SliderInt("seed", &ptri["seed"], ptri["seed"]- 15, ptri["seed"]+ 15);
  //ImGui::SliderFloat("lacunarity", &ptr["lacunarity"], 1.0f, 10.0f);      
  //ImGui::SliderFloat("persistence", &ptr["persistence"], -1.0f, 1.0f);      
  //ImGui::SliderFloat("flatcoef", &ptr["flatcoef"], 0.0f, 5.0f);      
  //ImGui::SliderFloat("mod freq", &ptr["modfreq"], 0.0f, 0.01f);
  ImGui::SliderFloat("hash freq", &ptr["hash freq"], 0.0f, 10.0f);
  ImGui::SliderFloat("f", &ptr["f"], 0.0f, 10.0f);
  ImGui::SliderFloat("s", &ptr["s"], 0.0f, 1.0f);
  ImGui::SliderFloat("a", &ptr["a"], 0.0f, 10.0f);
  ImGui::SliderFloat("b", &ptr["b"], 0.0f, 1.0f);
  ImGui::SliderFloat("sc", &ptr["sc"], 0.0f, 1.0f);
  ImGui::SliderInt("a=1, b=2, c=3", &ptri["choose"], 0, 3);
  ImGui::Text("Press SPACE while you need to move");
  ImGui::Text("the camera, J to release it");
  ImGui::Text("");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Text("Hello, world %d", 123);
  if (ImGui::Button("Reload"))
  {
    hasChanged = true;
    ImGui::Text("Reloading");
  }
  else{
    ImGui::Text("Done");
  }
}
