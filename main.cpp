#include <windows.h>
#include <string>
#include <cctype>
#include <uxtheme.h>
#include <sstream>
#include <iomanip>

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

// Returns a 32-bit integer hash for the input string (case-insensitive).
inline constexpr std::uint32_t to_hash(const char* str) {
    std::uint32_t res = 0;
    for (int c = *str; c != '\0'; ++str, c = *str) {
        int ch_lower = std::isalpha(c) ? std::tolower(c) : c;
        res = static_cast<std::uint32_t>(ch_lower) + 33U * res;
    }
    return res;
}

// Swap endianness (byte-by-byte reversal)
inline constexpr std::uint32_t invert_bytes(std::uint32_t value) {
    return ((value & 0x000000FFU) << 24) |
           ((value & 0x0000FF00U) << 8) |
           ((value & 0x00FF0000U) >> 8) |
           ((value & 0xFF000000U) >> 24);
}

// Global variables
HWND hwndInput, hwndOutput1, hwndLabel1, hwndOutput2, hwndLabel2, hwndOutput3, hwndCopyInt, hwndCopyByte, hwndCopyIByte;

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

        hwndOutput1 = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            20, 60, 300, 25,
            hwnd, NULL, NULL, NULL);
        
        hwndLabel1 = CreateWindow("STATIC", "Bytes:",
            WS_CHILD | WS_VISIBLE,
            20, 90, 300, 25,
            hwnd, NULL, NULL, NULL);
        
        hwndOutput2 = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            20, 110, 300, 25,
            hwnd, NULL, NULL, NULL);
        
        hwndLabel2 = CreateWindow("STATIC", "Inverted Bytes:",
            WS_CHILD | WS_VISIBLE,
            20, 140, 300, 25,
            hwnd, NULL, NULL, NULL);
        
        hwndOutput3 = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            20, 160, 300, 25,
            hwnd, NULL, NULL, NULL);

        hwndCopyInt = CreateWindow("BUTTON", "Copy",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 60, 75, 25,
            hwnd, (HMENU)1, NULL, NULL);
        
        hwndCopyByte = CreateWindow("BUTTON", "Copy",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 110, 75, 25,
            hwnd, (HMENU)2, NULL, NULL);
        
        hwndCopyIByte = CreateWindow("BUTTON", "Copy",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 160, 75, 25,
            hwnd, (HMENU)3, NULL, NULL);

        CreateWindow("BUTTON", "Hash!",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            330, 20, 75, 25,
            hwnd, (HMENU)4, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) {
            char buffer[256];
            GetWindowText(hwndOutput1, buffer, sizeof(buffer));
            CopyToClipboard(hwnd, buffer);
        } else if (LOWORD(wParam) == 4) {
            char inputBuffer[256];
            GetWindowText(hwndInput, inputBuffer, sizeof(inputBuffer));

            if (strlen(inputBuffer) > 0) {
                unsigned int hashValue = hash_string(inputBuffer);
                std::uint32_t byteValue = to_hash(inputBuffer);
                std::uint32_t IbyteValue = invert_bytes(byteValue);
                
                std::stringstream output2, output3;
                std::string output = std::to_string(hashValue);
                output2 << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << byteValue;
                output3 << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << IbyteValue;
                
                std::string foutput2 = output2.str();
                std::string foutput3 = output3.str();
                
                SetWindowText(hwndOutput1, output.c_str());
                SetWindowText(hwndOutput2, foutput2.c_str());
                SetWindowText(hwndOutput3, foutput3.c_str());
            } else {
                SetWindowText(hwndOutput1, "");
            }
        } else if (LOWORD(wParam) == 2) {
            char buffer[256];
            GetWindowText(hwndOutput2, buffer, sizeof(buffer));
            CopyToClipboard(hwnd, buffer);
        } else if (LOWORD(wParam) == 3) {
            char buffer[256];
            GetWindowText(hwndOutput3, buffer, sizeof(buffer));
            CopyToClipboard(hwnd, buffer);
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
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 250,
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
