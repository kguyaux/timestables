// Title, gameoverscreen, constructor, destructor, 
// ..everything non-gamelogic-stuff, that's in the other cppfile

#include "game.hpp"

unsigned int Game::succesivemode_value = 1;
unsigned int Game::singlemode_value = 1;
unsigned int Game::difficulty_value = 3;
GLboolean Game::Keys[1024] = {false};
GLboolean Game::KeysRepeat[1024] = {false}; // to prevent keys from repeating, this state is set whenever a number is pressed

// Constructor
Game::Game(GLuint width, GLuint height)
	:  Width(width), Height(height)
{
    camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	//float lastX = Width / 2.0f;
	//float lastY = Height / 2.0f;
	audio = new Audio();
    lang = new Lang(english);
}

// (re)initialize all game data
void Game::init()
{
    this->over = false; // the game is not over yet

    sum_over = true; // if true, then a newsum is generated
    sum_yPosStart = 16.0;
    sum_yPosDie = -20.0;
    sum_yPos = 0; // dropping/falling -offset
    sum_scale = 0.06f; // scalingfactor for displaying sum
    sum_xPos = 0; // sum display x position
    sum_size = 0;

    // set timers
    T_sum_firstfly = new Anim(1.5f);
    T_sum_wait = new Anim(0.0f);
    T_sum_correct = new Anim(2.0f);
    T_starfield_boost = new Anim(4.0f);
    T_sum_explode = new Anim(2.0f);
    T_gameover = new Anim(8.0f);
    T_titleForward = new Anim(3.0f);
    T_menushift_left = new Anim(0.2f);
    T_menushift_right = new Anim(0.2f);
    T_warpmessage_explode = new Anim(4.0f);
    T_warpmessage_explode_delay = new Anim(1.0f);


    menuswitch_counter = 0;  // set selectionmenu to first one

    //timeOffset = 0.0f;
    PAUSE = false;
    laserbeamstrobe = false;
    level = 1; // should be overridden by menuselection, or random-mode
    //speed = 4;
    lives = 3;
    correct = 0;
    State = GAME_TITLE;
    puzzlestate = PZL_INTRO;
    score = 0;
    gamemode_successive_left = 10;
    correct_till_speedupwarp = 5;
    speedupcounter = 0;
    for (unsigned int i = 0; i < 10; i++) { sumA[i] = i + 1; }
    showFirstMenu = true; //firstmenu means gamemodeselection-menu
}


void Game::updownSelector(Text* text, Bloxel* blox, float x_offset, float currentFrame, unsigned int* value)
{

	if (Keys[GLFW_KEY_UP] && !KeysRepeat[GLFW_KEY_UP]) {
		KeysRepeat[GLFW_KEY_UP] = GL_TRUE;
        ++*value;
    }
	if (Keys[GLFW_KEY_DOWN] && !KeysRepeat[GLFW_KEY_DOWN]) {
		KeysRepeat[GLFW_KEY_DOWN] = GL_TRUE;
        --*value;
    }

    if (*value > 10) *value = 10;
    if (*value < 1) *value = 1;

    TextLine menutext(text, blox);
    menutext.reset();
    menutext.x_move_offset = x_offset;
    menutext.centered = true;
    menutext.solidcolor = 5;
    //up-arrow
    menutext.PutString(174, 0.0f ,-0.6f ,0.02f, currentFrame);
    // value
    menutext.x_move_offset += 0.02f;
    menutext.PutString(std::to_string(*value), 0.14f ,-0.8f ,0.02f, currentFrame);
    // down-arrow
    menutext.x_move_offset -= 0.02f;
    menutext.PutString(173, 0.0f ,-1.0f ,0.02f, currentFrame);
    
}

