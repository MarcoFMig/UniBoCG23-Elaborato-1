#define __DEBUG_OUTPUT_

#include <iostream>
#include <optional>
#include <random>
#include <memory>
#include <vector>
#include <string>
#include <list>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <ft2build.h>

#include "inputhandler/simpleinputhandler.hpp"
#include "behaviours/charactercontroller.hpp"
#include "behaviours/gamecontroller.hpp"
#include "shaderloader/shaderloader.hpp"
#include "simplemath/hermiteparser.hpp"
#include "geometry/meshrenderer.hpp"
#include "geometry/textrenderer.hpp"
#include "geometry/label.hpp"
#include "geometry/mesh.hpp"
#include "logger/logger.hpp"
#include "initdata.hpp"
#include "object.hpp"
#include "scene.hpp"

#include "simplemath/simplemath.hpp"
#include "simplemath/hermitecurve.hpp"
#include "behaviours/physics/rectangularcollider.hpp"
#include "exceptions/initializationexception.hpp"

#include FT_FREETYPE_H

#define __HERMITE_CURVE_LENGTH 10
#define __FRAMES_PER_SECOND 60

#define __DEFAULT_CLEAR_COLOR 0.0f, 0.0f, 0.0f, 1.0f

unsigned int frames = 0;
FT_Library freeTypeLib;
FT_Face freeTypeFontFace;
std::random_device randomDev;
std::mt19937 generator(randomDev());
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
//std::list<std::unique_ptr<Behaviour>> behaviourList;
const char* programTitle = "UniBoCG23-Elaborato-1";
ShaderLoader shaderLoader;
ShaderLoader textShaderLoader;
std::shared_ptr<Shader> mainShader;
MeshRenderer* renderer;
TextRenderer* textRenderer;
std::shared_ptr<Scene> scene;
std::list<Label> text;
int currentWindowHeight = DEFAULT_WINDOW_HEIGHT;
int currentWindowWidth = DEFAULT_WINDOW_WIDTH;
int currentMouseX = 0;
int currentMouseY = 0;
float ratioedWindowHeight = 0.0f;
float ratioedWindowWidth = 0.0f;
float windowAspectRatio = (float) DEFAULT_WINDOW_WIDTH / (float) DEFAULT_WINDOW_HEIGHT;
SimpleInputHandler& inputHandler = SimpleInputHandler::getInstance();
glm::mat4 orthoCalc;
int windowId;

enum Phase {
  WORLD = 0,
  TEXT = 1
};
Phase currentPhase;

struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;

GLuint modelShaderValue;
GLuint worldShaderValue;

Shader::UniformVariable timeVariable;
Shader::UniformVariable resolutionVariable;
Shader::UniformVariable phaseVariable;
Shader::UniformVariable projectionVariable;

