;API SHGetFolderPathA, shell32.dll 


extern   GetVolumeInformationA
import   GetVolumeInformationA kernel32.dll 

extern   GetDriveTypeA
import   GetDriveTypeA kernel32.dll 

extern   CreateNamedPipeA
import   CreateNamedPipeA kernel32.dll 


extern  GetLogicalDriveStringsA
import  GetLogicalDriveStringsA kernel32.dll 

extern  FileTimeToSystemTime
import  FileTimeToSystemTime kernel32.dll 

extern  WriteConsoleOutputCharacterA
import  WriteConsoleOutputCharacterA kernel32.dll 

extern  WriteConsoleOutputA
import  WriteConsoleOutputA kernel32.dll 

extern  ReadConsoleOutputA
import  ReadConsoleOutputA kernel32.dll 

extern  FlushConsoleInputBuffer
import  FlushConsoleInputBuffer kernel32.dll 

extern  GetNumberOfConsoleInputEvents
import  GetNumberOfConsoleInputEvents kernel32.dll 

extern  ReadConsoleInputA
import  ReadConsoleInputA kernel32.dll 

extern  WriteConsoleA
import  WriteConsoleA kernel32.dll 

extern  AllocConsole
import  AllocConsole kernel32.dll 

extern  GetStdHandle
import  GetStdHandle kernel32.dll 

extern  GetConsoleScreenBufferInfo
import  GetConsoleScreenBufferInfo kernel32.dll 

extern  GetLargestConsoleWindowSize
import  GetLargestConsoleWindowSize kernel32.dll 

extern  FreeConsole
import  FreeConsole kernel32.dll 

extern  SetConsoleScreenBufferSize
import  SetConsoleScreenBufferSize kernel32.dll 

extern  SetConsoleWindowInfo
import  SetConsoleWindowInfo kernel32.dll 

extern  SetConsoleCursorPosition
import  SetConsoleCursorPosition kernel32.dll 

extern  SetConsoleTextAttribute
import  SetConsoleTextAttribute kernel32.dll 

extern  WriteConsoleA 
import  WriteConsoleA  kernel32.dll 

extern  FillConsoleOutputCharacterA 
import  FillConsoleOutputCharacterA  kernel32.dll 

extern  FillConsoleOutputAttribute 
import  FillConsoleOutputAttribute  kernel32.dll 


extern  GetCurrentHwProfileA
import GetCurrentHwProfileA advapi32.dll  

extern  GetComputerNameA 
import  GetComputerNameA kernel32.dll 
 
extern  GetUserNameA 
import  GetUserNameA advapi32.dll  


extern  CreateThread
import  CreateThread kernel32.dll 


extern  ExitThread 
import  ExitThread  kernel32.dll 

extern  GetExitCodeProcess
import  GetExitCodeProcess kernel32.dll 


extern  GetSystemInfo
import  GetSystemInfo kernel32.dll 


extern  GetCurrentProcess
import  GetCurrentProcess kernel32.dll 

extern  IsWow64Process
import  IsWow64Process kernel32.dll 


extern  RegQueryValueExA
import RegQueryValueExA advapi32.dll  RegQueryValueExA
 
extern  RegEnumValueA
import RegEnumValueA advapi32.dll  

extern  RegEnumKeyExA
import RegEnumKeyExA advapi32.dll  
extern MoveFileA
import MoveFileA kernel32.dll 
 
extern  GetLocalTime 
import  GetLocalTime  kernel32.dll 

extern  SetUnhandledExceptionFilter
import  SetUnhandledExceptionFilter kernel32.dll 


extern FreeLibrary
import FreeLibrary kernel32.dll 

extern lstrlen 
import lstrlen kernel32.dll 

extern  GetWindowsDirectoryA
import  GetWindowsDirectoryA kernel32.dll 
extern GetStartupInfoA
import GetStartupInfoA kernel32.dll 

extern lstrcmp 
import lstrcmp kernel32.dll 

