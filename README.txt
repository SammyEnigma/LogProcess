This is a handy little tool to log each time a process is run.

How to use:
1. Rename the process you want to monitor, adding a single underscore before it. "C:/path/to/file.exe" becomes "C:/path/to/_file.exe".
2. Rename/move the LogProcess.exe to the original process's name. It becomes "C:/path/to/file.exe".
3. Create an empty directory "C:/LogProcess", otherwise no log will be produced.
4. After the process has been run, open the file "C:/LogProcess/log.csv".

The log file is tab-separated, not comma-separated, so you can paste it directly in excel, or open it in OpenOffice Calc.
The table has 4 columns - "Start time", "Command line", "Finish time", and "Exit code".

I guess you can achieve similar logging using ProcessMonitor, but I couldn't find how to log the exit code, which I need.
