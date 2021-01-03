#include <iostream>
#include "windows.h"
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;

bool stop = true;
int sleepTime = 50;

void click() {
    mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

int __RPC_CALLEE ThreadFunc(LPVOID lpParam) {
    for (; !stop ;) {
        click();
        Sleep(sleepTime);
    }
    return 0;
}

char buf[1024];

int main() {
    ifstream config("autoclick.conf", std::ifstream::in);

    while (config.good()) {
        config.getline(buf, sizeof buf);
        if (memcmp(buf, "SEP=", 4) == 0) {
            sscanf_s(buf + 4, "%d", &sleepTime);
        }
    }

    config.close();

    MSG msg = {nullptr};
    HWND hConsole = GetActiveWindow();

    RegisterHotKey(hConsole, 1, 0, VK_F1);

    HANDLE handle;
    DWORD thread;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            switch (msg.wParam) {
                case 1 :
                    if (stop) {
                        stop = false;
                        handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadFunc), nullptr, 0, &thread);
                    } else {
                        stop = true;
                        CloseHandle(handle);
                    }
                    cout << "Touch F1" << endl;
                    break;
                default:
                    break;
            }
        }
    }
    UnregisterHotKey(hConsole, 1);

    return 0;
}