extern GetFileAttributesA 
import GetFileAttributesA kernel32.dll 

extern  lstrcat 
import   lstrcat  kernel32.dll  
extern   CreateProcessA
import    CreateProcessA kernel32.dll  


extern  lstrcpy 
import   lstrcpy kernel32.dll  


extern  lstrcpyn 
import   lstrcpyn kernel32.dll  

extern GetSystemMetrics
import  GetSystemMetrics user32.dll
extern CharLowerA
import  CharLowerA user32.dll

extern GetSystemMetrics
import  GetSystemMetrics user32.dll

extern GetSystemMetrics
import  GetSystemMetrics user32.dll


extern CreateDIBSection
import CreateDIBSection gdi32.dll

extern CreatePalette
import CreatePalette gdi32.dll

extern DeleteObject
import DeleteObject gdi32.dll

extern RealizePalette
import RealizePalette gdi32.dll


extern SelectPalette 
import SelectPalette gdi32.dll

extern CreateDCA
import CreateDCA gdi32.dll
extern DeleteDC
import DeleteDC gdi32.dll

extern SelectObject
import SelectObject gdi32.dll

extern CreateCompatibleDC
import CreateCompatibleDC gdi32.dll

extern SetPixel
import SetPixel gdi32.dll

extern BitBlt
import BitBlt gdi32.dll
;;;;;;;;;;;;;;;
extern SendMessage
extern GetSystemDirectoryA
extern GetTickCount
extern RegSetValueEx
extern RegCloseKey
extern RegOpenKeyEx
extern CopyFile
extern SetWindowsHookEx
extern OpenSCManager
extern CreateService
extern StartService
extern DeviceIoControl
extern ControlService
extern DeleteService
extern CloseServiceHandle
extern NtQuerySystemInformation
extern RegDeleteValue
extern OpenService
extern QueryServiceStatus 

extern SetFileAttributes
extern DeleteFile
extern RtlAnsiStringToUnicodeString
import RtlAnsiStringToUnicodeString ntdll.dll
extern ZwDeleteFile
import ZwDeleteFile ntdll.dll

extern SetKeyboardState
extern ToAscii

import  SetKeyboardState user32.dll
import  ToAscii user32.dll

extern GetUserName
extern FindFirstFileA
extern FindNextFile
extern FindClose
 
extern SetFilePointer
import  SetFilePointer kernel32.dll  SetFilePointer 

extern CreateFileMapping
import  CreateFileMapping kernel32.dll CreateFileMappingA

extern OpenFileMapping 
import OpenFileMapping kernel32.dll  OpenFileMappingA

extern   MapViewOfFile 
import   MapViewOfFile kernel32.dll 

extern   UnmapViewOfFile 
import   UnmapViewOfFile kernel32.dll

extern  GetLastError
import  GetLastError kernel32.dll 

import GetUserName advapi32.dll  GetUserNameA
import FindFirstFileA kernel32.dll FindFirstFileA
import FindNextFile kernel32.dll FindNextFileA
import FindClose kernel32.dll



import SetFileAttributes kernel32.dll SetFileAttributesA
import DeleteFile kernel32.dll DeleteFileA

extern  RegCreateKeyEx
import RegCreateKeyEx advapi32.dll  RegCreateKeyExA

import RegDeleteValue advapi32.dll RegDeleteValueA

import OpenService advapi32.dll OpenServiceA

import NtQuerySystemInformation ntdll.dll NtQuerySystemInformation
import SendMessage user32.dll  SendMessageA
import GetSystemDirectoryA kernel32.dll GetSystemDirectoryA 
import GetTickCount kernel32.dll GetTickCount 
import RegSetValueEx advapi32.dll RegSetValueExA 
import RegCloseKey advapi32.dll  RegCloseKey 
import CopyFile kernel32.dll  CopyFileA 
import SetWindowsHookEx user32.dll  SetWindowsHookExA 
import OpenSCManager  advapi32.dll OpenSCManagerA  
import CreateService advapi32.dll CreateServiceA 
import StartService advapi32.dll StartServiceA 
import DeviceIoControl kernel32.dll DeviceIoControl
import ControlService advapi32.dll ControlService
import DeleteService advapi32.dll DeleteService
import CloseServiceHandle advapi32.dll CloseServiceHandle
import RegOpenKeyEx advapi32.dll  RegOpenKeyExA
import QueryServiceStatus advapi32.dll  QueryServiceStatus

