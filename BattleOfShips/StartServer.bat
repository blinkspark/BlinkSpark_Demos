set ENGINE="E:\GameEngine\Epic Games\4.12\Engine\Binaries\Win64\UE4Editor.exe"
set PROJECT="F:\Dev\Projects\SLG\SLG.uproject"
set MAP="Test"

::%ENGINE% %PROJECT% -game
%ENGINE% %PROJECT% %MAP%?listen -server -log

pause
