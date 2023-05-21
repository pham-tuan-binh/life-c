#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <econio.h>

/* -------------------------------------------------------------------------- */
/*     Completed Version of Conway's Game of Life by Pham Tuan Binh    */
/* -------------------------------------------------------------------------- */

/* -------------------- Enumerated Types for The Program -------------------- */
// Enum For Animation mode choices: Continous and Step-by-step.
enum AnimationMode
{
    CONTINOUS = 1,
    STEP = 2,
};
typedef enum AnimationMode AnimationMode;

// Enum for Play Mode choices: Randomized board, User-defined board and Load Board from file.
enum PlayMode
{
    RANDOM_MODE = 1,
    FILE_MODE = 2,
    USER_MODE = 3
};
typedef enum PlayMode PlayMode;

/* -------------- Additional Utility Functions for the program -------------- */
// Creating deliberate delay in programm for loading screens
//    Param: miliseconds (The miliseconds for delay)
void delay(int miliseconds)
{
    usleep(miliseconds * 1000);
}

// Find the minimum interger between 2 intergers
//    Param: (int) a (First Interger)
//           (int) b (Second Interger)
//    Return: (int) minimum interger
int min(int a, int b) { return a < b ? a : b; }

// Find the maximum interger between 2 intergers
//    Param: (int) a (First Interger)
//           (int) b (Second Interger)
//    Return: (int) maximum interger
int max(int a, int b) { return a > b ? a : b; }

// Get Dynamic String From User
//    Return: (string) fileName (Name of Board)
char *dynamicStringInput()
{
    char c = '0';
    int count = 1;
    char *string = (char *)malloc(count * sizeof(char));

    fflush(stdin);
    while (1)
    {
        c = getchar();

        if (c == '\n')
        {
            break;
        }

        string[count - 1] = c;

        // Updated: Count + 1 to sub for '\0'
        char *new = (char *)calloc((count + 1), sizeof(char));
        for (int i = 0; i < count; i++)
        {
            new[i] = string[i];
        }

        count++;
        free(string);
        string = new;
    }

    return string;
}

// Allocate a Dynamic Array (Board Abstraction)
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
int **dynamicArrayAllocate(int HEIGHT, int WIDTH)
{
    int **array = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++)
    {
        array[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    return array;
}

// Allocate a Dynamic Array (Board Abstraction)
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
void dynamicArrayFree(int HEIGHT, int **board)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        free(board[i]);
    }

    free(board);
    return;
}

/* -- Functions for rendering the game: loading screens, board, board cell,... - */
// Print a Cell for Life's board
//    Param: (int) color (The color of the cell)
void printCell(int color)
{
    switch (color)
    {
    case 1:
        econio_textcolor(COL_GREEN);
        printf("██");
        econio_textcolor(COL_RESET);
        break;
    case 0:
        econio_textcolor(COL_BLACK);
        printf("██");
        econio_textcolor(COL_RESET);
        break;
    case 3:
        econio_textcolor(COL_YELLOW);
        printf("██");
        econio_textcolor(COL_RESET);
        break;
    default:
        econio_textcolor(COL_WHITE);
        printf("██");
        econio_textcolor(COL_RESET);
        break;
    }
}

// Print a row line (A row full of white Cell, acting as a border) for Life's board
//    Param: (int) WIDTH (The width of the board)
void printRowLine(int WIDTH)
{
    for (int i = 0; i < WIDTH + 2; i++)
    {
        printCell(4);
    }
    printf("\n");
}

// Print a Welcome Banner at the start of the game
void printBanner()
{
    printf("  ____                               _        ____                               __   _     _  __      \n");
    printf(" / ___|___  _ ____      ____ _ _   _( )___   / ___| __ _ _ __ ___   ___    ___  / _| | |   (_)/ _| ___ \n");
    printf("| |   / _ \\| '_ \\ \\ /\\ / / _` | | | |// __| | |  _ / _` | '_ ` _ \\ / _ \\  / _ \\| |_  | |   | | |_ / _ \\\n");
    printf("| |__| (_) | | | \\ V  V / (_| | |_| | \\__ \\ | |_| | (_| | | | | | |  __/ | (_) |  _| | |___| |  _|  __/\n");
    printf(" \\____\\___/|_| |_|\\_/\\_/ \\__,_|\\__, | |___/  \\____|\\__,_|_| |_| |_|\\___|  \\___/|_|   |_____|_|_|  \\___|\n");
    printf("                               |___/                                                  By Pham Tuan Binh               \n");
}

