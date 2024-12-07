#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib> 
#include <ctime>   
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

void draw(int** MAP, int snake[2148][2], int& len, bool& apple_exsists, int apple[2]) {
    bool printed = false;
    for (int i = 0; i < 30; i++) {
        for (int j = 0;j < 80;j++) {

            //рисуем змею
            for (int k = 0; k < len; k++) {
                if (snake[k][0] < 79 && snake[k][1] < 30) {
                    if (   (i == snake[k][1]) 
                        && (j == snake[k][0])) {
                        if (k == 0) 
                            cout << "@"; //выделяем голову
                        else
                            cout << "o";
                        printed = true;
                    }   
                }
            }
            //если есть то нарисуем яблоко
            if (apple_exsists == true && printed == false) {
                if (apple[0] == j && apple[1] == i) {
                    cout << "$";
                    printed = true;
                }
            }
            
            //если в клетке не яблоко и не змея то рисуем кусок карты
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
    cout << "SCORE\t" << len;
}

void main() {
    srand(static_cast<unsigned int>(time(0)));
    setScreen();
    setFont();
    system("color 0a"); //по приколу делаем всё зелёным


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
    int len = 1;
    for (int tail = 1; tail < len; tail++) {
		snake[tail][0] = 40;
		snake[tail][1] = 15+tail;
    }

    int apple[2];
    bool apple_exsists = false;

	#define UP 72
	#define DOWN 80
	#define LEFT 75
	#define RIGHT 77
	#define ESC 27


    char dir = 'U';
    bool run = true;
    while (run) {
        //если яблока нет, генерируем
        if (apple_exsists == false) {
            apple[0] = rand() % 78 + 1;
            apple[1] = rand() % 29 + 1;
            apple_exsists = true;
        }
        //захватываем кнопки
        if (_kbhit()) {
            switch (_getch()) {
            case UP:
                if(dir != 'D') //змея не может двигаться внутрь себя
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
        
        //проверка съедания яблока
        if (apple_exsists == true &&
            snake[0][0] == apple[0] &&
            snake[0][1] == apple[1]) {
            len++;                 //удлиняем хвост
            apple_exsists = false; //сообщаем что яблоко надо перегенерировать
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

        //перерисовываем кадр
        gotoStart();
        draw(MAP, snake, len, apple_exsists, apple);

        if (dir == 'U' || dir == 'D')
            Sleep(150); //символ примерно вдвое больше в длину чер в ширину
        else            //поэтому для перемещения по вертикали и горизонтали разная задержка
            Sleep(45);  //чтобы змея во всех направления ползола +- одинаково
    }
    delete MAP;
}