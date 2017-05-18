/****************************************************************************/
/*!
\file   Reverb.cpp
\author Kaan Palaz
\par    email: kaan.palaz\@digipen.edu
\par    DigiPen login: kaan.palaz
\par    Course: MAT321
\par    Assignment #1
\date   03/07/2017
\brief
compile under g++ using g++ Reverb.cpp -std=c++11 -o Reverb
use ./reverb <input.wav>
*/
/*************************************************************************/

#include "ReverbGui.h"

//namespace ImGui {
//
//  // Based on the code by krys-spectralpixel (https://github.com/krys-spectralpixel), posted here: https://github.com/ocornut/imgui/issues/261
//  /* pOptionalHoveredIndex: a ptr to an optional int that is set to -1 if no tab label is hovered by the mouse.
//  * pOptionalItemOrdering: an optional static array of unique integers from 0 to numTabs-1 that maps the tab label order. If one of the numbers is replaced by -1 the tab label is not visible (closed). It can be read/modified at runtime.
//  * allowTabReorder (requires pOptionalItemOrdering): allows tab reordering through drag and drop (it modifies pOptionalItemOrdering).
//  * allowTabClosingThroughMMB (requires pOptionalItemOrdering): closes the tabs when MMB is clicked on them, by setting the tab value in pOptionalItemOrdering to -1.
//  * pOptionalClosedTabIndex (requires allowTabClosingThroughMMB): out variable (int pointer) that returns the index of the closed tab in last call or -1.
//  * pOptionalClosedTabIndexInsideItemOrdering: same as above, but index of the pOptionalItemOrdering array.
//  */
//  IMGUI_API bool TabLabels(int numTabs, const char** tabLabels, int& selectedIndex, const char** tabLabelTooltips = NULL, bool wrapMode = true, int *pOptionalHoveredIndex = NULL, int* pOptionalItemOrdering = NULL, bool allowTabReorder = true, bool allowTabClosingThroughMMB = true, int *pOptionalClosedTabIndex = NULL, int *pOptionalClosedTabIndexInsideItemOrdering = NULL) {
//    ImGuiStyle& style = ImGui::GetStyle();
//
//    const ImVec2 itemSpacing = style.ItemSpacing;
//    const ImVec4 color = style.Colors[ImGuiCol_Button];
//    const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
//    const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
//    const ImVec4 colorText = style.Colors[ImGuiCol_Text];
//    style.ItemSpacing.x = 1;
//    style.ItemSpacing.y = 1;
//    const ImVec4 colorSelectedTab(color.x, color.y, color.z, color.w*0.5f);
//    const ImVec4 colorSelectedTabHovered(colorHover.x, colorHover.y, colorHover.z, colorHover.w*0.5f);
//    const ImVec4 colorSelectedTabText(colorText.x*0.8f, colorText.y*0.8f, colorText.z*0.6f, colorText.w*0.8f);
//    //ImGui::ClampColor(colorSelectedTabText);
//
//    if (numTabs > 0 && (selectedIndex < 0 || selectedIndex >= numTabs)) {
//      if (!pOptionalItemOrdering)  selectedIndex = 0;
//      else selectedIndex = -1;
//    }
//    if (pOptionalHoveredIndex) *pOptionalHoveredIndex = -1;
//    if (pOptionalClosedTabIndex) *pOptionalClosedTabIndex = -1;
//    if (pOptionalClosedTabIndexInsideItemOrdering) *pOptionalClosedTabIndexInsideItemOrdering = -1;
//
//    float windowWidth = 0.f, sumX = 0.f;
//    if (wrapMode) windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY()>0 ? style.ScrollbarSize : 0.f);
//
//    static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;   // These are indices inside pOptionalItemOrdering
//    static ImVec2 draggingTabSize(0, 0);
//    static ImVec2 draggingTabOffset(0, 0);
//
//    const bool isMMBreleased = ImGui::IsMouseReleased(2);
//    const bool isMouseDragging = ImGui::IsMouseDragging(0, 2.f);
//    int justClosedTabIndex = -1, newSelectedIndex = selectedIndex;
//
//
//    bool selection_changed = false; bool noButtonDrawn = true;
//    for (int j = 0, i; j < numTabs; j++)
//    {
//      i = pOptionalItemOrdering ? pOptionalItemOrdering[j] : j;
//      if (i == -1) continue;
//
//      if (!wrapMode) { if (!noButtonDrawn) ImGui::SameLine(); }
//      else if (sumX > 0.f) {
//        sumX += style.ItemSpacing.x;   // Maybe we can skip it if we use SameLine(0,0) below
//        sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f*style.FramePadding.x;
//        if (sumX > windowWidth) sumX = 0.f;
//        else ImGui::SameLine();
//      }
//
//      if (i == selectedIndex) {
//        // Push the style
//        style.Colors[ImGuiCol_Button] = colorSelectedTab;
//        style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
//        style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
//        style.Colors[ImGuiCol_Text] = colorSelectedTabText;
//      }
//      // Draw the button
//      ImGui::PushID(i);   // otherwise two tabs with the same name would clash.
//      if (ImGui::Button(tabLabels[i])) { selection_changed = (selectedIndex != i); newSelectedIndex = i; }
//      ImGui::PopID();
//      if (i == selectedIndex) {
//        // Reset the style
//        style.Colors[ImGuiCol_Button] = color;
//        style.Colors[ImGuiCol_ButtonActive] = colorActive;
//        style.Colors[ImGuiCol_ButtonHovered] = colorHover;
//        style.Colors[ImGuiCol_Text] = colorText;
//      }
//      noButtonDrawn = false;
//
//      if (wrapMode) {
//        if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
//      }
//      else if (isMouseDragging && allowTabReorder && pOptionalItemOrdering) {
//        // We still need sumX
//        if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
//        else sumX += style.ItemSpacing.x + ImGui::GetItemRectSize().x;
//
//      }
//
//      if (ImGui::IsItemHoveredRect()) {
//        if (pOptionalHoveredIndex) *pOptionalHoveredIndex = i;
//        if (tabLabelTooltips && tabLabelTooltips[i] && strlen(tabLabelTooltips[i]) > 0)  ImGui::SetTooltip("%s", tabLabelTooltips[i]);
//
//        if (pOptionalItemOrdering) {
//          if (allowTabReorder) {
//            if (isMouseDragging) {
//              if (draggingTabIndex == -1) {
//                draggingTabIndex = j;
//                draggingTabSize = ImGui::GetItemRectSize();
//                const ImVec2& mp = ImGui::GetIO().MousePos;
//                const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
//                draggingTabOffset = ImVec2(
//                  mp.x + draggingTabSize.x*0.5f - sumX + ImGui::GetScrollX(),
//                  mp.y + draggingTabSize.y*0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
//                  );
//
//              }
//            }
//            else if (draggingTabIndex >= 0 && draggingTabIndex < numTabs && draggingTabIndex != j) {
//              draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
//            }
//          }
//          if (allowTabClosingThroughMMB) {
//            if (isMMBreleased) {
//              justClosedTabIndex = i;
//              if (pOptionalClosedTabIndex) *pOptionalClosedTabIndex = i;
//              if (pOptionalClosedTabIndexInsideItemOrdering) *pOptionalClosedTabIndexInsideItemOrdering = j;
//              pOptionalItemOrdering[j] = -1;
//            }
//          }
//        }
//      }
//
//    }
//
//    selectedIndex = newSelectedIndex;
//
//    // Draw tab label while mouse drags it
//    if (draggingTabIndex >= 0 && draggingTabIndex < numTabs) {
//      const ImVec2& mp = ImGui::GetIO().MousePos;
//      const ImVec2 wp = ImGui::GetWindowPos();
//      ImVec2 start(wp.x + mp.x - draggingTabOffset.x - draggingTabSize.x*0.5f, wp.y + mp.y - draggingTabOffset.y - draggingTabSize.y*0.5f);
//      const ImVec2 end(start.x + draggingTabSize.x, start.y + draggingTabSize.y);
//      ImDrawList* drawList = ImGui::GetWindowDrawList();
//      const float draggedBtnAlpha = 0.65f;
//      const ImVec4& btnColor = style.Colors[ImGuiCol_Button];
//      drawList->AddRectFilled(start, end, ImColor(btnColor.x, btnColor.y, btnColor.z, btnColor.w*draggedBtnAlpha), style.FrameRounding);
//      start.x += style.FramePadding.x; start.y += style.FramePadding.y;
//      const ImVec4& txtColor = style.Colors[ImGuiCol_Text];
//      drawList->AddText(start, ImColor(txtColor.x, txtColor.y, txtColor.z, txtColor.w*draggedBtnAlpha), tabLabels[pOptionalItemOrdering[draggingTabIndex]]);
//
//      ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
//    }
//
//    // Drop tab label
//    if (draggingTabTargetIndex != -1) {
//      // swap draggingTabIndex and draggingTabTargetIndex in pOptionalItemOrdering
//      const int tmp = pOptionalItemOrdering[draggingTabTargetIndex];
//      pOptionalItemOrdering[draggingTabTargetIndex] = pOptionalItemOrdering[draggingTabIndex];
//      pOptionalItemOrdering[draggingTabIndex] = tmp;
//      //fprintf(stderr,"%d %d\n",draggingTabIndex,draggingTabTargetIndex);
//      draggingTabTargetIndex = draggingTabIndex = -1;
//    }
//
//    // Reset draggingTabIndex if necessary
//    if (!isMouseDragging) draggingTabIndex = -1;
//
//    // Change selected tab when user closes the selected tab
//    if (selectedIndex == justClosedTabIndex && selectedIndex >= 0) {
//      selectedIndex = -1;
//      for (int j = 0, i; j < numTabs; j++) {
//        i = pOptionalItemOrdering ? pOptionalItemOrdering[j] : j;
//        if (i == -1) continue;
//        selectedIndex = i;
//        break;
//      }
//    }
//
//    // Restore the style
//    style.Colors[ImGuiCol_Button] = color;
//    style.Colors[ImGuiCol_ButtonActive] = colorActive;
//    style.Colors[ImGuiCol_ButtonHovered] = colorHover;
//    style.Colors[ImGuiCol_Text] = colorText;
//    style.ItemSpacing = itemSpacing;
//
//    return selection_changed;
//  }
//
//}

