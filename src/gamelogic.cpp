#include "game.hpp"


// successive_gamemode decisions 
void Game::decisions_successivemode()
{
    if (gamemode_successive_left == 0 && sum_over) {
        // goto next level
            if (level < 10) { level++; } else {
                level = 1;
                difficulty_value++;
                // maybe for later...
                //puzzlestate = PZL_SUCCESSIVE_FINISHED;
            };
            gamemode_successive_left = 10;
            for (unsigned int i = 0; i < 10; i++) { sumA[i] = i + 1; } // init new multiplier-array to be diminished per turn
        }

        if (sum_over && gamemode_successive_left != 0) {
            if (puzzlestate != PZL_NEXTLEVEL) {
                this->sum = initNewSum(); // init new puzzle values and data
                gamemode_successive_left--;
            }

        }
    }


    void Game::decisions_randommode()
    {
        if (sum_over && speedupcounter >= correct_till_speedupwarp) {
            speedupcounter = 0;
            difficulty_value++;
        }
        if (sum_over) {
            this->sum = initNewSum();
    }
}		


void Game::decisions_singlemode()
{
    if (sum_over && speedupcounter >= correct_till_speedupwarp) {
        difficulty_value++;
        speedupcounter = 0;
    }
    if (sum_over) {
        this->sum = initNewSum();
    }
}

void Game::printHudText(float currentFrame)
{
    TextLine gametxt(this->text, this->blox); 
    // print hud text
    gametxt.reset();
    gametxt.shadow = true;
    gametxt.shadowcolor = 0;
    gametxt.flat = true;
    float size = 0.02f;
    gametxt.stretch = true;
	gametxt.solidcolor = 12;
    // bgrectangle
    gametxt.stretchfactor = glm::vec3(-50.0f, 1.2f, 1.0f); 
    gametxt.PutString(187, -3.2f, 1.5697, size, currentFrame);
    //countdownrectangle
	gametxt.solidcolor = 5;
    float sf = 0;
    switch(gameMode) {
        case successive:
            sf = -18.0f * ((float)gamemode_successive_left / 10.0f);
            break;
        case single:
            sf = -18.0f * (1.0f - (float)speedupcounter / ((float)correct_till_speedupwarp));
            break;
        case randomtable:
            sf = -18.0f * (1.0f - (float)speedupcounter / ((float)correct_till_speedupwarp));
            break;
    }
    gametxt.stretchfactor = glm::vec3(sf, 1.0f, 1.0f); 
    gametxt.PutString(187, -0.895f, 1.3497, size, currentFrame);
    gametxt.stretch = false;

    // lives
    std::stringstream lives_str;
    lives_str << lives;
	gametxt.solidcolor = 4;
    float length = (this->lang->dict.lives.size() + 1 ) * size * 5;
    gametxt.PutString(this->lang->dict.lives, -1.995, 1.5497, size, currentFrame);
	gametxt.solidcolor = 6;
    gametxt.PutString(lives_str.str(), -1.995 + length, 1.5497, size, currentFrame);

    // level
    std::stringstream level_str;
    level_str << this->difficulty_value;
	gametxt.solidcolor = 12;
    length = (this->lang->dict.speed.size() + 1 ) * size * 5;
    gametxt.PutString(this->lang->dict.speed, -0.495, 1.5497, size, currentFrame);
	gametxt.solidcolor = 9;
    gametxt.PutString(level_str.str(), -0.495 + length, 1.5497, size, currentFrame);

    //score
    std::stringstream score_str;
    score_str << this->score;
	gametxt.solidcolor = 11;
    gametxt.PutString(this->lang->dict.score, 0.795, 1.5497, size, currentFrame);
	gametxt.solidcolor = 13;
    length = (this->lang->dict.score.size() + 1 ) * size * 5;
    gametxt.PutString(score_str.str(), 0.795 + length, 1.5497, size, currentFrame);
   
    //countdownmetertext 
	gametxt.solidcolor = 2;
    gametxt.PutString(this->lang->dict.countdown, -1.995, 1.3497, size, currentFrame);
}