std::shared_ptr<std::function<void()>> updateFunction = std::make_shared<std::function<void()>>([]() {
  glUniform1f(timeVariable.id, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
});

std::shared_ptr<std::function<void()>> updateWindowSize = std::make_shared<std::function<void()>>([]() {
  glUniform2f(resolutionVariable.id, ratioedWindowWidth, ratioedWindowHeight);
});

std::shared_ptr<std::function<void()>> phaseSelector = std::make_shared<std::function<void()>>([]() {
  glUniform1i(phaseVariable.id, currentPhase);
});

std::shared_ptr<std::function<void()>> orthoCalcUpdate = std::make_shared<std::function<void()>>([]() {
  glUniformMatrix4fv(projectionVariable.id, 1, GL_FALSE, glm::value_ptr(orthoCalc));
});

void updateTimeShader() {
  mainShader.get()->editUniformVariable(updateFunction);
}

// It is imperative that the rendering / ticking order is respected, as it may cause shaders to break
void graphicsLoop() {
  // Pulisce il buffer con il colore impostato all'inizializzazione
  //glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  inputHandler.tick();
  //updateTimeShader();
  //currentPhase = WORLD;
  //printf("%i\n", glGetError());
  renderer->enqueueUniformUpdate(mainShader->getId(), updateFunction);
  renderer->enqueueUniformUpdate(mainShader->getId(), orthoCalcUpdate);
  //renderer->updateProjection(orthoCalc);
  renderer->render();
  scene.get()->tick();
  //currentPhase = TEXT;
  //mainShader.get()->editUniformVariable(updateFunction);
  glClear(GL_DEPTH_BUFFER_BIT);
  textRenderer->renderText(scene->peekText(), orthoCalc);
  frames++;
	glutSwapBuffers();
}

void onWindowSizeChange(int width, int height) {
	orthoCalc = glm::ortho(0.0f, (float) DEFAULT_WINDOW_WIDTH, 0.0f, (float) DEFAULT_WINDOW_HEIGHT);
  scene->updateWorld(orthoCalc);
  float currentAspectRatio = (float) width / (float) height;
  float widthDividedByAspectRatio = (float) width / windowAspectRatio;
	if (windowAspectRatio > currentAspectRatio) {
		glViewport(0, 0, width, widthDividedByAspectRatio);
		ratioedWindowWidth = (float) width;
		ratioedWindowHeight = ratioedWindowWidth / windowAspectRatio;
	}	else {
		glViewport(0, 0, height * windowAspectRatio, height);
		ratioedWindowWidth = (float) height * windowAspectRatio;
		//currentWindowHeight = ratioedWindowWidth;
    ratioedWindowHeight = height;
	}
  Logger::getInstance().log(Logger::Level::fine,
    "Resolution: [W: " + std::to_string(width) + " | H: " + std::to_string(height) + "]");
  //renderer->updateProjection(orthoCalc);
  currentWindowWidth = width;
  currentWindowHeight = height;
  scene->updateWindowData(ratioedWindowHeight, ratioedWindowWidth, currentWindowWidth, currentWindowHeight);
  renderer->enqueueUniformUpdate(mainShader->getId(), orthoCalcUpdate);
  renderer->enqueueUniformUpdate(mainShader->getId(), updateWindowSize);
}

Mesh generateRandomHermiteCurve() {
  std::vector<glm::vec3> vcx;
  std::vector<glm::vec4> colors;
  for (unsigned int index = 0; index < __HERMITE_CURVE_LENGTH; index ++) {
    vcx.push_back(glm::vec3(distribution(generator), distribution(generator), distribution(generator)));
  }
  /*vcx.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
  vcx.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
  vcx.push_back(glm::vec3(0.0f, 0.5f, 0.0f));*/
  HermiteCurve curve(vcx);
  curve.calculate();
  vcx = curve.getCurvePoints();
  printf("[DEBUG] Dumping control Vertexes\n");
  for (std::pair<glm::vec3, HermiteCurve::TCBParams> ctrl : curve.getControlPoints()) {
    #ifdef __DEBUG_OUTPUT_
      Logger::getInstance().log(Logger::Level::fine, std::string("x ") + std::to_string(ctrl.first.x)
        + std::string("y ") + std::to_string(ctrl.first.y)
        + std::string("z ") + std::to_string(ctrl.first.z));
      //printf("[DEBUG] x\t%+f | y\t%+f | z\t%+f\n", ctrl.first.x, ctrl.first.y, ctrl.first.z);
    #endif
  }
  printf("[DEBUG] Dumping Hermite curve Vertexes\n");
  unsigned int currIdx;
  for (glm::vec3 vertex : vcx) {
    #ifdef __DEBUG_OUTPUT_
      printf("[DEBUG] Index: %02u\t| x\t%+f | y\t%+f | z\t%+f\n", currIdx, vertex.x, vertex.y, vertex.z);
    #endif
    colors.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    currIdx++;
  }
  return Mesh(GL_LINE_STRIP, shaderLoader.getProgram(), vcx, colors);
}

void mouseFunction(int button, int state, int x, int y) {

}

void delegateInputDown(unsigned char key, int mouseX, int mouseY) {
  inputHandler.handleKeyDown(key, mouseX, mouseY);
}
void delegateInputUp(unsigned char key, int mouseX, int mouseY) {
  inputHandler.handleKeyUp(key, mouseX, mouseY);
}

bool checkData(std::vector<const char*> filesToCheck,
  std::vector<const char*> alternativeData) {
  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, "Checking essential data availability...");
  #endif
  unsigned int index = 0;
  for (const char* fileName : filesToCheck) {
    std::ifstream readingFile(fileName);
    if (readingFile) {
      #ifdef __DEBUG_OUTPUT_
        Logger::getInstance().log(Logger::Level::fine, "Shader " + std::string(fileName) + ": - Ok!");
      #endif
      readingFile.close();
    } else {
      std::ofstream file(fileName);
      #ifdef __DEBUG_OUTPUT_
        Logger::getInstance().log(Logger::Level::info, "Shader file " + std::string(fileName) + " not present! Generating... ");
      #endif
      std::string code(alternativeData[index]);
      file.write(code.c_str(), code.length());
      file.close();
      #ifdef __DEBUG_OUTPUT_
            Logger::getInstance().log(Logger::Level::fine, "Ok!");
      #endif
      index++;
    }
  }
  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, "Done!");
  #endif
  return false;
}

