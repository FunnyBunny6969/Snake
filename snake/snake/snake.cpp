#include <iostream>
#include <Windows.h>
using namespace std;

void gotoStart() {
	COORD coord = { static_cast<SHORT>(0), static_cast<SHORT>(0) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    SMALL_RECT windowSize = { 0, 0, 79, 29 }; // (левая, верхняя, правая, нижняя)
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

void draw(int** MAP) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0;j < 80;j++) {
            if (MAP[i][j] == 0)
                cout << " ";
            else if (MAP[i][j] == 1)
                cout << "#";
            else if (MAP[i][j] == 2)
                cout << "@";
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
    MAP[15][40] = 2;

    draw(MAP);

    gotoStart();


    cin.get();
    system("cls");
    system("pause");
}