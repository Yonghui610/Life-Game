#include <easyx.h>
#include <stdio.h>
#include <conio.h>       //Import the library files you need.
#include<iostream>
#include <string>
#include <fstream>
#include <graphics.h>

#define Height 800       
#define Width 600  
#define NODE_WIDTH 10    //Defines the Width Height and height width of the game window. Constant values cannot be changed

int Cell_h_count = Height / NODE_WIDTH;
int Cell_w_count = Width / NODE_WIDTH;    //Define a constant NODE_WIDTH with each cell being NODE_WIDTH and height.
int tempCells[Height / NODE_WIDTH][Width / NODE_WIDTH];
int reverseCells[Height / NODE_WIDTH][Width / NODE_WIDTH];
class Cells {                                              //(Third version)
public:
    int state[Height / NODE_WIDTH][Width / NODE_WIDTH];
    void Init() {                                          // init the grid (First version)
        for (int i = 0; i < Cell_h_count; ++i) {
            for (int j = 0; j < Cell_w_count; ++j) {
                state[i][j] = rand() % 2;
            }
        }

    }
    void display()                                        //(First version)
    {
        for (int i = 0; i < Cell_h_count; ++i) {
            for (int j = 0; j < Cell_w_count; ++j) {
                if (state[i][j]) solidrectangle(i * NODE_WIDTH, j * NODE_WIDTH, (i + 1) * NODE_WIDTH, (j + 1) * NODE_WIDTH);
            }                         //if the cell is alive then output; if the cell is dead then output nothing 
        }
        Sleep(1000);
    }

    void updateWithoutInput() {                           //game internal update
        int NeighbourNum = 0;                             //number of neighbour cells
        for (int i = 0; i < Cell_h_count; ++i) {
            for (int j = 0; j < Cell_w_count; ++j) {
                reverseCells[i][j] = state[i][j];
                if (i == 0 && j == 0)
                    NeighbourNum = state[i][j + 1] + state[i + 1][j] + state[i + 1][j + 1];
                else if (i == 0 && j == Cell_w_count - 1)
                    NeighbourNum = state[i][j - 1] + state[i + 1][j - 1] + state[i + 1][j];
                else if (i == Cell_h_count - 1 && j == 0)
                    NeighbourNum = state[i - 1][j] + state[i - 1][j + 1] + state[i][j + 1];
                else if (i == Cell_h_count - 1 && j == Cell_w_count - 1)
                    NeighbourNum = state[i - 1][j - 1] + state[i - 1][j] + state[i][j - 1];
                else NeighbourNum = state[i - 1][j - 1] + state[i - 1][j] + state[i - 1][j + 1] + state[i][j + 1] + state[i][j - 1] + state[i + 1][j - 1] + state[i + 1][j] + state[i + 1][j + 1];
                if (NeighbourNum < 2 && state[i][j] == 1)  //by changing the condition here,we can change the simulation in the life game
                    tempCells[i][j] = 0;
                else if (NeighbourNum <= 3 && state[i][j] == 1)  ////by changing the condition here,we can change the simulation in the life game
                    tempCells[i][j] = 1;
                else if (NeighbourNum > 3 && state[i][j] == 1)
                    tempCells[i][j] = 0;
                else if (NeighbourNum == 3 && state[i][j] == 0)
                    tempCells[i][j] = 1;
                else
                    tempCells[i][j] = state[i][j];
            }
        }
        for (int i = 0; i < Cell_h_count; ++i)
            for (int j = 0; j < Cell_w_count; ++j)
                state[i][j] = tempCells[i][j];
    }

};


LPCWSTR stringToLPCWSTR(std::string orig);

