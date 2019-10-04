#include "define.h"
#include "Shader.h"
#include "volumen/Volume.h"
#include "camera.h"

// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char *windowTitle = "Tarea 3";
// Window pointer
GLFWwindow *window;

Camera *camera;
// Shader object
Shader *shader, *raycastShader;
// Volume 
Volume *volume;
// Index (GPU) of the geometry buffer
unsigned int VBO;
// Index (GPU) vertex array object
unsigned int VAO;
// Index (GPU) of the texture
unsigned int frameBufferBackFace,textureBackFace;

glm::mat4 projection,model;

bool pressLeft;
int pressMenu;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);

bool createBuffers(){

    glGenFramebuffers(1,&frameBufferBackFace);
    glBindFramebuffer(GL_FRAMEBUFFER,frameBufferBackFace);

    glGenTextures(1,&textureBackFace);
    glBindTexture(GL_TEXTURE_2D,textureBackFace);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,textureBackFace,0);

    GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,buffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){

        std::cout<<"ERROR en creacion de los BUFFERS"<<std::endl;
        return false;

    }

    return true;
}

/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
}
/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
    // Initialize glfw
    glfwInit();
    // Sets the Opegl context to Opengl 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // Creates the window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

    // The window couldn't be created
    if (!window)
    {
        std::cout << "Failed to create the glfw window" << std::endl;
        glfwTerminate(); // Stops the glfw program
        return false;
    }

    // Creates the glfwContext, this has to be made before calling initGlad()
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, onMouseButton);
    // Window resize callback
    glfwSetFramebufferSizeCallback(window, resize);
    return true;
}
/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status)
    {
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}
/**
 * Initialize the opengl context
 * */
void initGL()
{
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildGeometry()
{
    float triangleVertices[] = {
		-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f
    };
    // Creates on GPU the vertex array
    glGenVertexArrays(1, &VAO);
    // Creates on GPU the vertex buffer object
    glGenBuffers(1, &VBO);
    // Binds the vertex array to set all the its properties
    glBindVertexArray(VAO);
    // Sets the buffer geometry data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices, GL_STATIC_DRAW);

    // Sets the vertex attributes
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    //glBindVertexArray(0);
}
/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
    // Initialize the window, and the glad components
    if (!initWindow() || !initGlad() || !createBuffers())
        return false;

    // Initialize the opengl context
    initGL();

    camera = new Camera;

    model = glm::mat4(1.0f);
    // Loads the shader
    shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    raycastShader = new Shader("assets/shaders/basic.vert","assets/shaders/raycast.frag");
    // Loads all the geometry into the GPU
    buildGeometry();
    // Loads the texture into the GPU
    volume = new Volume("volumen/dataSet/bonsai_256x256x256_uint8.raw");

    return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow *window)
{
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		camera->moveForward();

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		camera->moveBackward();

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		camera->strafeLeft();

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		camera->strafeRight();

	}

    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reloads the shader
        delete shader;
        shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    }
}
/**
 * Render Function
 * */

void renderBackFace(){

    glBindFramebuffer(GL_FRAMEBUFFER,frameBufferBackFace);
    // glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    projection = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight,0.001f,100.0f);

    shader->use();

    shader->setMat4("Model",model);
    shader->setMat4("View",camera->getWorlToViewMatrix());
    shader->setMat4("Projection",projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);
    glCullFace(GL_BACK);
}

void render()
{

    
    // Clears the color and depth buffers from the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projection = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight,0.001f,100.0f);
    /** Draws code goes here **/
    // Use the shader
    raycastShader->use();
    raycastShader->setMat4("Model",model);
    raycastShader->setMat4("View",camera->getWorlToViewMatrix());
    raycastShader->setMat4("Projection",projection);
    // Binds the vertex array to be drawn
    // raycastShader->setInt("texVolume",0);
    // raycastShader->setInt("texRender",1);

    raycastShader->setVec2("wSize",glm::vec2(windowWidth,windowHeight));

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(raycastShader->ID, "texRender"), 1);
    glBindTexture(GL_TEXTURE_2D,textureBackFace);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(raycastShader->ID, "texVolume"), 0);
	glBindTexture(GL_TEXTURE_3D, volume->volumeId);


    glBindVertexArray(VAO);
    // Renders the triangle gemotry
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    // Swap the buffer
   
}
/**
 * App main loop
 * */
void update()
{
    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Checks for keyboard inputs
        processKeyboardInput(window);

        renderBackFace();

        // Renders everything
        render();

        glfwSwapBuffers(window);
        // Check and call events
        glfwPollEvents();
    }
}
/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const *argv[])
{
    // Initialize all the app components
    if (!init())
    {
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;

    // Starts the app main loop
    update();

    // Deletes the texture from the gpu
    // Deletes the vertex array from the GPU
    glDeleteVertexArrays(1, &VAO);
    // Deletes the vertex object from the GPU
    glDeleteBuffers(1, &VBO);
    // Destroy the shader
    delete shader;
    delete raycastShader;
    delete volume;
    // Stops the glfw program
    glfwTerminate();

    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (pressLeft) {
		camera->mouseUpdate(glm::vec2(xpos, ypos));
	}

	TwMouseMotion(static_cast<int>(xpos), static_cast<int>(ypos));
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	auto a = action == GLFW_PRESS ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	auto b = TW_MOUSE_LEFT;

	//pressMenu = TwMouseButton(a, b);
	//std::cout << "hola" << std::endl;

	if (a) {

		pressLeft = true;

	}
	else {
		pressLeft = false;
	}

}