## <h2> Welcome to Doodle Jump!!!
  
  ## <h2> In order to install the software needed to play the game, follow the steps below.
  
  ![DoodleJump](doodle.PNG)
  
  * On your Linux terminal, run the command ```sudo apt-get install libsfml-dev ``` this will install the SFML Library
  * After the install, use the command ```git clone https://github.com/cbayley96/games.git ``` to clone into the repository
  * Use the cd command to change into the games directory, by using ```cd games```
  * After changing into the games directory, use the command ```cd 02\ Doodle\ Jump/``` to change into the Doodle Jump directory
  * Once in the directory, use the command ```g++ -std=c++11 -c main.cpp``` to compile the game
  * After compiling, use ```g++ main.o -o main.exe -lsfml-graphics -lsfml-window -lsfml-system```
