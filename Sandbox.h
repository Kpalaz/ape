#ifndef Sandbox
#include "Visual Scripting/VisualScripting.h"
#include "windows.h"
#include <iostream>
class Sandbox
{
private:
  NodeGraph graph = NodeGraph();
public:
  void Init();
  //HWND window_;
  ImVec2 windowsize;
  Sandbox() {}
  void Update();
};

#endif // !Sandbox