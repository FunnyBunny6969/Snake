#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib> 
#include <ctime>   
#include <fstream> 
#include <string>  
using namespace std;

void gotoStart() {
	COORD coord = { static_cast<SHORT>(0), static_cast<SHORT>(0) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setScreen(int& WIDTH, int&HEIGHT) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT }; // (левая, верхняя, правая, нижняя)
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
    cfi.dwFontSize.Y = 20; // Высота шрифта
    cfi.dwFontSize.X = 0; // Ширина шрифта
    cfi.FontFamily = FF_DONTCARE; // Тип шрифта
    cfi.FontWeight = FW_BOLD; // Нормальная жирность
    wcscpy_s(cfi.FaceName, L"Consolas"); // Имя шрифта
    // Устанавливаем шрифт
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

void draw(int snake[2148][2], int& len, bool& apple_exsists, int apple[2],
    int&WIDTH, int&HEIGHT,
    char& head, char& body, char& field, char& ch_apple, char& wall ) {
    bool printed = false;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0;j < WIDTH;j++) {

            //рисуем змею
            for (int k = 0; k < len; k++) {
                if (snake[k][0] < (WIDTH) && snake[k][1] < HEIGHT) {
                    if (   (i == snake[k][1]) 
                        && (j == snake[k][0])) {
                        if (k == 0) {
                            cout << "\033[48;5;9m";
                            cout << head; //выделяем голову
                            cout << "\033[40m";
                        }
                        else {
                            cout << "\033[48;5;11m";
                            cout << body;
                            cout << "\033[40m";
                        }
                        printed = true;
                        break;
                    }   
                }
            }
            //если есть то нарисуем яблоко
            if (apple_exsists == true && printed == false) {
                if (apple[0] == j && apple[1] == i) {
                    cout << ch_apple;
                    printed = true;
                }
            }
            
            //если в клетке не яблоко и не змея то рисуем кусок карты
            //MAP убрали, тк в процессе подход к отрисовке изменился, теперь можно без MAP
            if (printed == false) {
                if (j == 0 || j == (WIDTH - 1) || i == 0 || i == (HEIGHT - 1))
                    cout << wall;
                else
                    cout << field;
            }
            printed = false;
        }
        cout << endl;
    }
    cout << "SCORE\t" << len<<"        "; //пробелами очищаем поле счёта(на случай если змея стала короче)
}

void Start() {
	gotoStart();
	ifstream inputFile("StartScreen.txt");
    string line;
    while (getline(inputFile, line))
        cout << line << std::endl; 
    inputFile.close(); 
}

void GameOver() {
	gotoStart();
	ifstream inputFile("GameOverScreen.txt");
    string line;
    while (getline(inputFile, line))
        cout << line << std::endl; 
    inputFile.close(); 
}

void main() {
    int WIDTH = 80;     //Для нового режима потребуется динамически изменяемое поле
    int HEIGHT = 30;

    char ch_apple = '$';
    char field = ' ';
    char head = '@';
    char body = 'o';
    char wall = '#';

    
    srand(static_cast<unsigned int>(time(0)));
    setScreen(WIDTH, HEIGHT);
    setFont();
    system("color 0a"); //по приколу делаем всё зелёным

    int snake[2148][2];
    snake[0][0] = 10;
    snake[0][1] = 20;
    //генерируем хвост
    int len = 50;
    for (int tail = 1; tail < len; tail++) {
		snake[tail][0] = snake[0][0] + tail;
		snake[tail][1] = snake[0][1];
    }

    int apple[2];
    bool apple_exsists = false;

    //Стартовое окно
    Start();
    cin.get();

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
            apple[0] = rand() % (WIDTH-2) + 1;
            apple[1] = rand() % (HEIGHT-2) + 1;
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

        //проверяем столкновение со стенами
        if (snake[0][0] == 0 ||
            snake[0][0] == (WIDTH-1) ||
            snake[0][1] == 0 ||
            snake[0][1] == (HEIGHT-1)) {
            run = false;
            Sleep(2000); //даём 2сек чтобы понять ошибку
        }

        //если змея наезжает на хвост, она его откусывает
        for (int i = 1;i < len;i++) {
            if (snake[0][0] == snake[i][0] &&
                snake[0][1] == snake[i][1]) {
                len = i;
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

        //перерисовываем кадр
        if (run) {
            gotoStart();
            draw(snake, len, apple_exsists, apple, WIDTH, HEIGHT,
                head, body, field, ch_apple, wall);
        }

        if (dir == 'U' || dir == 'D')
            Sleep(20); //символ примерно вдвое больше в длину чер в ширину
        else            //поэтому для перемещения по вертикали и горизонтали разная задержка
            Sleep(10);  //чтобы змея во всех направления ползола +- одинаково
        
        
        if (!run) {
            GameOver();
            cin.get();
        }
    }
}