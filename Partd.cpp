//Matthew Buehring
//COSC 4328
//12/9/2021
//Dr.King

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtc/type_ptr.hpp>
using namespace std;
//#include "loadShader.hpp"             
//#include <GL/glut.h>
//using namespace glm;
GLuint vertexbuffer;
GLuint colorbuffer;
int randomX, randomY;
bool activegame = false;//game flag
int snakeheadx;
int snakeheady;
//int snakelength;
int directionx;
int directiony;
int applex;//apples coordinates
int appley;
int tailx;//tail coordinates
int taily;
int length=2;
bool readytoplay = false;
float deltax = 2.0f / 40;//gives easier set to how the orgional square is set up
float deltay = 2.0f / 40;
GLint objectToWorldUniformLocation;
static double currenttime = glfwGetTime();//get real time
static double moved = 999;
//float random()//generates random vertices
//{
//	return (2 * (float)rand() / (float)(RAND_MAX)) - 1;
//
//}//generates random colors
//float rando()
//{
//	return ((float)rand() / (float)(RAND_MAX));
//
//}

int applerandom()
{
	return (rand() % 40);//held with in the 40 by 40 grid

}

GLfloat g_vertex_buffer_data[] =//origional buffer set
{
	 // -0.05f,  0.05f, 0.0f,  // Top-left
	 //-0.05f,  0.0f, 0.0f,  // Top-right
	 //0.0f, 0.0f, 0.0f, // Bottom-right

	 // -0.05f,  0.05f, 0.0f,  // Top-left
	 //0.0f,  0.05f, 0.0f,  // Top-right
	 //0.0f, 0.0f, 0.0f, // Bottom-right

		-1, -1, 0,
		-1, -1 + deltay, 0,
		- 1 + deltax, -1, 0,

		- 1, -1 + deltay, 0,
		- 1 + deltax, -1, 0,
		- 1 + deltax, -1 + deltay, 0,


};


int game[40][40]//game board
{
	
};

GLint arrayx[1600]//snake arrays
{
};

GLint arrayy[1600]
{
};



void gamesetup()
{
	
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)//sets all inital board values to empty
		{
			
			game[i][j] = 0;
			

		}
	}

	applex = applerandom();//sets random positions
	appley = applerandom();
	game[20][20] = 1;//snake head
	game[applex][appley] = -1;//apple local;
	directionx = 0;
	directiony = 0;
	snakeheadx = 20;//position
	snakeheady = 20;//position
	tailx = snakeheadx;
	taily = snakeheady;
	length = 2;
	for (int i = 0; i < 1600; i++)//sets to blank values
	{
		arrayx[i] = 0;
		arrayy[i] = 0;
	}
	arrayx[0] = snakeheadx;
	arrayy[0] = snakeheady;
	activegame = true;
	readytoplay = false;
	
}












GLuint LoadShaders(const char* vertex, const char* fragment)//load shader function allows for the shaders to be loaded in from the files
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex, std::ios::in);

	if (VertexShaderStream.is_open())//checks if vertex shader is open then runs accordingly
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else
	{
		printf("Cannot open. Check directory\n", vertex);
		getchar();
		return 0;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment, std::ios::in);
	if (FragmentShaderStream.is_open())//checks if fragment shader is open then runs accordingly
	{
	std::stringstream sstr;
	sstr << FragmentShaderStream.rdbuf();
	FragmentShaderCode = sstr.str();
	FragmentShaderStream.close();

	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Gathering Shader: %s\n", vertex);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);

	}

	printf("Gathering shader : %s\n", fragment);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);


	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);


	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void endgame()//ascii endgame screen
{
	game[7][8] = -1;
	game[8][8] = -1;
	game[9][8] = -1;
	game[10][8] = -1;
	game[16][8] = -1;
	game[21][8] = -1;
	game[27][8] = -1;
	game[28][8] = -1;
	game[29][8] = -1;

	game[7][9] = -1;
	game[16][9] = -1;
	game[17][9] = -1;
	game[21][9] = -1;
	game[27][9] = -1;
	game[30][9] = -1;

	game[7][10] = -1;
	game[16][10] = -1;
	game[17][10] = -1;
	game[18][10] = -1;
	game[21][10] = -1;
	game[27][10] = -1;
	game[31][10] = -1;

	game[7][11] = -1;
	game[8][11] = -1;
	game[9][11] = -1;
	game[16][11] = -1;
	game[18][11] = -1;
	game[19][11] = -1;
	game[21][11] = -1;
	game[27][11] = -1;
	game[31][11] = -1;

	game[7][12] = -1;
	game[16][12] = -1;
	game[19][12] = -1;
	game[20][12] = -1;
	game[21][12] = -1;
	game[27][12] = -1;
	game[31][12] = -1;

	game[7][13] = -1;
	game[16][13] = -1;
	game[20][13] = -1;
	game[21][13] = -1;
	game[27][13] = -1;
	game[30][13] = -1;

	game[7][14] = -1;
	game[8][14] = -1;
	game[9][14] = -1;
	game[10][14] = -1;
	game[16][14] = -1;
	game[21][14] = -1;
	game[27][14] = -1;
	game[28][14] = -1;
	game[29][14] = -1;
		
	
}

