// Tetris.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tetris.h"
#include <time.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_BUTTON:
            game_start(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        OnCreate(hWnd);
        break;
    case WM_TIMER:
        OnTimer(hWnd, wParam);
        break;
    case WM_KEYDOWN:
        if (g_Tetris.isPause)
            break;
        switch (wParam)
        {
        case VK_LEFT:
            OnLeft(hWnd);
            break;
        case VK_RIGHT:
            OnRight(hWnd);
            break;
        case VK_UP:
            OnUp(hWnd);
            break;
        case VK_DOWN:
            OnDown(hWnd);
            break;
        default:
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

VOID OnPaint(HDC hdc)
{
    u8 i, j;
    u8 x_num, y_num, x_offset;
    u8 value;
    HPEN hPen = NULL;
    HDC hdcMem = NULL;
    HBITMAP hBitmap = NULL;
    TCHAR buf[64];
    
#ifdef SHOW_BORDER
    x_num = TETRIS_X_NUM + BORDER_WIDTH * 2;
    y_num = TETRIS_Y_NUM + BORDER_WIDTH;
    x_offset = 0;
#else
    x_num = TETRIS_X_NUM;
    y_num = TETRIS_Y_NUM;
    x_offset = BORDER_WIDTH;
#endif

    RECT st1 = { (x_num + 1) * TETRIS_UNIT, 3 * TETRIS_UNIT + 3, (x_num + 3) * TETRIS_UNIT, 4 * TETRIS_UNIT };
    RECT st2 = { (x_num + 1) * TETRIS_UNIT, 13 * TETRIS_UNIT + 3, (x_num + 6) * TETRIS_UNIT, 14 * TETRIS_UNIT };
    RECT st3 = { (x_num + 1) * TETRIS_UNIT, 14 * TETRIS_UNIT + 3, (x_num + 6) * TETRIS_UNIT, 15 * TETRIS_UNIT };

    hPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x_num * TETRIS_UNIT, 0, NULL);
    LineTo(hdc, x_num * TETRIS_UNIT, TETRIS_UNIT * y_num);
    DeleteObject(hPen);

    // next tetromino rectangle
    Rectangle(hdc, (x_num + 1) * TETRIS_UNIT, TETRIS_UNIT * 6, (x_num + TETROMINO_NUM + 1) * TETRIS_UNIT, TETRIS_UNIT * 10);

    hPen = CreatePen(PS_DOT, 1, RGB(192, 192, 192));
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdc, hPen);

    // matrix grid
    for (i = 1; i < x_num; i++)
    {
        MoveToEx(hdc, i * TETRIS_UNIT, 0, NULL);
        LineTo(hdc, i * TETRIS_UNIT, TETRIS_UNIT * y_num);
    }
    for (i = 1; i < y_num; i++)
    {
        MoveToEx(hdc, 0, i * TETRIS_UNIT, NULL);
        LineTo(hdc, TETRIS_UNIT * x_num, TETRIS_UNIT * i);
    }
    // next tetromino grid
    for (i = 1; i < TETROMINO_NUM; i++)
    {
        MoveToEx(hdc, (x_num + 1 + i) * TETRIS_UNIT, 6 * TETRIS_UNIT, NULL);
        LineTo(hdc, (x_num + 1 + i) * TETRIS_UNIT, (6 + TETROMINO_NUM) * TETRIS_UNIT);
    }
    for (i = 1; i < TETROMINO_NUM; i++)
    {
        MoveToEx(hdc, (x_num + 1) * TETRIS_UNIT, (6 + i) * TETRIS_UNIT, NULL);
        LineTo(hdc, (x_num + 1 + TETROMINO_NUM) * TETRIS_UNIT, (6 + i) * TETRIS_UNIT);
    }
    DeleteObject(hPen);

    // draw 'Static' control
    DrawText(hdc, _T("Level:"), _tcslen(_T("Level:")), &st1, DT_LEFT);
    wsprintf(buf, _T("Score: %d"), g_Tetris.score);
    DrawText(hdc, buf, _tcslen(buf), &st2, DT_LEFT);;
    wsprintf(buf, _T("Steps: %d"), g_Tetris.steps);
    DrawText(hdc, buf, _tcslen(buf), &st3, DT_LEFT);;

    // draw matrix
    for (i = 0; i < y_num; i++)
    {
        for (j = 0; j < x_num; j++)
        {
            value = g_Tetris.matrix[i][j + x_offset];
            if (value & 0x0F)
            {
                hBitmap = GetBitmap((Tetromino_type_t)((value & 0xF0) >> 4));
                SelectObject(hdcMem, hBitmap);
                BitBlt(hdc, j * TETRIS_UNIT, i * TETRIS_UNIT, TETRIS_UNIT, TETRIS_UNIT, hdcMem, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);
            }
        }
    }

    // draw current
    if (g_Tetris.current)
    {
        for (i = 0; i < TETROMINO_NUM; i++)
        {
            for (j = 0; j < TETROMINO_NUM; j++)
            {
                if (c_Tetromino[g_Tetris.current->type][g_Tetris.current->angle][i][j])
                {
                    hBitmap = GetBitmap(g_Tetris.current->type);
                    SelectObject(hdcMem, hBitmap);
                    BitBlt(hdc, (g_Tetris.current->x + j - x_offset) * TETRIS_UNIT, (g_Tetris.current->y + i) * TETRIS_UNIT, TETRIS_UNIT, TETRIS_UNIT, hdcMem, 0, 0, SRCCOPY);
                    DeleteObject(hBitmap);
                }
            }
        }
    }

    // draw next
    if (g_Tetris.next)
    {
        for (i = 0; i < TETROMINO_NUM; i++)
        {
            for (j = 0; j < TETROMINO_NUM; j++)
            {
                if (c_Tetromino[g_Tetris.next->type][g_Tetris.next->angle][i][j])
                {
                    hBitmap = GetBitmap(g_Tetris.next->type);
                    SelectObject(hdcMem, hBitmap);
                    BitBlt(hdc, (x_num + 1 + j) * TETRIS_UNIT, TETRIS_UNIT * (6 + i), TETRIS_UNIT, TETRIS_UNIT, hdcMem, 0, 0, SRCCOPY);
                    DeleteObject(hBitmap);
                }
            }
        }
    }

    DeleteDC(hdcMem);
}

