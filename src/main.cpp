#include <math.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_transform.hpp>

//interfacestuff:
#ifdef TWEAKIMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

#include "framebuffers.hpp"
#include "compositor.hpp"
#include "bloxel.hpp"
#include "text.hpp"
#include "audio.hpp"
#include "game.hpp"

#define GAME_NAME "timestables"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void SetFullScreen(GLFWwindow* window, GLFWmonitor* monitor);
// for testing:
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool TEXRESIZE = false;
bool MSTATE = false;
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 960;
int OLD_WINSIZE_X = 1280;
int OLD_WINSIZE_Y = 960;
int WINPOS_X = 0;
int WINPOS_Y = 0;
bool firstMouse = true;
float deltaTime;
float lastFrame;
float currentFrame;

float lastX;
float lastY;

// Declare the game-instance globally:
Game game = Game(SCR_WIDTH, SCR_HEIGHT);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, GAME_NAME, NULL, NULL);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();     
    if (window == nullptr || monitor == nullptr)
    {
        std::cout << "Failed to create GLFW window/monitor" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

	// Set the backcallers
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    // testing: glfwSetScrollCallback(window, scroll_callback);
    // testing: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

#ifdef TWEAKIMGUI
    // ImGui setup:
    // Do this after glad has been loaded
    const char* glsl_version = "#version 330 core";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
	io.Fonts->AddFontFromFileTTF("resources/Roboto-Medium.ttf", 16.0f);
#endif

    Compositor compositor((const GLchar*)"shaders/screen.fs");
	Buffers buffers(SCR_WIDTH, SCR_HEIGHT);
	game.blox = new Bloxel(); // the handy bloxel-object
	game.text = new Text();
    
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

    // setup renderbuffer
    while (!glfwWindowShouldClose(window))
    {
        if (game.over) game.init(); // re init the game if it's over

        // get timecounter(in Ms)  
        currentFrame = glfwGetTime();
		game.deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;   

		if (TEXRESIZE == true) {
            game.Width = SCR_WIDTH;
            game.Height = SCR_HEIGHT;
			buffers.re_init(SCR_WIDTH, SCR_HEIGHT);	
			TEXRESIZE = false;
		}
	
		if (game.Keys[GLFW_KEY_F] && !game.KeysRepeat[GLFW_KEY_F]) {
			game.KeysRepeat[GLFW_KEY_F] = GL_TRUE;
			SetFullScreen(window, monitor);
		}

        game.processInput(window);
        game.setWorldStuff();
        glBindFramebuffer(GL_FRAMEBUFFER, buffers.ms_FBO[0]);
        glDisable(GL_BLEND); //blending disabled
        glEnable(GL_DEPTH_TEST); // enable depth testing

        
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
          
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game.blox->drawStars(currentFrame);

        glEnable(GL_BLEND); //blending enable
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        switch (game.State)
        {
            case GAME_TITLE:
                game.processTitle(currentFrame);
                break;
            case GAME_ACTIVE:
                game.gamePlay(currentFrame);
                break;
            case GAME_WIN:
				break;
            case GAME_ATTRACT:
				break;
			break;
        }
        buffers.MStoIntermediate_Blit(0); // blit all multisamples to intermediatFBO->screentexture
        // render to final buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        compositor.composit(buffers.screenTexture[0]);

#ifdef TWEAKIMGUI
		// draw imgui stuff 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        ImGui::Begin("things");
            ImGui::SliderFloat("X", &game.imgui_x, 0.0f, 640.0f);
            ImGui::SliderFloat("Y", &game.imgui_y, 0.0f, 480.0f);
            ImGui::SliderFloat("size", &game.imgui_size, 1.0f, 4.0f);
            ImGui::ColorEdit3("color", game.imgui_color);
            ImGui::Text(
                    "Application average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate
                    );
        ImGui::End();
	    // Render dear imgui into screen
		ImGui::Render();
	    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (game.quit) glfwSetWindowShouldClose(window, GL_TRUE);
    }

#ifdef _TWEAK_
	// ImGui opruimen
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif

    game.destroy();
    //delete compositor;
    compositor.destroy();
    // destroy framebuffers
    buffers.destroy();

    glfwTerminate();

    return 0;
}


void SetFullScreen(GLFWwindow* window, GLFWmonitor* monitor)
{
    if ( glfwGetWindowMonitor( window ) == nullptr)
    {
        // backup window position and window size
        glfwGetWindowPos(window, &WINPOS_X, &WINPOS_Y );
        glfwGetWindowSize(window, &OLD_WINSIZE_X, &OLD_WINSIZE_Y);

        // get resolution of monitor
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // switch to full screen
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        // restore last window size and position
        glfwSetWindowMonitor(window, nullptr, WINPOS_X, WINPOS_Y, OLD_WINSIZE_X, OLD_WINSIZE_Y, 0);
    }

}


// GLFWCALLBACK: handle keys pressd
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if  (action == GLFW_PRESS)
            game.Keys[key] = GL_TRUE;
			
        else if (action == GLFW_RELEASE)
            game.Keys[key] = GL_FALSE;
            // prevent 'bouncing', or how it is called
			game.KeysRepeat[key] = GL_FALSE;
    }
}

// GLFWCALLBACK: when main window is resized, this callback is called
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = (unsigned int)width;
    SCR_HEIGHT = (unsigned int)height;
    glViewport(0, 0, width, height);
	TEXRESIZE = true;
}


// GLFW mousewheelscroll-callback.. used for testing..
/*
// GLFWCALLBACK: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    game.camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    game.camera->ProcessMouseScroll(yoffset);
}
*/
