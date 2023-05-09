#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include<windows.h>

class progressBar{
    public:
        progressBar();
        int nakadashi;
        void updateNakadashi(int);
        virtual ~progressBar();
        void LoadImages();              //Ucitavanje slika
        HBITMAP nag_images[10];
        void InitGUI(HWND);
        void DrawTrenutnaVrijednost();
        HWND hBar, hTrenutnaVrijednost, hSet;
        void setButton();

        void startGUI();
        void showGUI();
        int CreateProgressBarWindow(HINSTANCE,HINSTANCE,LPSTR,int);
        int progressBarVisible;
        HWND hwnd;

        void hideGUI();
        int isVisible();

        void readA0();
        void changeLoop();
        void readLoop();
        bool loop;
};

DWORD WINAPI barLoop(LPVOID);
#endif // TEST_H_INCLUDED
