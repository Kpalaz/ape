#include "imgui_addons.h"
namespace ImGui
{
  IMGUI_API bool TabLabels(int numTabs, const char** tabLabels, int& selectedIndex, const char** tabLabelTooltips, bool wrapMode, int *pOptionalHoveredIndex, int* pOptionalItemOrdering, bool allowTabReorder, bool allowTabClosingThroughMMB, int *pOptionalClosedTabIndex, int *pOptionalClosedTabIndexInsideItemOrdering)
  {
    ImGuiStyle& style = ImGui::GetStyle();

    const ImVec2 itemSpacing = style.ItemSpacing;
    const ImVec4 color = style.Colors[ImGuiCol_Button];
    const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
    const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
    const ImVec4 colorText = style.Colors[ImGuiCol_Text];
    style.ItemSpacing.x = 1;
    style.ItemSpacing.y = 1;
    const ImVec4 colorSelectedTab(color.x, color.y, color.z, color.w*0.5f);
    const ImVec4 colorSelectedTabHovered(colorHover.x, colorHover.y, colorHover.z, colorHover.w*0.5f);
    const ImVec4 colorSelectedTabText(colorText.x*0.8f, colorText.y*0.8f, colorText.z*0.6f, colorText.w*0.8f);
    //ImGui::ClampColor(colorSelectedTabText);

    if (numTabs > 0 && (selectedIndex < 0 || selectedIndex >= numTabs)) {
      if (!pOptionalItemOrdering)  selectedIndex = 0;
      else selectedIndex = -1;
    }
    if (pOptionalHoveredIndex) *pOptionalHoveredIndex = -1;
    if (pOptionalClosedTabIndex) *pOptionalClosedTabIndex = -1;
    if (pOptionalClosedTabIndexInsideItemOrdering) *pOptionalClosedTabIndexInsideItemOrdering = -1;

    float windowWidth = 0.f, sumX = 0.f;
    if (wrapMode) windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY()>0 ? style.ScrollbarSize : 0.f);

    static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;   // These are indices inside pOptionalItemOrdering
    static ImVec2 draggingTabSize(0, 0);
    static ImVec2 draggingTabOffset(0, 0);

    const bool isMMBreleased = ImGui::IsMouseReleased(2);
    const bool isMouseDragging = ImGui::IsMouseDragging(0, 2.f);
    int justClosedTabIndex = -1, newSelectedIndex = selectedIndex;


    bool selection_changed = false; bool noButtonDrawn = true;
    for (int j = 0, i; j < numTabs; j++)
    {
      i = pOptionalItemOrdering ? pOptionalItemOrdering[j] : j;
      if (i == -1) continue;

      if (!wrapMode) { if (!noButtonDrawn) ImGui::SameLine(); }
      else if (sumX > 0.f) {
        sumX += style.ItemSpacing.x;   // Maybe we can skip it if we use SameLine(0,0) below
        sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f*style.FramePadding.x;
        if (sumX > windowWidth) sumX = 0.f;
        else ImGui::SameLine();
      }

      if (i == selectedIndex) {
        // Push the style
        style.Colors[ImGuiCol_Button] = colorSelectedTab;
        style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
        style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
        style.Colors[ImGuiCol_Text] = colorSelectedTabText;
      }
      // Draw the button
      ImGui::PushID(i);   // otherwise two tabs with the same name would clash.
      if (ImGui::Button(tabLabels[i])) { selection_changed = (selectedIndex != i); newSelectedIndex = i; }
      ImGui::PopID();
      if (i == selectedIndex) {
        // Reset the style
        style.Colors[ImGuiCol_Button] = color;
        style.Colors[ImGuiCol_ButtonActive] = colorActive;
        style.Colors[ImGuiCol_ButtonHovered] = colorHover;
        style.Colors[ImGuiCol_Text] = colorText;
      }
      noButtonDrawn = false;

      if (wrapMode) {
        if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
      }
      else if (isMouseDragging && allowTabReorder && pOptionalItemOrdering) {
        // We still need sumX
        if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
        else sumX += style.ItemSpacing.x + ImGui::GetItemRectSize().x;

      }

      if (ImGui::IsItemHoveredRect()) {
        if (pOptionalHoveredIndex) *pOptionalHoveredIndex = i;
        if (tabLabelTooltips && tabLabelTooltips[i] && strlen(tabLabelTooltips[i]) > 0)  ImGui::SetTooltip("%s", tabLabelTooltips[i]);

        if (pOptionalItemOrdering) {
          if (allowTabReorder) {
            if (isMouseDragging) {
              if (draggingTabIndex == -1) {
                draggingTabIndex = j;
                draggingTabSize = ImGui::GetItemRectSize();
                const ImVec2& mp = ImGui::GetIO().MousePos;
                const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
                draggingTabOffset = ImVec2(
                  mp.x + draggingTabSize.x*0.5f - sumX + ImGui::GetScrollX(),
                  mp.y + draggingTabSize.y*0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
                  );

              }
            }
            else if (draggingTabIndex >= 0 && draggingTabIndex < numTabs && draggingTabIndex != j) {
              draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
            }
          }
          if (allowTabClosingThroughMMB) {
            if (isMMBreleased) {
              justClosedTabIndex = i;
              if (pOptionalClosedTabIndex) *pOptionalClosedTabIndex = i;
              if (pOptionalClosedTabIndexInsideItemOrdering) *pOptionalClosedTabIndexInsideItemOrdering = j;
              pOptionalItemOrdering[j] = -1;
            }
          }
        }
      }

    }

