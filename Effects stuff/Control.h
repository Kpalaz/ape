// Control.h
// -- interface for cross-platform audio controls?
// jsh 11/15

#ifndef JSH_CONTROL_H
#define JSH_CONTROL_H

#include <vector>
#include <windows.h>


class Control {
  public:
    Control(unsigned n, const char *name="Control Class 0.2");
    virtual ~Control(void);
    void SetRange(unsigned index, int min, int max);
    void SetLabel(unsigned index, const char *label);
    void SetValue(unsigned index, int value);
    void Show(bool visible=true);
    void SetTitle(const char *title);
    virtual void ValueChanged(unsigned index, int value);
  private:
    HWND window;
    HANDLE thread;
    HANDLE event;
	const char *name;
    struct Data {
      HWND scroll_handle,
           label_handle;
    };
    std::vector<Data> controls;
    static LRESULT CALLBACK WinProc(HWND,UINT,WPARAM,LPARAM);
    static DWORD WINAPI ThrdProc(LPVOID);
};


#endif

