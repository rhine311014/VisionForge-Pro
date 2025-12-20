xcopy /s /y Win32 ..\Bin\Win32
xcopy /s /y x64 ..\Bin\x64
copy /y Win32\SystemInfoTool.exe ..\Bin\Tools
copy /y Win32\HaspRegisterTool.exe ..\Bin\Tools
pause