VOID OnCreate(HWND hWnd)
{
    u8 i;
    RECT rect = { 0 };
    const TCHAR *Planets[] =
    {
        _T("1"), _T("2"), _T("3"), _T("4"),
        _T("5"), _T("6"), _T("7"), _T("8"), _T("9")
    };
    u8 size = sizeof(Planets) / sizeof(TCHAR*);
    
#ifdef SHOW_BORDER
    const u32 LEFT_CLIENT_WIDTH = (TETRIS_X_NUM + BORDER_WIDTH * 2) * TETRIS_UNIT;
    const u32 RIGHT_CLIENT_WIDTH = (TETROMINO_NUM + 2) * TETRIS_UNIT;
    rect.bottom = TETRIS_UNIT * (TETRIS_Y_NUM + BORDER_WIDTH);
#else
    const u32 LEFT_CLIENT_WIDTH = TETRIS_X_NUM * TETRIS_UNIT;
    const u32 RIGHT_CLIENT_WIDTH = (TETROMINO_NUM + 2) * TETRIS_UNIT;
    rect.bottom = TETRIS_UNIT * TETRIS_Y_NUM;
#endif
    rect.right = LEFT_CLIENT_WIDTH + RIGHT_CLIENT_WIDTH;

    // create 'start' buttton
    g_Tetris.hBtn = CreateWindow(_T("BUTTON"), _T("Start"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        LEFT_CLIENT_WIDTH + TETRIS_UNIT, TETRIS_UNIT, TETRIS_UNIT * TETROMINO_NUM, TETRIS_UNIT, 
        hWnd, (HMENU)IDC_BUTTON, hInst, NULL);

    // create 'level' Combobox
    g_Tetris.hCmb = CreateWindow(_T("COMBOBOX"), TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        LEFT_CLIENT_WIDTH + TETRIS_UNIT*3, TETRIS_UNIT*3, TETRIS_UNIT * TETROMINO_NUM / 2, size * TETRIS_UNIT, 
        hWnd, (HMENU)IDC_COMBOBOX, hInst, NULL);
    for (i = 0; i < size; i++)
    {
        SendMessage(g_Tetris.hCmb, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Planets[i]);
    }

    SetClientRect(hWnd, &rect);
}

VOID OnTimer(HWND hWnd, WPARAM timerId)
{
    u8 i, j;
    u8 eraser;
    Tetromino_t *tetromino;

    switch (timerId)
    {
    case IDT_TIMER:
        if (!g_Tetris.next)
        {
            // game over
            game_over(hWnd);
        }
        else
        {
            if (g_Tetris.current)
            {
                if (move_able(g_Tetris.current->type, g_Tetris.current->angle, g_Tetris.current->x, g_Tetris.current->y + 1))
                {
                    g_Tetris.current->y++;
                    Repaint(hWnd, DOWN);
                }
                else
                {
                    for (i = 0; i < TETROMINO_NUM; i++)
                    {
                        for (j = 0; j < TETROMINO_NUM; j++)
                        {
                            if (c_Tetromino[g_Tetris.current->type][g_Tetris.current->angle][i][j])
                            {
                                g_Tetris.matrix[g_Tetris.current->y + i][g_Tetris.current->x + j] = (g_Tetris.current->type << 4) | 1;
                            }
                        }
                    }
                    tetromino = g_Tetris.current;
                    g_Tetris.current = NULL;
                    eraser_line(hWnd, tetromino);
                    release_tetromino(tetromino);
                }
            }
            else
            {
                SetTimer(hWnd, IDT_TIMER, g_Tetris.speed, NULL);
                g_Tetris.current = g_Tetris.next;
                if (move_able(g_Tetris.current->type, g_Tetris.current->angle, g_Tetris.current->x, g_Tetris.current->y))
                {
                    g_Tetris.next = create_tetromino();
                    Repaint(hWnd, NEXT);
                    Repaint(hWnd, STEPS);
                }
                else
                {
                    g_Tetris.next = NULL;
                    SetTimer(hWnd, IDT_TIMER, g_Tetris.fast_speed, NULL);
                }
                Repaint(hWnd, UP);
            }
        }
        break;
    case IDT_TIMER2:
        KillTimer(hWnd, IDT_TIMER2);
        for (i = 0; i < TETRIS_Y_NUM; i++)
        {
            eraser = 1;
            for (j = BORDER_WIDTH; j < TETRIS_X_NUM + BORDER_WIDTH; j++)
            {
                if (g_Tetris.matrix[i][j])
                {
                    eraser = 0;
                    break;
                }
            }
            if (eraser)
            {
                memcpy(&g_Tetris.matrix[1], &g_Tetris.matrix[0], i*(TETRIS_X_NUM + BORDER_WIDTH * 2));
                memset(&g_Tetris.matrix[0][BORDER_WIDTH], 0, TETRIS_X_NUM);
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    default:
        break;
    }
}

VOID OnLeft(HWND hWnd)
{
    if (!g_Tetris.current)
        return;

    if (move_able(g_Tetris.current->type, g_Tetris.current->angle, g_Tetris.current->x - 1, g_Tetris.current->y))
    {
        g_Tetris.current->x--;
        Repaint(hWnd, LEFT);
    }
}

VOID OnRight(HWND hWnd)
{
    if (!g_Tetris.current)
        return;

    if (move_able(g_Tetris.current->type, g_Tetris.current->angle, g_Tetris.current->x + 1, g_Tetris.current->y))
    {
        g_Tetris.current->x++;
        Repaint(hWnd, RIGHT);
    }
}

VOID OnUp(HWND hWnd)
{
    Rotation_angle_t angle;

    if (!g_Tetris.current)
        return;

    angle = (Rotation_angle_t)((g_Tetris.current->angle + 1) % ROTATION_ANGLES);
    if (move_able(g_Tetris.current->type, angle, g_Tetris.current->x, g_Tetris.current->y))
    {
        g_Tetris.current->angle = angle;
        Repaint(hWnd, UP);
    }
}

VOID OnDown(HWND hWnd)
{
    if (!g_Tetris.current)
        return;

    SetTimer(hWnd, IDT_TIMER, g_Tetris.fast_speed, NULL);
}

VOID SetClientRect(HWND hWnd, LPRECT lpRect)
{
    RECT rcWin;
    RECT rcCli;
    LONG width;
    LONG height;

    GetWindowRect(hWnd, &rcWin);
    GetClientRect(hWnd, &rcCli);
    width = (rcWin.right - rcWin.left) - (rcCli.right - rcCli.left) + (lpRect->right - lpRect->left);
    height = (rcWin.bottom - rcWin.top) - (rcCli.bottom - rcCli.top) + (lpRect->bottom - lpRect->top);
    MoveWindow(
        hWnd,
        (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - width) / 2,
        width,
        height,
        TRUE
    );
}

HBITMAP GetBitmap(Tetromino_type_t type)
{
    HBITMAP bmp = NULL;

    switch (type)
    {
    case TETROMINO_I:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        break;
    case TETROMINO_J:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        break;
    case TETROMINO_L:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        break;
    case TETROMINO_O:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        break;
    case TETROMINO_S:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
        break;
    case TETROMINO_Z:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
        break;
    case TETROMINO_T:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
        break;
    default:
        bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        break;
    }

    return bmp;
}

VOID Repaint(HWND hWnd, Repaint_type_t type)
{
    u8 border;
    u8 border2;
    RECT rect;

#ifdef SHOW_BORDER
    border = 0;
    border2 = BORDER_WIDTH * 2;
#else
    border = BORDER_WIDTH;
    border2 = 0;
#endif

    switch (type)
    {
    case LEFT:
        rect.left = (g_Tetris.current->x - border) * TETRIS_UNIT;
        rect.right = rect.left + ((1 + TETROMINO_NUM) * TETRIS_UNIT);
        rect.top = g_Tetris.current->y * TETRIS_UNIT;
        rect.bottom = rect.top + (TETROMINO_NUM * TETRIS_UNIT);
        InvalidateRect(hWnd, &rect, TRUE);
        break;
    case RIGHT:
        rect.left = (g_Tetris.current->x - 1 - border) * TETRIS_UNIT;
        rect.right = rect.left + ((1 + TETROMINO_NUM) * TETRIS_UNIT);
        rect.top = g_Tetris.current->y * TETRIS_UNIT;
        rect.bottom = rect.top + (TETROMINO_NUM * TETRIS_UNIT);
        InvalidateRect(hWnd, &rect, TRUE);
        break;
    case UP:
        rect.left = (g_Tetris.current->x - border) * TETRIS_UNIT;
        rect.right = rect.left + (TETROMINO_NUM * TETRIS_UNIT);
        rect.top = g_Tetris.current->y * TETRIS_UNIT;
        rect.bottom = rect.top + (TETROMINO_NUM * TETRIS_UNIT);
        InvalidateRect(hWnd, &rect, TRUE);
        break;
    case DOWN:
        rect.left = (g_Tetris.current->x - border) * TETRIS_UNIT;
        rect.right = rect.left + (TETROMINO_NUM * TETRIS_UNIT);
        rect.top = (g_Tetris.current->y - 1) * TETRIS_UNIT;
        rect.bottom = rect.top + ((1 + TETROMINO_NUM) * TETRIS_UNIT);
        InvalidateRect(hWnd, &rect, TRUE);
        break;
    case NEXT:
        rect.left = (border2 + TETRIS_X_NUM + 1) * TETRIS_UNIT;
        rect.right = rect.left + (TETROMINO_NUM * TETRIS_UNIT);
        rect.top = TETRIS_UNIT * 6;
        rect.bottom = rect.top + (TETROMINO_NUM * TETRIS_UNIT);
        InvalidateRect(hWnd, &rect, TRUE);
        break;
    case STEPS:
        rect = { (border2 + TETRIS_X_NUM + 1) * TETRIS_UNIT, 14 * TETRIS_UNIT + 3, (border2 + TETRIS_X_NUM + 6) * TETRIS_UNIT, 15 * TETRIS_UNIT };
        InvalidateRect(hWnd, &rect, TRUE);
        break;
    default:
        break;
    }
}

void game_start(HWND hWnd)
{
    int i, j;
    TCHAR text[64] = { 0 };

    GetWindowText(g_Tetris.hBtn, text, 64);
    if (0 == _tcscmp(text, _T("Pause")))
    {
        KillTimer(hWnd, IDT_TIMER);
        SetWindowText(g_Tetris.hBtn, _T("Resume"));
        InvalidateRect(hWnd, NULL, TRUE);
        g_Tetris.isPause = 1;
    }
    else if (0 == _tcscmp(text, _T("Resume")))
    {
        SetTimer(hWnd, IDT_TIMER, g_Tetris.speed, NULL);
        SetWindowText(g_Tetris.hBtn, _T("Pause"));
        InvalidateRect(hWnd, NULL, TRUE);
        g_Tetris.isPause = 0;
    }
    else
    {
        memset(((u8*)&g_Tetris) + sizeof(HWND) * 2, 0, sizeof(g_Tetris) - sizeof(HWND) * 2);
        g_Tetris.speed = 1000;
        g_Tetris.fast_speed = 20;
        for (i = 0; i < TETRIS_Y_NUM + BORDER_WIDTH; i++)
        {
            for (j = 0; j < TETRIS_X_NUM + BORDER_WIDTH * 2; j++)
            {
                if (j < BORDER_WIDTH || j >= TETRIS_X_NUM + BORDER_WIDTH || i >= TETRIS_Y_NUM)
                {
                    g_Tetris.matrix[i][j] = TETROMINO_TYPES << 4 | 1;
                }
            }
        }

        g_Tetris.current = create_tetromino();
        g_Tetris.next = create_tetromino();
        g_Tetris.steps = 1;
        SetTimer(hWnd, IDT_TIMER, g_Tetris.speed, NULL);

        SetWindowText(g_Tetris.hBtn, _T("Pause"));
        InvalidateRect(hWnd, NULL, TRUE);
    }

    SetFocus(hWnd);
}

void game_over(HWND hWnd)
{
    KillTimer(hWnd, IDT_TIMER);

    if (g_Tetris.current)
    {
        release_tetromino(g_Tetris.current);
    }
    if (g_Tetris.next)
    {
        release_tetromino(g_Tetris.next);
    }
    memset(((u8*)&g_Tetris) + sizeof(HWND) * 2, 0, sizeof(g_Tetris) - sizeof(HWND) * 2);

    SetWindowText(g_Tetris.hBtn, _T("Start"));
    MessageBox(hWnd, _T("Game over!"), _T(""), MB_OK);
}

Tetromino_t *create_tetromino(void)
{
    static BOOL  flag = FALSE;
    Tetromino_t *tetromino = (Tetromino_t *)malloc(sizeof(Tetromino_t));
    memset(tetromino, 0, sizeof(Tetromino_t));

    if (!flag)
    {
        srand((unsigned int)time(NULL));
        flag = TRUE;
    }

    tetromino->type = (Tetromino_type_t)(rand() % TETROMINO_TYPES);
    tetromino->angle = (Rotation_angle_t)(rand() % ROTATION_ANGLES);
    tetromino->x = DEFAULT_X + BORDER_WIDTH;
    tetromino->y = DEFAULT_Y;

    g_Tetris.steps++;

    return tetromino;
}

void release_tetromino(Tetromino_t *tetromino)
{
    if (tetromino)
    {
        free(tetromino);
    }
}

bool move_able(Tetromino_type_t type, Rotation_angle_t angle, u8 x, u8 y)
{
    u8 i, j;

    if (!g_Tetris.current)
        return false;

    for (i = 0; i < TETROMINO_NUM; i++)
    {
        for (j = 0; j < TETROMINO_NUM; j++)
        {
            if (c_Tetromino[type][angle][i][j] && g_Tetris.matrix[y + i][x + j])
            {
                return false;
            }
        }
    }
    return true;
}

void eraser_line(HWND hWnd, Tetromino_t *tetromino)
{
    u8 i, j;
    u8 eraser;
    u8 count = 0;
    u32 bonus;

    for (i = tetromino->y; i < tetromino->y + TETROMINO_NUM && i < TETRIS_Y_NUM; i++)
    {
        eraser = 1;
        for (j = BORDER_WIDTH; j < TETRIS_X_NUM + BORDER_WIDTH; j++)
        {
            if (!g_Tetris.matrix[i][j])
            {
                eraser = 0;
                break;
            }
        }
        if (eraser)
        {
            count++;
            //memcpy(&g_Tetris.matrix[1], &g_Tetris.matrix[0], i*(TETRIS_X_NUM + BORDER_WIDTH * 2));
            //memset(&g_Tetris.matrix[0][BORDER_WIDTH], 0, TETRIS_X_NUM);
            memset(&g_Tetris.matrix[i][BORDER_WIDTH], 0, TETRIS_X_NUM);
        }
    }

    if (count)
    {
        bonus = ((1 << (count-1)) - 1) * 10;
        g_Tetris.score += count * 10 + bonus;
        InvalidateRect(hWnd, NULL, TRUE);
        SetTimer(hWnd, IDT_TIMER2, 20, NULL);
    }
}