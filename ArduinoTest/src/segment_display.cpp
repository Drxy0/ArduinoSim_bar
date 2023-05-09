#include "segment_display.h"
#include "windowsx.h"
#include <iostream>

#define LED_LENGTH 50
#define LED_THICK 14
#define LED_X 30
#define LED_Y 40
#define LED_SPACE 3


segment_display s_display(26);


void segment_display::loadImages(){

}

segment_display::segment_display(int sp)
{
    startPin = sp;
    segmentDisplayVisible = 0;

}

segment_display::~segment_display()
{
    //nothing
}

void segment_display::startGUI()
{
    CreateSegmentDisplayWindow(NULL,NULL,NULL,1);
}

void segment_display::showGUI()
{
    ShowWindow(hwndWindow,1);
    segmentDisplayVisible = 1;
    UpdateWindow(hwndWindow);
}


void segment_display::hideGUI()
{
    ShowWindow(hwndWindow,0);
    segmentDisplayVisible = 0;
}

int segment_display::isVisible()
{
    return segmentDisplayVisible == 1;
}


void segment_display::CreateSegmentDisplayGUI(HWND hwnd)
{
    CreateWindow(TEXT("static"), TEXT("OUTPUT: "), WS_VISIBLE | WS_CHILD,
                                         5, 5, 70, 20, hwnd, (HMENU) 100, NULL, NULL);

    onVertImg = (HBITMAP) LoadImageW(NULL, L"Images\\verticalOn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    offVertImg = (HBITMAP) LoadImageW(NULL, L"Images\\verticalOff.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    onHorImg = (HBITMAP) LoadImageW(NULL, L"Images\\horisontalOn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    offHorImg = (HBITMAP) LoadImageW(NULL, L"Images\\horisontalOff.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //making them transparent
    //onVertImg = CreateTransparentBitmap("Images\\verticalOn.bmp");
    //offVertImg = CreateTransparentBitmap("Images\\verticalOff.bmp");
    //onHorImg = CreateTransparentBitmap("Images\\horisontalOn.bmp");
    //offHorImg = CreateTransparentBitmap("Images\\horisontalOff.bmp");



    hwndLEDs[0] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X,LED_Y,100,100,hwnd,(HMENU)-1,NULL,NULL);
    hwndLEDs[1] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X,LED_Y + LED_SPACE + LED_LENGTH,100,100,hwnd,(HMENU)-1,NULL,NULL);
    hwndLEDs[2] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X + 2*LED_SPACE + LED_LENGTH + LED_THICK,LED_Y,100,100,hwnd,(HMENU)-1,NULL,NULL);
    hwndLEDs[3] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X + 2*LED_SPACE + LED_LENGTH + LED_THICK,LED_Y + LED_SPACE + LED_LENGTH,100,100,hwnd,(HMENU)-1,NULL,NULL);
    hwndLEDs[4] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X + LED_THICK + LED_SPACE,LED_Y -(LED_THICK/2),100,100,hwnd,(HMENU)-1,NULL,NULL);
    hwndLEDs[5] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X + LED_THICK + LED_SPACE,LED_Y -(LED_THICK/2) + LED_LENGTH + LED_SPACE,100,100,hwnd,(HMENU)-1,NULL,NULL);
    hwndLEDs[6] = CreateWindow(TEXT("static"),NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
                                                        LED_X + LED_THICK + LED_SPACE,LED_Y -(LED_THICK/2) + 2*LED_LENGTH + 2*LED_SPACE,100,100,hwnd,(HMENU)-1,NULL,NULL);

    for(int i = 0; i < 4; i++){
        SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
    }
    for(int i = 4; i < 7; i++){
        SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
    }
}

