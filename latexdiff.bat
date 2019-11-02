@echo off
call :loadconfig
cd /d %~dp1
if "%CFONT%"=="true" (
    latexdiff-vc --git --flatten -t CFONT -r %DIFFHASH% %~nx1
) else (
    latexdiff-vc --git --flatten -r %DIFFHASH% %~nx1
)

call "%~dp0rewrite_diff.exe" %~n1 %DIFFHASH% %REMOVETEXROOT% %BIB% %CFONT% %STYLE% %FADD% %FDEL% %HEAD%

del %~n1-diff%DIFFHASH%.tex

call %TEX% temp.tex
call %TEX% temp.tex
call %DVIPDF% temp.dvi

if exist diff.tex ( del diff.tex )
if exist diff.pdf ( del diff.pdf )

ren temp.tex diff.tex
ren temp.pdf diff.pdf

del temp*
pause

:loadconfig
for /f "usebackq delims== tokens=1,2" %%i in ("%~dp0config") do (
    set KEY=%%i
    if not "!KEY:~0,1!"=="#" (
        set %%i=%%j
    )
)
exit /b
