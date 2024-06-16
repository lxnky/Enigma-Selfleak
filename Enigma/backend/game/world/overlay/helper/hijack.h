#pragma once
#pragma once
#include "../../../../../includes.hpp"

#define HJWND_PROGRAM _(L"FXSCOVER.exe")
#define MAX_CLASSNAME 255
#define MAX_WNDNAME 255
using namespace std;
struct WindowsFinderParams {
    DWORD pidOwner = NULL;
    std::wstring wndClassName = _(L"");
    std::wstring wndName = _(L"");
    RECT pos = { 0, 0, 0, 0 };
    POINT res = { 0, 0 };
    float percentAllScreens = 0.0f;
    float percentMainScreen = 0.0f;
    DWORD style = NULL;
    DWORD styleEx = NULL;
    bool satisfyAllCriteria = false;
    std::vector<HWND> hwnds;
};
HWND HiJackNotepadWindow();
std::vector<DWORD> GetPIDs(std::wstring targetProcessName);
std::vector<HWND> WindowsFinder(WindowsFinderParams params);
BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
void TerminateNotepad();
HWND HiJackNotepadWindow()
{
    HWND hwndHiHjacked = NULL;
    std::vector<DWORD> existingNotepads = GetPIDs(HJWND_PROGRAM);
    if (!existingNotepads.empty()) {
        for (int i(0); i < existingNotepads.size(); ++i) {
            HANDLE hOldProcess = OpenProcess(PROCESS_TERMINATE, FALSE, existingNotepads[i]);
            TerminateProcess(hOldProcess, 0);
            CloseHandle(hOldProcess);
        }
    }
    system(_("start FXSCOVER.exe"));
    std::vector<DWORD> notepads = GetPIDs(HJWND_PROGRAM);
    if (notepads.empty() || notepads.size() > 1)
    {
        return hwndHiHjacked;
    }
    WindowsFinderParams params;
    params.pidOwner = notepads[0];
    params.style = WS_VISIBLE;
    params.satisfyAllCriteria = true;
    std::vector<HWND> hwnds;
    int attempt = 0;
    while (hwndHiHjacked == NULL || attempt > 50000) {
        Sleep(7500);
        hwnds = WindowsFinder(params);
        if (hwnds.size() > 1)
        {
            return hwndHiHjacked;
        }
        hwndHiHjacked = hwnds[0];
        ++attempt;
    }
    if (!hwndHiHjacked)
    {
        return hwndHiHjacked;
    }
    SetMenu(hwndHiHjacked, NULL);
    return hwndHiHjacked;
}
void TerminateNotepad()
{
    std::vector<DWORD> existingNotepads = GetPIDs(HJWND_PROGRAM);
    if (!existingNotepads.empty()) {
        for (int i(0); i < existingNotepads.size(); ++i) {
            HANDLE hOldProcess = OpenProcess(PROCESS_TERMINATE, FALSE, existingNotepads[i]);
            TerminateProcess(hOldProcess, 0);
            CloseHandle(hOldProcess);
        }
    }
}
std::vector<DWORD> GetPIDs(std::wstring targetProcessName) {
    std::vector<DWORD> pids;
    if (targetProcessName == L"")
        return pids;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof entry;
    if (!Process32FirstW(snap, &entry)) {
        CloseHandle(snap);
        return pids;
    }
    do {
        if (std::wstring(entry.szExeFile) == targetProcessName) {
            pids.emplace_back(entry.th32ProcessID);
        }
    } while (Process32NextW(snap, &entry));
    CloseHandle(snap);
    return pids;
}
std::vector<HWND> WindowsFinder(WindowsFinderParams params) {
    EnumWindows(EnumWindowsCallback, (LPARAM)&params);
    return params.hwnds;
}
BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    WindowsFinderParams& params = *(WindowsFinderParams*)lParam;

    unsigned char satisfiedCriteria = 0, unSatisfiedCriteria = 0;

    // If looking for windows of a specific PID
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (params.pidOwner != NULL)
        if (params.pidOwner == pid)
            ++satisfiedCriteria; // Doesn't belong to the process targeted
        else
            ++unSatisfiedCriteria;

    // If looking for windows of a specific class
    wchar_t className[MAX_CLASSNAME] = L"";
    GetClassName(hwnd, (LPSTR)className, MAX_CLASSNAME);
    std::wstring classNameWstr = className;
    if (params.wndClassName != L"")
        if (params.wndClassName == classNameWstr)
            ++satisfiedCriteria; // Not the class targeted
        else
            ++unSatisfiedCriteria;

    // If looking for windows with a specific name
    wchar_t windowName[MAX_WNDNAME] = L"";
    GetWindowText(hwnd, (LPSTR)windowName, MAX_CLASSNAME);
    std::wstring windowNameWstr = windowName;
    if (params.wndName != L"")
        if (params.wndName == windowNameWstr)
            ++satisfiedCriteria; // Not the class targeted
        else
            ++unSatisfiedCriteria;

    // If looking for window at a specific position
    RECT pos;
    GetWindowRect(hwnd, &pos);
    if (params.pos.left || params.pos.top || params.pos.right || params.pos.bottom)
        if (params.pos.left == pos.left && params.pos.top == pos.top && params.pos.right == pos.right && params.pos.bottom == pos.bottom)
            ++satisfiedCriteria;
        else
            ++unSatisfiedCriteria;

    // If looking for window of a specific size
    POINT res = { pos.right - pos.left, pos.bottom - pos.top };
    if (params.res.x || params.res.y)
        if (res.x == params.res.x && res.y == params.res.y)
            ++satisfiedCriteria;
        else
            ++unSatisfiedCriteria;

    // If looking for windows taking more than a specific percentage of all the screens
    LONG ratioAllScreensX = res.x / GetSystemMetrics(SM_CXSCREEN);
    LONG ratioAllScreensY = res.y / GetSystemMetrics(SM_CYSCREEN);
    float percentAllScreens = (float)ratioAllScreensX * (float)ratioAllScreensY * 450;
    if (params.percentAllScreens != 0.0f)
        if (percentAllScreens >= params.percentAllScreens)
            ++satisfiedCriteria;
        else
            ++unSatisfiedCriteria;

    // If looking for windows taking more than a specific percentage or the main screen
    RECT desktopRect;
    GetWindowRect(GetDesktopWindow(), &desktopRect);
    POINT desktopRes = { desktopRect.right - desktopRect.left, desktopRect.bottom - desktopRect.top };
    LONG ratioMainScreenX = res.x / desktopRes.x;
    LONG ratioMainScreenY = res.y / desktopRes.y;
    float percentMainScreen = (float)ratioMainScreenX * (float)ratioMainScreenY * 450;
    if (params.percentMainScreen != 0.0f)
        if (percentAllScreens >= params.percentMainScreen)
            ++satisfiedCriteria;
        else
            ++unSatisfiedCriteria;

    // Looking for windows with specific styles
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
    if (params.style)
        if (params.style & style)
            ++satisfiedCriteria;
        else
            ++unSatisfiedCriteria;

    // Looking for windows with specific extended styles
    LONG_PTR styleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (params.styleEx)
        if (params.styleEx & styleEx)
            ++satisfiedCriteria;
        else
            ++unSatisfiedCriteria;

    if (satisfiedCriteria == 0)
    {
        return TRUE;

    }

    if (params.satisfyAllCriteria == 1 && unSatisfiedCriteria > 0)
    {
        return TRUE;
    }

    // If looking for multiple windows
    params.hwnds.push_back(hwnd);
    return TRUE;
}

