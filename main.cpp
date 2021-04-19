#include <iostream>
#include "glad/glad.h";

#include <GLFW/glfw3.h>

void error_callback(int error_code, const char *message)
{
  std::cerr << "GLFW error: " << message;
}

static struct {
  float x1;
  float y1;
  float x2;
  float y2;
} boundaries;

static const char *vertex_shader_src = "#version 110 \n"
                                       "attribute vec2 pos; \n"
                                       "void main() { \n"
                                       "  gl_Position = vec4(pos, 0.0, 1.0); \n"
                                       "}";

// static const char* fragment_shader_src = "#version 110\n"
// " void main()  { \n"
// "   \n"
// "   gl_FragColor = vec4(vec3(0.0,gl_FragCoord.x/800.0,0.0),1.0);"
// " }";

static const char *fragment_shader_src = "#version 110\n"
                                         " uniform vec4 boundaries;"  
                                         " void main()  { \n"
                                         "   float x0, y0,x ,y;\n"
                                         "   x0 = gl_FragCoord.x*((boundaries.z - boundaries.x)/800.0) + boundaries.x ;\n"
                                         "   y0 = gl_FragCoord.y*((boundaries.w - boundaries.y)/800.0) + boundaries.y ;\n"
                                         "   int maxIteration, iteration;\n"
                                         "   maxIteration = 256;\n"
                                         "   iteration = 0;\n"
                                         "   while((x*x + y*y <= 4.0) && (iteration < maxIteration)) {\n"
                                         "      float tmp;\n"
                                         "      tmp = x*x - y*y + x0;\n"
                                         "      y = 2.0*x*y + y0;\n"
                                         "      x = tmp;\n"
                                         "   iteration = iteration + 1;\n"
                                         "   }\n"
                                         "   gl_FragColor = vec4(vec3(0.0,float(iteration)/256.0,0.0),1.0);"
                                         " }";

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS) {
     double xpos, ypos;
     glfwGetCursorPos(window, &xpos, &ypos);

ypos = 600 - ypos;
     double xclick, yclick;
     xclick = xpos*((boundaries.x2 - boundaries.x1)/800.0) + boundaries.x1;
     yclick = ypos*((boundaries.y2 - boundaries.y1)/600.0) + boundaries.y1;
     
     double currentWidth = (boundaries.x2 - boundaries.x1) - (boundaries.x2 - boundaries.x1)/10;
     double currentHeight = (boundaries.y2 - boundaries.y1) - (boundaries.y2 - boundaries.y1)/10;
     boundaries.x1 = (float)( xclick - currentWidth/2);
     boundaries.x2 = (float)( xclick + currentWidth/2);

     boundaries.y1 = (float)( yclick - currentHeight/2);
     boundaries.y2 = (float)( yclick + currentHeight/2);
     std::cout << ypos << " " << xclick << " " << yclick << "\n";
     std::cout << currentHeight << " " << boundaries.x1 << " " << boundaries.y1 << " " << boundaries.x2 << " " << boundaries.y2 << "\n"; 
  }
}

int main(int, char **)
{
  std::cout << "Hello, world!\n";
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
  {
    return -1;
  }

  window = glfwCreateWindow(
      800,
      600,
      "Shader example",
      NULL,
      NULL);

  if (!window)
  {
    glfwTerminate();
    return -1;
  }


  glfwMakeContextCurrent(window);
  gladLoadGL();


   glfwSetMouseButtonCallback(window, mouseCallback);


  // vertices for two triangles forming a square

  GLfloat vertices[] = {
      -1.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, -1.0f,

      1.0f, -1.0f,
      -1.0f, -1.0f,
      -1.0f, 1.0f};

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertices),
      vertices,
      GL_STATIC_DRAW);

  GLint shader_compiled;
  GLuint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compiled);
  if (shader_compiled != GL_TRUE)
  {
    std::cout << "Vertex shader not compiled";
    GLchar message[1023];
    GLsizei log_size;
    glad_glGetShaderInfoLog(vertex_shader, 1024, &log_size, message);
    std::cout << message;
  }

  GLuint fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compiled);
  if (shader_compiled != GL_TRUE)
  {
    std::cout << "Fragment shader not compiled";
    GLchar message[1023];
    GLsizei log_size;
    glad_glGetShaderInfoLog(fragment_shader, 1024, &log_size, message);
    std::cout << message << " END OF MESSAGE";
  }

  GLuint program;
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  GLuint vpos_location;
  vpos_location = glGetAttribLocation(program, "pos");
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(
      vpos_location,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(float) * 2, 0);

  boundaries.x1 = -2.0;
  boundaries.y1 = -1.0;
  boundaries.x2 = 1.0;
  boundaries.y2 = 1.0;

  while (!glfwWindowShouldClose(window))
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    int coordinatesUniformLocation = glGetUniformLocation(program, "boundaries");
    //glUniform4f(coordinatesUniformLocation, -2.0, 1.0, 1.0, -1.0);
    glUniform4f(coordinatesUniformLocation, boundaries.x1, boundaries.y1, boundaries.x2, boundaries.y2);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
