// TesterDemo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TesterDemo.h"
#include "Settings.h"
#include <shellapi.h>  // For ShellExecute
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <tchar.h>
//#include <cassert>
std::unordered_map<std::string, void(*)(std::unordered_map<std::string, std::string>)>& getFunctionMap() {
    static std::unordered_map<std::string, void(*)(std::unordered_map<std::string, std::string>)> functionMap;
    return functionMap;
}

class TestInformation {
public:
    // Member variables
    std::string datasheetFileName;
    std::vector<TestFlow> testFlows;
    std::vector<Parameter> parameters;

    // Default constructor
    TestInformation() = default;

    // Parameterized constructor
    TestInformation(const std::string& datasheetFileName) : datasheetFileName(datasheetFileName) {}


    // Functions to add data to vectors
    void addTestFlow(const std::string& name, bool enabled, std::unordered_map<std::string, std::string> inputArgs) {
        testFlows.emplace_back(name, enabled, inputArgs);
    }

    void addParameter(const std::string& parameterName, double minSpec, double maxSpec, const std::string& unit,
        const std::string& format, const std::string& comment) {
        parameters.emplace_back(parameterName, minSpec, maxSpec, unit, format, comment);
    }

    // ... Other member functions as needed ...
};
TestInformation datasheetInfo;


//helper functions
std::unordered_map<std::string, std::string> parseKeyValue(const std::string& str) {
    std::unordered_map<std::string, std::string> resultMap;
    std::istringstream iss(str);
    std::string pair;

    while (std::getline(iss, pair, ';')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            resultMap[key] = value;
        }
    }

    return resultMap;
}
std::wstring StringToWString(const std::string& s)
{
    int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), (int)s.length(), NULL, 0);
    std::wstring ws(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), (int)s.length(), &ws[0], len);
    return ws;
}


void LogToStreamWindow(std::string message);
//Tester functions:
void datalog(const std::string& parameterName, double value) {
    auto it = std::find_if(datasheetInfo.parameters.begin(), datasheetInfo.parameters.end(), [&](const Parameter& param) {
        return param.ParameterName == parameterName;
        });
    if (it != datasheetInfo.parameters.end()) {
        int lead, decPlaces;
        sscanf_s(it->Format.c_str(), "%d.%d", &lead, &decPlaces);

        std::ostringstream oss;
        oss << std::left << std::setw(15) << it->ParameterName
            << " " << std::right << std::setw(lead + decPlaces + 2) << std::fixed << std::setprecision(decPlaces) << value
            << " " << std::setw(lead + decPlaces + 2) << it->MinSpec
            << " " << std::setw(lead + decPlaces + 2) << it->MaxSpec
            << " " << std::left << std::setw(15) << it->Unit;

        if (value < it->MinSpec || value > it->MaxSpec) {
            oss << " FAIL";
        }

        oss << "\r\n";
        LogToStreamWindow(oss.str());
    }
    else {
        auto invalidParameterName = parameterName;
        __debugbreak();
    }
}


