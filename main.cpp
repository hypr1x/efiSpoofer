#include "Windows.h"
#include "iostream"
#include <conio.h>
#include <urlmon.h>
#include <fstream>
#include <string>
#include <sstream>
#include "efi.h"
#pragma comment(lib, "urlmon.lib")

std::wstring GetExecutablePath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    std::wcout << L"Executable Path: " << std::wstring(buffer).substr(0, pos) << std::endl;
    return std::wstring(buffer).substr(0, pos);
}

void ExtractZip(const std::wstring& zipFile, const std::wstring& extractTo) {
    std::wstringstream command;
    command << L"powershell Expand-Archive -Path \"" << zipFile << L"\" -DestinationPath \"" << extractTo << L"\" -Force";
    _wsystem(command.str().c_str());
    std::wcout << L"Extracted File" << std::endl;  
}

void DeleteFileW(const std::wstring& filePath) {
    if (DeleteFile(filePath.c_str())) {
        std::wcout << L"Deleted file" << std::endl;
    }
}

std::wstring ModifySerial(const std::wstring& serial) {
    if (serial.length() < 4) {
        return serial; 
    }

    std::wstring modifiedSerial = serial;
    // Change middle numbers (if length > 4)
    for (size_t i = 3; i < modifiedSerial.length() - 2; ++i) {
        if (std::isdigit(modifiedSerial[i])) { // Check if character is a digit
            // Generate a random number different from the original
            do {
                modifiedSerial[i] = static_cast<wchar_t>('0' + rand() % 10); // Random number between '0' and '9'
            } while (modifiedSerial[i] == serial[i]);
        }
    }
    return modifiedSerial;
}

void ExtractValuesFromFile(const std::wstring& filePath) {
    std::wifstream file(filePath);
    if (!file.is_open()) {
        std::wcerr << L"Failed to open file: " << filePath << std::endl;
        return;
    }

    std::wstring line;
    while (std::getline(file, line)) {
        std::wistringstream iss(line);
        std::wstring command, identifier, serial;

        iss >> command >> identifier; // Read command and identifier
        std::getline(iss, serial); // Read the rest of the line as serial

        // Modify serial numbers only for known identifiers
        if (identifier == L"/BS" || identifier == L"/SU") {
            std::wstring originalSerial = serial;
            serial = ModifySerial(serial);
            std::wcout << "Original: " << identifier << L" " << originalSerial << std::endl;
            std::wcout << "New: " << identifier << L" " << serial << std::endl;
        }
    }

    file.close();
}

void SetConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    SetConsoleTitle(L"Hyper Spoofer");

    SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "    ___ ___                                 _________                     _____             " << std::endl;
    std::cout << "   /   |   \\ ___ __ ______   ___________   /   _____/_____   ____   _____/ ____\\___________ " << std::endl;
    std::cout << "  /    ~    <   |  |\\____ \\_/ __ \\_  __ \\  \\_____  \\\\____ \\ /  _ \\ /  _ \\   __\\/ __ \\_  __ \\" << std::endl;
    std::cout << "  \\    Y    /\\___  ||  |_> >  ___/|  | \\/  /        \\  |_> >  <_> |  <_> )  | \\  ___/|  | \\/" << std::endl;
    std::cout << "   \\___|_  / / ____||   __/ \\___  >__|    /_______  /   __/ \\____/ \\____/|__|  \\___  >__|   " << std::endl;
    std::cout << "         \\/  \\/     |__|        \\/                \\/|__|                           \\/       " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "  Enter License Key: ";
    std::string key;
    std::cin >> key;
    system("cls");
    std::ofstream outputFile("efi.zip", std::ios::binary);
    outputFile.write(reinterpret_cast<char*>(efiData), sizeof(efiData));
    outputFile.close();
    std::wstring exePath = GetExecutablePath();
    std::wstring zipFile = exePath + L"\\efi.zip"; // Download to the same directory as the executable

    ExtractZip(zipFile, exePath); // Extract to the same directory as the executable
    DeleteFileW(zipFile);
    //std::wstring startupFilePath = exePath + L"\\efi\\boot\\startup.nsh";
    //ExtractValuesFromFile(startupFilePath);
    std::wcout << L"Successfully created USB files" << std::endl;
    return _getch();
}