// Print a Loading Screen
void printLoadingScr()
{
    econio_clrscr();
    for (int i = 0; i < 3; i++)
    {
        printf(".");
        fflush(stdout);
        delay(100);
    }
    printf("\n");
    econio_clrscr();
}

// Print the next game's board of Cells or any game's board of Cells
//    Param: (int) delayTime (The time the board will be on screen in milisecconds)
//           (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) nextBoard (Pointer to A 2D Dynamic Interger Array that serves as the next Game Board after calculations, or any board in this case)
void printBoard(int delayTime, int HEIGHT, int WIDTH, int **nextBoard)
{
    econio_clrscr();
    printRowLine(WIDTH);
    for (int i = 0; i < HEIGHT; i++)
    {
        printCell(4);
        for (int j = 0; j < WIDTH; j++)
        {
            if (nextBoard[i][j] == 1)
            {
                printCell(1);
            }
            else
            {
                printCell(0);
            }
        }
        printCell(4);
        printf("\n");
    }
    printRowLine(WIDTH);
    printf("\nPress [ESC] to Escape and Save file | Press [ARROW DOWN] to continuosly calculate the board | Press [ARROW RIGHT] to calculate the board step-by-step\n");

    if (delayTime != 0)
    {
        delay(delayTime);
        econio_clrscr();
    }
}

// Print the next game's board of Cells or any game's board of Cells with a yellow Cell serves as the cursor (For the drawing of User-defined Board)
//    Param: (int) delayTime (The time the board will be on screen in milisecconds)
//           (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int) x (X coordinate of cursor)
//           (int) y (Y coordinate of cursor)
//           (int**) nextBoard (Pointer to A 2D Dynamic Interger Array that serves as the next Game Board after calculations, or any board in this case)
void printBoardWithCursor(int delayTime, int HEIGHT, int WIDTH, int x, int y, int **nextBoard)
{
    printRowLine(WIDTH);
    for (int i = 0; i < HEIGHT; i++)
    {
        printCell(4);
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == y && j == x)
            {
                printCell(3);
                continue;
            }

            if (nextBoard[i][j] == 1)
            {
                printCell(1);
            }
            else
            {
                printCell(0);
            }
        }
        printCell(4);
        printf("\n");
    }
    printRowLine(WIDTH);
    printf("\nPress [ENTER] to toggle the cursor | Press [ARROW UP-DOWN-LEFT_RIGHT] to move the cursor | Press [ESCAPE] to proceed\n");

    if (delayTime != 0)
    {
        delay(delayTime);
        econio_clrscr();
    }
}

/* ------ Core functions for the calculations of Conway's game of life ------ */

// Assign a board with another board (newBoard = originalBoard)
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) originalBoard (Pointer to A 2D Dynamic Interger Array that serves as the orginal board)
//           (int**) newBoard (Pointer to A 2D Dynamic Interger Array that serves as the new board)
void assignBoard(int HEIGHT, int WIDTH, int **newBoard, int **originalBoard)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            newBoard[i][j] = originalBoard[i][j];
        }
    }
}

// Count Live Neighbour Cell for Conway's Game of life
//    Param: (int) r (X coordinate of Cell)
//           (int) c (Y coordinate of Cell)
//           (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Arra that serves as the Game Board)
//    Return: (int) Number of Live Cells
int countLiveNeighbourCell(int r, int c, int HEIGHT, int WIDTH, int **currentBoard)
{
    // Live Surrounging Cell Counter
    int count = 0;

    // Loop over the surrounding cells
    for (int i = r - 1; i <= r + 1; i++)
    {
        for (int j = c - 1; j <= c + 1; j++)
        {

            // Skip in case the cell is out of bound or the cell itself
            if ((i == r && j == c) || (i < 0 || j < 0) || (i >= HEIGHT || j >= WIDTH))
            {
                continue;
            }

            // If live then counter goes up by one
            if (currentBoard[i][j] == 1)
            {
                count++;
            }
        }
    }
    return count;
}

