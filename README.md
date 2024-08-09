# Unibo Computer Graphics - Elaborato 1

## Obiettivi del Progetto\:

* Imparare ad inizializzare una finestra di gioco utilizzando FreeGLUT
* Utilizzare funzioni di disegno di base per creare forme geometriche per rappresentare il giocatore, i nemici e i proiettili
* Implementare la gestione del giocatore e dei nemici
* Gestire le collisioni tra i proiettili del giocatore e i nemici
* Aggiungere uno sfondo animato utilizzando un fragment shader
* Utilizzare la libreria [glm](https://github.com/g-truc/glm) per gestire le animazioni del giocatore e dei proiettili
* Utilizzare curve interpolanti di Hermite per definire la forma dei nemici e del giocatore
* Memorizzare le geometrie del giocatore, dei nemici e dei proiettili su Vertex Buffer Objects (VBOs) per una gestione efficiente dei dati grafici
* Aggiungere funzionalità come punti

### Funzionalità extra
* Aggiungere punti
* Aggiungi una schermata di game over quando il giocatore perde tutte le vite


## How to build the project
### Windows - using Visual Studio Code
#### If MinGW is not installed with required modules
1. Install from here [Mingw (32/64)](https://sourceforge.net/projects/mingw/)
2. Once installed launch MSYS2 Mingw64
3. Run this command `pacman -S mingw-w64-x86_64-toolchain base-devel gcc make mingw-w64-i686-gcc mingw-w64-x86_64-gcc cmake`
4. Install CMake Extension in Visual Studio Code
#### Once MinGW and Visual Studio Code are ready
1. Open project folder
2. Perform a scan of the available kits in the system
3. Select `GCC ... x86_64-w64-mingw32 (mingw64)` as a compiler
4. CTRL + SHIFT + P, configure project TWO times
5. Finally run the project selecting `UniboCG23-Elaborato-1` as target if Visual Studio Code asks
### Windows - using Visual Studio
Unfortunately, as for now Visual Studio's compiler does not support
some of the features of C++23 used in this project and is unable to build the project.

## Screenshots

![ingame screenshot](https://github.com/MarcoFMig/UniBoCG23-Elaborato-1/blob/main/docs/images/ingame.png?raw=true "Ingame Screenshot")