void Game::subMenuA(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame)
{
    // Display succesive mode menu
    TextLine menutext(text, blox);
    menutext.reset();
    menutext.x_move_offset = x_offset;
    menutext.centered = true;
    menutext.solidcolor = 9;
    menutext.PutString(lang->dict.submenuA_line1, -1.9f ,-0.2f ,0.02f, currentFrame);
    menutext.solidcolor = 12;
    menutext.PutString(lang->dict.submenuA_line2, -1.9f, -0.4f, 0.02f, currentFrame);

    // choose start table:
    updownSelector(text, blox, x_offset, currentFrame, &succesivemode_value);


}


void Game::subMenuB(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame)
{
    // single mode, choose table
    TextLine menutext(text, blox);
    menutext.reset();
    menutext.solidcolor = 9;
    menutext.centered = true;
    menutext.x_move_offset = x_offset;
    menutext.PutString(lang->dict.submenuB_line1, -1.9f + x_offset ,-0.2f ,0.02f, currentFrame);
    menutext.solidcolor = 12;
    menutext.PutString(lang->dict.submenuB_line2, -1.9f, -0.4f, 0.02f, currentFrame);

    // choose table:
    updownSelector(text, blox,  x_offset, currentFrame, &singlemode_value);
}


void Game::subMenuC(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame)
{
    // Random mode
    TextLine menutext(text, blox);
    menutext.reset();
    menutext.centered = true;
    menutext.x_move_offset = x_offset;
    menutext.solidcolor = 9;
    menutext.PutString(lang->dict.submenuC_line1, -1.9f ,-0.5f ,0.02f, currentFrame);
}

void Game::chooseDiff(Text* text, Bloxel* blox, Lang* lang, float x_offset, float currentFrame)
{
    // succesive mode
    TextLine menutext(text, blox);
    menutext.reset();
    menutext.x_move_offset = x_offset;
    menutext.centered = true;
    menutext.solidcolor = 9;
    menutext.PutString(lang->dict.chooseDiff, -1.9f ,-0.4f ,0.02f, currentFrame);
   
    // choose start table:
    updownSelector(text, blox,  x_offset, currentFrame, &difficulty_value);


}

