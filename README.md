# mini-jam-game

## Team Members:

### RV Nayan | Shubham Daule | Gaddey Hemanth Chowdary

## How to run the above:

If you're using linux and have installed sfml library(via pacman) or libsfml-dev(via apt), navigate to this folder and run:

"g++ -c main.cpp && g++ -c spike.cpp && g++ -c MainMenu.cpp && g++ -c Minion.cpp && g++ -c map.cpp && g++ -c wire.cpp && g++ -c portal.cpp && g++ main.o MainMenu.o portal.o spike.o Minion.o map.o wire.o -o SuperHard -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system && ./SuperHard"
