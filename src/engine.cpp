#include "engine.hpp"

#include <gl/glew.h>
#include <gl/freeglut.h>

#include "initdata.hpp"

void Engine::setupGl(int argv, char* argc[]) {
  /*#ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, "Initializing OpenGL");
  #endif
  // Inizializza glut con i parametri della riga di comando
  glutInit(&argv, argc);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  // Imposta la versione di OpenGL
  glutInitContextVersion(4, 0);
  // Seleziona il profilo standard per il contesto di OpenGL
  glutInitContextProfile(GLUT_CORE_PROFILE);
  /* Configurazione di GLUT usando le varie impostazioni con parametri
  *  GLUT_DOUBLE usa una feature che permette di usare un secondo buffer
  *  il primo mostra a video il risultato, il secondo viene elaborato e una
  *  volta elaborato viene sostituito al primo e così ricomincia il ciclo
  *  GLUT_RGBA Imposta la modalità di colori della finestra con un canale Rosso Verde Blu e Alpha
  *  GLUT_DEPTH Rappresenta una finestra con un buffer di profondità, (l'asse Z)
  */
  /*
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  // Inizializza una finestra data una dimensione
  glutInitWindowSize(this->context.getWindow()->currentWidth,
    this->context.getWindow()->currentHeight);
  // Imposta la posizione iniziale della finestra nel desktop
  glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) / 2) 
      - (this->context.getWindow()->currentWidth / 2),
    (glutGet(GLUT_SCREEN_HEIGHT) / 2)
       - (this->context.getWindow()->currentHeight / 2));
  // Imposta il titolo della finestra
  this->context.getWindow()->id =
    glutCreateWindow(this->context.getWindow()->title);
  // Funzione che viene invocata per ogni ciclo di rendering da GLUT
  glutDisplayFunc(Engine::graphicsLoopCallback);

  glutSetKeyRepeat(false);
  // Abilita le feature sperimentali di OpenGL
  glewExperimental = GL_TRUE;
  // Inizializza GLEW
  glewInit();

  // Abilita il blending dei colori di elementi sovrapposti
  glEnable(GL_BLEND);
  glLineWidth(1.0f);
  glPointSize(1.0f);
  glClearColor(__DEFAULT_CLEAR_COLOR);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shaderLoader.readShadersFromFile(VERTEX_SHADER_NAME);
  shaderLoader.readShadersFromFile(FRAGMENT_SHADER_NAME);
  shaderLoader.compileShaders("Main");
  //printf("Mainshader %i\n", shaderLoader.getProgram());

  textShaderLoader.readShadersFromFile("text.vert");
  textShaderLoader.readShadersFromFile("text.frag");
  textShaderLoader.compileShaders("Text");

  Shader compiledShader = shaderLoader.getShader();
  mainShader = std::make_shared<Shader>(std::move(compiledShader));
  std::string timeVariableName = "time";
  timeVariable = mainShader.get()->queryUniform(timeVariableName);
  std::string resolutionVariableName = "resolution";
  resolutionVariable = mainShader.get()->queryUniform(resolutionVariableName);
  std::string phaseVariableName = "phase";
  phaseVariable = mainShader.get()->queryUniform(phaseVariableName);
  std::string projectionVariableName = "Projection";
  projectionVariable = mainShader.get()->queryUniform(projectionVariableName);

  // Abilita la vista del wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, " - Ok!");
  #endif*/
}

Engine& Engine::getInstance() {
  static Engine instance;
  return instance;
}

void Engine::graphicsLoopCallback() {
  Engine::getInstance().graphicsLoop();
}

void Engine::graphicsLoop() {
}