    selectedIndex = newSelectedIndex;

    // Draw tab label while mouse drags it
    if (draggingTabIndex >= 0 && draggingTabIndex < numTabs) {
      const ImVec2& mp = ImGui::GetIO().MousePos;
      const ImVec2 wp = ImGui::GetWindowPos();
      ImVec2 start(wp.x + mp.x - draggingTabOffset.x - draggingTabSize.x*0.5f, wp.y + mp.y - draggingTabOffset.y - draggingTabSize.y*0.5f);
      const ImVec2 end(start.x + draggingTabSize.x, start.y + draggingTabSize.y);
      ImDrawList* drawList = ImGui::GetWindowDrawList();
      const float draggedBtnAlpha = 0.65f;
      const ImVec4& btnColor = style.Colors[ImGuiCol_Button];
      drawList->AddRectFilled(start, end, ImColor(btnColor.x, btnColor.y, btnColor.z, btnColor.w*draggedBtnAlpha), style.FrameRounding);
      start.x += style.FramePadding.x; start.y += style.FramePadding.y;
      const ImVec4& txtColor = style.Colors[ImGuiCol_Text];
      drawList->AddText(start, ImColor(txtColor.x, txtColor.y, txtColor.z, txtColor.w*draggedBtnAlpha), tabLabels[pOptionalItemOrdering[draggingTabIndex]]);

      ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
    }

    // Drop tab label
    if (draggingTabTargetIndex != -1) {
      // swap draggingTabIndex and draggingTabTargetIndex in pOptionalItemOrdering
      const int tmp = pOptionalItemOrdering[draggingTabTargetIndex];
      pOptionalItemOrdering[draggingTabTargetIndex] = pOptionalItemOrdering[draggingTabIndex];
      pOptionalItemOrdering[draggingTabIndex] = tmp;
      //fprintf(stderr,"%d %d\n",draggingTabIndex,draggingTabTargetIndex);
      draggingTabTargetIndex = draggingTabIndex = -1;
    }

    // Reset draggingTabIndex if necessary
    if (!isMouseDragging) draggingTabIndex = -1;

    // Change selected tab when user closes the selected tab
    if (selectedIndex == justClosedTabIndex && selectedIndex >= 0) {
      selectedIndex = -1;
      for (int j = 0, i; j < numTabs; j++) {
        i = pOptionalItemOrdering ? pOptionalItemOrdering[j] : j;
        if (i == -1) continue;
        selectedIndex = i;
        break;
      }
    }

    // Restore the style
    style.Colors[ImGuiCol_Button] = color;
    style.Colors[ImGuiCol_ButtonActive] = colorActive;
    style.Colors[ImGuiCol_ButtonHovered] = colorHover;
    style.Colors[ImGuiCol_Text] = colorText;
    style.ItemSpacing = itemSpacing;

