#include <iostream>
#include "pch.h"
#include "GameObject.h"
#include "GraphicDevice.h"
#include "CubeObject.h"
#include "PyramidObject.h"
#include "PrismObject.h"
#include "Camera.h"


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void update(GLFWwindow* window);
void render();
void init();

// Screen dimansion
unsigned int width = 1920, height = 1080;

// App globals
GraphicDevice device;
vector<GameObject*> objects;
Camera* camera;
int obj = 0;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
int space = 1;// regular


int main(int argc, char** argv)
{

    // Initializing window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //should comment this line if we want to show that changng window sizes  doesnt effect on objects 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    init();
   

    while (!glfwWindowShouldClose(window))
    {
        update(window);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void init()
{

    device.SetupDevice();
    camera = new Camera(width, height);
    camera->Setup();

    for (auto object = objects.begin(); object != objects.end(); object++)

    {
        (*object)->Render();
    }
  
}
void selectmode(GLFWwindow* window)
{
    camera->Update(window);
    if (space == 0) {
        (objects.back())->Update(window);
        
       for (auto object = objects.begin(); object != objects.end(); object++){
         
           glm::vec3 forward = camera->GetForward();
           forward.z = -forward.z;
           (objects.back())->SetPosition(camera->GetPosition() + forward);
        }

    }
}


void update(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
  
      selectmode(window);
     if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && space==1) {

         auto cube = new CubeObject(&device);
         cube->Setup();
        // cube->SetPosition(glm::vec3(1.5f, 0.0f, -5.0f));
         objects.push_back(cube);
         space = 0;
     }
     if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && space == 1)
     {
         auto pyramid = new PyramidObject(&device);
         pyramid->Setup();
        // pyramid->SetPosition(glm::vec3(3.0f, 0.0f, -5.0f));
         objects.push_back(pyramid);
         space = 0;
     }

     if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && space == 1)
     {
         auto prism = new PrismObject(&device);
         prism->Setup();
        // prism->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
         objects.push_back(prism);
         space = 0;
     }

     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
     {
         space = 1;
         obj = 0;

     }
    //for (auto object = objects.begin(); object != objects.end(); object++)
    //{
    //    (*object)->Update(window);
  
    //
    //}
}



void render()
{
    unsigned int projLoc = glGetUniformLocation(device.GetShader()->ID, "projection");
    unsigned int viewLoc = glGetUniformLocation(device.GetShader()->ID, "view");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    device.Render(&objects);
    
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{   
    glViewport(0, 0, w, h);
    camera->SetViewPortSize(w, h, true);
    width = w;
    height = h;
}

