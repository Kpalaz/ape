#include "VisualScripting.h"

Node::Node(int id, const char* name, const ImVec2& pos, std::vector<std::pair<std::string, bool>> inputs, std::vector<std::pair<std::string, bool>> outputs)
{
  ID = id; strncpy(Name, name, 31);
  Name[31] = 0;
  Pos = pos;
  Inputs = inputs;
  Outputs = outputs;
}

NodeLink::NodeLink(int input_idx, int input_slot, int output_idx, int output_slot, ImVec2 start, ImVec2 end)
{
  InputIdx = input_idx;
  InputSlot = input_slot;
  OutputIdx = output_idx;
  OutputSlot = output_slot;
  Start = start;
  End = end;
}

NodeGraph::NodeGraph()
{
  //first and second node will always be output
  //nodes[0] = new Node(0, "Input", ImVec2(100, 100), 0, 1);
  //nodes[1] = new Node(1, "Output", ImVec2(100, 200), 1, 0);
  //nodes[2] = new Node(2, "Test", ImVec2(200, 200), 4,4);
  CreateNode("Input", ImVec2(100, 100), std::vector<std::pair<std::string, bool>>(), std::vector<std::pair<std::string, bool>>(1, std::make_pair(std::string("output"), false)));
  CreateNode("Output", ImVec2(100, 200), std::vector<std::pair<std::string, bool>>(1, std::make_pair(std::string("input"), false)), std::vector<std::pair<std::string, bool>>());
  CreateNode("Test", ImVec2(200, 200), std::vector<std::pair<std::string, bool>>(4, std::make_pair(std::string("inputs"), false)), std::vector<std::pair<std::string, bool>>(4, std::make_pair(std::string("outputs"), false)));
}

void NodeGraph::Update()
{
  open_context_menu = false;
  node_hovered_in_list = -1;
  node_hovered_in_scene = -1;

  ImGui::BeginChild(100, ImVec2(1900, 1040), true, ImGuiWindowFlags_NoScrollbar); // no scroll bar region
  ImGui::Columns(2);
  DisplayList();
  ImGui::SameLine();
  //graph
  ImGui::NextColumn();
  ImGui::SetColumnOffset(1, 300);
  ImGui::BeginGroup(); // scrollable region start
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(60, 60, 70, 200));
  ImGui::BeginChild("scrolling_region", ImVec2(1600, 800), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
  ImGui::PushItemWidth(120.0f);

  offset = ImGui::GetCursorScreenPos() - scrolling;
  //ImVec2 offset = ImVec2(ImGui::GetCursorScreenPos().x - scrolling.x, ImGui::GetCursorScreenPos().y - scrolling.y);
  draw_list = ImGui::GetWindowDrawList();
  draw_list->ChannelsSplit(2);

  DisplayGrid();
  DisplayNodes();
  DisplayLinks();
  draw_list->ChannelsMerge();





  //right click to open context
  if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
  {
    node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
    open_context_menu = true;
  }
  if (open_context_menu)
  {
    ImGui::OpenPopup("context_menu");
    if (node_hovered_in_list != -1)
      node_selected = node_hovered_in_list;
    if (node_hovered_in_scene != -1)
      node_selected = node_hovered_in_scene;
  }

  //// Draw context menu
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
  if (ImGui::BeginPopup("context_menu"))
  {

    Node* node = NULL;
    if (node_selected != -1)
      node = nodes[node_selected];
    ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup();// -offset;
    if (node)
    {
      ImGui::Text("Node '%s'", node->Name);
      ImGui::Separator();
      //if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
      if (ImGui::Button("Delete")) { DestroyNode(node); }
      //if (ImGui::MenuItem("Copy", NULL, false, false)) {}
    }
    else
    {
      //if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.size(), "name", scene_pos, 1, 1)); }
      if (ImGui::MenuItem("Paste", NULL, false, false)) {}
    }
    ImGui::EndPopup();
  }
  ImGui::PopStyleVar();

  // Scrolling
  if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
    scrolling = scrolling - ImGui::GetIO().MouseDelta;

  ImGui::PopItemWidth();
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(2);
  ImGui::BeginGroup();//console start
  ImGui::BeginChild("Output", ImVec2(1600, 220), true);
  DisplayConsole();
  ImGui::EndChild();
  ImGui::EndGroup();//console end
  ImGui::EndGroup();// scrollable region end
  ImGui::SameLine();
  //ImGui::NextColumn();
  //  ImGui::BeginGroup(); //modify start
  //    ImGui::BeginChild("modify", ImVec2(300, 1020), true);
  //      ImGui::Text("test");
  //      ImGui::Text("test2");
  //    ImGui::EndChild();
  //  ImGui::EndGroup(); //modify end
  ImGui::EndChild();// no scroll bar region end
}