// Calculate next game board
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
//           (int**) nextBoard (Pointer to A 2D Dynamic Interger Array that serves as the next Game Board after calculations)
void calculateNextBoard(int HEIGHT, int WIDTH, int **currentBoard, int **nextBoard)
{
    // Stores the value of live neighbours of a cell
    int cell;

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {

            // Update live neighbours count
            cell = countLiveNeighbourCell(i, j, HEIGHT, WIDTH, currentBoard);

            // Rules of the game
            if (currentBoard[i][j] == 1 && (cell == 2 || cell == 3))
            {
                nextBoard[i][j] = 1;
            }

            else if (currentBoard[i][j] == 0 && cell == 3)
            {
                nextBoard[i][j] = 1;
            }

            else
            {
                nextBoard[i][j] = 0;
            }
        }
    }

    // Copy Next Board to Current Board
    assignBoard(HEIGHT, WIDTH, currentBoard, nextBoard);
}

/* ------------- Functions for different game modes of the game ------------- */
// Mode 1: Randomize Board

// Randommize the game's board of cells
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
void randomBoard(int HEIGHT, int WIDTH, int **currentBoard)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            currentBoard[i][j] = rand() % 2;
        }
    }
}

// Mode 2: Load board from file

// Get file name from user
//    Return: (string) fileName (Name of Board)
char *getFileName()
{
    econio_clrscr();
    printf("Please enter the name of the board you want to load: ");
    char *fileName = dynamicStringInput();

    FILE *board;
    board = fopen(fileName, "r");

    while (board == NULL)
    {
        free(fileName);
        econio_clrscr();
        printLoadingScr();
        printf("File not found.\n");
        delay(500);
        printLoadingScr();
        fileName = getFileName();
        board = fopen(fileName, "r");
    }

    fclose(board);

    return fileName;
}

// Get Board Size from a save file
//    Param: (int*) HEIGHT (HEIGHT of Board)
//           (int*) WIDTH (WIDTH of Board)
//           (string) fileName (Name of Board)
void readBoardSizeFromFile(int *HEIGHT, int *WIDTH, char *fileName)
{

    FILE *board;

    board = fopen(fileName, "rb");

    fread(HEIGHT, sizeof(int), 1, board);
    fread(WIDTH, sizeof(int), 1, board);

    fclose(board);
}

// Read Board from a save file
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
//           (string) fileName (Name of Board)
void readBoardFromFile(int HEIGHT, int WIDTH, int **currentBoard, char *fileName)
{
    FILE *board;

    board = fopen(fileName, "rb");

    fseek(board, sizeof(int) * 2, SEEK_CUR);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            fread(&currentBoard[i][j], sizeof(int), 1, board);
        }
    }

    fclose(board);
}

// Mode 3: User-defined Board

// Draw a board and allow user to draw on it using keyboard input
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
//           (int**) nextBoard (Pointer to A 2D Dynamic Interger Array that serves as the next Game Board after calculations)
void userInputBoard(int HEIGHT, int WIDTH, int **currentBoard)
{

    int x = 0;
    int y = 0;

    // Draw board
    econio_gotoxy(0, HEIGHT + 4);
    econio_clrscr();
    printBoardWithCursor(0, HEIGHT, WIDTH, x, y, currentBoard);

    int key = 0;

    econio_rawmode();
    while (1)
    {
        econio_gotoxy(0, HEIGHT + 4);
        if (econio_kbhit())
        {
            key = econio_getch();

            if (key == KEY_UP)
                y = max(y - 1, 0);
            else if (key == KEY_DOWN)
                y = min(y + 1, HEIGHT - 1);
            else if (key == KEY_LEFT)
                x = max(x - 1, 0);
            else if (key == KEY_RIGHT)
                x = min(x + 1, WIDTH - 1);
            else if (key == KEY_ENTER)
            {
                currentBoard[y][x] = currentBoard[y][x] == 0 ? 1 : 0;
            }
            else if (key == KEY_ESCAPE)
            {
                break;
            }

            econio_clrscr();
            printBoardWithCursor(0, HEIGHT, WIDTH, x, y, currentBoard);
        }
    }

    printLoadingScr();
}

