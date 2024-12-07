#include <iostream>
#include <Windows.h>
#include <conio.h> 
using namespace std;

void gotoStart() {
	COORD coord = { static_cast<SHORT>(0), static_cast<SHORT>(0) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    SMALL_RECT windowSize = { 0, 0, 79, 30 }; // (левая, верхняя, правая, нижняя)
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void setFont() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.Y = 24; // Высота шрифта
    cfi.dwFontSize.X = 0; // Ширина шрифта
    cfi.FontFamily = FF_DONTCARE; // Тип шрифта
    cfi.FontWeight = FW_BOLD; // Нормальная жирность
    wcscpy_s(cfi.FaceName, L"Consolas"); // Имя шрифта
    // Устанавливаем шрифт
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

void draw(int** MAP, int snake[2148][2], int& len) {
    bool printed = false;
    for (int i = 0; i < 30; i++) {
        for (int j = 0;j < 80;j++) {
            for (int k = 0; k < len; k++) {
                if (snake[k][0] < 79) {
                    if (i == snake[k][1] && j == snake[k][0]) {
                        cout << "@";
                        printed = true;
                    }   
                }
            }

            if (printed == false) {
                if (MAP[i][j] == 0)
                    cout << " ";
                else if (MAP[i][j] == 1)
                    cout << "#";
            }
            printed = false;
        }
        cout << endl;
    }
}

void main() {
    setScreen();
    setFont();


    int** MAP = new int* [30];
    for (int i = 0; i < 30; i++) {
        MAP[i] = new int[80];
        for (int j = 0;j < 80;j++){
            if (j == 0 || j == 79 || i == 0 || i == 29)
                MAP[i][j] = 1;
            else
                MAP[i][j] = 0;
        }   
    }
    
    int snake[2148][2];
    snake[0][0] = 40;
    snake[0][1] = 15;
    int len = 1;


	#define UP 72
	#define DOWN 80
	#define LEFT 75
	#define RIGHT 77
	#define ESC 27

    char dir = 'U';

    bool run = true;
    while (run) {
        if (_kbhit()) {
            switch (_getch()) {
            case UP:
                dir = 'U';
                break;
            case DOWN:
                dir = 'D';
                break;
            case LEFT:
                dir = 'L';
                break;
            case RIGHT:
                dir = 'R';
                break;
            case ESC:
                run = false;
                break;
            default:
                break;
            }
        }
        switch (dir) {
        case 'U':
            snake[0][1] -= 1;
            break;
        case 'D':
            snake[0][1] += 1;
            break;
        case 'L':
            snake[0][0] -= 2;
            break;
        case 'R':
            snake[0][0] += 2;
            break;
        }
        gotoStart();
        draw(MAP, snake, len);
        Sleep(200);
    }
}