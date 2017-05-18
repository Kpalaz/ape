#include "Sandbox.h"

void Sandbox::Update()
{
  //, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
  //ImGui::NewFrame();
  ImGui::Begin("1",NULL,windowsize,-1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
  ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
  ImGui::SetWindowSize(windowsize, ImGuiSetCond_Always);
  graph.Update();
  ImGui::End();
  //ImGui::Render();
}

void Sandbox::Init()
{
  //LPRECT temp;
  //GetWindowRect(window_, temp);
  windowsize.x = 1980;//GetSystemMetrics(SM_CXSIZEFRAME);
  windowsize.y = 1080;//GetSystemMetrics(SM_CYSIZEFRAME);
  graph.windowsize = windowsize;
  graph.logger << "memes" << std::endl;
  //std::cout << "x is " << windowsize.x << " y is " << windowsize.y << std::endl;
}