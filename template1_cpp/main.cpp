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
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
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

void processPlayerMovement(Player &player, MovementDir &dir, std::vector<std::vector<char>> &charMap, Image &background)
{
  if (Input.keys[GLFW_KEY_W]) {
    player.ProcessInput(MovementDir::UP, charMap, Action::MOVE, background);
    dir = MovementDir::UP;
  } else if (Input.keys[GLFW_KEY_S]) {
    player.ProcessInput(MovementDir::DOWN, charMap, Action::MOVE, background);
    dir = MovementDir::DOWN;
  } if (Input.keys[GLFW_KEY_A]) {
    player.ProcessInput(MovementDir::LEFT, charMap, Action::MOVE, background);
    dir = MovementDir::LEFT;
  } else if (Input.keys[GLFW_KEY_D]) {
    player.ProcessInput(MovementDir::RIGHT, charMap, Action::MOVE, background);
    dir = MovementDir::RIGHT;
  } else if (Input.keys[GLFW_KEY_Q]) {
    player.ProcessInput(MovementDir::DOWN, charMap, Action::OPEN, background);
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
  std::cout << "Stand in front of a chest and press Q to open it  "<< std::endl;
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

  // make the window smaller due to high resolution display
  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH , WINDOW_HEIGHT , "task1 base project", nullptr, nullptr);
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
  Image playerImg("../resources/tile007.png");
  Image dead("../resources/dead.png");
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
  //game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents();

    processPlayerMovement(player, dir, charMap, background);
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