void movethegame()
{
	currenttime = glfwGetTime();
	if (currenttime-moved > 0.15)//allow movement every 0.15 seconds
	{
		//game[i][j] = 0;

		moved = glfwGetTime();
		
		int tempx;
		int tempy;

		if (snakeheadx + directionx > 39 || snakeheadx + directionx < 0 || snakeheady + directiony > 39 || snakeheady + directiony < 0)//boundry checks
		{
			endgame();
			activegame = false;
			return;
		}

		if (game[snakeheadx + directionx][snakeheady + directiony] == 1 && readytoplay == true)//see if ya hit yourself
		{
			endgame();
			activegame = false;
			return;
		}

		if (snakeheadx == applex && snakeheady == appley)//collision detection
		{
			arrayx[length] = arrayx[length - 1];
			arrayy[length] = arrayy[length - 1];//prob bug
			length += 1;
			

			do
			{
				tempx = applerandom();//temp values so that the apple can change places
				tempy = applerandom();

			} while (game[tempx][tempy] != 0);
			game[applex][appley] = 1;
			applex = tempx;
			appley = tempy;
			game[applex][appley] = -1;

			
		}
		
		

		

		for (int i = length-1; i > 0; i--)//length of the snake setting
		{
			arrayx[i] = arrayx[i - 1];
			arrayy[i] = arrayy[i - 1];


		}


		game[arrayx[length - 1]][arrayy[length - 1]] = 0;//tail follow

		snakeheadx += directionx;//position
		snakeheady += directiony;//position

		arrayx[0] = snakeheadx;
		arrayy[0] = snakeheady;

		game[snakeheadx][snakeheady] = 1;//head reset

			
		
	}


	

}

int main()
{

	srand(time(0));//seed srand

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear the buffers


	glewExperimental = true;//needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(1920, 1920, "snake game", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	

	//GLint vPosition = glGetAttribLocation(programID, "vPosition");//next few line sets check parts of shader to cause successful link
	//if (vPosition < 0) std::cerr << "couldn't find vPosition in shader\n";
	//glEnableVertexAttribArray(vPosition);
	//glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);//vao

	





	//GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);//sets buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);//binds ther vertexs
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);//sets the vertexs to draw

	//static const GLfloat g_color_buffer_data[] =//origional color set
	//{
	//	0.0f,1.0f,0.0f,
	//	0.0f,1.0f,0.0f,
	//	0.0f,1.0f,0.0f,
	//	0.0f,1.0f,0.0f,
	//	0.0f,1.0f,0.0f,
	//	0.0f,1.0f,0.0f

	//};
	////GLuint colorbuffer;
	//glGenBuffers(1, &colorbuffer); // sets buffer
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);//binds the colors
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);//sets teh colors to fill
	glm::vec3 snakecolor(0, 1, 0);
	glm::vec3 applecolor(1, 0, 0);

	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

	glUseProgram(programID);


	objectToWorldUniformLocation = glGetUniformLocation(programID, "objectToWorld");
	if (objectToWorldUniformLocation < 0) std::cerr << "couldn't find objectToWorld in shader\n";

	GLint colorsUniform = glGetUniformLocation(programID, "colors");
	if (colorsUniform < 0) std::cerr << "couldn't find colors in shader\n";
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	gamesetup();

	moved = glfwGetTime();
	
	do {

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


		for (int i = 0; i < 40; i++)
		{

			for (int j = 0; j < 40; j++)
			{
				if (game[i][j] >= 1)//draw snake
				{


					glm::mat4 gamemove = glm::translate(glm::mat4(1.0f), glm::vec3(i * deltax, j * deltay, 0));
					glUniformMatrix4fv(objectToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(gamemove));
					glUniform3fv(colorsUniform, 1, glm::value_ptr(snakecolor));
					glDrawArrays(GL_TRIANGLES, 0, 6); //draws the actual array for the triangle

				}
				else if (game[i][j] == -1)//draw apple
				{
					glm::mat4 gamemove = glm::translate(glm::mat4(1.0f), glm::vec3(i * deltax, j * deltay, 0));
					glUniformMatrix4fv(objectToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(gamemove));
					glUniform3fv(colorsUniform, 1, glm::value_ptr(applecolor));
					glDrawArrays(GL_TRIANGLES, 0, 6);//draws the actual array for the triangle

				}

			}
		}


		glDisableVertexAttribArray(0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//movements
		{
			directionx = 0;
			directiony = 1;
			//deltax += 0;
			//deltay += 0.0005;
			readytoplay = true;

		}


		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			directionx = -1;
			directiony = 0;
			//deltax += -0.0005;
			//deltay += 0;
			readytoplay = true;

		}


		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			directionx = 1;
			directiony = 0;
			//deltax += 0.0005;
			//deltay += 0;
			readytoplay = true;

		}


		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			directionx = 0;
			directiony = -1;
			//deltax += 0;
			//deltay += -0.0005;
			readytoplay = true;

		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			gamesetup();

		}


		if (activegame == true)
		{			
			movethegame();//allow game to move
			//double for loop						
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}