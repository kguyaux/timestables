#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <iostream>
#include <sstream>
#include <vector>
#include <time.h> // for srand(time(NULL))

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <AL/al.h>
#include "debug.hpp"
#include "camera.hpp"
#include "bloxel.hpp"
#include "text.hpp"
#include "anim_timer.hpp"
#include "language.hpp"


// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_TITLE,
    GAME_WIN,
	GAME_ATTRACT
};

enum PuzzleState {
    PZL_INTRO,
    PZL_INPUT,
    PZL_WRONG,
    PZL_CORRECT,
	PZL_NEXTLEVEL,
	PZL_DIE,
    PZL_GAMEOVER_MESSAGE,
    PZL_SUCCESSIVE_FINISHED
};

enum enumModes {
    successive,
    single,
    randomtable
};


class Game 
{
    public:
		Game(GLuint width, GLuint height);
        ~Game();
		static GLboolean Keys[1024];
		static GLboolean KeysRepeat[1024]; // to prevent keys from repeating, this state is set whenever a number is pressed
		GLuint  Width, Height; // screen dimension
		GameState State;
        bool over = true;
        bool quit = false;
        PuzzleState puzzlestate = PZL_INTRO;
        ALuint introAsource;
        void destroy();
        void setWorldStuff();
		void processTitle(float currentFrame);
		void gamePlay(float currentFrame);
        void processInput(GLFWwindow *window);
		void drawLaserBeam(float currentFrame);
        void gameover(float currentFrame);
        

        int MSTATE = false;
        int TEXRESIZE = false;
        bool PLAYDIE = true;
		
        void init();
        glm::mat4 view;
        glm::mat4 projection;
        float deltaTime; //deprecated?

		// puzzle/sum data:
        struct puzzle {
            std::string notation;
            unsigned int a; // the "table" (1,2,3,4,5,6,7,8,9 or 10)
            unsigned int b; // multiplier
            std::vector<Sprite> sprites;
            std::vector<int> answer_digits;
        } sum;  // `sum` stores the stuff for displaying and solving the questions

        bool sum_over; // if true, then a newsum is generated
		float sum_yPosStart;
		float sum_yPosDie;
		float sum_yPos; // dropping/falling -offset
    	float sum_scale; // scalingfactor for displaying sum
		float sum_xPos; // sum display x position
		float sum_size;

		// imgui tweaking, values are passed/set from sliders in main.cpp
		float imgui_x = 0.0f;
		float imgui_y = 0.0f;
		float imgui_size = 0.0f; // for calculating middle pos; 
        float imgui_color[4] = { 1.0f,1.0f,1.0f,1.0f };
        Camera* camera;
        Bloxel* blox;
        Text* text;

    private:
        Audio* audio;
        Lang* lang;
        void printHudText(float currentFrame);
        void sum_inputfase(puzzle* sum, float currentFrame);
        void sum_wrong(puzzle sum, float currentFrame);
		void sum_nextlevel(puzzle sum, float currentFrame);
        void sum_correct(puzzle* sum, float currentFrame);
        void sum_firstfly(puzzle sum, float currentFrame);
        void sum_die(puzzle sum, float currentFrame);
 		const double pi = 3.1415926535897;
        

        static void subMenuA(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame);
        static void subMenuB(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame);
        static void subMenuC(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame);
        static void chooseDiff(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame);
        typedef void (*SubMenuPointer)(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame);
        SubMenuPointer current_menu_ptr;
        SubMenuPointer old_menu_ptr;

        static void updownSelector(Text* text, Bloxel* blox, float x_offset, float currentFrame, unsigned int* value);

        //void YesNo(Text* text, Bloxel* blox, float currentFrame, selectYesNo* value);

        int correct_till_speedupwarp; // amount of correct ones till speedup
        static unsigned int succesivemode_value;
        static unsigned int singlemode_value;
        static unsigned int difficulty_value;

        bool showFirstMenu;

        puzzle initNewSum();

        float timeOffset; //deprecated,
        int speed; //deprecated, to set falling speed
        bool INTROSPEECH; //deprecated
		bool PAUSE; //deprecated
		bool laserbeamstrobe; // laserbeam-strobe? (strobing does not make is cooler though)
        unsigned int lives; // don't hack this you naughty!!
        unsigned int level; // or this..
        unsigned int correct; // how many correct answers (deprecated)
        int score; // how many correct answers
        unsigned int gamemode_successive_left; // how many has to be solved before next level


        enumModes gameMode;  // what type/mode of game do we play?
        unsigned int sumA[10];  //multipliers for the 10 questions
        int menuswitch_counter = 0; //what selectionmenu to display
        int speedupcounter = 0; // to count tries for speedupwarp in gamemodes random or single

        void decisions_successivemode();
        void decisions_singlemode();
        void decisions_randommode();

        //animationtimers:
        Anim* T_sum_firstfly; //question flies into position
        Anim* T_sum_wait; // falling-rate, stupid name yes
        Anim* T_sum_correct; // exploding letter when correct
        Anim* T_starfield_boost;  //starwars hyperdrive-effect
        Anim* T_sum_explode; // desitegrating digits towards screen
        Anim* T_gameover;  // how long gameover is displayed
        Anim* T_titleForward; // title towards screen
        Anim* T_menushift_left; // selectionmenu-movement to left
        Anim* T_menushift_right; //selectionmenu-movement to the right
        Anim* T_warpmessage_explode; 
        Anim* T_warpmessage_explode_delay;
};

#endif
