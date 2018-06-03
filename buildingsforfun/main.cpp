#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <sys/time.h>
#include <iostream>

#include <controls.hpp>

using namespace glm;

GLFWwindow* window; 

int init();
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
void RD_courbe(GLfloat courbe[40][2]);

int main(){
  init();
}

int init(){
  glewExperimental = true; // Needed for core profile
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return -1;
  }
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window); // Initialize GLEW
  glewExperimental=true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(window, 1024/2, 768/2);

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "vshader.txt", "fshader.txt" );
  GLuint matrixID = glGetUniformLocation(programID, "MVP");

  GLfloat g_vertex_buffer_data[151800];

  GLfloat temp_vertex[] = {
    -22.1f, 0.0f, -22.2f,
    -22.0f, 0.0f, -22.0f
  }; 

  GLfloat courbe1[40][2];
  GLfloat courbe2[40][2];  
  RD_courbe(courbe1);
  RD_courbe(courbe2);
  std::cout << "courbe1" << std::endl;
  for (int i = 0; i < 40; i++){
    for (int j = 0; j < 2; j++){   
      std::cout << courbe1[i][j];
      if (j == 1)
        std::cout << "\n";
      else
        std::cout << ", ";
    } 
  }

  std::cout << "courbe2" << std::endl;
  for (int i = 0; i < 40; i++){
    for (int j = 0; j < 2; j++){   
      std::cout << courbe2[i][j];
      if (j == 1)
        std::cout << "\n";
      else
        std::cout << ", ";
    } 
  }

  for (int i = 0; i < 50600; i++){
    if (i%460 < 230){
      g_vertex_buffer_data[(i*3)] = temp_vertex[0] + (0.2 * (i % 230));
      g_vertex_buffer_data[(i*3) + 2] = temp_vertex[2] + (0.4 * (i / 460));
    }
    else{
      g_vertex_buffer_data[(i*3)] = temp_vertex[3] + (0.2 * (i % 230));
      g_vertex_buffer_data[(i*3) + 2] = temp_vertex[5] + (0.4 * (i / 460));
    }
    int n = 0;
    int m = 0;
    while (courbe1[n][1] < g_vertex_buffer_data[i*3])
      n++;
    while (courbe2[m][1] < g_vertex_buffer_data[(i*3) + 2])
      m++;
    //for (int n = 0; courbe1[n][1] < g_vertex_buffer_data[i*3]; n++){
    //  for (int m = 0; courbe2[m][1] < g_vertex_buffer_data[(i*3) + 2]; m++){
    g_vertex_buffer_data[(i*3) + 1] = 
      (courbe1[n][0] * ((g_vertex_buffer_data[i*3] - courbe1[n-1][0]) / (courbe1[n][0] - courbe1[n-1][0]))) *
      (courbe2[m][0] * ((g_vertex_buffer_data[(i*3) + 2] - courbe2[m-1][0]) / (courbe2[m][0] - courbe2[m-1][0])));
    //  }
    //}
  }

  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  GLfloat g_color_buffer_data[151800];

  struct timeval random_color;

  /*for (int i = 0; i < 1518; i++){
    gettimeofday(&random_color, NULL);
    srand(random_color.tv_usec);
    g_color_buffer_data[i] = static_cast<GLfloat>(rand() % 1001) / 1000.0f;
  }*/

  for (int i = 0; i < 50600; i++){
    gettimeofday(&random_color, NULL);
    srand(random_color.tv_usec);  
    GLfloat bw = static_cast<GLfloat>(rand() % 2);
    g_color_buffer_data[(i*3)] = bw;
    g_color_buffer_data[(i*3) + 1] = bw;
    g_color_buffer_data[(i*3) + 2] = bw;
  }

  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

  GLuint indices[12*22*10*100];

  for (int j = 0; j < 100; j++){
    for (int i = 0; i < 220; i++){
      indices[(j * 2640) + (12 * i)] = i + (j * 460);
      indices[(j * 2640) + (12 * i) + 1] = 1 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 2] = 230 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 3] = 1 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 4] = 230 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 5] = 231 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 6] = 230 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 7] = 231 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 8] = 461 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 9] = 230 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 10] = 460 + (i + (j * 460));
      indices[(j * 2640) + (12 * i) + 11] = 461 + (i + (j * 460));
    }
  }
  //0,1,23,  1,23,24  23,24,47  23,46,47

  // Generate a buffer for the indices as well
  GLuint elementbuffer;
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  do{
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use our shader
    glUseProgram(programID);

    // Compute the MVP matrix from keyboard and mouse input
    computeMatricesFromInputs();
    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    // Draw nothing, see you in tutorial 2 !
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
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

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
      GL_TRIANGLES,      // mode
      11*22*12*100,    // count
      GL_UNSIGNED_INT,   // type
      (void*)0           // element array buffer offset
    );

    glDisableVertexAttribArray(0);  
    glDisableVertexAttribArray(1);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
  glDeleteBuffers(1, &elementbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  return 0;
}

void RD_courbe(GLfloat courbe[40][2]){
  courbe[0][1] = -30.0f;
  struct timeval random_y;
  gettimeofday(&random_y, NULL);
  srand(random_y.tv_usec);
  courbe[0][0] = static_cast<GLfloat>(((rand() % 800) - 400) / 100.0f);
  for(int i = 1; courbe[i - 1][1] < 30; i++){
    gettimeofday(&random_y, NULL);
    srand(random_y.tv_usec);
    courbe[i][1] = static_cast<GLfloat>(courbe[i - 1][1] + ((rand() % 30) / 10.0f) + 5);
    gettimeofday(&random_y, NULL);
    srand(random_y.tv_usec);
    courbe[i][0] = static_cast<GLfloat>(((rand() % 800) - 400) / 100.0f);
  }
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open()){
    std::stringstream sstr;
    sstr << VertexShaderStream.rdbuf();
    VertexShaderCode = sstr.str();
    VertexShaderStream.close();
  }else{
    printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
    getchar();
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::stringstream sstr;
    sstr << FragmentShaderStream.rdbuf();
    FragmentShaderCode = sstr.str();
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }



  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> ProgramErrorMessage(InfoLogLength+1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  
  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);
  
  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}