// this is the game-loop,. You know, when you're actually playing
void Game::gamePlay(float currentFrame)
{
    
    if (Keys[GLFW_KEY_ESCAPE] && !KeysRepeat[GLFW_KEY_ESCAPE]) {
        KeysRepeat[GLFW_KEY_ESCAPE] = GL_TRUE;
        this->over = true;
    }


    switch(gameMode) {
        case successive:
            decisions_successivemode();
            break;
        case single:
            decisions_singlemode();
            break;

        case randomtable:
            decisions_randommode();
            break;
    }

    // draw laserbeam or not
	if (puzzlestate != PZL_NEXTLEVEL) drawLaserBeam(currentFrame);
    
    if (lives == 0) puzzlestate = PZL_GAMEOVER_MESSAGE;
   
    if (puzzlestate != PZL_NEXTLEVEL) printHudText(currentFrame);

    // select the fase of the gameplay
    switch (puzzlestate) {
        case PZL_INTRO:
            sum_firstfly(sum, currentFrame);
            break;
    
        case PZL_INPUT: // game inputfase, where you enter the solution
            sum_inputfase(&sum, currentFrame); // now pass `sum` as pointer because value needs change
            break;
    
        case PZL_CORRECT:
            sum_correct(&sum, currentFrame);
            break;
    
        case PZL_NEXTLEVEL:
            sum_nextlevel(sum, currentFrame);
            break;
    
        case PZL_WRONG:
            sum_wrong(sum, currentFrame);
            break;

        case PZL_DIE:
            sum_die(sum, currentFrame);
            break;

        case PZL_GAMEOVER_MESSAGE:
            gameover(currentFrame);
            break;

        case PZL_SUCCESSIVE_FINISHED:
            // TODO
            // should have some nice congratulation-message
            break;
        break; 
    }
}

Game::puzzle Game::initNewSum()
{
    Game::puzzle _sum;
    std::srand(time(NULL));
    unsigned int pos;
    switch(gameMode) {
        case successive:
            _sum.a = level;
            pos = std::rand() % (gamemode_successive_left > 0 ? gamemode_successive_left : 1);
            _sum.b = sumA[pos];
            sumA[pos] = sumA[(gamemode_successive_left > 0 ? gamemode_successive_left : 1) - 1];

#ifdef TT_DEBUG
            std::cout << "TABLE: " <<  _sum.b  << std::endl;
            for (unsigned int i=0; i < 10; i++) std::cout << sumA[i] << " ";
            std::cout  << std::endl;
#endif 
            break;

        case randomtable:
            _sum.a = std::rand() % 10 + 1;
            _sum.b = std::rand() % 10 + 1;
            break;

        case single:
            _sum.a = level;
            _sum.b = std::rand() % 10 + 1;
            break;
        break;
    }
    char buff[32];
    snprintf(buff, sizeof(buff), "%d x %d = ", _sum.a, _sum.b);
    _sum.notation = buff;

	// get sprite characters
    for (char& c: _sum.notation ) {
        Sprite spr = text->charSprites[text->charmapper(c)];
        spr.name = c;
        _sum.sprites.push_back(spr);
#ifdef TT_DEBUG
        std::cout << c;
#endif
    }

#ifdef TT_DEBUG
    std::cout << std::endl; // endline for the debug
#endif

    sum_over = false; 
	sum_yPos = 0.0f; // the initial Position
	sum_size = (float)(_sum.sprites.size()*6);
	sum_xPos = 6.0 - sum_size/2.0;
    return _sum;
}


void Game::sum_firstfly(puzzle sum, float currentFrame)
// flying into screen
{
	if (T_sum_firstfly->awaiting) {
		T_sum_firstfly->run(currentFrame);
		ALuint soundsrc;
		audio->PlayEffect(4, &soundsrc);
	}
    float tdelta = T_sum_firstfly->countdown_check(currentFrame);
    for (unsigned int i=0; i < sum.sprites.size(); i++) {
		Sprite spr = sum.sprites[i];
		spr.scale(glm::vec3(sum_scale));
		// - static_cast<float>(i)/2.0 );
		float zzz = std::pow(tdelta,4.0)*30;
		spr.move(glm::vec3(sum_xPos + i*6, sum_yPosStart, zzz));
		spr.move(glm::vec3(0.0, -4.0, 0.0));
		spr.rotate(zzz, glm::vec3(1.0, 0.0, 0.0));
		spr.move(glm::vec3(0.0, 4.0, 0.0));
        spr.verticalPalRotate(currentFrame);
		blox->drawSprite(&spr);
    }
    if (T_sum_firstfly->finished) {
		T_sum_firstfly->reset();
		puzzlestate = PZL_INPUT;
	}
}