void segment_display::UpdateLEDs(){//todo
    int number = digitalRead(startPin+3)<<3 | digitalRead(startPin+2)<<2 | digitalRead(startPin+1)<<1 | digitalRead(startPin);
    switch(number){
    case 0:
        for(int i = 0; i < 4; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
        }
        for(int i = 4; i < 7; i++){
            if(i != 5){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }

        }
        break;
    case 1:
        for(int i = 0; i < 4; i++){
            if(i == 2 | i == 3){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }

        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
        }
        break;
    case 2:
        for(int i = 0; i < 4; i++){
            if(i == 2 | i == 1){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
        }
        break;
    case 3:
        for(int i = 0; i < 4; i++){
            if(i == 2 | i == 3){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
        }
        break;
    case 4:
        for(int i = 0; i < 4; i++){
            if(i == 0 | i == 2 | i == 3 ){
               SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }

        }
        for(int i = 4; i < 7; i++){
            if(i == 5){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }
        }
        break;
    case 5:
        for(int i = 0; i < 4; i++){
            if(i == 0 | i == 3 ){
               SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }

        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
        }
        break;
    case 6:
        for(int i = 0; i < 4; i++){
            if(i != 2){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }

        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
        }
        break;
    case 7:
        for(int i = 0; i < 4; i++){
            if(i == 2 | i == 3){
               SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }

        }
        for(int i = 4; i < 7; i++){
            if(i == 4){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }
        }
        break;
    case 8:
        for(int i = 0; i < 4; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
        }
        break;
    case 9:
        for(int i = 0; i < 4; i++){
            if(i == 1){
               SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }

        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
        }
        break;
    case 10:
        for(int i = 0; i < 4; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
        }
        for(int i = 4; i < 7; i++){
            if(i == 6){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }
        }
        break;
    case 11:
        for(int i = 0; i < 4; i++){
            if(i == 2){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
            if(i == 4){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }
        }
        break;
    case 12:
        for(int i = 0; i < 4; i++){
            if(i == 3 | i == 2){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
            if(i == 5){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }
        }
        break;
    case 13:
        for(int i = 0; i < 4; i++){
            if(i == 0 ){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
            if(i == 4){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }
        }
        break;
    case 14:
        for(int i = 0; i < 4; i++){
            if(i == 2 | i == 3){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);            }
        break;
    case 15:
        for(int i = 0; i < 4; i++){
            if(i == 2 | i == 3){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onVertImg);
            }
        }
        for(int i = 4; i < 7; i++){
            if(i == 6){
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
            }else{
                SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)onHorImg);
            }
        }
        break;
    default:
        for(int i = 0; i < 4; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offVertImg);
        }
        for(int i = 4; i < 7; i++){
            SendMessageW(hwndLEDs[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)offHorImg);
        }
        break;
    }
}

LRESULT CALLBACK SegmentDisplayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg)
    {
    case WM_CREATE:
        //Serial.CreateSerialGUI(hwnd);
        s_display.CreateSegmentDisplayGUI(hwnd);
        break;
    case WM_CLOSE:
        //DestroyWindow(hwnd);
        s_display.hideGUI();
        break;
    case WM_DESTROY:
        break;
    case WM_PAINT:
        break;
    case WM_DRAWITEM:
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_SIZE:
        //Serial.ChangeSize(LOWORD(lParam), HIWORD(lParam));
    case WM_COMMAND:
      /*{
        switch(HIWORD(wParam)){
         case BN_CLICKED:
             if (LOWORD(wParam)==104){
                Serial.buttonPressed();
             }
             break;
         default:
             return DefWindowProc(hwnd, msg, wParam, lParam);
         }
        break;
      }*/break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int segment_display::CreateSegmentDisplayWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                                LPSTR lpCmdLine, int nCmdShow){

    WNDCLASSEX wc;
    MSG Msg;
   //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = SegmentDisplayWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "SegmentDisplayWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwndWindow = CreateWindowEx(
               WS_EX_CLIENTEDGE,
               "SegmentDisplayWindowClass",
               "7 SD",
               WS_OVERLAPPEDWINDOW ,
               CW_USEDEFAULT, CW_USEDEFAULT, 165, 230,
               NULL, NULL, hInstance, NULL);

    if(hwndWindow == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //ShowWindow(hwndWindow, nCmdShow);
    UpdateWindow(hwndWindow);
    return Msg.wParam;
}

DWORD WINAPI segmentDisplayLoop(LPVOID lpParam){
    segment_display* seg_dis = (segment_display*)lpParam;
    while(true){
        seg_dis->UpdateLEDs();
    }
}


