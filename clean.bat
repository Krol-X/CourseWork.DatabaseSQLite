@echo off

rd ipch /s /q
rd TEMP /s /q

attrib *.suo -h -s

for /r %%i in (*.o; *.res; *.gch; *.orig; *.layout; *.ilk; *.pdb; *.suo; *.sdf; *_private.*) do (
    del %%i /f
)

cd SRC

del Makefile.win

:: Перемещаем *.exe из SRC\ в BIN\
for /r %%i in (*.exe) do (
    copy /b %%i ..\bin\
    del %%i
)

cd ..\