void LoadDatasheet(const std::string& name) {
    // Buffer to hold the current working directory
    char buffer[MAX_PATH];
    std::string currentWorkingDir;

    // Get the current working directory; if successful, append the filename
    if (GetCurrentDirectoryA(MAX_PATH, buffer)) {
        currentWorkingDir = buffer;
        currentWorkingDir += "\\" + name; // Append the filename to the directory with a backslash
    }

    // Prepare the command with the full path to the script and the filename
    //std::string command = "cscript.exe ReadExcel.vbs " + currentWorkingDir;  // Assuming the VBScript file is named ReadExcel.vbs
    ShellExecuteA(NULL, "open", "cscript.exe", (std::string("ReadExcel.vbs ") + currentWorkingDir).c_str(), NULL, SW_HIDE);

    datasheetInfo = TestInformation(name);
    // Process TestFlow.csv
    std::ifstream testFlowFile("TestFlow.csv");
    std::string line;
    std::getline(testFlowFile, line);  // skip header line
    while (std::getline(testFlowFile, line)) {
        std::istringstream iss(line);
        std::string name, enabledStr, inputArgsStr;
        std::getline(iss, name, ',');
        std::getline(iss, enabledStr, ',');
        std::getline(iss, inputArgsStr, ',');
        bool enabled = (enabledStr == "Yes") ? true : false;
        auto inputArgs = parseKeyValue(inputArgsStr);
        datasheetInfo.addTestFlow(name, enabled, inputArgs);
    }
    testFlowFile.close();

    // Process TestLimits.csv
    std::ifstream testLimitsFile("TestLimits.csv");
    std::getline(testLimitsFile, line);  // skip header line
    while (std::getline(testLimitsFile, line)) {
        std::istringstream iss(line);
        std::string parameterName, minSpecStr, maxSpecStr, unit, format, comment;
        double minSpec, maxSpec;

        std::getline(iss, parameterName, ',');
        std::getline(iss, minSpecStr, ',');
        std::getline(iss, maxSpecStr, ',');
        std::getline(iss, unit, ',');
        std::getline(iss, format, ',');
        std::getline(iss, comment, ',');

        minSpec = std::stod(minSpecStr);
        maxSpec = std::stod(maxSpecStr);

        datasheetInfo.addParameter(parameterName, minSpec, maxSpec, unit, format, comment);
    }
    testLimitsFile.close();

    // Remember to handle file errors and clean up resources
}