void setupGl(int argv, char* argc[]) {
  #ifdef __DEBUG_OUTPUT_
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  // Inizializza una finestra data una dimensione
  glutInitWindowSize(currentWindowWidth, currentWindowHeight);
  // Imposta la posizione iniziale della finestra nel desktop
  glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) / 2) - (currentWindowWidth / 2),
    (glutGet(GLUT_SCREEN_HEIGHT) / 2) - (currentWindowHeight / 2));
  // Imposta il titolo della finestra
  windowId = glutCreateWindow(programTitle);
  // Funzione che viene invocata per ogni ciclo di rendering da GLUT
  glutDisplayFunc(graphicsLoop);

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
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, " - Ok!");
  #endif
}

void graphicsLoopProxy(int killScheduler) {
  if (glutGetWindow() != FALSE) {
    glutPostRedisplay();
    glutTimerFunc((unsigned int) 1000/__FRAMES_PER_SECOND, graphicsLoopProxy, 0);
  }
}

void bindListeners() {
  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, "Binding action listeners");
  #endif
  glutReshapeFunc(onWindowSizeChange);
  glutKeyboardFunc(delegateInputDown);
  glutKeyboardUpFunc(delegateInputUp);
  glutMouseFunc(mouseFunction);
  glutTimerFunc((unsigned int) 1000/__FRAMES_PER_SECOND, graphicsLoopProxy, 0);
  inputHandler.registerKeyDownListener(27, [](unsigned char key, int mouseX, int mouseY) {
    glutLeaveMainLoop();
  });
  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, "Ok !");
  #endif
}

void initShaderValues() {
  modelShaderValue = glGetUniformLocation(shaderLoader.getProgram(), "Model");
  worldShaderValue = glGetUniformLocation(shaderLoader.getProgram(), "Projection");
  renderer = new MeshRenderer(modelShaderValue, worldShaderValue, GL_STATIC_DRAW);
  textRenderer = new TextRenderer(textShaderLoader.getProgram(), std::string("eurostile"), std::string("./fonts/eurostile.ttf"));
}