// Detect User Keyboard Hit and Act Accordingly (Switch Animation mode or Escape Animation mode)
//    Param: (int*) animation (Pointer to Current Animation Mode)
//           (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
//           (int**) nextBoard (Pointer to A 2D Dynamic Interger Array that serves as the next Game Board after calculations)
//    Return: (int) escape (1 means escape, 0 means keep playing)
int animationController(AnimationMode *animation, int HEIGHT, int WIDTH, int **currentBoard, int **nextBoard)
{
    int key = econio_getch();

    // Escape Animation Mode
    if (key == KEY_ESCAPE)
    {
        return 1;
    }

    if (key == KEY_DOWN)
    {
        *animation = CONTINOUS;
    }
    else if (key == KEY_RIGHT)
    {
        *animation = STEP;
        calculateNextBoard(HEIGHT, WIDTH, currentBoard, nextBoard);
    }

    return 0;
}

/* --------------- Functions for phases or stages of the game --------------- */
// Prompt user to start the game by pressing ENTER
void gameBegin()
{
    econio_clrscr();

    printBanner();
    printf("\nPress [ENTER] to start the game.");

    econio_rawmode();

    char c;

    do
    {
        if (econio_kbhit())
        {
            c = econio_getch();
        }
    } while (c != KEY_ENTER);

    econio_normalmode();

    printLoadingScr();
}

// Print a menu for the game's modes and let user input the modes
//    Param: (int*) mode (Playing Mode of the game)
void gameSetup(PlayMode *mode)
{
    printf("The game has 3 Modes. Please select one to play.\n");
    printf("    [1][Default]. Play with a Randomized board.\n");
    printf("    [2]. Load the Board from a file.\n");
    printf("    [3]. Draw the board.\n");
    printf("Type the mode you want to play: ");
    fflush(stdin);
    scanf("%d", mode);

    printLoadingScr();

    if (*mode >= 1 && *mode <= 3)
    {
        printf("You have selected [%d] mode.\n", *mode);
    }
    else
    {
        *mode = 1;
        printf("You have chosen a number outside the range and selected [%d] mode by default.\n", *mode);
    }

    delay(1000);
    printLoadingScr();
}

// Write Board to file
//    Param: (int) HEIGHT (HEIGHT of Board)
//           (int) WIDTH (WIDTH of Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
//           (string) fileName (Name of Board)
void writeToFile(int HEIGHT, int WIDTH, int **currentBoard, char *fileName)
{
    FILE *board;

    board = fopen(fileName, "wb");

    if (board == NULL)
    {
        printf("Error!");
        printf("Something is wrong.");
        return;
    }

    fwrite(&HEIGHT, sizeof(int), 1, board);
    fwrite(&WIDTH, sizeof(int), 1, board);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            fwrite(&currentBoard[i][j], sizeof(int), 1, board);
        }
    }

    fclose(board);

    return;
}

// Prompt user to enter the name of the save file and end the game
//    Param: (int) HEIGHT (Height of the Board)
//           (int) WIDTH (Width of the Board)
//           (int**) currentBoard (Pointer to A 2D Dynamic Interger Array that serves as the Game Board)
//           (int**) nextBoard (Pointer to A 2D Dynamic Interger Array that serves as the next Game Board after calculations)
void gameEnd(int HEIGHT, int WIDTH, int **currentBoard, int **nextBoard, char *fileName)
{

    econio_clrscr();
    econio_normalmode();

    printf("Saving your file! Please enter a name for your file: ");
    fileName = dynamicStringInput();

    writeToFile(HEIGHT, WIDTH, currentBoard, fileName);

    dynamicArrayFree(HEIGHT, currentBoard);
    dynamicArrayFree(HEIGHT, nextBoard);
    free(fileName);

    printLoadingScr();

    printf("The Game has ended. Thanks For Playing.");
}

