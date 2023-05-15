#ifndef PROGRESSBAR_H_INCLUDED
#define PROGRESSBAR_H_INCLUDED

#include <windows.h>
#include <iostream>
#include "arduinoPlatform.h"
#include "serial.h"

extern serial Serial;
HWND hBar, hProcenat;
HBITMAP pBar_images[11];
HWND hwnd;

extern struct BarParameters bp;

DWORD WINAPI childDisplayLoop(LPVOID lpParam){
    while(true){
        static int oldState = 0;
        static int startTime = millis();
        static int first = true;

        BarParameters* p = (BarParameters*)lpParam;
        int duzinaPWM, periodPWM;
        int newState = digitalRead(p->pin);
        if (oldState == 1  && newState == 0) {
            duzinaPWM = millis() - startTime;

        } else if (oldState == 0 && newState == 1) {
            int time0 = millis();
            periodPWM = time0 - startTime;
            startTime = time0;
            if (!first) {
                double procenat = duzinaPWM/(double)periodPWM * 100;
                int index = procenat / 10;
                char procenatString[4];
                itoa((int) procenat, procenatString, 10);
                strcat(procenatString, "%");
                SetWindowText(hProcenat, procenatString);
                SendMessageW(hBar, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)pBar_images[index]);
                }
            first = false;
        }
        oldState = newState;
    }
}


int WINAPI createProgressBarWindow(HWND* retVal){
    HWND parentWnd = FindWindowA("ArduinoWindowClass",NULL);
    if(parentWnd == NULL){
        return 1;
    }

    MSG msg;
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = DefWindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = NULL;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "ProgressBarWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
               WS_EX_CLIENTEDGE,
               "ProgressBarWindowClass",
               "pBar",
               (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_EX_CONTROLPARENT,
               CW_USEDEFAULT, CW_USEDEFAULT, 260, 150,
               NULL, NULL,NULL, NULL);
    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, 0);
    UpdateWindow(hwnd);
    *retVal = hwnd;

    for (int i = 0; i <= 10; i++)
        pBar_images[i] = (HBITMAP) LoadImageW(NULL, (L"Images\\pBar" + std::to_wstring(i*10) + L".bmp").c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    CreateWindow(TEXT("static"), "0%", WS_VISIBLE | WS_CHILD, 20, 40, 20, 20, hwnd, (HMENU) NULL, NULL, NULL);
    CreateWindow(TEXT("static"), "100%", WS_VISIBLE | WS_CHILD, 195, 40, 50, 20, hwnd, (HMENU) NULL, NULL, NULL);
    hBar = CreateWindowW(L"Static",NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 50, 35, 25, 25, hwnd, (HMENU) NULL, NULL, NULL);
    hProcenat = CreateWindow(TEXT("static"), NULL, WS_VISIBLE | WS_CHILD, 100, 80, 50, 20, hwnd, (HMENU) NULL, NULL, NULL);

    //     Step 3: The  Loop

    DWORD threadId;
    HANDLE threadHandle = CreateThread(NULL, 0, &childDisplayLoop, &bp, 0, &threadId);
    CloseHandle(threadHandle);

    return msg.wParam;
}
#endif // PROGRESSBAR_H_INCLUDED
