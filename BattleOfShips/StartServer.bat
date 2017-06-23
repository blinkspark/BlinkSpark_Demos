set ENGINE="E:\GameEngine\UE_4.15\Engine\Binaries\Win64\UE4Editor.exe"
set PROJECT="F:\GitProjects\BlinkSpark_Demos\BattleOfShips\BattleOfShips.uproject"
set MAP="Sandbox"

::%ENGINE% %PROJECT% -game
%ENGINE% %PROJECT% %MAP%?listen -server -log

pause