//************** UI Of Demo Tester*************//
//************** ******************************//
#define MAX_LOADSTRING 100
#define ID_TEST_BUTTON 101
#define ID_TEXT_WINDOW 102
#define ID_RELOAD_LOT_BUTTON 103
#define ID_DATASHEET_BUTTON 106

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HFONT hMonospaceFont = NULL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    STS_LoadSettings(STS_USER_KEY, STS_HOSTNAME);
    // TODO: Place code here.
    LOGFONT lf = { 0 };
    lf.lfHeight = 18;  // Height of the font. Adjust as needed.
    wcscpy_s(lf.lfFaceName, L"Courier New");
    hMonospaceFont = CreateFontIndirect(&lf);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTPROGRAMDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPROGRAMDEMO));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_TESTPROGRAMDEMO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTPROGRAMDEMO);
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
HWND g_hWnd = NULL;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    // Create "(Re)Load Lot" button
    HWND hReloadLotButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Load Lot",    // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        10,         // x position 
        10,         // y position, place it below the "Test" button
        100,        // Button width
        30,         // Button height
        hWnd,       // Parent window
        (HMENU)ID_RELOAD_LOT_BUTTON, // No menu
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);   // Pointer not needed

    HWND hTestButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Test",    // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        10,         // x position 
        50,         // y position 
        100,        // Button width
        30,         // Button height
        hWnd,       // Parent window
        (HMENU)ID_TEST_BUTTON, // No menu
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);   // Pointer not needed

    // Create "Datasheet" button
    HWND hDatasheetButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Datasheet",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        10,         // x position 
        90,         // y position, place it below the "(Re)Load Lot" button
        100,        // Button width
        30,         // Button height
        hWnd,       // Parent window
        (HMENU)ID_DATASHEET_BUTTON, // No menu
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);   // Pointer not needed

    RECT rcClient;  // For the dimensions of the client area
    GetClientRect(hWnd, &rcClient);

    HWND hTextWindow = CreateWindowEx(
        0, L"EDIT",   // Extended style, predefined class; Unicode assumed
        NULL,       // No window text
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        120,       // x position
        10,        // y position 
        rcClient.right - 120, // width calculated as client area width minus 120px
        rcClient.bottom - 10, // height calculated as client area height minus 10px
        hWnd,      // Parent window
        (HMENU)ID_TEXT_WINDOW,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);  // Pointer not needed

    SendMessage(hTextWindow, WM_SETFONT, (WPARAM)hMonospaceFont, TRUE);

    // Check if button and text window were created successfully
    if (!hTestButton || !hTextWindow)
    {
        MessageBox(NULL,
            _T("Could not create button or text window!"),
            _T("Error!"),
            MB_ICONEXCLAMATION);
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
//void runTestFunctions()
//{
//
//}

int DeviceNumber = 1;
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
        case ID_TEST_BUTTON: // Handle our "Test" button click here
        {
            g_hWnd = hWnd;
            wchar_t textToStream[256];  // Ensure this buffer is large enough for your needs.
            swprintf(textToStream, sizeof(textToStream) / sizeof(wchar_t), L"\r\nTest Results for Device %d:\r\n", DeviceNumber);

            HWND hTextWindow = GetDlgItem(hWnd, ID_TEXT_WINDOW);
            SendMessage(hTextWindow, EM_REPLACESEL, TRUE, (LPARAM)textToStream);
            resetMessageFile();
            BeforeTestFlow();
            auto& functionMap = getFunctionMap();
            for (const auto& testFlow : datasheetInfo.testFlows) {
                if (testFlow.Enabled) {
                    auto it = functionMap.find(testFlow.FunctionName);
                    if (it != functionMap.end()) {
                        it->second(testFlow.InputArgs);  // Call the function
                    }
                    else {
                        // Trigger a breakpoint when debugging
                        auto invalidFunctionName = testFlow.FunctionName;
                        __debugbreak();
                    }
                }
            }
            AfterTestFlow();
            DeviceNumber++;

        }
        break;
        case ID_RELOAD_LOT_BUTTON:
        {
            resetMessageFile();
            LoadLot();  // Call the function to load the lot
            DeviceNumber = 1;
            // Get the handle to the text window
            HWND hTextWindow = GetDlgItem(hWnd, ID_TEXT_WINDOW);

            // Clear the existing text
            SetWindowText(hTextWindow, L"");

            // Print the new message
            const wchar_t* message = L"Lot loaded, proceed to test devices...\r\n";
            SendMessage(hTextWindow, EM_SETSEL, (WPARAM)-1, (LPARAM)-1); // Set selection to the end for appending text
            SendMessage(hTextWindow, EM_REPLACESEL, FALSE, (LPARAM)message); // Append the text
        }
        break;

        case ID_DATASHEET_BUTTON:
        {
            // Construct the path to the Datasheet.xlsx file
            wchar_t path[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, path); // Get the current directory
            wcscat_s(path, MAX_PATH, L"\\Datasheet.xlsx"); // Append the file name to the path

            // Use ShellExecute to open the file
            INT_PTR hRet = reinterpret_cast<INT_PTR>(ShellExecute(
                hWnd,           // Parent window
                L"open",        // Operation to perform
                path,           // File to open
                nullptr,        // Parameters
                nullptr,        // Default directory
                SW_SHOW));      // Show command

            if ((int)hRet <= 32) // If the function fails, the return value is less than or equal to 32
            {
                MessageBox(hWnd, L"Failed to open Datasheet.xlsx!", L"Error", MB_ICONERROR);
            }
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_SIZE:
    {
        // Get the dimensions of the client area.
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);

        // Calculate the width and height of the text window.
        int textWindowWidth = rcClient.right - 120;  // Assuming the button takes 120px, and we're leaving that space
        int textWindowHeight = rcClient.bottom - 10; // Assuming a margin of 10px at the bottom

        // Resize the text window.
        HWND hTextWindow = GetDlgItem(hWnd, ID_TEXT_WINDOW);
        SetWindowPos(hTextWindow, NULL, 0, 0, textWindowWidth, textWindowHeight, SWP_NOZORDER | SWP_NOMOVE);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
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

//UI functions
void LogToStreamWindow(std::string message) {
    std::wstring wMessage = StringToWString(message);
    HWND hTextWindow = GetDlgItem(g_hWnd, ID_TEXT_WINDOW);
    if (hTextWindow) {
        SendMessage(hTextWindow, EM_REPLACESEL, TRUE, (LPARAM)wMessage.c_str());
    }
}
