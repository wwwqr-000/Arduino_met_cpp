#include <iostream>
#include <Windows.h>
#include <vector>

bool Arduino(const char* command, const char* SP) {
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    hSerial = CreateFile(SP, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600; //baud
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Set timeouts
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting serial port timeouts." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    DWORD bytesWritten;
    if (!WriteFile(hSerial, command, strlen(command), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    CloseHandle(hSerial);

    return 0;
}

int main() {
    while (true) {
        std::cout << "Enter number.\n> ";
        std::string number = "";
        std::cin >> number;
        Arduino(("txt_" + number).c_str(), "COM3");
        system("cls");
        std::cout << "Sending...";
        Sleep(1200);
        system("cls");
    }
    //Zet boud op 9600

    /*
    while (true) {
        Arduino("TurnOn", "COM3");
        Sleep(2000);
        Arduino("TurnOff", "COM3");
        Sleep(2000);
    }
    */
}
