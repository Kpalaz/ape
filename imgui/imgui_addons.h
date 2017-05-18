#ifndef imgui_addons
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imconfig.h"
#include <string>


namespace ImGui {
  // Based on the code by krys-spectralpixel (https://github.com/krys-spectralpixel), posted here: https://github.com/ocornut/imgui/issues/261
  /* pOptionalHoveredIndex: a ptr to an optional int that is set to -1 if no tab label is hovered by the mouse.
  * pOptionalItemOrdering: an optional static array of unique integers from 0 to numTabs-1 that maps the tab label order. If one of the numbers is replaced by -1 the tab label is not visible (closed). It can be read/modified at runtime.
  * allowTabReorder (requires pOptionalItemOrdering): allows tab reordering through drag and drop (it modifies pOptionalItemOrdering).
  * allowTabClosingThroughMMB (requires pOptionalItemOrdering): closes the tabs when MMB is clicked on them, by setting the tab value in pOptionalItemOrdering to -1.
  * pOptionalClosedTabIndex (requires allowTabClosingThroughMMB): out variable (int pointer) that returns the index of the closed tab in last call or -1.
  * pOptionalClosedTabIndexInsideItemOrdering: same as above, but index of the pOptionalItemOrdering array.
  */
  IMGUI_API bool TabLabels(int numTabs, const char** tabLabels, int& selectedIndex, const char** tabLabelTooltips = NULL, bool wrapMode = true, int *pOptionalHoveredIndex = NULL, int* pOptionalItemOrdering = NULL, bool allowTabReorder = true, bool allowTabClosingThroughMMB = true, int *pOptionalClosedTabIndex = NULL, int *pOptionalClosedTabIndexInsideItemOrdering = NULL);
  IMGUI_API bool InputNodePin(std::string name, bool connection = false, ImVec2 pos = ImVec2(0, 0));
  IMGUI_API bool OutputNodePin(std::string  name, bool connection = false, ImVec2 pos = ImVec2(0,0));

  //ImVec2 operator+(ImVec2 left, ImVec2 right);
  //ImVec2 operator-(ImVec2 left, ImVec2 right);
  //ImVec2 operator+(const ImVec2& rhs);
}

#endif // !imgui_addons