void Game::sum_inputfase(puzzle* sum, float currentFrame) {
    if (T_sum_wait->awaiting) {
        T_sum_wait->duration = 10.0f / difficulty_value;  // the falling-speed
        T_sum_wait->run(currentFrame);
    }

	float ystep = (static_cast<float>(sum_yPosStart - sum_yPosDie) / static_cast<float>(T_sum_wait->duration) );
	sum_yPos = static_cast<float>(sum_yPosStart) - (ystep * T_sum_wait->countup_check(currentFrame));

    // float tdelta = T_sum_wait.countup_check(currentFrame);
    for (unsigned int i=0; i < sum->sprites.size(); i++) {
		Sprite spr = sum->sprites[i];
        spr.scale(glm::vec3(sum_scale));
        spr.move(glm::vec3(sum_xPos + i*6, sum_yPos, 0.0f));
        spr.verticalPalRotate(currentFrame);
        blox->drawSprite(&spr);
    }

	//check numberspressed 
	int pressed = -1;
	for (int i=0; i < 10; i++) {
		if (this->Keys[GLFW_KEY_0 + i] && !this->KeysRepeat[GLFW_KEY_0+i]) {
			pressed = i;
			this->KeysRepeat[GLFW_KEY_0 +i] = GL_TRUE;
			break;
		} 
	}

	if (this->Keys[GLFW_KEY_BACKSPACE] && !this->KeysRepeat[GLFW_KEY_BACKSPACE] && sum->answer_digits.size() >0) {
		this->KeysRepeat[GLFW_KEY_BACKSPACE] = GL_TRUE;
		sum->answer_digits.pop_back();
        sum->sprites.pop_back();
        sum_size = (float)(sum->sprites.size()*6);
        sum_xPos = 6.0 - sum_size/2.0;
	}

	if (sum->answer_digits.size() < 3 && pressed > -1) {
        Sprite spr = text->charSprites[text->charmapper(static_cast<char>(pressed+48))];
        std::stringstream ss;
        ss << pressed; 
        spr.name = ss.str();
        sum->sprites.push_back(spr); // push sprite on the spriteslist
		sum->answer_digits.push_back(pressed);
        sum_size = (float)(sum->sprites.size()*6);
        sum_xPos = 6.0 - sum_size/2.0;
		
	}
		
    if (Keys[GLFW_KEY_ENTER] && !this->KeysRepeat[GLFW_KEY_ENTER]) {
		this->KeysRepeat[GLFW_KEY_ENTER] = GL_TRUE;
		unsigned int product = 0;
		for (unsigned int i=0; i < sum->answer_digits.size(); i++) {
			product += std::pow(10, sum->answer_digits.size() - i-1) * sum->answer_digits[i];
		}
		if (product == (sum->a * sum->b)) {
            correct ++;
    		this->score = correct * difficulty_value;
            if (gameMode != successive) speedupcounter++;
            switch(gameMode) {
                case successive:
                   
                    if (gamemode_successive_left == 0) {
                        puzzlestate = PZL_NEXTLEVEL;
                    } else {
			            puzzlestate = PZL_CORRECT;
                    }
                    break;
                case single:
                    if (speedupcounter < correct_till_speedupwarp) {
                        puzzlestate = PZL_CORRECT;
                    } else {
                        puzzlestate = PZL_NEXTLEVEL;
                    }
                    break;
                case randomtable:
                    if (speedupcounter < correct_till_speedupwarp) {
                        puzzlestate = PZL_CORRECT;
                    } else {
                        puzzlestate = PZL_NEXTLEVEL;
                    }
                    break;
            }
			T_sum_wait->reset();
		} else {
			puzzlestate = PZL_WRONG;
		}
    }

	if (T_sum_wait->finished) {
		T_sum_wait->reset();
		puzzlestate = PZL_DIE;
	}
}


