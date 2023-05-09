#include <windows.h>
#include "test.h"
#include <stdlib.h>
#include <iostream>
#include "arduinoPlatform.h"
#include "tasks.h"
#include <time.h>
#define BUTTON_SET 1
#define BUTTON_LOOP 2

progressBar bar;
progressBar::progressBar(){loop = true;};
progressBar::~progressBar(){};

void progressBar::setButton(){
    char text[10];
    GetWindowText(bar.hSet, text, 10);
    int k = atoi(text);
    if (k<0)
        updateN(0);
    else if (k>1023)
       updateN(1023);
    else
        updateN(k);
}

void progressBar::updateN(int k){
        n = k;
        char vr[4];
        itoa(n, vr, 10);
        SetWindowText(hTrenutnaVrijednost, vr);
        analogWrite(A0, k);
        int index = n / 100;
        SendMessage(hBar, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) nag_images[index]);
    }


//Ucitavanje slika
void progressBar::LoadImages(){
    nag_images[11];
    for (int i = 0; i <= 10; i++)
        nag_images[i] = (HBITMAP) LoadImageW(NULL, (L"Images\\nag" + std::to_wstring(i*10) + L".bmp").c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

}

void progressBar::InitGUI(HWND hwnd){
    LoadImages();
    CreateWindow(TEXT("static"), "0", WS_VISIBLE | WS_CHILD, 30, 40, 20, 20, hwnd, (HMENU) NULL, NULL, NULL);
    CreateWindow(TEXT("static"), "1023", WS_VISIBLE | WS_CHILD, 195, 40, 50, 20, hwnd, (HMENU) NULL, NULL, NULL);
    hBar = CreateWindowW(L"Static",NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 50, 35, 25, 25, hwnd, (HMENU) NULL, NULL, NULL);

    hTrenutnaVrijednost = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD, 100, 75, 50, 20, hwnd, (HMENU) NULL, NULL, NULL);
    hSet = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD, 100, 100, 50, 20, hwnd, (HMENU) NULL, NULL, NULL);
    CreateWindowW(L"Button",L"SET", WS_VISIBLE | WS_CHILD, 150, 100, 50, 20, hwnd, (HMENU) BUTTON_SET, NULL, NULL);
    CreateWindowW(L"Button",L"O/X A0 read", WS_VISIBLE | WS_CHILD, 250, 30, 100, 70, hwnd, (HMENU) BUTTON_LOOP, NULL, NULL);
}

void progressBar::startGUI()
{
    CreateProgressBarWindow(NULL,NULL,NULL,1);
}

void progressBar::showGUI()
{
    ShowWindow(hwnd,1);
    progressBarVisible = 1;
    UpdateWindow(hwnd);
}

void progressBar::hideGUI()
{
    ShowWindow(hwnd,0);
    progressBarVisible = 0;
}

int progressBar::isVisible()
{
    return progressBarVisible == 1;
}

void progressBar::readA0(){
    int k = analogRead(A0);
    updateN(k);
}

void progressBar::changeLoop(){
    if (loop == true)
        loop = false;
    else
        loop = true;
}

void progressBar::readLoop(){
    while (loop == true){
        bar.readA0();
        delay(1);
    }

}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

        case WM_COMMAND:
            switch(wParam)
            {
            case BUTTON_SET:
                bar.setButton();
                break;
            case BUTTON_LOOP:
                bar.changeLoop();
                break;
            }

            break;
        case WM_CLOSE:
                bar.hideGUI();
        case WM_CREATE:
            bar.InitGUI(hwnd);
            bar.updateN(0);
            break;

        case WM_DESTROY:
            //CloseHandle(barThread);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


int progressBar::CreateProgressBarWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszArgument, int nCmdShow)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wc;        /* Data structure for the windowclass */

   //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WindowProcedure;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "progressBarWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    /* Register the window class, and if it fails quit the program */
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed! in GUI", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
               WS_EX_CLIENTEDGE,
               "progressBarWindowClass",
               "Progress Bar",
               (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_EX_CONTROLPARENT,
               CW_USEDEFAULT, CW_USEDEFAULT, 380, 160,
               NULL, NULL, hInstance, NULL);

    /* Make the window visible on the screen */
   // ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

DWORD WINAPI barLoop(LPVOID lpParam)
{
    while (true){
        bar.readLoop();
        delay(1);
    }
}