void GUI::Init()
{
  Pa_Initialize();


  output_params.device = Pa_GetDefaultOutputDevice();
  output_params.channelCount = 1;
  output_params.sampleFormat = paFloat32;
  output_params.suggestedLatency = Pa_GetDeviceInfo(output_params.device)
    ->defaultLowOutputLatency;
  output_params.hostApiSpecificStreamInfo = 0;
  if (output_params.device == paNoDevice) {
    std::cout << "no output device" << std::endl;
    Pa_Terminate();
    return;
  }
}



void GUI::Update()
{
  //if (!globalplay)
  //{
  //  Pa_StopStream(stream);
  //  Pa_CloseStream(stream);
  //  Pa_Terminate();
  //}
  ImGui::Begin("Potion GUI", 0, ImVec2(2000, 1100), -1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar);


  // ImGui::NewLine();
  // ImGui::NewLine();
  // ImGui::NewLine();


  if (ImGui::Button("Choose Input:"))
  {
    filename = ask_user_for_file("Choose a .wav file to import", LPCSTR("WAV Files\0*.wav"));
    if (filename.size() != 0)
    {
      outname = get_path_from_input(filename);
      Calc(filename);
      CalcImpulse();
    }
  }
  ImGui::SameLine();
  ImGui::Text("Current File:");
  ImGui::SameLine();
  ImGui::Text(filename.c_str());

  if (ImGui::Button("Play"))
  {
    //cursoroffset = 0;
    //playheadoffset = -5.0f;
    if (selection == 0)
    {
      // ShellExecute(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWMINIMIZED);
        //port = new PA(grate, floatdata, count, false);
        //verb = new Reverb(rate, samples);
        //std::cout << error << std::endl;
        //Pa_StartStream(stream);
      globalplay = true;
      internalverb = new Reverb(rate, samples);
      internalverb->myindex = passedindex;
      temp = new PA(rate, internalverb, 1, false);
      PaError error = Pa_OpenStream(&stream, 0, &output_params, (float)rate, 0, 0,
        PA::OnWrite, temp);
    }
    if (selection == 1)
    {
      //ShellExecute(NULL, "open", outname.c_str(), NULL, NULL, SW_SHOWMINIMIZED);
      //port = new PA(rate, floatdataout, count, false);
      //verb = new Reverb(rate, samples2);
      //std::cout << error << std::endl;
      globalplay = true;
      internalverb = new Reverb(rate, samples2);
      internalverb->myindex = passedindex;
      temp = new PA(rate, internalverb, 1, false);
      PaError error = Pa_OpenStream(&stream, 0, &output_params, (float)rate, 0, 0,
        PA::OnWrite, temp);
    }
    Pa_StartStream(stream);
  }
  ImGui::SameLine();
  // if (ImGui::Button("View"))
  // {
  //   showgraphs = !showgraphs;
  // }
  // ImGui::SameLine();
  if (ImGui::Button("Calculate"))
  {
    if (filename.size() > 0)
    {
      samples = 0;
      samples2 = 0;
      Calc(filename);
      CalcImpulse();
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Reset"))
  {
    Reset();
  }
  ImGui::SameLine();
  ImGui::TabLabels(numTabs, tabNames, selectedTab, tabTooltips, true, &optionalHoveredTab, &tabItemOrdering[0], true, true);

  ImGui::BeginChild("top", ImVec2(1800, 900), false);
  ImGui::Columns(2, "", false);
  ImGui::PushItemWidth(150); ImGui::SliderFloat("G Modifier:", &gmod, 0.1, 1.0f); ImGui::PopItemWidth();
  ImGui::PushItemWidth(150); ImGui::SliderFloat("G Ratio:", &ratioval, 0.1, 0.999f); ImGui::PopItemWidth();
  ImGui::PushItemWidth(150); ImGui::SliderFloat("Wet \% :", &wet_m, 0.0, 1.0f); ImGui::PopItemWidth();
  ImGui::PushItemWidth(150); ImGui::SliderFloat("Dry \% (visual only) :", &dry_m, 0.0, 1.0f); ImGui::PopItemWidth();
  ImGui::PushItemWidth(150); ImGui::SliderInt("M Modifier (ms) :", &m_m, 1, 100); ImGui::PopItemWidth();
  ImGui::PushItemWidth(150); ImGui::SliderFloat("a Modifier:", &a_m, 0.0f, 0.999f); ImGui::PopItemWidth();
  ImGui::PushItemWidth(150); ImGui::SliderInt("L Modifier :", &l_m, 1, 100); ImGui::PopItemWidth();



  ImGui::NextColumn();
  ImGui::SetColumnOffset(1, ImGui::GetColumnOffset(0) + 325.0f);
  Delta();
  if (selectedTab == 0)
  {
    ImGui::Text("G1: %f \t \t", gcal[0]);  ImGui::SameLine(); ImGui::Text("\t \t G2: %f \t \t", gcal[1]); ImGui::SameLine(); ImGui::Text("\t \t G3: %f", gcal[2]);
    ImGui::Text("G4: %f \t \t", gcal[3]);  ImGui::SameLine(); ImGui::Text("\t \t G5: %f \t \t", gcal[4]); ImGui::SameLine(); ImGui::Text("\t \t G6: %f ", gcal[5]);
    ImGui::Text("\n  ");
    ImGui::Text("R1: %f \t \t", rcal[0]);  ImGui::SameLine(); ImGui::Text("\t \t R2: %f \t \t", rcal[1]); ImGui::SameLine(); ImGui::Text("\t \t R3: %f", rcal[2]);
    ImGui::Text("R4: %f \t \t", rcal[3]);  ImGui::SameLine(); ImGui::Text("\t \t R5: %f \t \t", rcal[4]); ImGui::SameLine(); ImGui::Text("\t \t R6: %f", rcal[5]);
    ImGui::Text("\n ");
    ImGui::Text("L1: %i \t \t \t", Lcal[0]);  ImGui::SameLine(); ImGui::Text(" \t \t L2: %i \t \t \t", Lcal[1]); ImGui::SameLine(); ImGui::Text(" \t \t L3: %i \t \t", Lcal[2]);
    ImGui::Text("L4: %i \t \t \t", Lcal[3]);  ImGui::SameLine(); ImGui::Text(" \t \t L5: %i \t \t \t", Lcal[4]); ImGui::SameLine(); ImGui::Text(" \t \t L6: %i \t \t", Lcal[5]);
  }

  if (selectedTab == 1)
  {
    EditorNodeGraph();
  }



  if (showgraphs && selectedTab == 2)
  {
    //ImGui::SliderInt("Level of Detail", &display_count, 1, max_count);
    //ImGui::SliderInt("Window Offset", &display_offset, 1, max_count);
    // ImGui::Text("PlayHead: V");
    ImGui::PushItemWidth(100); ImGui::Combo("", &selection, Selections, IM_ARRAYSIZE(Selections)); ImGui::PopItemWidth();
    if (outname.size() > 0)
    {
      ImGui::PushItemWidth(150); ImGui::SliderInt("Zoom: ", &display_count, 1, max_count); ImGui::PopItemWidth();
    }
    //ImGui::Text("Passed Index: %i, Playheadoffset: %f", passedindex, playheadoffset);
    //ImGui::Text("Playhead Region -------------------------------------------------------------------------");
    //
    //float temp = ImGui::GetCursorPos().x;
    //if (ImGui::Button("timeline region", ImVec2(2000, 75))) {}
    //if (ImGui::IsItemHovered() && ImGui::IsItemClicked(0))
    //{
    //  
    //  passedindex = static_cast<int>(((ImGui::GetMousePos().x - temp) * max_count) / 2000.0f);// -4256;
    //
    //
    //  playheadoffset = static_cast<float>(passedindex * 0.01103f ) - 2.0f;
    //
    //}
    //ImGui::Text("-----------------------------------------------------------------------------------------");

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (selection == 0 && filename.size() > 0)
    {
      struct Funcs
      {
        static float Normal(void*, int i) { return floatdata[i] * 0.5f; }
        //static float Impulse(void*, int i) { return (i & 1) ? 1.0f : 0.0f; }
      };
      float(*func)(void*, int) = Funcs::Normal;
      // float(*func2)(void*, int) = Funcs::Impulse;
      if (selectedTab == 2)
      {
        //ImGui::Text("Custom Index = %i", passedindex);
        //ImGui::SameLine();
        draw_list->ChannelsSetCurrent(0);
        draw_list->AddLine(ImVec2(ImGui::GetCursorPos().x + playheadoffset, ImGui::GetCursorPos().y + 65.0f), ImVec2(ImGui::GetCursorPos().x + playheadoffset, ImGui::GetCursorPos().y + 350.0f), ImU32(ImColor(255, 0, 0)), 2.0f);
        ImGui::PlotLines("", func, NULL, display_count, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(1850, 300));
        draw_list->ChannelsMerge();

      }
    }

    if (selection == 1 && outname.size() > 0)
    {
      //static ImDrawList* draw_list = ImGui::GetWindowDrawList();
      struct Funcs
      {
        static float Normal(void*, int i) { return floatdataout[i] * 0.5f; }
        static float Impulse(void*, int i) { return impulseout[i] * 0.001f; }
      };
      float(*func)(void*, int) = Funcs::Normal;
      float(*func2)(void*, int) = Funcs::Impulse;
      draw_list->ChannelsSetCurrent(0);
      draw_list->AddLine(ImVec2(ImGui::GetCursorPos().x + playheadoffset, ImGui::GetCursorPos().y + 65.0f), ImVec2(ImGui::GetCursorPos().x + playheadoffset, ImGui::GetCursorPos().y + 350.0f), ImU32(ImColor(255, 0, 0)), 2.0f);
      ImGui::PlotLines("", func, NULL, display_count, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(1850, 300));


    }
  }
  if (selectedTab == 3)
  {
    if (outname.size() > 0)
    {
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      struct Funcs
      {
        static float Normal(void*, int i) { return floatdataout[i] * 0.5f; }
        static float Impulse(void*, int i) { return impulseout[i] * 0.001f; }
      };
      float(*func)(void*, int) = Funcs::Normal;
      float(*func2)(void*, int) = Funcs::Impulse;
      draw_list->AddLine(ImVec2(ImGui::GetCursorPos().x + playheadoffset, ImGui::GetCursorPos().y), ImVec2(ImGui::GetCursorPos().x + playheadoffset, ImGui::GetCursorPos().y + 350.0f), ImU32(ImColor(255, 0, 0)), 2.0f);
      ImGui::PlotLines("", func2, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(2000, 300));
      draw_list->ChannelsMerge();
    }
  }

  ImGui::EndChild();
  ImGui::End();
}

void GUI::Delta()
{
  gcal[0] = gmod * g[0];
  gcal[1] = gmod * g[1];
  gcal[2] = gmod * g[2];
  gcal[3] = gmod * g[3];
  gcal[4] = gmod * g[4];
  gcal[5] = gmod * g[5];
  rcal[0] = ratioval * (1.0f - gcal[0]);
  rcal[1] = ratioval * (1.0f - gcal[1]);
  rcal[2] = ratioval * (1.0f - gcal[2]);
  rcal[3] = ratioval * (1.0f - gcal[3]);
  rcal[4] = ratioval * (1.0f - gcal[4]);
  rcal[5] = ratioval * (1.0f - gcal[5]);
  Lcal[0] = L[0] * l_m;
  Lcal[1] = L[1] * l_m;
  Lcal[2] = L[2] * l_m;
  Lcal[3] = L[3] * l_m;
  Lcal[4] = L[4] * l_m;
  Lcal[5] = L[5] * l_m;
  dry_m = 1.0f - wet_m;
  wet_m = (1.0f - dry_m);
}

void GUI::Reset()
{
  gmod = 1.0f;
  dry_m = dry;
  wet_m = wet;
  ratioval = 0.83f;
  m_m = m;
  l_m = 1;
  a_m = a;
}

void GUI::Calc(std::string filename)
{
  //init
  std::fstream in(filename, std::ios_base::binary | std::ios_base::in);
  char header[44];
  in.read(header, 44);
  size = *reinterpret_cast<unsigned*>(header + 40);
  rate = *reinterpret_cast<unsigned*>(header + 24);
  // grate = rate;
  count = size / 2;
  //std::cout << rate << std::endl;
  data = new  char[size];
  data2 = new  char[size];
  samples = reinterpret_cast<short*>(data);
  samples2 = reinterpret_cast<short*>(data2);
  in.read(data, size);
  floatdata = new float[count];
  floatdataout = new float[count];

  for (int i = 0; i < count; ++i)
  {
    floatdata[i] = (float)samples[i];
  }
  LowPassComb comb[6] = { LowPassComb(Lcal[0] / 1000.0f * rate,gcal[0],rcal[0]),
                          LowPassComb(Lcal[1] / 1000.0f * rate,gcal[1],rcal[1]),
                          LowPassComb(Lcal[2] / 1000.0f * rate,gcal[2],rcal[2]),
                          LowPassComb(Lcal[3] / 1000.0f * rate,gcal[3],rcal[3]),
                          LowPassComb(Lcal[4] / 1000.0f * rate,gcal[4],rcal[4]),
                          LowPassComb(Lcal[5] / 1000.0f * rate,gcal[5],rcal[5]) };

  AllPass allpass(m_m, a);
  for (int i = 0; i < count; ++i)
  {

    floatdataout[i] = (allpass.operator()(comb[0].operator()(floatdata[i]) +
      comb[1].operator()(floatdata[i]) +
      comb[2].operator()(floatdata[i]) +
      comb[3].operator()(floatdata[i]) +
      comb[4].operator()(floatdata[i]) +
      comb[5].operator()(floatdata[i])) * wet_m) + (floatdata[i] * dry_m);
  }

  //grab largest value
  float m1 = -1.0f;
  for (int i = 0; i < count; ++i)
  {
    float temp = std::fabs((floatdataout[i]));
    if (temp > m1)
    {
      m1 = temp;
    }
  }

  //pre compute normal coefficient
  float normalize = (float)NORMAL / m1;
  for (int i = 0; i < count; ++i)
  {
    floatdataout[i] *= (normalize);
  }

  //cast to shorts
  for (int i = 0; i < count; ++i)
  {
    samples2[i] = (short)floatdataout[i];
  }

  //clean up
  //std::fstream out("output.wav", std::ios_base::binary | std::ios_base::out);
  //out.write((header), 44);
  //out.write(data2, size);
  //delete[] data;
  //delete[] data2;
  //verb = new Reverb(rate);
  //verb->datain = floatdata;
  //verb->dataout = floatdata;
  //delete[] floatdata;
  //delete[] floatdataout;
 // ::floatdataout = floatdataout;
  //grate = rate;
  max_count = count;
  display_count = count;
  //std::cout << max_count << std::endl;
  return;
}

void GUI::CalcImpulse()
{

  float* impulsein = new float[count];// new float[count];
  impulseout = new float[count];
  for (int i = 0; i < count; ++i)
    impulsein[i] = 0.0f;

  impulsein[0] = 0.99f;

  LowPassComb comb[6] = { LowPassComb(Lcal[0] / 1000.0f * rate,gcal[0],rcal[0]),
    LowPassComb(Lcal[1] / 1000.0f * rate,gcal[1],rcal[1]),
    LowPassComb(Lcal[2] / 1000.0f * rate,gcal[2],rcal[2]),
    LowPassComb(Lcal[3] / 1000.0f * rate,gcal[3],rcal[3]),
    LowPassComb(Lcal[4] / 1000.0f * rate,gcal[4],rcal[4]),
    LowPassComb(Lcal[5] / 1000.0f * rate,gcal[5],rcal[5]) };

  AllPass allpass(m_m, a);
  for (int i = 0; i < count; ++i)
  {

    impulseout[i] = (allpass.operator()(comb[0].operator()(impulsein[i]) +
      comb[1].operator()(impulsein[i]) +
      comb[2].operator()(impulsein[i]) +
      comb[3].operator()(impulsein[i]) +
      comb[4].operator()(impulsein[i]) +
      comb[5].operator()(impulsein[i])));
  }

  //grab largest value
  float m1 = -1.0f;
  for (int i = 0; i < count; ++i)
  {
    float temp = impulseout[i];// std::fabs((impulseout[i]));
    if (temp > m1)
    {
      m1 = temp;
    }
  }

  //pre compute normal coefficient
  float normalize = (float)NORMAL / m1;
  for (int i = 0; i < count; ++i)
  {
    impulseout[i] *= (normalize);
  }

  impulseout[0] = 0;
  //clean up
  //std::fstream out("output.wav", std::ios_base::binary | std::ios_base::out);
  //out.write((header), 44);
  //out.write(data2, size);
  //delete[] data;
  //delete[] data2;
  //verb = new Reverb(rate);
  //verb->datain = floatdata;
  //verb->dataout = floatdata;
  //delete[] floatdata;
  //delete[] floatdataout;
 // delete[] impulsein;
  // ::floatdataout = floatdataout;
  //grate = rate;
  // = count;
  //display_count = count;
  //std::cout << max_count << std::endl;
  return;
}

float LowPassComb::operator()(float sample)
{
  float outm1 = Yqueue.back();
  float outmL = Yqueue.front();
  Yqueue.pop();
  float prevmLp1 = Xqueue.front();
  Xqueue.pop();
  float prevmL = Xqueue.front();


  float out = prevmL - (g * prevmLp1) + (g * outm1) + (R * outmL);
  Xqueue.push(sample);
  Yqueue.push(out);
  return out;
}

float AllPass::operator()(float summation)
{
  float Xm = Xqueue.front();
  float Ym = Yqueue.front();
  Xqueue.pop();
  Yqueue.pop();

  float out = (a_m * summation) + Xm - (a_m * Ym);
  Yqueue.push(out);
  Xqueue.push(summation);
  return out;
}

std::string GUI::ask_user_for_file(std::string window_prompt, LPCSTR filetypes)
{
  char filename[MAX_PATH];
  std::string temp = "C:/";//IFRToolLog.txt";
  for (int i = 0; i < temp.size(); ++i)
    filename[i] = temp[i];
  filename[temp.size()] = '\0';
  OPENFILENAME ofn;
  GetModuleFileName(NULL, filename, MAX_PATH);
  ZeroMemory(&filename, sizeof(filename));
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = window_;  // If you have a window to center over, put its HANDLE here
  ofn.lpstrFilter = filetypes;
  ofn.lpstrFile = filename;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrTitle = window_prompt.c_str();
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileNameA(&ofn))
  {
    return std::string(filename);
  }
  else
  {
    return std::string("\0");
  }
}

float Reverb::operator()(float x)
{
  if (globalplay == false)
    return 0.0f;


  //std::cout << myindex << std::endl;
  if (myindex == max_count)
  {
    Pa_StopStream(stream);
    //Pa_CloseStream(stream);
    //Pa_Terminate();
    Reset();
    return 0.0f;
  }
  float out = data[myindex++];
  cursoroffset++;
  playheadoffset += 0.01103f;
  //std::cout << out << std::endl;
  return out;
}

void Reverb::Reset()
{
  if (data)
  {
    //delete[] data;
    //data = 0;
  }
  globalplay = false;
  myindex = 0;
  cursoroffset = 0;
  playheadoffset = -2.0f;
  passedindex = 0;
  //f0 = freq;
  //numberofsemitones = semitones;
  //datasize = myrate*numberofsemitones;
//  data = new float[count];
}

Reverb::Reverb(float rate, short * in)
{
  myindex = 0;
  myrate = rate;
  data = in;
  this->count = 1;

}

Reverb::~Reverb()
{
  //myrate = rate;
}


std::string GUI::get_path_from_input(std::string input)
{

  return input.substr(0, input.find_last_of("\\") + 1) + "output.wav";
}


PA::PA(float R, Reverb *fverb, unsigned fcount, bool fstereo)
{
  rate = R;
  verb = fverb;
  count = 1;
  stereo = fstereo;
}

int PA::OnWrite(const void *vin, void *vout, unsigned long frames,
  const PaStreamCallbackTimeInfo *tinfo,
  PaStreamCallbackFlags flags, void *user)
{
  const float IMAX = 1.0f / float(1 << 15);

  PA& demo = *reinterpret_cast<PA*>(user);
  float *out = reinterpret_cast<float*>(vout);

  for (unsigned long i = 0; i < frames; ++i) {
    float x = IMAX * demo.verb->operator()(0.0f);// (x);// [demo.index];
    out[i] = x;// demo.samples[demo.index];
    demo.index = (demo.index + 1) % demo.count;
  }


  return paContinue;
}



void GUI::EditorNodeGraph()
{
  //ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);
  ImGui::BeginChild(100, ImVec2(1600, 800));
  static ImVector<Node> nodes;
  static ImVector<NodeLink> links;
  static bool inited = false;
  static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
  static bool show_grid = true;
  static int node_selected = -1;
  if (!inited)
  {

    nodes.push_back(Node(0, "Input", ImVec2(0, 361), 0, 1));
    nodes.push_back(Node(1, "Output", ImVec2(1284, 359), 1, 0));
    nodes.push_back(Node(2, "Comb 1", ImVec2(242, 41), 1, 1));
    nodes.push_back(Node(3, "Comb 2", ImVec2(242, 169), 1, 1));
    nodes.push_back(Node(4, "Comb 3", ImVec2(242, 293), 1, 1));
    nodes.push_back(Node(5, "Comb 4", ImVec2(242, 433), 1, 1));
    nodes.push_back(Node(6, "Comb 5", ImVec2(242, 551), 1, 1));
    nodes.push_back(Node(7, "Comb 6", ImVec2(242, 680), 1, 1));
    nodes.push_back(Node(8, "Allpass", ImVec2(771, 361), 1, 1));
    nodes.push_back(Node(9, "Wet Dry", ImVec2(1038, 361), 2, 1));
    nodes.push_back(Node(10, "Sum", ImVec2(508, 361), 6, 1));
    nodes.push_back(Node(11, "test", ImVec2(200, 766), 1, 1));
    nodes.push_back(Node(12, "test", ImVec2(1000, 766), 1, 1));

    links.push_back(NodeLink(0, 0, 2, 0));
    links.push_back(NodeLink(0, 0, 3, 0));
    links.push_back(NodeLink(0, 0, 4, 0));
    links.push_back(NodeLink(0, 0, 5, 0));
    links.push_back(NodeLink(0, 0, 6, 0));
    links.push_back(NodeLink(0, 0, 7, 0));

    links.push_back(NodeLink(2, 0, 10, 0));
    links.push_back(NodeLink(3, 0, 10, 1));
    links.push_back(NodeLink(4, 0, 10, 2));
    links.push_back(NodeLink(5, 0, 10, 3));
    links.push_back(NodeLink(6, 0, 10, 4));
    links.push_back(NodeLink(7, 0, 10, 5));

    links.push_back(NodeLink(10, 0, 8, 0));
    links.push_back(NodeLink(0, 0, 11, 0));
    links.push_back(NodeLink(8, 0, 9, 0));
    links.push_back(NodeLink(9, 0, 1, 0));
    links.push_back(NodeLink(0, 0, 11, 0));
    links.push_back(NodeLink(11, 0, 12, 0));
    links.push_back(NodeLink(12, 0, 9, 1));





    //if (gnode->DownLinked)
    //{
    //    int input = 0;
    //    int inputslot = 0;
    //    int output = 0;
    //    int outputslot = 0;
    //    int i = 0;
    //    for (; i < nodes.size();++i)
    //    {
    //        if (nodes[i].Name == "1")
    //        {
    //           
    //        }
    //    }
    //
    //
    //   links.push_back(NodeLink(nodes.size(),(int)Direction::Down,,(int)Direction::Up));
    //}

    //if (gnode->UpLinked)
    //{
    //   // links.push_back(NodeLink(nodes.end()->ID, (int)Direction::Up, gnode->UpID, (int)Direction::Down));
    //}
    //
    //if (gnode->RightLinked)
    //{
    //    //links.push_back(NodeLink(nodes.end()->ID, (int)Direction::Right, gnode->RightID, (int)Direction::Left));
    //}
    //
    //if (gnode->LeftLinked)
    //{
    //    //links.push_back(NodeLink(nodes.end()->ID, (int)Direction::Left, gnode->LeftID, (int)Direction::Right));
    //}
    //
//  }
//}
//nodes.push_back(Node(0, "MainTex", ImVec2(40, 50), 0.5f, ImColor(255, 100, 100), 1, 1));
//nodes.push_back(Node(1, "BumpMap", ImVec2(40, 150), 0.42f, ImColor(200, 100, 200), 1, 1));
//nodes.push_back(Node(2, "Combine", ImVec2(270, 80), 1.0f, ImColor(0, 200, 100), 2, 2));

// links.push_back(NodeLink(0, (int)Direction::Right, 1, (int)Direction::Left));
//
// links.push_back(NodeLink(1, (int)Direction::Left, 2, (int)Direction::Right));
//
// links.push_back(NodeLink(2, (int)Direction::Left, 3, (int)Direction::Right));
//
// //links.push_back(NodeLink(3, (int)Direction::Left, 4, (int)Direction::Right));
    inited = true;
  }

  // Draw a list of nodes on the left side
  bool open_context_menu = false;
  int node_hovered_in_list = -1;
  int node_hovered_in_scene = -1;
  // ImGui::BeginChild("node_list", ImVec2(100, 0));
  // ImGui::Text("Nodes");
  // ImGui::Separator();
  // for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
  // {
  //   Node* node = &nodes[node_idx];
  //   ImGui::PushID(node->ID);
  //   if (ImGui::Selectable(node->Name, node->ID == node_selected))
  //     node_selected = node->ID;
  //   if (ImGui::IsItemHovered())
  //   {
  //     node_hovered_in_list = node->ID;
  //     open_context_menu |= ImGui::IsMouseClicked(1);
  //   }
  //   ImGui::PopID();
  // }
  // ImGui::EndChild();

  ImGui::SameLine();
  ImGui::BeginGroup();

  const float NODE_SLOT_RADIUS = 4.0f;
  const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

  // Create our child canvas
  //ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
 // ImGui::SameLine(ImGui::GetWindowWidth() - 100);
 // ImGui::Checkbox("Show grid", &show_grid);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(60, 60, 70, 200));
  ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
  ImGui::PushItemWidth(120.0f);

  ImVec2 offset = ImVec2(ImGui::GetCursorScreenPos().x - scrolling.x, ImGui::GetCursorScreenPos().y - scrolling.y);
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->ChannelsSplit(2);

  // Display grid
  if (show_grid)
  {
    ImU32 GRID_COLOR = ImColor(200, 200, 200, 40);
    float GRID_SZ = 64.0f;
    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();
    for (float x = fmodf(offset.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
      draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, win_pos.y + canvas_sz.y), GRID_COLOR);
    for (float y = fmodf(offset.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
      draw_list->AddLine(ImVec2(win_pos.x, win_pos.y + y), ImVec2(canvas_sz.x + win_pos.x, y + win_pos.y), GRID_COLOR);
  }

  // Display links
  draw_list->ChannelsSetCurrent(0); // Background
  for (int link_idx = 0; link_idx < links.Size; link_idx++)
  {
    NodeLink* link = &links[link_idx];
    Node* node_inp = &nodes[link->InputIdx];
    Node* node_out = &nodes[link->OutputIdx];
    ImVec2 p1 = ImVec2(offset.x + node_inp->GetOutputSlotPos(link->InputSlot).x, offset.y + node_inp->GetOutputSlotPos(link->InputSlot).y);
    ImVec2 p2 = ImVec2(offset.x + node_out->GetInputSlotPos(link->OutputSlot).x, offset.y + node_out->GetInputSlotPos(link->OutputSlot).y);
    draw_list->AddBezierCurve(p1, ImVec2(p1.x + 50, p1.y + 0), ImVec2(p2.x + -50, p2.y + 0), p2, ImColor(200, 200, 100), 3.0f);
  }

  // Display nodes
  for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
  {
    Node* node = &nodes[node_idx];
    ImGui::PushID(node->ID);
    ImVec2 node_rect_min = ImVec2(offset.x + node->Pos.x, offset.y + node->Pos.y);

    // Display node contents first
    draw_list->ChannelsSetCurrent(1); // Foreground
    bool old_any_active = ImGui::IsAnyItemActive();
    ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + NODE_WINDOW_PADDING.x, node_rect_min.y + NODE_WINDOW_PADDING.y));
    ImGui::BeginGroup(); // Lock horizontal position
    if (node->ID != 11 && node->ID != 12)
    {

      //ImGui::Text("%s \n Position: x = %f \n y = %f", node->Name, node->Pos.x, node->Pos.y);
    }

    if (node->ID == 0)
    {
      ImGui::Text("Input");
    }
    if (node->ID == 1)
    {
      ImGui::Text("OutPut");
    }

    if (node->ID == 2)
    {
      ImGui::Text("Comb 1 \n G1: %f \n R1: %f", gcal[0], rcal[0]);
    }

    if (node->ID == 3)
    {
      ImGui::Text("Comb 2 \n G2: %f \n R2: %f", gcal[1], rcal[1]);
    }

    if (node->ID == 4)
    {
      ImGui::Text("Comb 3 \n G3: %f \n R3: %f", gcal[2], rcal[2]);
    }

    if (node->ID == 5)
    {
      ImGui::Text("Comb 4 \n G4: %f \n R4: %f", gcal[3], rcal[3]);
    }
    if (node->ID == 6)
    {
      ImGui::Text("Comb 5 \n G5: %f \n R5: %f", gcal[4], rcal[4]);
    }
    if (node->ID == 7)
    {
      ImGui::Text("Comb 6 \n G6: %f \n R6: %f", gcal[5], rcal[5]);
    }
    if (node->ID == 8)
    {
      ImGui::Text("AllPass \n A: %f \n m: %f \n L: %i", a_m, m_m, l_m);
    }
    if (node->ID == 9)
    {
      ImGui::Text("Wet: %i%% \nDry: %i%% ", static_cast<int>(wet_m * 100.0f), static_cast<int>(dry_m * 100.0f));
    }
    if (node->ID == 10)
    {
      ImGui::Text("Sum");
    }
    /*
    0, "Input",Im
    1, "Output",I
    2, "Comb 1",I
    3, "Comb 2",I
    4, "Comb 3",I
    5, "Comb 4",I
    6, "Comb 5",I
    7, "Comb 6",I
    8, "Allpass",
    9, "Wet Dry",
    10, "Sum",ImV
    11, "test",Im
    12, "test",Im

    */

    ImGui::EndGroup();

    // Save the size of what we have emitted and whether any of the widgets are being used
    bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
    node->Size = ImVec2(ImGui::GetItemRectSize().x + NODE_WINDOW_PADDING.x + NODE_WINDOW_PADDING.x, ImGui::GetItemRectSize().y + NODE_WINDOW_PADDING.y + NODE_WINDOW_PADDING.y);
    ImVec2 node_rect_max = ImVec2(node_rect_min.x + node->Size.x, node_rect_min.y + node->Size.y);

    // Display node box
    draw_list->ChannelsSetCurrent(0); // Background
    ImGui::SetCursorScreenPos(node_rect_min);
    ImGui::InvisibleButton("node", node->Size);
    if (ImGui::IsItemHovered())
    {
      node_hovered_in_scene = node->ID;
      open_context_menu |= ImGui::IsMouseClicked(1);
    }
    bool node_moving_active = ImGui::IsItemActive();
    if (node_widgets_active || node_moving_active)
      node_selected = node->ID;
    if (node_moving_active && ImGui::IsMouseDragging(0))
      node->Pos = ImVec2(node->Pos.x + ImGui::GetIO().MouseDelta.x, node->Pos.y + ImGui::GetIO().MouseDelta.y);

    ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? ImColor(75, 75, 75) : ImColor(60, 60, 60);
    draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
    draw_list->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);
    for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
      draw_list->AddCircleFilled(ImVec2(offset.x + node->GetInputSlotPos(slot_idx).x, offset.y + node->GetInputSlotPos(slot_idx).y), NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));
    for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
      draw_list->AddCircleFilled(ImVec2(offset.x + node->GetOutputSlotPos(slot_idx).x, offset.y + node->GetOutputSlotPos(slot_idx).y), NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));

    ImGui::PopID();
  }
  draw_list->ChannelsMerge();

  //// Open context menu
  //if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
  //{
  //  node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
  //  open_context_menu = true;
  //}
  //if (open_context_menu)
  //{
  //  ImGui::OpenPopup("context_menu");
  //  if (node_hovered_in_list != -1)
  //    node_selected = node_hovered_in_list;
  //  if (node_hovered_in_scene != -1)
  //    node_selected = node_hovered_in_scene;
  //}
  //
  //// Draw context menu
  //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
  //if (ImGui::BeginPopup("context_menu"))
  //{
  //  Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
  //  ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup();// -offset;
  //  if (node)
  //  {
  //    ImGui::Text("Node '%s'", node->Name);
  //    ImGui::Separator();
  //    if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
  //    if (ImGui::MenuItem("Delete", NULL, false, false)) {}
  //    if (ImGui::MenuItem("Copy", NULL, false, false)) {}
  //  }
  //  else
  //  {
  //    if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.Size, "name", scene_pos, 1, 1)); }
  //    if (ImGui::MenuItem("Paste", NULL, false, false)) {}
  //  }
  //  ImGui::EndPopup();
  //}
  //ImGui::PopStyleVar();

   // Scrolling
   //if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
   //  scrolling = ImVec2(scrolling.x - ImGui::GetIO().MouseDelta.x, scrolling.y - ImGui::GetIO().MouseDelta.y);

  ImGui::PopItemWidth();
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(2);
  ImGui::EndGroup();

  ImGui::EndChild();
}
