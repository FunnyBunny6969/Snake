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

    CONSOLE_CURSOR_INFO cursorInfo;
    // Получаем текущую информацию о курсоре
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    // Скрываем курсор
    cursorInfo.bVisible = FALSE; // Устанавливаем видимость курсора в FALSE
    SetConsoleCursorInfo(hConsole, &cursorInfo);
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
                if (snake[k][0] < 79 && snake[k][1] < 30) {
                    if (   (i == snake[k][1]) 
                        && (j == snake[k][0])) {
                        if (k == 0)
                            cout << "@";
                        else
                            cout << "o";
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
    //генерируем хвост
    int len = 10;
    for (int tail = 1; tail < len; tail++) {
		snake[tail][0] = 40;
		snake[tail][1] = 15+tail;
    }

	#define UP 72
	#define DOWN 80
	#define LEFT 75
	#define RIGHT 77
	#define ESC 27

    char dir = 'U';

    bool run = true;
    while (run) {
        //захватываем кнопки
        if (_kbhit()) {
            switch (_getch()) {
            case UP:
                if(dir != 'D')
					dir = 'U';
                break;
            case DOWN:
                if(dir != 'U')
					dir = 'D';
                break;
            case LEFT:
                if(dir != 'R')
					dir = 'L';
                break;
            case RIGHT:
                if(dir != 'L')
					dir = 'R';
                break;
            case ESC:
                run = false;
                break;
            default:
                break;
            }
        }
        //пересчитываем хвост
        if (len > 1) {
            for (int i = (len-1); i > 0;i--) {
                snake[i][0] = snake[i - 1][0];
                snake[i][1] = snake[i - 1][1];
            }
        }

        //перемещаем голову
        switch (dir) {
        case 'U':
            snake[0][1] -= 1;
            break;
        case 'D':
            snake[0][1] += 1;
            break;
        case 'L':
            snake[0][0] -= 1;
            break;
        case 'R':
            snake[0][0] += 1;
            break;
        }
        gotoStart();
        draw(MAP, snake, len);
        if (dir == 'U' || dir == 'D')
            Sleep(150);
        else
            Sleep(45);
    }
    delete MAP;
}