class Automaton {                                //(Second version)
public:
    int flag_while;
    Cells updateWithInput(Cells cells)
    {
        int x, y;
        HWND hnd;                        //check if there is data in the cache area
        if (_kbhit() != 0)               
        {
            char c = _getch();          //function _getch get the data from the input cache
            if (c == 0x1b) {
                flag_while = 0;
            }
            switch (c)
            {
            case 'p':                     //pause (Second version)
                cells.display();
                system("pause");
                break;
            case 'b':                    //add a block (Sixth version)
                do {
                    x = rand() % Cell_h_count;
                    y = rand() % Cell_w_count;
                } while (cells.state[x][y] || cells.state[x + 1][y] || cells.state[x][y + 1] || cells.state[x + 1][y + 1]);
                cells.state[x][y] = 1;
                cells.state[x][y + 1] = 1;
                cells.state[x + 1][y] = 1;
                cells.state[x + 1][y + 1] = 1;
                break;
            case 'l':
                do {
                    x = rand() % Cell_h_count;
                    y = rand() % Cell_w_count;
                } while (cells.state[x][y] || cells.state[x][y + 1] || cells.state[x][y + 2]);
                cells.state[x][y] = 1;
                cells.state[x][y + 1] = 1;
                cells.state[x][y + 2] = 1;
                break;
            case 'c':
                for (int i = 0; i < Cell_h_count; ++i) {
                    for (int j = 0; j < Cell_w_count; ++j) {
                        cells.state[i][j] = 0;
                    }
                }
                break;
            case 'f':
            {  std::ofstream out("cells.statetate.txt");     //open the file  (Fourth version)
            for (int i = 0; i < Cell_h_count; ++i) {
                for (int j = 0; j < Cell_w_count; ++j)
                {
                    out << cells.state[i][j];
                }
                out << std::endl;                    //add a new line when each line of output is in the end
            }
            hnd = GetHWnd();
            SetWindowText(hnd, L"save in file");
            MessageBox(hnd, L"save successful", L"message", MB_OKCANCEL);
            break; }

            case 'r':                    //(Fifth version)
            {
                std::ifstream in("cells.statetate.txt");
                for (int i = 0; i < Cell_h_count; ++i) {
                    for (int j = 0; j < Cell_w_count; ++j)
                    {
                        in >> cells.state[i][j];
                    }
                }
                hnd = GetHWnd();
                SetWindowText(hnd, L"read from file");
                MessageBox(hnd, L"read successful", L"message", MB_OKCANCEL);
                break;
            }
            case 'v':                           //(Fifth Version)
            {
                cells.display();
                Sleep(1000);
                cleardevice();
                hnd = GetHWnd();
                SetWindowText(hnd, L"reverse");
                MessageBox(hnd, L"reverse successful enter to continue", L"message", MB_OKCANCEL);
                for (int i = 0; i < Cell_h_count; ++i) {
                    for (int j = 0; j < Cell_w_count; ++j)
                    {
                        cells.state[i][j] = reverseCells[i][j];
                    }
                }
                cells.display();
                system("pause");
                break;
            }
            case 'd':                   //(Fourth version)
            {
                int flag_block = 1, flag_Bee_hive = 1;
                cells.display();
                for (int i = 0; i < Cell_h_count; ++i) {
                    for (int j = 0; j < Cell_w_count; ++j)
                    {
                        if (flag_block && i + 1 < Cell_h_count && j + 1 < Cell_w_count && cells.state[i][j] && cells.state[i + 1][j] && cells.state[i][j + 1] && cells.state[i + 1][j + 1]) {
                            hnd = GetHWnd();
                            SetWindowText(hnd, L"detect");
                            std::string s = "find Block pattern in(" + std::to_string(i) + "," + std::to_string(j) + ")";
                            MessageBox(hnd, stringToLPCWSTR(s), L"message", MB_OKCANCEL);
                            flag_block = 0;
                        }

                        else if (flag_Bee_hive && i + 3 < Cell_h_count && j + 1 < Cell_w_count && j - 1 >= 0 && cells.state[i][j] && cells.state[i + 1][j + 1] && cells.state[i + 2][j + 1] && cells.state[i + 3][j] && cells.state[i + 1][j - 1] && cells.state[i + 2][j - 1]) {
                            hnd = GetHWnd();
                            SetWindowText(hnd, L"detect");
                            std::string s = "find Bee_hive pattern in(" + std::to_string(i) + "," + std::to_string(j) + ")";
                            MessageBox(hnd, stringToLPCWSTR(s), L"message", MB_OKCANCEL);
                            flag_Bee_hive = 0;
                        }
                    }
                }
                system("pause");
                break;
            }
            case 'g':                   //(Fifth version)
            {
                int flag = 1;
                ExMessage msg;          //the substitution of MOUSEMSG
                cleardevice();
                cells.display();
                while (flag)
                {
                    if (peekmessage(&msg, EM_MOUSE))  //EM_MOUSE is to get the message og Mouse,if there is a mouse message returning true,else returning false
                    {
                        switch (msg.message)
                        {
                        case WM_LBUTTONDOWN:
                            if (msg.x >= 0 && msg.x <= Height && msg.y >= 0 && msg.y <= Width)
                            {
                                cells.state[msg.x / NODE_WIDTH][msg.y / NODE_WIDTH] ? cells.state[msg.x / NODE_WIDTH][msg.y / NODE_WIDTH] = 0 : cells.state[msg.x / NODE_WIDTH][msg.y / NODE_WIDTH] = 1;
                                cells.display();
                            }
                            break;
                        case WM_RBUTTONDOWN:
                            flag = 0;
                            break;
                        }
                    }
                }
            }
            }

        }
        return cells;
    }
    void execute(Cells cells) {
        std::cout << "=========================================================" << std::endl;
        std::cout << std::endl;
        std::cout << "               Welcome to Game of Life" << std::endl;
        std::cout << std::endl;
        std::cout << "  This is a game of life simulation based" << std::endl;
        std::cout << "generate and iterate cells in a regular manner" << std::endl;
        std::cout << std::endl;
        std::cout << "=========================================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "         'b'  -  generate a Block cell" << std::endl;
        std::cout << "         'c'  -  clear all cell" << std::endl;
        std::cout << "         'l'  -  generate a Blinker cell " << std::endl;
        std::cout << "         'p'  -  pause " << std::endl;
        std::cout << "         'r'  -  read from file " << std::endl;
        std::cout << "         'f'  -  save into file " << std::endl;
        std::cout << "         'v'  -  revert the simulation (only lastTime)" << std::endl;
        std::cout << "         'd'  -  detect some patterns(Same type only found once)" << std::endl;
        std::cout << "         'g'  -  change a cell with mouse (click left change, click right exit)" << std::endl;
        std::cout << "        'esc' -  exit" << std::endl;
        std::cout << std::endl;
        system("pause");
        initgraph(Height, Width);
        setbkcolor(RGB(0, 0, 0));           //set background color
        cleardevice();
        flag_while = 1;
        cells.Init();
        while (flag_while) {
            int flag_while2 = 0;
            cleardevice();
            cells.display();
            cells.updateWithoutInput();
            cells = updateWithInput(cells);
        }
    }
};




LPCWSTR stringToLPCWSTR(std::string orig)
{
    wchar_t* wcstring = 0;
    try
    {
        size_t origsize = orig.length() + 1;
        const size_t newsize = 100;
        size_t convertedChars = 0;
        if (orig == "")
        {
            wcstring = (wchar_t*)malloc(0);
            mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
        }
        else
        {
            wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
            mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
        }
    }
    catch (std::exception e)
    {
    }
    return wcstring;
}
int main()
{
    Cells cells;
    Automaton autoexe;
    autoexe.execute(cells);
    closegraph();
    return 0;
}