extern MessageBoxA
import MessageBoxA user32.dll MessageBoxA
extern PeekMessage 
import PeekMessage user32.dll PeekMessageA
extern GetMessage 
import GetMessage user32.dll GetMessageA
extern TranslateMessage 
import TranslateMessage user32.dll
extern DispatchMessage 
import DispatchMessage user32.dll DispatchMessageA
extern ExitProcess
import ExitProcess  kernel32.dll
extern  SetCurrentDirectoryA
import  SetCurrentDirectoryA kernel32.dll
extern  GetCurrentDirectoryA
import  GetCurrentDirectoryA kernel32.dll
extern  WriteFile 
import  WriteFile kernel32.dll
extern GetVersionExA
import GetVersionExA kernel32.dll
extern ShowWindow
import ShowWindow user32.dll
extern UpdateWindow
import UpdateWindow user32.dll
extern ValidateRect
import ValidateRect user32.dll
extern GetStockObject
import GetStockObject gdi32.dll
extern LoadLibrary
import LoadLibrary kernel32.dll LoadLibraryA
extern LoadLibraryEx
import LoadLibraryEx kernel32.dll LoadLibraryExA
extern GetProcAddress 
import GetProcAddress kernel32.dll
extern GetModuleHandleA
import GetModuleHandleA kernel32.dll
extern GetFileSize 
import GetFileSize kernel32.dll
extern GetModuleFileNameA
import GetModuleFileNameA kernel32.dll
 extern GetCommandLineA
import GetCommandLineA kernel32.dll

extern GetCursorPos 
extern WindowFromPoint 
extern GetWindowTextA 
import GetCursorPos user32.dll GetCursorPos 
import WindowFromPoint user32.dll WindowFromPoint 
import GetWindowTextA user32.dll GetWindowTextA 

extern LocalAlloc 
extern LocalFree 
extern CallNextHookEx 
import LocalAlloc kernel32.dll LocalAlloc
import LocalFree kernel32.dll LocalFree
import CallNextHookEx user32.dll CallNextHookEx



extern GetCurrentThread
extern GetCurrentThreadId
extern RegisterSystemThread

import GetCurrentThread  kernel32.dll GetCurrentThread
import GetCurrentThreadId  kernel32.dll GetCurrentThreadId
import RegisterSystemThread user32.dll  RegisterSystemThread

extern SetTimer
import SetTimer user32.dll 


extern GetWindowTextLengthA
import GetWindowTextLengthA user32.dll 

extern EnumWindows
import EnumWindows user32.dll 

extern ShowScrollBar
import ShowScrollBar user32.dll 

extern LoadCursor
import LoadCursor user32.dll LoadCursorA
extern LoadIcon
import LoadIcon user32.dll LoadIconA
extern RegisterClassEx
import RegisterClassEx user32.dll RegisterClassExA
extern CreateWindowEx
import CreateWindowEx user32.dll CreateWindowExA
extern DefWindowProc
import DefWindowProc user32.dll DefWindowProcA
extern PostQuitMessage 
import PostQuitMessage  user32.dll
extern SetCursor 
import SetCursor  user32.dll 
extern CreateFileA
import CreateFileA kernel32.dll
extern ReadFile
import ReadFile kernel32.dll
extern CloseHandle
import CloseHandle kernel32.dll
extern  Sleep 
import  Sleep  kernel32.dll

extern  ExitWindowsEx 
import  ExitWindowsEx user32.dll 

extern   GetSystemTime
import   GetSystemTime kernel32.dll



