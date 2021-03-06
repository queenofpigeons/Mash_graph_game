#include "common.h"
#include "Image.h"
#include "Player.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include <unistd.h>

constexpr GLsizei WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 1280;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашяим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player, MovementDir &dir, std::vector<std::vector<char>> &charMap, Image &background, Image &screen)
{
  if (Input.keys[GLFW_KEY_W]) {
    player.ProcessInput(MovementDir::UP, charMap, Action::MOVE, background, screen);
    dir = MovementDir::UP;
  } else if (Input.keys[GLFW_KEY_S]) {
    player.ProcessInput(MovementDir::DOWN, charMap, Action::MOVE, background, screen);
    dir = MovementDir::DOWN;
  } if (Input.keys[GLFW_KEY_A]) {
    player.ProcessInput(MovementDir::LEFT, charMap, Action::MOVE, background, screen);
    dir = MovementDir::LEFT;
  } else if (Input.keys[GLFW_KEY_D]) {
    player.ProcessInput(MovementDir::RIGHT, charMap, Action::MOVE, background, screen);
    dir = MovementDir::RIGHT;
  } else if (Input.keys[GLFW_KEY_E]) {
    player.ProcessInput(dir, charMap, Action::OPEN, background, screen);
  }
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "Stand in front of a chest and press E to open it  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void fadeToBlack(Image &screen, const std::string &text, GLFWwindow* window) {
  int fade_time = 100;
  Image fading_screen(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  for (int k = 0; k < fade_time; k++) {
    for (int i = 0; i < WINDOW_WIDTH ; i++) {
      for (int j = 0; j < WINDOW_HEIGHT; j++) {
        Pixel p = screen.GetPixel(i, j);
        int coef = 1 - 1 / fade_time * k;
        p.r *= coef;
        p.g *= coef;
        p.b *= coef;
        p.a *= coef;
        fading_screen.PutPixel(i, j, p);
      }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, fading_screen.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
  }
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "epic gamer moment", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};

	Image img("../resources/tex.png");
  Image level2("../resources/level2.png");
  Image dead("../resources/dead.png");
  Image GG("../resources/gg.png");
	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  Image background(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  int starting_x, starting_y;
  std::vector<std::vector<char>> charMap(lvlHeight, std::vector<char>(lvlWidth));

  initLevel("../resources/lvl1.txt", background, starting_x, starting_y, charMap);
  for (int i = 0; i < background.Width() ; i++) {
    for (int j = 0; j < background.Height(); j++) {
      screenBuffer.PutPixel(i, j, background.GetPixel(i, j));
    }
  }

  Point starting_pos{.x = starting_x, .y = starting_y};
  Player player{starting_pos};
  player.Active = true;

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

  MovementDir dir = MovementDir::DOWN;
  int t = 0;
  //game loop
	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents();

    processPlayerMovement(player, dir, charMap, background, screenBuffer);
    if (player.Dead) {
      for(int y_draw = 0; y_draw < dead.Height(); ++y_draw)
      {
        for(int x_draw = 0; x_draw < dead.Width(); ++x_draw)
        {
          int new_x = WINDOW_WIDTH / 2 - dead.Width() / 2 + x_draw;
          int new_y = WINDOW_HEIGHT/2 - dead.Height() / 2 + y_draw;
          screenBuffer.PutPixel(new_x, new_y, blend(screenBuffer.GetPixel(new_x, new_y), dead.GetPixel(x_draw, dead.Height() - y_draw)));
        }
      }
    }
    player.Draw(screenBuffer, background, dir);

    if (player.Level == Level1Fade || player.Level == Level2Fade) {
      player.ChangeLvl = true;
      for (int i = 0; i < background.Width() ; i++) {
        for (int j = 1; j <= background.Height(); j++) {
          Pixel p = background.GetPixel(i, j);
          p.b = p.b - p.b / fade_frames * t;
          p.r = p.r - p.r / fade_frames * t;
          p.g = p.g - p.g / fade_frames * t;
          screenBuffer.PutPixel(i, j, p);
        }
      }
    }

    if (player.Level == Level2Picture) {
      for (int i = 0; i < background.Width() ; i++) {
        for (int j = 1; j <= background.Height(); j++) {
          screenBuffer.PutPixel(i, background.Height() - j, level2.GetPixel(i, j));
        }
      }
      initLevel("../resources/lvl2.txt", background, starting_x, starting_y, charMap);
    }

    if (player.Level == Level2Transition) {
      for (int i = 0; i < background.Width() ; i++) {
        for (int j = 1; j <= background.Height(); j++) {
          screenBuffer.PutPixel(i, j, background.GetPixel(i, j));
        }
      }
      player.SetStartingPosition(starting_x, starting_y);
      player.Level++;
      player.ChangeLvl = false;
    }

    if (player.Level == GameEnd) {
      player.ChangeLvl = true;
      for (int i = 0; i < background.Width() ; i++) {
        for (int j = 1; j <= background.Height(); j++) {
          screenBuffer.PutPixel(i, background.Height() - j, GG.GetPixel(i, j));
        }
      }
    }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window);

    if (player.Level == Level2Picture) {
      double startingTime = glfwGetTime();
      while(1) {
        double currentTime = glfwGetTime();
        if (currentTime - startingTime > 2)
          break;
      }
      player.Level++;
    }
    if (player.Level == Level1Fade || player.Level == Level2Fade) {
      t++;
      double startingTime = glfwGetTime();
      while(1) {
        double currentTime = glfwGetTime();
        if (currentTime - startingTime > 5 / fade_frames)
          break;
      }
      if (t == fade_frames || t == fade_frames * 10) {
        //t = 0;
        player.Level++;
      }
    }
	}

	glfwTerminate();
	return 0;
}