void freeTypeInit() {
  FT_Error operationCode = FT_Init_FreeType(&freeTypeLib);
  if (operationCode) {
    throw InitializationException("Error while initializing module FreeType - init");
  }
  operationCode = FT_New_Face(freeTypeLib, DEFAULT_FONT_LOCATION, 0, &freeTypeFontFace);
  if (operationCode) {
    throw InitializationException("Error while initializing module Freetype - font");
  }
	FT_Set_Pixel_Sizes(freeTypeFontFace, 0, 20);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
// load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++) {
	  // Load character glyph 
		if (FT_Load_Char(freeTypeFontFace, c, FT_LOAD_RENDER)) {
      printf("ERROR::FREETYTPE: Failed to load Glyph \n");
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			freeTypeFontFace->glyph->bitmap.width,
			freeTypeFontFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			freeTypeFontFace->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(freeTypeFontFace->glyph->bitmap.width, freeTypeFontFace->glyph->bitmap.rows),
			glm::ivec2(freeTypeFontFace->glyph->bitmap_left, freeTypeFontFace->glyph->bitmap_top),
			static_cast<unsigned int>(freeTypeFontFace->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
  FT_Done_Face(freeTypeFontFace);
	FT_Done_FreeType(freeTypeLib);
}

void RenderText(int Program, glm::mat4 Projection_text, std::string text, unsigned int VAO_Text, unsigned int VBO_Text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	glUseProgram(Program);
	glUniform3f(glGetUniformLocation(Program, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(Program, "Projection_text"), 1, GL_FALSE, value_ptr(Projection_text));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_Text);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Text);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6)* scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void setupScene() {
  //behaviourList.push_back(std::make_unique<CharacterController>());
  std::vector<glm::vec3> vcx;
  vcx.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
  vcx.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
  vcx.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
  std::vector<glm::vec4> colors;
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  Object tmpObject(shaderLoader.getProgram(), vcx, colors);
  vcx.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
  vcx.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
  vcx.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  Object tmpObjectsnd(shaderLoader.getProgram(), vcx, colors);
  tmpObjectsnd.getScale().x = 500.0f;
  tmpObjectsnd.getScale().y = 500.0f;
  tmpObjectsnd.getPosition().x = 400.0f;
  tmpObjectsnd.getPosition().y = 400.0f;
  std::shared_ptr<Object> objectPtr = std::make_shared<Object>(std::move(tmpObject));
  CharacterController behaviour = CharacterController(objectPtr, scene);
  RectangularCollider sndbehaviour = RectangularCollider(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
  std::shared_ptr<CharacterController> behaviourPtr = std::make_shared<CharacterController>(std::move(behaviour));
  unsigned int objectId = scene.get()->addObject(objectPtr);
  unsigned int sndobjectId = scene.get()->addObject(std::make_shared<Object>(std::move(tmpObjectsnd)));
  scene->attachBehaviour(objectId, std::make_shared<CharacterController>(behaviour));
  scene->attachBehaviour(sndobjectId, std::make_shared<RectangularCollider>(sndbehaviour));
}

void debugActions() {
  HermiteParser parser;
  std::vector<glm::vec3> hermiteVector = parser.readHermite("sottomarino.herm");
  HermiteCurve curveGenerator(hermiteVector);
  curveGenerator.calculate();
  std::vector<glm::vec4> colorVect;
  for (glm::vec3 curvePoint : curveGenerator.getCurvePoints()) {
    colorVect.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  }
  colorVect.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  std::vector<glm::vec3> curve = curveGenerator.getCurvePoints();
  std::vector<glm::vec3> mainCurveVector;
  mainCurveVector.reserve(curve.size() + 1);
  mainCurveVector.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  mainCurveVector.insert(mainCurveVector.end(), curve.begin(), curve.end());
  Object hermiteCurve(GL_TRIANGLE_FAN, shaderLoader.getProgram(), mainCurveVector, colorVect);
  hermiteCurve.getScale().x = 500.0f;
  hermiteCurve.getScale().y = 500.0f;
  hermiteCurve.getPosition().x = 400.0f;
  hermiteCurve.getPosition().y = 400.0f;

  std::shared_ptr<Object> hermiteCurvePtr = std::make_shared<Object>(hermiteCurve);
  scene->addObject(hermiteCurvePtr);
}

void createBackground() {
  std::vector<glm::vec3> vertexes;
  std::vector<glm::vec4> colors;
	vertexes.push_back(glm::vec3(-1.0,-1.0, 0.0));
	vertexes.push_back(glm::vec3(1.0, -1.0, 0.0));
	vertexes.push_back(glm::vec3(-1.0, 1.0, 0.0));
	vertexes.push_back(glm::vec3(1.0, 1.0, 0.0));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  colors.push_back(glm::vec4(distribution(generator), distribution(generator), distribution(generator), 1.0f));
  std::shared_ptr<Object> objectPtr = std::make_shared<Object>(GL_TRIANGLE_STRIP, shaderLoader.getProgram(), vertexes, colors);
  objectPtr->getScale().x = (float) currentWindowWidth;
  objectPtr->getScale().y = (float) currentWindowHeight;
  objectPtr->getPosition().x = (float) currentWindowWidth / 2.0f;
  objectPtr->getPosition().y = (float) currentWindowHeight / 2.0f;
  unsigned int backgroundId = scene->addObject(objectPtr);
  scene->attachBehaviour(objectPtr, std::make_shared<GameController>(objectPtr, scene));
}

void createText() {
  Label label("Esempio", glm::vec3(0.5f, 0.0f, 0.0f));
  label.getPosition().x = 100.0f;
  label.getPosition().y = 600.0f;
  label.getScale().x = 1.0f;
  label.getScale().y = 1.0f;
  label.setColor(glm::vec3(0.5f, 0.0f, 0.0f));
  text.push_back(std::move(label));
}

void bindGameController() {
  std::vector<glm::vec3> vertexes;
  std::vector<glm::vec4> colors;
  std::shared_ptr<Object> objectPtr = std::make_shared<Object>(GL_POINTS, shaderLoader.getProgram(),
    vertexes, colors);
  unsigned int backgroundId = scene->addObject(objectPtr);
  scene->attachBehaviour(objectPtr, std::make_shared<GameController>(objectPtr, scene));
}

void initAndStartGame() {
  //createBackground();
  //createText();
  bindGameController();
}

int main(int argv, char* argc[]) {
  Logger::getInstance().setMinimalInfoLevel(Logger::Level::fine);
  if (checkData(std::vector({VERTEX_SHADER_NAME, FRAGMENT_SHADER_NAME}), std::vector({BASIC_VERTEX_SHADER_CODE, BASIC_FRAGMENT_SHADER_CODE}))) {
    return 1;
  }
  if (checkData(std::vector({VERTEX_TEXT_SHADER_NAME, FRAGMENT_TEXT_SHADER_NAME}), std::vector({BASIC_VERTEX_TEXT_SHADER_CODES, BASIC_FRAGMENT_TEXT_SHADER_CODE}))) {
    return 1;
  }
  int drawmode = GL_TRIANGLES;
  std::vector<glm::vec3> vertexes;
  std::vector<glm::vec4> vertexColors;
  std::vector<unsigned int> connections;
  setupGl(argv, argc);
  initShaderValues();
  //freeTypeInit();
  scene = std::make_shared<Scene>(std::move(Scene(mainShader)));
  onWindowSizeChange(currentWindowWidth, currentWindowHeight);
  bindListeners();
  //setupScene();
  //debugActions();
  initAndStartGame();
  #ifdef __DEBUG_OUTPUT_
    Logger::getInstance().log(Logger::Level::fine, "Engine start");
  #endif
  // Fa partire il main loop di glut
  renderer->prepareBuffers(scene);
  renderer->bindScene(scene);
  Logger::getInstance().log(Logger::Level::fine, "Current FPS cap: " + std::to_string(__FRAMES_PER_SECOND));
  glutMainLoop();

  Logger::getInstance().log(Logger::Level::fine, "Goodbye\n");
  return 0;
}
