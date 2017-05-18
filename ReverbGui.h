#ifndef reverbgui
#include <fstream>
#include <algorithm>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <iostream>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include <vector>
#include <string>
#include <list>
#include "Filter.h"
#include "windows.h"
#include "portaudio.h"
#include <limits>

#pragma comment (lib, "portaudio_x86.lib")
#define NORMAL 27570.836
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class Reverb;
class PA;
struct Node;

namespace
{
  int L[6] = { 50,56,61,68,72,78 };//25khz
  //int L50[6] = { 50,56,61,68,72,78 };//50khz
  unsigned size = 0.0;
  const float g[6] = { 0.408f,0.428f,0.448f,0.466f,0.476f,0.496f };
  float ratioval = 0.83;
  const float R[6] = { 0.83f * (1.0f - g[0]),0.83f * (1.0f - g[1]),0.83f * (1.0f - g[2]),0.83f * (1.0f - g[3]),0.83f * (1.0f - g[4]),0.83f * (1.0f - g[5]) };
  float dry = 0.7f;
  float wet = (1.0f - dry);
  float a = 0.7f;
  int m = 6;  //265
  int count = 0;
  int rate = 0;
  float gmod = 1.0f;
  int l_m = 1;
  int Lcal[6] = { l_m * 50,l_m * 56,l_m * 61,l_m * 68,l_m * 72,l_m * 78 };
  float gcal[6] = { gmod * g[0],gmod * g[1],gmod * g[2],gmod * g[3],gmod * g[4],gmod * g[5] };
  float rcal[6] = { ratioval * (1.0f - gcal[0]),ratioval * (1.0f - gcal[1]),ratioval * (1.0f - gcal[2]),ratioval * (1.0f - gcal[3]),ratioval * (1.0f - gcal[4]),ratioval * (1.0f - gcal[5]) };
  int m_m = m;
  float a_m = a;
  float dry_m = 0.7f;
  float wet_m = (1.0f - dry_m);
  PA *port = 0;
  bool globalplay = false;
  PA *temp = 0;
  Reverb * internalverb = 0;
  int cursoroffset = 0;
  PaStreamParameters output_params;
  PaStream *stream;
  //c short *portsamples = 0;
  float* floatdataout = 0;
  float* floatdata = 0;
  float* impulsein = 0;
  float* impulseout = 0;
  int selection = -1;
  int display_count = 70;
  int display_offset = 0;
  int max_count = -1;
  //c int grate = 0;
  int func_type = 0;
  float float_max = (std::numeric_limits<float>::max)();
  float float_min = (std::numeric_limits<float>::min)();
  //static const char* tabNames[] = { "Boring View","Node View" };
  //static int tabOrder[] = { 0,1 };
  //static int tabSelected = 0;
  float playheadoffset = -2.0f;
  static const char* tabNames[] = { "Boring View","Node View", "Signal Graph","Impulse Response"};
  static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
  static const char* tabTooltips[numTabs] = { "","","",""};
  static int tabItemOrdering[numTabs] = { 0,1,2,3};
  static int selectedTab = 0;
  static int optionalHoveredTab = 0;
  static int passedindex = 0;
}

class GUI
{
public:
  void Calc(std::string filename);
  void CalcImpulse();
  void Init();
  void Update();
  void Delta();
  void Reset();
  std::string ask_user_for_file(std::string window_prompt, LPCSTR filetypes);
  std::string get_path_from_input(std::string input);
  const char * Selections[2] = { "Input","Output" };
  HWND window_ = 0;
  std::string filename;
  std::string outname;
  char *data = 0;
  char *data2 = 0;
  short *samples = 0;
  short *samples2 = 0;

  bool showgraphs = true;
  std::string playfile;
  void EditorNodeGraph(void);
};
 

class LowPassComb
{
public:
  LowPassComb(int Li = 0.0, float gi = 0.0, float Ri = 0.0)
  {
    L = Li;
    g = gi;
    R = Ri;
    for (int i = 0; i <= L; ++i)
      Xqueue.push(0.0);
    for (int i = 0; i < L; ++i)
      Yqueue.push(0.0);
  }
  float operator()(float sample);
private:
  std::queue<float> Xqueue;
  std::queue<float> Yqueue;
  int L = 0;
  float g = 0.0;
  float R = 0.0;
};

class AllPass
{
public:
  float operator()(float summation);
  AllPass(int mi, float ai)
  {
    m = (int)((double)mi / 1000.0 * rate);
    a = ai;
    for (int i = 0; i < m; ++i)
      Xqueue.push(0.0);
    for (int i = 0; i < m; ++i)
      Yqueue.push(0.0);
  };
private:
  int m = 0;
  float a = 0;
  std::queue<float> Xqueue;
  std::queue<float> Yqueue;
};

class Reverb {
public:
  Reverb(float rate = 44100,short * in = 0);
  ~Reverb(void);
  float operator()(float x);
  short *data = 0;
  bool playing = false;
  void Reset();
  int myrate = 0;
  int myindex = 0;
  int count = 0;
private:
};

class PA
{
public:
  
  PA(float R, Reverb * fverb, unsigned fcount, bool fstereo);
  static int OnWrite(const void*, void*, unsigned long,
    const PaStreamCallbackTimeInfo*,
    PaStreamCallbackFlags, void*);
  Reverb * verb = 0;
  unsigned count;
    int index = 0;
  bool stereo;
  //float rate;
  
  std::vector<std::string> labels;
};


struct Node
{
  int     ID;
  char    Name[32];
  ImVec2  Pos, Size;
  int InputsCount;
  int OutputsCount;
  Node(int id, const char* name, const ImVec2& pos, int inputs_count, int outputs_count)
  {
    ID = id; strncpy(Name, name, 31);
    Name[31] = 0;
    Pos = pos;
    InputsCount = inputs_count;
    OutputsCount = outputs_count;
  }
  ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
  ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }

};

struct NodeLink
{
  int     InputIdx, InputSlot, OutputIdx, OutputSlot;

  NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
};



#endif // !reverbgui
