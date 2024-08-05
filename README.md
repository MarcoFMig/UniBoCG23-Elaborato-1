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
### Requirements
**Windows**:
1. [Mingw (32/64)](https://sourceforge.net/projects/mingw/)
2. [CMake](https://cmake.org/download/) version > 3.5

**Linux**:
TBA

### Build

Use CMake's build task to build the project, if all of the requirements are satisfied you will see an executable with it's required dll's inside the `bin/` folder.