// Prompts the user to enter the Width and Height of their map
//    Param: (int*) HEIGHT (Height of the Board)
//           (int*) WIDTH (Width of the Board)
void boardSetUp(int *HEIGHT, int *WIDTH)
{
    printf("First, enter the height of your map: \n");
    fflush(stdin);
    scanf("%d", HEIGHT);
    while (*HEIGHT < 1)
    {
        econio_clrscr();
        printf("Renter the height of your map: \n");
        fflush(stdin);
        scanf("%d", HEIGHT);
    }
    printLoadingScr();

    printf("Then, enter the width of your map. \n");
    fflush(stdin);
    scanf("%d", WIDTH);
    if (*WIDTH < 1)
    {
        econio_clrscr();
        printf("Renter the width of your map: \n");
        fflush(stdin);
        scanf("%d", WIDTH);
    }
    printLoadingScr();

    printf("The size of your board is %dx%d (Height X Width).\n", *HEIGHT, *WIDTH);
    delay(1000);
    printLoadingScr();
}

/* -------------------------- Main Loop of the game ------------------------- */
int main()
{
    // Init Variables for the game
    int inPlay = 0;
    int HEIGHT = 0, WIDTH = 0;
    int key = 0;
    PlayMode mode = RANDOM_MODE;
    AnimationMode animation = STEP;

    // Init pointer to pointer for Dynamic Array
    int **currentBoard;
    int **nextBoard;

    // Dynamic Filename
    char *fileName = NULL;

    /* --------------------------- Phase 1: Game Begin -------------------------- */
    gameBegin();

    /* -------------------------- Phase 2: Game Set Up -------------------------- */
    gameSetup(&mode);

    /* ------------------------ Phase 3: Game In Progress ----------------------- */
    // Get Board Size
    switch (mode)
    {
    case RANDOM_MODE:
        // Get board size
        boardSetUp(&HEIGHT, &WIDTH);
        break;
    case FILE_MODE:
        // Get Board Name
        fileName = getFileName();

        // Get board size
        readBoardSizeFromFile(&HEIGHT, &WIDTH, fileName);
        break;
    case USER_MODE:
        // Get board size
        boardSetUp(&HEIGHT, &WIDTH);
        break;
    default:
        return 0;
    }

    // Initialize Board Array
    currentBoard = dynamicArrayAllocate(HEIGHT, WIDTH);
    nextBoard = dynamicArrayAllocate(HEIGHT, WIDTH);

    // Populate Board Cells
    switch (mode)
    {
    case RANDOM_MODE:
        // Randomize board
        randomBoard(HEIGHT, WIDTH, currentBoard);
        break;
    case FILE_MODE:
        // Init board
        readBoardFromFile(HEIGHT, WIDTH, currentBoard, fileName);
        break;
    case USER_MODE:
        // Init Board
        userInputBoard(HEIGHT, WIDTH, currentBoard);
        break;
    default:
        return 0;
    }

    // Animation Process
    econio_rawmode();
    while (1)
    {
        if (animation == CONTINOUS)
        {

            calculateNextBoard(HEIGHT, WIDTH, currentBoard, nextBoard);
            printBoard(100, HEIGHT, WIDTH, currentBoard);

            // econio_getch() blocks code from running without user input, I have used this "feature" to implement Step-By-Step and Continous Mode.
            if (econio_kbhit())
            {
                if (animationController(&animation, HEIGHT, WIDTH, currentBoard, nextBoard))
                    break;
            }
        }
        else if (animation == STEP)
        {
            printBoard(0, HEIGHT, WIDTH, currentBoard);

            if (animationController(&animation, HEIGHT, WIDTH, currentBoard, nextBoard))
                break;
        }
    }

    /* ---------------------------- Phase 4: Game End --------------------------- */
    gameEnd(HEIGHT, WIDTH, currentBoard, nextBoard, fileName);

    return 0;
}