void NodeGraph::DisplayList()
{
  //node list
  ImGui::BeginChild("node_list", ImVec2(300, 1020), true);
  ImGui::Text("Nodes");
  ImGui::Separator();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (it->second)
    {
      ImGui::PushID(it->second->ID);
      if (ImGui::Selectable(it->second->Name, it->second->ID == node_selected))
        node_selected = it->second->ID;
      if (ImGui::IsItemHovered())
      {
        node_hovered_in_list = it->second->ID;
        open_context_menu |= ImGui::IsMouseClicked(1);
      }
      ImGui::PopID();
    }
  }
  ImGui::EndChild();
}

void NodeGraph::DisplayGrid()
{
  // Display grid
  if (show_grid)
  {
    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();
    for (float x = fmodf(offset.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
      draw_list->AddLine(win_pos + ImVec2(x, 0), win_pos + ImVec2(x, canvas_sz.y), GRID_COLOR);
    //draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, win_pos.y + canvas_sz.y), GRID_COLOR);
    for (float y = fmodf(offset.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
      draw_list->AddLine(win_pos + ImVec2(0, y), win_pos + ImVec2(canvas_sz.x, y), GRID_COLOR);
    //draw_list->AddLine(ImVec2(win_pos.x, win_pos.y + y), ImVec2(canvas_sz.x + win_pos.x, y + win_pos.y), GRID_COLOR);
  }
}
void NodeGraph::DisplayLinks()
{
  //logic for links
  for (auto it = links.begin(); it != links.end(); ++it)
  {

    ImVec2 p1 = offset + GetNodeFromID(it->InputIdx)->GetInputSlotPos(it->InputSlot);
    ImVec2 p2 = offset + it->End;// offset + GetNodeFromID(it->OutputIdx)->GetInputSlotPos(it->OutputSlot);

                                 //ImVec2 p1 = ImVec2(offset.x + GetNodeFromID(it->InputIdx)->GetOutputSlotPos(it->InputSlot).x, offset.y + GetNodeFromID(it->InputIdx)->GetOutputSlotPos(it->InputSlot).y);
                                 //ImVec2 p2 = ImVec2(offset.x + node_out->GetInputSlotPos(link->OutputSlot).x, offset.y + node_out->GetInputSlotPos(link->OutputSlot).y);
    draw_list->AddBezierCurve(p1, p1 + ImVec2(50, 0), p2 + ImVec2(-50, 0), p2, ImColor(200, 200, 100), 3.0f);
  }


  //draw_list->ChannelsMerge();
  if (dragging)
  {
    ImVec2 p1 = offset + GetNodeFromID(dragging->InputIdx)->GetInputSlotPos(dragging->InputSlot);
    ImVec2 p2 = dragging->End;
    draw_list->AddBezierCurve(p1, p1 + ImVec2(50, 0), p2 + ImVec2(-50, 0), p2, ImColor(200, 200, 100), 3.0f);
  }
  if (DraggingLink)
  {
    if (ImGui::IsMouseDown(0))
    {
      if (ImGui::IsMouseDragging(0, 0.0f))
      {
        //links.end()->End = ImGui::GetMousePos();
        dragging->End = ImGui::GetMousePos();


      }
    }
    else
    {
      links.push_back(*dragging);
      dragging = NULL;
      DraggingLink = false;
    }
  }
}

void NodeGraph::DisplayNodes()
{
  //logic for nodes
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (it->second == NULL)
      continue;
    ImGui::PushID(it->first);

    //ImVec2 node_rect_min = ImVec2(offset.x + it->second->Pos.x, offset.y + it->second->Pos.y);
    ImVec2 node_rect_min = offset + it->second->Pos;

    // Display node contents first
    draw_list->ChannelsSetCurrent(1); // Foreground
    bool old_any_active = ImGui::IsAnyItemActive();
    //ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + NODE_WINDOW_PADDING.x, node_rect_min.y + NODE_WINDOW_PADDING.y));
    ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
    ImGui::BeginGroup();
    //ImGui::Columns(2, " ", false);
    //ImGui::BeginChild("inputs");
    //for (int slot_idx = 0; slot_idx < it->second->Inputs.size(); slot_idx++)
    //{
    //  ImGui::PushID(slot_idx);
    //  ImGui::Text(it->second->Inputs[slot_idx].first.c_str());
    //  ImGui::PopID();
    //}
    //ImGui::EndChild();
    //ImGui::NextColumn();
    //ImGui::BeginChild("outputs");
    //for (int slot_idx = 0; slot_idx < it->second->Outputs.size(); slot_idx++)
    //{
    //  ImGui::PushID(slot_idx);
    //  ImGui::Text(it->second->Inputs[slot_idx].first.c_str());
    //  ImGui::PopID();
    //}
    //ImGui::EndChild();
    ImGui::SetCursorScreenPos(offset + it->second->Pos);
    ImGui::Text(it->second->Name);
    //draw a line under the name of the node.
    draw_list->AddLine(offset + ImVec2(it->second->Pos.x, it->second->Pos.y + ImGui::GetFontSize()), offset + ImVec2(it->second->Pos.x + it->second->Size.x, it->second->Pos.y + ImGui::GetFontSize()), ImColor(120, 120, 120));
    //ImGui::NewLine();
    //ImGui::BeginChild("Column Data",ImVec2(it->second->Size));
    //draw_list->ChannelsMerge();
    //ImGui::PushItemWidth(it->second->Size.y);
    //ImGui::Separator();
    //ImGui::PopItemWidth();
    //ImGui::Columns(2, " ", false);
    //ImGui::BeginChild("inputs");
    for (int slot_idx = 0; slot_idx < it->second->Inputs.size(); slot_idx++)
    {
      ImGui::PushID(slot_idx);
      //ImColor slotcol = ImColor(150, 150, 150, 150);
      //if (it->second->Inputs[slot_idx].second)
      //{
      //  slotcol = ImColor(10, 10, 10);
      //}
      //draw_list->AddCircleFilled(offset + it->second->GetInputSlotPos(slot_idx) + ImVec2(NODE_SLOT_RADIUS, NODE_SLOT_RADIUS), NODE_SLOT_RADIUS, slotcol);
      //ImGui::SetCursorPos(it->second->GetInputSlotPos(slot_idx));
      //ImGui::InvisibleButton(it->second->Inputs[slot_idx].first.c_str(), ImVec2(NODE_SLOT_RADIUS * 2, NODE_SLOT_RADIUS * 2));
      ImGui::InputNodePin(it->second->Inputs[slot_idx].first, it->second->Inputs[slot_idx].second, offset + it->second->GetInputSlotPos(slot_idx));
      //if (ImGui::IsItemHovered())
      //{
      //  int slot = IfClickingOnSlot(it->second, ImGui::GetIO().MouseClickedPos[0] - offset);
      //  if (slot != -1)
      //  {
      //    //std::cout << "slot is: " << slot << std::endl;
      //    it->second->Inputs[slot_idx].second = true;
      //    dragging = new NodeLink(it->second->ID, slot_idx, -1, -1, ImGui::GetIO().MouseClickedPos[0] - offset, ImGui::GetIO().MouseClickedPos[0] - offset);
      //    //links.push_back(NodeLink(-1, -1, -1, -1, ImGui::GetIO().MouseClickedPos[0] - offset, ImGui::GetIO().MouseClickedPos[0] - offset));
      //    // *dragging = *(links.end());
      //    DraggingLink = true;
      //  }
      //}
      //ImGui::SameLine(); ImGui::Text(it->second->Inputs[slot_idx].first.c_str());
      ImGui::PopID();
    }

    for (int slot_idx = 0; slot_idx < it->second->Outputs.size(); slot_idx++)
    {
      ImGui::PushID(slot_idx);
      //ImGui::SetCursorPos(offset + it->second->GetOutputSlotPos(slot_idx));
      ImGui::OutputNodePin(it->second->Outputs[slot_idx].first, it->second->Outputs[slot_idx].second, offset + it->second->GetOutputSlotPos(slot_idx));
      //ImColor slotcol = ImColor(150, 150, 150, 150);
      //if (it->second->Outputs[slot_idx].second)
      //{
      //  slotcol = ImColor(10, 10, 10);
      //}

      //ImGui::Text(it->second->Outputs[slot_idx].first.c_str());
      //ImGui::SameLine();
      //draw_list->AddCircleFilled(offset + it->second->GetOutputSlotPos(slot_idx) + ImVec2(NODE_SLOT_RADIUS, NODE_SLOT_RADIUS), NODE_SLOT_RADIUS, slotcol);
      //if (ImGui::InvisibleButton(it->second->Outputs[slot_idx].first.c_str(), ImVec2(NODE_SLOT_RADIUS * 2, NODE_SLOT_RADIUS * 2)))
      //{
      //  ImVec2 pos = ImGui::GetIO().MouseClickedPos[0];

      //  int slot = IfClickingOnSlot(it->second, pos - offset);
      //  if (slot != -1)
      //  {
      //    std::cout << "slot is: " << slot << std::endl;
      //    //it->second->Outputs[slot_idx].second = true;
      //    dragging = new NodeLink(it->second->ID, slot_idx, -1, -1, pos - offset, pos - offset);
      //    //links.push_back(NodeLink(-1, -1, -1, -1, ImGui::GetIO().MouseClickedPos[0] - offset, ImGui::GetIO().MouseClickedPos[0] - offset));
      //    // *dragging = *(links.end());
      //    DraggingLink = true;
      //  }
      //}

      /*if (ImGui::IsItemHovered())
      {
        ImGui::BeginTooltip();
        ImGui::Text("create ping here");
        ImGui::EndTooltip();
      }*/
      //{
      //  int slot = IfClickingOnSlot(it->second, ImGui::GetIO().MouseClickedPos[0] - offset);
      //  if (slot != -1)
      //  {
      //    //std::cout << "slot is: " << slot << std::endl;
      //    //it->second->Outputs[slot_idx].second = true;
      //    dragging = new NodeLink(it->second->ID, slot_idx, -1, -1, ImGui::GetIO().MouseClickedPos[0] - offset, ImGui::GetIO().MouseClickedPos[0] - offset);
      //    //links.push_back(NodeLink(-1, -1, -1, -1, ImGui::GetIO().MouseClickedPos[0] - offset, ImGui::GetIO().MouseClickedPos[0] - offset));
      //    // *dragging = *(links.end());
      //    DraggingLink = true;
      //  }
      //}


      ImGui::PopID();
    }

    ImGui::EndGroup();
    bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
    //it->second->Size = ImVec2(ImGui::GetItemRectSize().x + NODE_WINDOW_PADDING.x + NODE_WINDOW_PADDING.x, ImGui::GetItemRectSize().y + NODE_WINDOW_PADDING.y + NODE_WINDOW_PADDING.y);
    ImVec2 node_rect_max = ImVec2(node_rect_min.x + it->second->Size.x, node_rect_min.y + it->second->Size.y);

    // Display node box
    draw_list->ChannelsSetCurrent(0); // Background
    ImGui::SetCursorScreenPos(node_rect_min);
    ImGui::InvisibleButton("node", it->second->Size);
    if (ImGui::IsItemHovered())
    {
      node_hovered_in_scene = it->second->ID;
      open_context_menu |= ImGui::IsMouseClicked(1);
    }

    //moving node logic
    it->second->node_moving_active = ImGui::IsItemActive();
    if (node_widgets_active || it->second->node_moving_active)
      node_selected = it->second->ID;
    if (it->second->node_moving_active && ImGui::IsMouseDragging(0))
      it->second->Pos = it->second->Pos + ImGui::GetIO().MouseDelta;

    //draw the background rest of draw logic should be under this for draw logic reasons.
    ImU32 node_bg_color = (node_hovered_in_list == it->second->ID || node_hovered_in_scene == it->second->ID || (node_hovered_in_list == -1 && node_selected == it->second->ID)) ? ImColor(75, 75, 75) : ImColor(60, 60, 60);
    draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
    draw_list->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

    ImGui::PopID();
  }
}


int NodeGraph::IfClickingOnSlot(Node* inputs, ImVec2 pos)
{
  //ImVec2 offset(NODE_SLOT_RADIUS, NODE_SLOT_RADIUS);
  //run through  slots to see if you are on them
  for (int i = 0; i <= inputs->Inputs.size(); ++i)
  {
    ImVec2 dist = pos - inputs->GetInputSlotPos(i);
    if (dist.x >= -4.0f && dist.x <= 4.0f)
      if (dist.y >= -4.0f && dist.y <= 4.0f)
        return i;
  }

  for (int i = 0; i <= inputs->Outputs.size(); ++i)
  {
    ImVec2 dist = pos - inputs->GetOutputSlotPos(i);
    if (dist.x >= -4 && dist.x <= 4)
      if (dist.y >= -4 && dist.y <= 4)
        return i;
  }

  return -1;
}




int NodeGraph::GetNextFreeID()
{
  if (recycledids.size() != 0)
  {
    int val = recycledids.back();
    recycledids.pop_back();
    return val;
  }

  return nodes.size();
}

void NodeGraph::CreateLink()
{
}

int Node::GetLongestPinName()
{
  int longest = 0;

  for (auto it = Inputs.begin(); it != Inputs.end(); ++it)
  {
    if (it->first.length() > longest)
      longest = it->first.length();
  }

  for (auto it = Outputs.begin(); it != Outputs.end(); ++it)
  {
    if (it->first.length() > longest)
      longest = it->first.length();
  }

  return longest;
}

Node* NodeGraph::CreateNode(std::string name, ImVec2 spawnpos, std::vector<std::pair<std::string, bool>> inputs, std::vector<std::pair<std::string, bool>> outputs)
{
  int id = GetNextFreeID();
  nodes[id] = new Node(id, name.c_str(), spawnpos, inputs, outputs);
  ImVec2 tempsize = ImVec2(75, 30);
  Node * fresh = nodes[id];
  fresh->GetLongestPinName();
  if (inputs > outputs)
  {
     tempsize.y *= inputs.size();
  }
  else
  {
     tempsize.y *= outputs.size();
  }
  ImGuiContext *test = ImGui::GetCurrentContext();
  tempsize.x += fresh->GetLongestPinName() * 10;

  fresh->Size = tempsize;
  return fresh;
}

void NodeGraph::DestroyNode(Node* dead)
{
  recycledids.push_back(dead->ID);
  nodes.erase(dead->ID); //[dead.ID]
  delete dead;
}
Node* NodeGraph::GetNodeFromID(int id)
{
  return nodes.find(id)->second;
}

void NodeGraph::DisplayConsole()
{
  //TODO set up logging system.
  //ImGui::Text("%s", logger.getline);
}