void Game::processTitle(float currentFrame)
{
    //for all of our text in the title-screen, we re-use one textobject, 
    TextLine tl(this->text, this->blox);


    tl.reset();
    tl.centered = true;
    tl.solidcolor = 9;
    tl.PutString(this->lang->dict.main_text1, 0.0f ,0.4f ,0.02f, currentFrame);
    tl.solidcolor = 10;
    tl.wave = true;
    tl.PutString(this->lang->dict.main_text2, 0.0f, 0.15, 0.02f, currentFrame);
    //tl.wave = false;
    //tl.solidcolor = 11;
    //tl.centered = false;
    //tl.PutString(this->lang->dict.main_select1, -1.9f ,-0.2f ,0.02f, currentFrame);
    //tl.solidcolor = 12;
    //tl.PutString(this->lang->dict.main_select2, -1.9f, -0.4f, 0.02f, currentFrame);
    //tl.solidcolor = 14;
    //tl.PutString(this->lang->dict.main_select3, -1.9f, -0.6f, 0.02f, currentFrame);

    tl.centered = true; 
    tl.wave = false;
    tl.solidcolor = 6;
    tl.verticalPalRot = true;
    tl.PutString(this->lang->dict.main_copyright, 0.0f, -1.8f, 0.0125f, currentFrame);
    tl.verticalPalRot = false;
 


    if (showFirstMenu) {
        if (Keys[GLFW_KEY_ESCAPE] && !KeysRepeat[GLFW_KEY_ESCAPE]) {
            // don't need to set Keysrepeat because this is the last esc before quit
            this->quit = true;
        }

        if (Keys[GLFW_KEY_SPACE] && !KeysRepeat[GLFW_KEY_SPACE]) {
			KeysRepeat[GLFW_KEY_SPACE] = GL_TRUE;
            showFirstMenu = false;
        }

        if (Keys[GLFW_KEY_LEFT]) {
            // shift submenu left
            if (T_menushift_left->awaiting && T_menushift_right->awaiting ) {
                T_menushift_left->run(currentFrame);
                menuswitch_counter--;

                // swap the cuurent menu to the old
                old_menu_ptr = current_menu_ptr;
            }
        }
        // left&right arrows
        if (!(T_menushift_left->running || T_menushift_right->running)) {
            tl.verticalPalRot = true;
            tl.centered = false;
            tl.wave = true;
            tl.PutString(176, -1.6f ,-0.4f ,0.06f, currentFrame);
            tl.PutString(175, 2.0f ,-0.4f ,0.06f, currentFrame);
            tl.verticalPalRot = false;
            tl.wave = false;
            tl.centered = true;
        }


        if (Keys[GLFW_KEY_RIGHT]) {
            // shift submenu right
            if (T_menushift_left->awaiting && T_menushift_right->awaiting) {
                T_menushift_right->run(currentFrame);
                menuswitch_counter++;

                // swap the cuurent menu to the old
                old_menu_ptr = current_menu_ptr;
            }
        }
        if (menuswitch_counter < 0) menuswitch_counter = 2;
        if (menuswitch_counter > 2) menuswitch_counter = 0;

        float x_offset = 0;

        // do submenustuff:
        // set the pointer to the right submneu-function
        switch(menuswitch_counter) {
             case 0:
                current_menu_ptr = &Game::subMenuA;
                break;
             case 1:
                current_menu_ptr = &Game::subMenuB;
                break;
             case 2:
                current_menu_ptr = &Game::subMenuC;
                break;
        }

        if (T_menushift_left->running) {
            x_offset = T_menushift_left->countdown_check(currentFrame) * 30;
            if (T_menushift_left->finished) T_menushift_left->reset();
            // move the old menu
            this->old_menu_ptr(this->text, this->blox, this->lang, x_offset -6 , currentFrame);
        }
        if (T_menushift_right->running) {
            x_offset = -6.0 + T_menushift_right->countup_check(currentFrame) * 30;
            // move the old menu
            this->old_menu_ptr(this->text, this->blox, this->lang, x_offset +6 , currentFrame);
            //std::cout << x_offset << std::endl;
            if (T_menushift_right->finished) T_menushift_right->reset();
        }
       
        // set the position of the old menu
        this->current_menu_ptr(this->text, this->blox, this->lang, x_offset, currentFrame);
        if (T_titleForward->awaiting) {
            tl.solidcolor = 2;
            tl.reset();
            tl.centered = true;
            tl.blink = true;
            //tl.setPos_grid(0.0f, 400.0f, 0);
            tl.PutString(this->lang->dict.main_spaceproceed, 0, -1.2, 0.02f, currentFrame);
        } 
    } else {
        // choose difficulty menu
        if (Keys[GLFW_KEY_ESCAPE] && !KeysRepeat[GLFW_KEY_ESCAPE]) {
            KeysRepeat[GLFW_KEY_ESCAPE] = GL_TRUE;
            this->showFirstMenu = true;
        }
        tl.blink = false;
        tl.wave = false;
        tl.verticalPalRot = true;
        switch(menuswitch_counter) {
            case 0:
                tl.PutString(this->lang->dict.submenuA_line1, 0, -0.2, 0.02f, currentFrame);
                gameMode = successive;
                break;
            case 1:
                tl.PutString(this->lang->dict.submenuB_line1, 0, -0.2, 0.02f, currentFrame);
                gameMode = single;
                break;
            case 2:
                tl.PutString(this->lang->dict.submenuC_line1, 0, -0.2, 0.02f, currentFrame);
                gameMode = randomtable;
                break;
        }
        tl.blink = false;
        chooseDiff(this->text, this->blox, this->lang, 0.0, currentFrame);
        // press space    
        if (Keys[GLFW_KEY_SPACE] && !KeysRepeat[GLFW_KEY_SPACE]) {
            KeysRepeat[GLFW_KEY_SPACE] = true;
            // if space is pressed during logo-animation
            if (T_titleForward->running) {
                T_titleForward->stop();
                audio->StopEffect(introAsource);
            }

            if (T_titleForward->awaiting) {
                T_titleForward->run(currentFrame); // start timer

                audio->PlayEffect(1, &introAsource);
                //timeOffset = 0.0f;
            }
        }
        // blinking push space
        if (T_titleForward->awaiting) {
            tl.solidcolor = 2;
            tl.reset();
            tl.centered = true;
            tl.blink = true;
            //tl.setPos_grid(0.0f, 400.0f, 0);
            tl.PutString(this->lang->dict.main_spacestart, 0, -1.2, 0.02f, currentFrame);
        } 
    }


    // finally, draw the logo
    tl.reset();
    tl.verticalPalRot = true;
    tl.flat = false;
    //tl.funky = true;
    tl.rotate = true;
    tl.outlinecolor = glm::vec3(1/255.0* 6.0f, 1/255.0 * 44.0f, 1/255.0 * 37.0f);
    tl.outlinescale = glm::vec3(2.0);
    tl.centered = true;
    tl.outline = true;
    tl.scale = 0.07f;

    if (T_titleForward->running) {
        tl.forwardAnim = T_titleForward;
        tl.forwardanim = true;
        tl.funky = false;
    }

    tl.PutString(this->lang->dict.main_title, 0, 1.3f, 0.07f, currentFrame);
    
    if (T_titleForward->finished) {
        T_titleForward->reset();
        State = GAME_ACTIVE;
        switch(gameMode) {
            case successive:
                level = succesivemode_value;
                break;
            case single:
                level = singlemode_value;
                break;
            case randomtable:
                break;
            break; 
        }
    }

    //if (introAsource != 0) {
    //    ALint audiostate;
    //    int err = audio->get_source_state(introAsource, &audiostate);
    //    if ((err > -1) && (audiostate != AL_PLAYING) ) {
    //        T_titleForward->stop();
    //        T_titleForward->reset();
    //        State = GAME_ACTIVE;
    //    }
    //}
}





