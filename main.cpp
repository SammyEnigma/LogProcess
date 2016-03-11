#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

/// Adds an underscore before the filename in a command line
/// that contains a filename and parameters.
/// "C:/path/to/file.exe -foo C:/path/to/file1" becomes
/// "C:/path/to/_file.exe -foo C:/path/to/file1"
wstring addUnderscore(const wstring &cmdLine)
{
    int pos = -1;
    bool quoted = false;
    for (int ii = 0; ii < cmdLine.size(); ++ii) {
        if (cmdLine[ii] == '"' && !(ii > 0 && cmdLine[ii-1] == '"')) {
            quoted = !quoted;
        }

        if (cmdLine[ii] == ' ' && !quoted) {
            break;
        }

        if (cmdLine[ii] == '\\' || cmdLine[ii] == '/') {
            pos = ii;
        }
    }
    if (pos < 0) {
        return wstring(L"_") + wstring(cmdLine);
    }

    pos++;
    return cmdLine.substr(0, pos) + L"_" + cmdLine.substr(pos);
}

/// Gets the absolute path to the current process's
/// executable file.
wstring getFileName()
{
    wchar_t fileName[MAX_PATH];
    GetModuleFileNameW(0, fileName, MAX_PATH);
    return fileName;
}

/// Runs a process, waits until it finishes, and returns the exit code.
/// Returns -9999 if the process could not be started.
int runProcess(const wstring &cmdLine) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    CreateProcessW(0, (wchar_t*) cmdLine.c_str(), 0, 0, 0, 0, 0, 0, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD dwExitCode = -9999;
    GetExitCodeProcess(pi.hProcess, &dwExitCode);
    return dwExitCode;
}

/// Returns the current time as text.
string currentTime()
{
    DWORD ticks = GetTickCount();
    DWORD milliseconds = ticks % 1000;
    ticks /= 1000;
    DWORD seconds = ticks % 60;
    ticks /= 60;
    DWORD minutes = ticks % 60;
    ticks /= 60;
    DWORD hours = ticks; // may exceed 24 hours.
    char buf[100];
    _snprintf(buf, 100, "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
    return buf;
}

int main(int argc, char *argv[])
{
    string startTime = currentTime();
    int result = runProcess(addUnderscore(GetCommandLineW()));
    const wchar_t *logFileName = L"C:\\LogProcess\\log.csv";
    bool exists = std::ifstream(logFileName).good();
    wofstream f(logFileName, std::ofstream::out | std::ofstream::app);
    if (f.good()) {
        if (!exists) {
            f << "Start time\tCommand line\tFinish time\tExit code\n";
        }
        f << startTime.c_str() << "\t" << GetCommandLineW() << "\t" << currentTime().c_str() << "\t" << result << endl;
    }
    return result;
}