void Game::sum_die(puzzle sum, float currentFrame)
{
    if (T_sum_explode->awaiting) {
        T_sum_explode->run(currentFrame);
		ALuint soundsrc;
		audio->PlayEffect(7, &soundsrc);
	}
	this->laserbeamstrobe = true;	
	float tdelta = T_sum_explode->countdown_check(currentFrame);
    for (unsigned int i=0; i < sum.sprites.size(); i++) {
		Sprite spr = sum.sprites[i];
        spr.scale(glm::vec3(sum_scale));

        spr.move(glm::vec3(sum_xPos + i*6, sum_yPos-((T_sum_explode->duration - tdelta)*2), 0.0f));
		spr.desintegrate(tdelta);
        blox->drawSprite(&spr);
    }

	if (T_sum_explode->finished) {
		puzzlestate = PZL_INTRO;
		T_sum_explode->reset();
		this->sum_over = true;
        lives--;
		this->laserbeamstrobe = false;	
        if (gameMode == successive && gamemode_successive_left == 0) {
            puzzlestate = PZL_NEXTLEVEL;
        }
	}
}

void Game::sum_wrong(puzzle sum, float currentFrame) {
	ALuint soundsrc;
	audio->PlayEffect(6, &soundsrc);
    this->puzzlestate = PZL_INPUT;
}


void Game::gameover(float currentFrame)
{
    // play game-over animation:
    if (T_gameover->awaiting) {
        T_gameover->run(currentFrame);
    }
	TextLine menutext(text, blox);
    menutext.reset();
    menutext.centered = true;
	menutext.solidcolor = 12;
    menutext.PutString(this->lang->dict.yourscore, 0.0f, 0.8f, 0.03f, currentFrame);
    std::stringstream score_str;
    score_str << score;
	menutext.solidcolor = 13;
    menutext.PutString(score_str.str(), 0.0f ,0.4f ,0.03f, currentFrame);
    menutext.solidcolor = 12;
	menutext.flat = false;
	menutext.outlinecolor = glm::vec3(0.0f, 0.0f, 0.9f);
    menutext.outlinescale = glm::vec3(2.0);
	menutext.outline = true;
	menutext.rotate = true;
    menutext.PutString("GAME OVER", 0.0f ,0.0f ,0.06f, currentFrame);

    if (Keys[GLFW_KEY_ENTER] && !this->KeysRepeat[GLFW_KEY_ENTER]) {
		this->KeysRepeat[GLFW_KEY_ENTER] = GL_TRUE;
		this->over = true;
	}
    if (Keys[GLFW_KEY_SPACE] && !this->KeysRepeat[GLFW_KEY_SPACE]) {
		this->KeysRepeat[GLFW_KEY_SPACE] = GL_TRUE;
		this->over = true;
	}
    T_gameover->countup_check(currentFrame);
    if (T_gameover->finished) this->over = true;
}


void Game::sum_correct(puzzle* sum, float currentFrame)
{
    // init the animation, including sfx
    if (T_sum_correct->awaiting) {
		T_sum_correct->duration = 4.0f;
        T_sum_correct->run(currentFrame);
		// init each letter animation:
        int count = 0;
        int lettercount = 0;
    	for (auto spr = sum->sprites.begin(); spr < sum->sprites.end(); ++spr) {
            if (spr->name.compare(" ") != 0) {
                spr->init_animtimer(0.5f, currentFrame + static_cast<float>(lettercount) / 5.0f);
                spr->anim.run(currentFrame);
                spr->soundeffect = new SoundEffect(audio, 5); 
                lettercount ++;
            }
            count ++;
		}
	}
    // play the sound?
    int count = 0;
    int lettercount = 0;
    for (auto spr = sum->sprites.begin(); spr < sum->sprites.end(); ++spr) {
        if (spr->name.compare(" ") != 0) {
            if (spr->anim.countup_check(currentFrame) > 0.0) spr->soundeffect->start();
            lettercount ++;
        }
        count ++;
    } 
    //
    T_sum_correct->countup_check(currentFrame);
    for (unsigned int i=0; i < sum->sprites.size(); i++) {
        // manipulating sprites directly
        sum->sprites[i].model = glm::mat4(1.0f);  //reset model!!!
        sum->sprites[i].scale(glm::vec3(sum_scale));
        sum->sprites[i].move(glm::vec3(sum_xPos + i*6, sum_yPos, 0.0f));
		sum->sprites[i].explode_vertical(currentFrame);
        blox->drawSprite(&sum->sprites[i]);
    }
    Sprite* lastsprite = &sum->sprites.back();
    //Sprite* lastsprite = &sum->sprites[sum->sprites.size()-1];
    lastsprite->anim.countup_check(currentFrame);
	if (lastsprite->anim.finished) {
		puzzlestate = PZL_INTRO;
		T_sum_correct->reset();
		this->sum_over = true;
	}
}