    return selection_changed;
  }
  bool InputNodePin(std::string name, bool connection, ImVec2 pos)
  {

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
      return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name.c_str());
    const ImVec2 label_size = CalcTextSize(name.c_str(), NULL, true);

    if (label_size.x < 0.0f)
    {
      return false;
    }


    SetCursorScreenPos(pos);
    const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2 - 1, label_size.y + style.FramePadding.y * 2 - 1));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;

    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) - label_size);
    if (label_size.x > 0)
    {
      ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
      total_bb.Add(text_bb);
    }

    if (!ItemAdd(total_bb, &id))
      return false;

    ImVec2 center = check_bb.GetTL();
    center.x = (float)(int)center.x + 0.5f;
    center.y = (float)(int)center.y + 0.5f;
    const float radius = check_bb.GetHeight() * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    //TODO find a better solution
    //if (label_size.x > 0)
    //  SameLine(0, style.ItemInnerSpacing.x);
    window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    if (connection)
    {
      const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
      const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
      window->DrawList->AddCircleFilled(center, radius - pad, GetColorU32(ImGuiCol_CheckMark), 16);
    }

    if (window->Flags & ImGuiWindowFlags_ShowBorders)
    {
      window->DrawList->AddCircle(center + ImVec2(1, 1), radius, GetColorU32(ImGuiCol_BorderShadow), 16);
      window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border), 16);
    }

    RenderText(ImVec2(center.x + radius + name.length(),center.y - radius), name.c_str());
    return pressed;
  }

  bool ImGui::OutputNodePin(std::string name, bool connection, ImVec2 pos)
  {
    

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
      return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name.c_str());
    const ImVec2 label_size = CalcTextSize(name.c_str(), NULL, true);

    if (label_size.x < 0.0f)
    {
      return false;
    }


   SetCursorScreenPos(pos);
    const ImRect check_bb(window->DC.CursorPos,window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2 - 1,label_size.y + style.FramePadding.y * 2 - 1));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) - label_size);
    if (label_size.x > 0)
    {
      ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
      total_bb.Add(text_bb);
    }

    if (!ItemAdd(total_bb, &id))
      return false;

    ImVec2 center =  check_bb.GetTL();
    center.x = (float)(int)center.x + 0.5f;
    center.y = (float)(int)center.y + 0.5f;
    const float radius = check_bb.GetHeight() * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    
    //TODO find a better solution
    RenderText(ImVec2(center.x - label_size.x - name.length() - radius, center.y - radius), name.c_str());
    if (label_size.x > 0)
      SameLine(0, style.ItemInnerSpacing.x);
    window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    if (connection)
    {
      const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
      const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
      window->DrawList->AddCircleFilled(center, radius - pad, GetColorU32(ImGuiCol_CheckMark), 16);
    }

    if (window->Flags & ImGuiWindowFlags_ShowBorders)
    {
      window->DrawList->AddCircle(center + ImVec2(1, 1), radius, GetColorU32(ImGuiCol_BorderShadow), 16);
      window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border), 16);
    }
    
    return pressed;
  }

}

/*
ImGuiStyle& style = ImGui::GetStyle();
const ImVec2 itemSpacing = style.ItemSpacing;
const ImVec4 color = style.Colors[ImGuiCol_Button];
const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
const ImVec4 colorText = style.Colors[ImGuiCol_Text];

ImColor slotcol = ImColor(150, 150, 150, 150);
ImDrawList *draw_list = ImGui::GetWindowDrawList();
//draw_list->ChannelsSplit(2);

ImGui::SetCursorScreenPos(pos - ImVec2(name.length()* ImGui::GetFontSize() *0.6f, ImGui::GetFontSize() *0.6f));
//ImGui::AlignFirstTextHeightToWidgets();
ImGui::Text("%s", name.c_str());
ImGui::SameLine();
//pos + ImVec2(name.length()* ImGui::GetFontSize(),0)
draw_list->AddCircleFilled(GetCursorScreenPos() + ImVec2(0, ImGui::GetFontSize() - 5.0f), 4.0f, slotcol, 16);
ImGui::NewLine();
//draw_list->ChannelsMerge();
return false;



*/