void Game::setWorldStuff()
{
	view = this->camera->GetViewMatrix();
	projection = glm::perspective(glm::radians(this->camera->Zoom),
 		       					  (float)this->Width / (float)this->Height,
								  0.1f,
                                  100.0f);
    blox->setWorldStuff(view, projection);
}



void Game::destroy()
{
    delete audio;
	//debug("audio freed");
}


void Game::processInput(GLFWwindow *window)
{
    //if (Keys[GLFW_KEY_W]) camera->ProcessKeyboard(FORWARD, deltaTime);
    //if (Keys[GLFW_KEY_S]) camera->ProcessKeyboard(BACKWARD, deltaTime);
    //if (Keys[GLFW_KEY_A]) camera->ProcessKeyboard(LEFT, deltaTime);
    //if (Keys[GLFW_KEY_D]) camera->ProcessKeyboard(RIGHT, deltaTime);
    if (Keys[GLFW_KEY_L] && !KeysRepeat[GLFW_KEY_L]) {
        KeysRepeat[GLFW_KEY_L] = true;
        this->lang->cycle(); 
    }

    //if (Keys[GLFW_KEY_M]) toggleMouse())
}
/*
void Game::toggleMouse()

{  
    if (MSTATE == false) // M-key pressed down
    {
        MSTATE = true;  // prevent another event
        int mode = glfwGetInputMode(window, GLFW_CURSOR);
        if (mode == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
            std::cout << "Mouse disabled" << std::endl;
            glfwSetCursorPosCallback(window, 0);
        } else {
            firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
            glfwSetCursorPosCallback(window, mouse_callback);
            std::cout << "Mouse enabled " << std::endl;
        }
    }
}
*/
Game::~Game() {}

