#ifndef SEGMENT_DISPLAY_H_INCLUDED
#define SEGMENT_DISPLAY_H_INCLUDED
#include "Windows.h"
#include "stdlib.h"
#include "serial.h"
#include "arduinoPlatform.h"

class segment_display {
public:
    segment_display(int);
    int CreateSegmentDisplayWindow(HINSTANCE,HINSTANCE,LPSTR,int);
    void CreateSegmentDisplayGUI(HWND);
    void startGUI();
    void showGUI();
    void hideGUI();
    void loadImages();
    void UpdateLEDs();
    int isVisible();
    ~segment_display();
private:
    int startPin;
    int segmentDisplayVisible;
    HBITMAP onVertImg;
    HBITMAP offVertImg;
    HBITMAP onHorImg;
    HBITMAP offHorImg;
    HWND hwndWindow;
    HWND hwndLEDs[7];
};
DWORD WINAPI segmentDisplayLoop(LPVOID lpParam);

#endif // SEGMENT_DISPLAY_H_INCLUDED
