#include <windows.h>
#include <string>
#include <cctype>
#include <uxtheme.h>

// Lemon's function
unsigned int hash_string(const char* str) {
    const char* v1 = str;
    unsigned int v2 = 0;
    int v3;

    if (*str) {
        do {
            if (isalpha(*v1))
                v3 = tolower(*v1);
            else
                v3 = *v1;
            v2 = v3 + 33 * v2;
        } while (*++v1);
    }
    return v2;
}

// Global variables
HWND hwndInput, hwndOutput, hwndCopyButton;

void CopyToClipboard(HWND hwnd, const std::string& text) {
    if (OpenClipboard(hwnd)) {
        EmptyClipboard();
        HGLOBAL hGlob = GlobalAlloc(GMEM_FIXED, text.size() + 1);
        memcpy(hGlob, text.c_str(), text.size() + 1);
        SetClipboardData(CF_TEXT, hGlob);
        CloseClipboard();
    }
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        hwndInput = CreateWindow("EDIT", "PK_S01_FATHERS_PRIDE",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            20, 20, 300, 25,
            hwnd, NULL, NULL, NULL);

        hwndOutput = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            20, 60, 300, 25,
            hwnd, NULL, NULL, NULL);

        hwndCopyButton = CreateWindow("BUTTON", "Copy",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 60, 75, 25,
            hwnd, (HMENU)1, NULL, NULL);

        CreateWindow("BUTTON", "Hash!",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 20, 75, 25,
            hwnd, (HMENU)2, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) {
            char buffer[256];
            GetWindowText(hwndOutput, buffer, sizeof(buffer));
            CopyToClipboard(hwnd, buffer);
        } else if (LOWORD(wParam) == 2) {
            char inputBuffer[256];
            GetWindowText(hwndInput, inputBuffer, sizeof(inputBuffer));

            if (strlen(inputBuffer) > 0) {
                unsigned int hashValue = hash_string(inputBuffer);
                std::string output = std::to_string(hashValue);
                SetWindowText(hwndOutput, output.c_str());
            } else {
                SetWindowText(hwndOutput, "");
            }
        }
        break;
    }
    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH hBrush = GetSysColorBrush(COLOR_3DFACE);
        FillRect(hdc, &rect, hBrush);
        return 1;
    }
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
        return (LRESULT)GetSysColorBrush(COLOR_3DFACE);
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetThemeAppProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS | STAP_ALLOW_WEBCONTENT);
    const char CLASS_NAME[] = "HashTool";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Hash Tool",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 150,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
