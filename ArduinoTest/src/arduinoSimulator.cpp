#include "arduinoSimulator.h"
#include "arduinoPlatform.h"
#include "tasks.h"
#include "interrupts.h"
#include "serial.h"
#include "GUI.h"
#include "demo.h"
#include "segment_display.h"
#include "test.h"

extern digitalPinsType digitalPins;
extern analogPinsType analogPins;
extern void loop();
extern void setup();
extern serial Serial;
extern segment_display s_display;
extern progressBar bar;

void hideConsoleWindow(){
    AllocConsole();
    HWND konzola = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(konzola, 0);
}

void closeConsole(){
    AllocConsole();
    HWND konzola = FindWindowA("ConsoleWindowClass", NULL);
    SendMessageW(konzola, WM_CLOSE, 0, 0);
}

void startArduinoSimulator(int showGUI, int hideConsole, int startTasks, int startInterrupts, int startSerial, int startSegmentDisplay, int startProgressBar)
{
    HANDLE mainThread;
    HANDLE segmentDisplayThread;
    DWORD mainThreadId;
    DWORD segnentDThreadID;

    HANDLE barThread;
    DWORD barThreadId;

    GUI *basicIOShield;

    for (int i = 0; i<DIGITAL_NUM_PINS; i++) {
        digitalPins.mode[i] = INPUT;
        digitalPins.state[i] = LOW;
    }
    for (int i = 0; i<ANALOG_NUM_PINS; i++) {
        analogPins.mode[i] = INPUT;
        analogPins.state[i] = LOW;
    }

    if (hideConsole)
        hideConsoleWindow();
    if (startTasks)
        startTaskManager();
    if (startInterrupts)
        startInterruptManager();
    Serial.startGUI();
    if (startSerial) {
        Serial.showGUI();
    }
    s_display.startGUI();
    if(startSegmentDisplay){
        s_display.showGUI();
    }
    bar.startGUI();             //novo
    if(startProgressBar){
        bar.showGUI();
    }
    mainThread = CreateThread(NULL, 0, &mainLoop, NULL, 0, &mainThreadId);
    segmentDisplayThread = CreateThread(NULL,0,&segmentDisplayLoop,&s_display,0,&segnentDThreadID);
    barThread = CreateThread(NULL, 0, &barLoop, NULL, 0, &barThreadId);
    if (showGUI) {
        basicIOShield = new GUI();
        basicIOShield->startGUI();
    }
    if (startTasks)
        endTaskManager();
    if (startInterrupts)
        endInterruptManager();
    CloseHandle(mainThread);
    CloseHandle(segmentDisplayThread);
    CloseHandle(barThread);
    closeConsole();
}