void Game::sum_nextlevel(puzzle sum, float currentFrame)
{
    TextLine tl(text, blox);
    tl.reset();
    if (T_starfield_boost->awaiting)
        T_starfield_boost->run(currentFrame);

    if (T_sum_explode->awaiting) {
        T_sum_explode->run(currentFrame);
		T_sum_explode->init_random_LUT(1024);
        SoundEffect warpsound(audio, 2);
        warpsound.start();
	}
    if (T_warpmessage_explode_delay->awaiting) {
        T_warpmessage_explode_delay->run(currentFrame);
    }
    if (T_warpmessage_explode_delay->running) {
        T_warpmessage_explode_delay->countup_check(currentFrame);
        if (T_warpmessage_explode_delay->finished) {
            T_warpmessage_explode->init_random_LUT(1024);
            T_warpmessage_explode->run(currentFrame);
        }
    }
    
    tl.centered = true;
    tl.blink = true;
    tl.blink_interval = 0.01f;
    tl.solidcolor = 5;
    tl.outlinecolor = glm::vec3(0.5, 0.1, 0.3);
    tl.outlinescale = glm::vec3(2.5);
    tl.outline = true;
    if (T_warpmessage_explode->running) {
        tl.explode = true;
        tl.blink = false;
        tl.explodeOffset = T_warpmessage_explode->countup_check(currentFrame) * 1.5;
        tl.explodeLut = &T_warpmessage_explode->fLut;
    }
	tl.flat = false;
    tl.PutString(this->lang->dict.warpnextlevel_1 , 0.0, 0.4, 0.04, currentFrame);
    tl.PutString(this->lang->dict.warpnextlevel_2 , 0.0, -0.4, 0.04, currentFrame);
	float x = T_starfield_boost->countup_check(currentFrame);

	// starfield boost
	blox->starfieldBooster = (glm::cos(((x*pi)-2* pi)/2.0) + 1)/10.0;
    for (unsigned int i=0; i < sum.sprites.size(); i++) {
		Sprite spr = sum.sprites[i];
        spr.scale(glm::vec3(sum_scale));
        spr.move(glm::vec3(sum_xPos + i*6, sum_yPos, 0.0f));
		spr.Z_explode(T_sum_explode->countup_check(currentFrame), T_sum_explode->fLut);
        blox->drawSprite(&spr);
    }

	if (T_starfield_boost->finished) {
		puzzlestate = PZL_INTRO;
        T_starfield_boost->stop();
		T_starfield_boost->reset();
		T_sum_explode->reset();
        T_warpmessage_explode->stop();
        T_warpmessage_explode->reset();
        T_warpmessage_explode_delay->stop();
        T_warpmessage_explode_delay->reset();
		this->sum_over = true;
	}
}

void Game::drawLaserBeam(float currentFrame)
{
    // this draws a laserbeam, made of bloxels
    // it chooses X random points and connects them with a 'line'
	std::vector<glm::vec3> laserBeamBloxels;
	std::vector<float> rnd;
	int nodes = 32;
	int bloxs = 128;
	int step = bloxs / nodes;
	for (int i=0; i< nodes + 1; i++) rnd.push_back(static_cast<float>(std::rand() % 80) / 10.0f);	//get the random points(nodes)

    // connect the lines between the nodes with a line
	for (int i = 0; i < 128; i++) {
		float a = rnd[i / step];
		float b = rnd[i / step + 1];
		int modstap = i % step;
		float ldiff = b - a;
		float lstep = (ldiff / (float)step) * modstap;
		float result = a + lstep;
		laserBeamBloxels.push_back(glm::vec3( static_cast<float>(i), 
										result,
										0.0f));
	}
	Sprite spr = Sprite(laserBeamBloxels, "laserbeam");
    unsigned int lightningcolors[5] = {2,2,11,12,13};
    spr.setIndexColor(lightningcolors[std::rand() % 5]);
	spr.monochrome = true;
	spr.scale(glm::vec3(0.05f));
	if (this->laserbeamstrobe && fmod(currentFrame, 0.16) > 0 && fmod(currentFrame, 0.16) < 0.08) {
		spr.move(glm::vec3(64.0f,-256.0f,0.0f));
	} else {
		spr.move(glm::vec3(64.0f,-28.0f,0.0f));
	}
	blox->drawSprite(&spr);
}

