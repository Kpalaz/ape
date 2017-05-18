#ifndef VisalScripting

#include "../imgui/imgui_addons.h"
#include <vector>
#include <map> 
#include <string>
#include <iostream>
#include <sstream>

class Node;
class NodeLink;
class NodeGraph;
class Sandbox;

class Connections
{
public:
  Connections() {}
  void Update() {}
private:
  ImVec2 pos;
  
  //need some form of type check here eventually

};


//base class for nodes.
class Node
{
public:
  friend NodeGraph;
  ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x + 7.5f, Pos.y +  Size.y * ((float)slot_no + 1) / ((float)Inputs.size() + 1) + 10); }
  Node(int id, const char* name, const ImVec2& pos, std::vector<std::pair<std::string, bool>> inputs, std::vector<std::pair<std::string, bool>> outputs);
  ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x - 7.5f, Pos.y + Size.y * ((float)slot_no + 1) / ((float)Outputs.size() + 1) + 10); }
  int GetLongestPinName();
private:
  int     ID;
  char    Name[32];
  ImVec2  Pos, Size;
  std::vector<std::pair<std::string,bool>> Inputs;
  std::vector<std::pair<std::string,bool>> Outputs;
  bool node_moving_active;

};

class NodeLink
{
public:
  NodeLink(int input_idx, int input_slot, int output_idx, int output_slot,ImVec2 start, ImVec2 end);
  int     InputIdx, InputSlot, OutputIdx, OutputSlot;
  ImVec2 Start;
  ImVec2 End;
};

class NodeGraph
{
public:
  friend Sandbox;
  NodeGraph();
  void Update();
  int GetNextFreeID();
  std::vector<int> recycledids;
  ImVec2 windowsize;
private:
  void CreateLink();
  bool show_grid = true;
  ImU32 GRID_COLOR = ImColor(200, 200, 200, 40);
  ImVec2 scrolling = ImVec2(0.0f, 0.0f);
  const ImVec2 NODE_WINDOW_PADDING = ImVec2(8.0f, 8.0f);
  Node* NodeGraph::CreateNode(std::string name, ImVec2 spawnpos, std::vector<std::pair<std::string, bool>> inputs, std::vector<std::pair<std::string, bool>> outputs);
  void DestroyNode(Node* dead);
  Node* GetNodeFromID(int id);
  int IfClickingOnSlot(Node* inputs, ImVec2 pos);
  void DisplayGrid();
  void DisplayLinks();
  void DisplayNodes();
  void DisplayList();
  void DisplayConsole();
  
  int node_selected = -1;
  NodeLink *dragging = NULL;
  const float NODE_SLOT_RADIUS = 4.0f;
  float GRID_SZ = 64.0f;
  std::map<int,Node*> nodes;
  std::vector<NodeLink> links;
  bool DraggingLink = false;
  ImVec2 offset;
  ImDrawList* draw_list;
  bool open_context_menu = false;
  int node_hovered_in_list = -1;
  int node_hovered_in_scene = -1;
  std::stringstream logger;
};


#endif // !VisalScripting