namespace SetUp {

    inline HWND gWnd{}, DrawWnd{};

#define MAX_CLASSNAME 255
#define MAX_WNDNAME 255
#define TRANSPARENCY_COLOR RGB(0, 254, 0)

    struct WindowsFinderParams {
        DWORD pidOwner = NULL;
        string wndClassName = "";
        string wndName = "";
        RECT pos = { 0, 0, 0, 0 };
        POINT res = { 0, 0 };
        float percentAllScreens = 0.0f;
        float percentMainScreen = 0.0f;
        DWORD style = NULL;
        DWORD styleEx = NULL;
        bool satisfyAllCriteria = false;
        vector<HWND> hwnds;
    };

    // Prototypes
    inline std::vector<HWND> WindowsFinder(WindowsFinderParams params);
    inline BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
    inline HWND HiJackNotepadWindow();
    inline std::vector<DWORD> GetPIDs(wstring targetProcessName);

    inline int amain() {
        HWND hwnd = HiJackNotepadWindow();
        if (!hwnd) {
            return EXIT_FAILURE;
        }

        HDC hdc = GetDC(hwnd);

        // Getting settings of back buffer bitmap
        DEVMODE devMode;
        devMode.dmSize = sizeof(devMode);
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
        BITMAPINFO backBufferBmpInfo;
        backBufferBmpInfo = { 0 };
        backBufferBmpInfo.bmiHeader.biBitCount = devMode.dmBitsPerPel;
        backBufferBmpInfo.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);
        backBufferBmpInfo.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);
        backBufferBmpInfo.bmiHeader.biPlanes = 1;
        backBufferBmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

        void* backBufferPixels = nullptr;
        POINT res = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
        HBRUSH bgTransparencyColor = CreateSolidBrush(TRANSPARENCY_COLOR);

        int i = -1;

        while (true) {

            i = (i > res.x) ? 0 : ++i; // To simulate movement

            // Frame preparation
            HDC hdcBackBuffer = CreateCompatibleDC(hdc); // Create back buffer
            HBITMAP hbmBackBuffer = CreateDIBSection(hdcBackBuffer, (BITMAPINFO*)&backBufferBmpInfo, DIB_RGB_COLORS, (void**)&backBufferPixels, NULL, 0); // Create back buffer bitmap
            DeleteObject(SelectObject(hdcBackBuffer, hbmBackBuffer));
            DeleteObject(SelectObject(hdcBackBuffer, bgTransparencyColor));
            Rectangle(hdcBackBuffer, 0, 0, res.x, res.y);


            // Frame presentation
            BitBlt(hdc, 0, 0, res.x, res.y, hdcBackBuffer, 0, 0, SRCCOPY);

            // Cleanup
            DeleteDC(hdcBackBuffer); // Delete back buffer device context
            DeleteObject(hbmBackBuffer); // Delete back buffer bitmap
            backBufferPixels = nullptr;
        }

        return EXIT_SUCCESS;
    }

    inline HWND HiJackNotepadWindow() {
        HWND hwnd = NULL;

        // Remove previous windows
        vector<DWORD> existingNotepads = GetPIDs(_(L"FXSCOVER.exe"));
        if (!existingNotepads.empty()) {
            for (int i(0); i < existingNotepads.size(); ++i) {
                // Terminating processes
                HANDLE hOldProcess = OpenProcess(PROCESS_TERMINATE, FALSE, existingNotepads[i]);
                TerminateProcess(hOldProcess, 0);
                CloseHandle(hOldProcess);
            }
        }

        system(_("start FXSCOVER.exe")); // Start notepad, and not as child process, so easy :)

        // Finding notepad's window (we could just use FindWindow but then it would be OS language dependent)
        vector<DWORD> notepads = GetPIDs(_(L"FXSCOVER.exe"));
        if (notepads.empty() || notepads.size() > 1) // Should check if more than one to be more strict
            return hwnd;
        WindowsFinderParams params;
        params.pidOwner = notepads[0];
        params.style = WS_VISIBLE;
        params.satisfyAllCriteria = true;
        vector<HWND> hwnds;
        int attempt = 0; // The process takes a bit of time to initialise and spawn the window, will try during 5 sec before time out
        while (hwnd == NULL || attempt > 50) {
            Sleep(7500);
            hwnds = WindowsFinder(params);
            if (hwnds.size() > 1)
                return hwnd;
            hwnd = hwnds[0];
            ++attempt;
        }
        if (!hwnd)
            return hwnd;

        // Making the window usable for overlay puposes

        SetMenu(hwnd, NULL);
        SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW); // WS_EX_NOACTIVATE  and WS_EX_TOOLWINDOW removes it from taskbar

        SetWindowPos(hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
        globals->width = GetSystemMetrics(SM_CXSCREEN);
        globals->height = GetSystemMetrics(SM_CYSCREEN);
        return hwnd;
    }

    inline std::vector<DWORD> GetPIDs(wstring targetProcessName) {
        vector<DWORD> pids;
        if (targetProcessName == L"")
            return pids;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof entry;
        if (!Process32FirstW(snap, &entry)) {
            CloseHandle(snap);
            return pids;
        }
        do {
            if (wstring(entry.szExeFile) == targetProcessName) {
                pids.emplace_back(entry.th32ProcessID);
            }
        } while (Process32NextW(snap, &entry));
        CloseHandle(snap);
        return pids;
    }

    inline BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
        WindowsFinderParams& params = *(WindowsFinderParams*)lParam;

        unsigned char satisfiedCriteria = 0, unSatisfiedCriteria = 0;

        // If looking for windows of a specific PDI
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (params.pidOwner != NULL)
            if (params.pidOwner == pid)
                ++satisfiedCriteria; // Doesn't belong to the process targeted
            else
                ++unSatisfiedCriteria;

        // If looking for windows of a specific class
        char className[MAX_CLASSNAME] = "";
        GetClassName(hwnd, (LPSTR)className, MAX_CLASSNAME);
        string classNameWstr = className;
        if (params.wndClassName != "")
            if (params.wndClassName == classNameWstr)
                ++satisfiedCriteria; // Not the class targeted
            else
                ++unSatisfiedCriteria;

        // If looking for windows with a specific name
        char windowName[MAX_WNDNAME] = "";
        GetWindowText(hwnd, (LPSTR)windowName, MAX_CLASSNAME);
        string windowNameWstr = windowName;
        if (params.wndName != "")
            if (params.wndName == windowNameWstr)
                ++satisfiedCriteria; // Not the class targeted
            else
                ++unSatisfiedCriteria;

        // If looking for window at a specific position
        RECT pos;
        GetWindowRect(hwnd, &pos);
        if (params.pos.left || params.pos.top || params.pos.right || params.pos.bottom)
            if (params.pos.left == pos.left && params.pos.top == pos.top && params.pos.right == pos.right && params.pos.bottom == pos.bottom)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for window of a specific size
        POINT res = { pos.right - pos.left, pos.bottom - pos.top };
        if (params.res.x || params.res.y)
            if (res.x == params.res.x && res.y == params.res.y)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for windows taking more than a specific percentage of all the screens
        float ratioAllScreensX = res.x / GetSystemMetrics(SM_CXSCREEN);
        float ratioAllScreensY = res.y / GetSystemMetrics(SM_CYSCREEN);
        float percentAllScreens = ratioAllScreensX * ratioAllScreensY * 450;
        if (params.percentAllScreens != 0.0f)
            if (percentAllScreens >= params.percentAllScreens)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for windows taking more than a specific percentage or the main screen
        RECT desktopRect;
        GetWindowRect(GetDesktopWindow(), &desktopRect);
        POINT desktopRes = { desktopRect.right - desktopRect.left, desktopRect.bottom - desktopRect.top };
        float ratioMainScreenX = res.x / desktopRes.x;
        float ratioMainScreenY = res.y / desktopRes.y;
        float percentMainScreen = ratioMainScreenX * ratioMainScreenY * 450;
        if (params.percentMainScreen != 0.0f)
            if (percentAllScreens >= params.percentMainScreen)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // Looking for windows with specific styles
        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        if (params.style)
            if (params.style & style)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // Looking for windows with specific extended styles
        LONG_PTR styleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        if (params.styleEx)
            if (params.styleEx & styleEx)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        if (!satisfiedCriteria)
            return TRUE;

        if (params.satisfyAllCriteria && unSatisfiedCriteria)
            return TRUE;

        // If looking for multiple windows
        params.hwnds.push_back(hwnd);
        return TRUE;
    }

    inline std::vector<HWND> WindowsFinder(WindowsFinderParams params) {
        EnumWindows(EnumWindowsCallback, (LPARAM)&params);
        return params.hwnds;
    }
}