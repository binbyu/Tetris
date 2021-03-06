#pragma once

#include "resource.h"

typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned long           u32;
typedef unsigned long long      u64;
typedef char                    s8;
typedef short                   s16;
typedef long                    s32;
typedef long long               s64;

#define TETROMINO_NUM			4
#define TETROMINO_TYPES         7       // Tetromino_type_t
#define ROTATION_ANGLES         4       // Rotation_angle_t
#define TETRIS_X_NUM			10
#define TETRIS_Y_NUM			20
#define DEFAULT_X               (((TETRIS_X_NUM) - (TETROMINO_NUM)) / 2)
#define DEFAULT_Y               0
#define TETRIS_UNIT				20
#define TETRIS_WIDTH			((TETRIS_UNIT) * (TETRIS_X_NUM))
#define TETRIS_HEIGHT			((TETRIS_UNIT) * (TETRIS_Y_NUM))
#define IDT_TIMER               (WM_USER+100)
#define IDT_TIMER2              (WM_USER+101)
#define IDC_BUTTON              (WM_USER+102)
#define IDC_COMBOBOX            (WM_USER+103)

/*
            { 1, 1, 1, 1 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
The bottom border must be greater than or equal to 3, otherwise the matrix array will be out of bounds.

            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 }
The left border must be greater than or equal to 1, otherwise the matrix array will be out of bounds.

            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 }
The right border must be greater than or equal to 2, otherwise the matrix array will be out of bounds.
*/
#define BORDER_WIDTH            3

//#define SHOW_BORDER             __ON

typedef enum
{
    /*
         __
        |__|
        |__|
        |__|
        |__|
    */
    TETROMINO_I,

    /*
            __
           |__|
         __|__|
        |__|__|
    */
    TETROMINO_J,

    /*
         __
        |__|
        |__|__
        |__|__|
    */
    TETROMINO_L,

    /*
         __ __
        |__|__|
        |__|__|
    */
    TETROMINO_O,

    /*
            __ __
         __|__|__|
        |__|__|
    */
    TETROMINO_S,

    /*
         __ __
        |__|__|__
           |__|__|
    */
    TETROMINO_Z,

    /*
         __ __ __
        |__|__|__|
           |__|
    */
    TETROMINO_T
} Tetromino_type_t;

typedef enum
{
    ANGLE_0,
    ANGLE_90,
    ANGLE_180,
    ANGLE_270
} Rotation_angle_t;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEXT,
    STEPS
} Repaint_type_t;

const u8 c_Tetromino[TETROMINO_TYPES][ROTATION_ANGLES][TETROMINO_NUM][TETROMINO_NUM] =
{
    // TETROMINO_I
    {
        {
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 }
        },
        {
            { 1, 1, 1, 1 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 }
        },
        {
            { 1, 1, 1, 1 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        }
    },
    // TETROMINO_J
    {
        {
            { 0, 0, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 1, 0, 0, 0 },
            { 1, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 1, 1, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        }
    },
    //TETROMINO_L
    {
        {
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 1, 1, 1, 0 },
            { 1, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 0, 1, 0 },
            { 1, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        }
    },
    // TETROMINO_O
    {
        {
            { 0, 1, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        }
    },
    // TETROMINO_S
    {
        {
            { 0, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 1, 1, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 1, 0 },
            { 1, 1, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        }
    },
    // TETROMINO_Z
    {
        {
            { 1, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 0, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 1, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 0, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 0, 0 }
        }
    },
    // TETROMINO_T
    {
        {
            { 1, 1, 1, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 0, 1, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 0, 0 },
            { 1, 1, 1, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 }
        },
        {
            { 0, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 0, 0 }
        }
    }
};

typedef struct
{
    Tetromino_type_t    type;
    Rotation_angle_t    angle;
    u8                  x;
    u8                  y;
} Tetromino_t;

typedef struct
{
    HWND                hBtn;
    HWND                hCmb;
    // high 4bit: Tetromino_type_t  |  low 4bit: 0 or 1;
    // You can define SHOW_BORDER to show the border to understand this logic.
    u8                  matrix[TETRIS_Y_NUM + BORDER_WIDTH][TETRIS_X_NUM + BORDER_WIDTH * 2];
    u32                 speed;
    u32                 fast_speed;
    u32                 score;
    u32                 steps;
    Tetromino_t        *current;
    Tetromino_t        *next;
    BOOL                isPause;
} Tetris_t;

Tetris_t g_Tetris       = { 0 };

VOID OnPaint(HDC hdc);
VOID OnCreate(HWND hWnd);
VOID OnTimer(HWND hWnd, WPARAM timerId);
VOID OnLeft(HWND hWnd);
VOID OnRight(HWND hWnd);
VOID OnUp(HWND hWnd);
VOID OnDown(HWND hWnd);
VOID SetClientRect(HWND hWnd, LPRECT lpRect);
HBITMAP GetBitmap(Tetromino_type_t type);
VOID Repaint(HWND hWnd, Repaint_type_t type);

void game_start(HWND hWnd);
void game_over(HWND hWnd);
Tetromino_t *create_tetromino(void);
void release_tetromino(Tetromino_t *tetromino);
bool move_able(Tetromino_type_t type, Rotation_angle_t angle, u8 x, u8 y);
void eraser_line(HWND hWnd, Tetromino_t *tetromino);