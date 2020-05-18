#ifndef __TEXT_HPP__
#define __TEXT_HPP__

#include <glad/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "charlie.hpp"
#include "bloxel.hpp"
#include "anim_timer.hpp"


enum textColorMode {
    solid,
    randomcol
};

// class for text-stuff
class Text {
    private:
        float scale = 1.0f;

    public:
        Charlie* charlie;
        Text();
        ~Text();
        float spacesize = 0.6f;
        int charmapper(char val);
        std::vector<int> getStringBloxels(std::string &str);
        std::vector<glm::vec3> getTextBloxels(int charindex);
        std::vector<std::vector<glm::vec3>> charBloxels; // list of all character-bloxels
        std::vector<Sprite> charSprites;

};

// class for putting text on screen, in lines or per single character
// it provides some funny animations
class TextLine {
    public:
        bool flat = true;
        bool rotate = false;
        bool wave = false;
        bool verticalPalRot = false;
        bool centered = false;
        bool blink = false;
        bool funky = false;
        bool outline = false;
        bool shadow = false;
        bool stretch = false;
        glm::vec3 stretchfactor = glm::vec3(1.0f);
        int shadowcolor = 1;
        glm::vec3 outlinecolor = glm::vec3(1.0f);
        glm::vec3 outlinescale = glm::vec3(1.0f);
        bool forwardanim = false;
        float spacesize = 0.6;
        textColorMode colorMode = solid;
        float imgui_x = 100.0f;
        float imgui_y = 100.0f;

        float x_move_offset = 0; // for moving animations, like in the submenu
        float blink_interval = 2.0;
        Anim* forwardAnim; 
        Text* fontObject;


        float scale = 1.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f; 
        bool explode = false;
        std::vector<float> *explodeLut = NULL;
        float explodeOffset = 0;
        unsigned int solidcolor = 14;
        
        Bloxel* bloxelObject;

        TextLine(Text* _fontObject, Bloxel* _bloxelObject) 
            : fontObject(_fontObject), bloxelObject(_bloxelObject)
        {
            this->reset();
        }

        ~TextLine() {}

        void reset()
        {
            flat = true;
            rotate = false;
            wave = false;
            verticalPalRot = false;
            centered = false;
            blink = false;
            funky = false;
            outline = false;
            stretch = false;
            explode = false;
            explodeOffset = 0;
            explodeLut = NULL;
            stretchfactor = glm::vec3(1.0f);
            blink_interval = 2.0;
            shadowcolor = 1;
            outlinecolor = glm::vec3(1.0f);
            outlinescale = glm::vec3(1.0f);
            forwardanim = false;
            shadow = false;
            spacesize = 0.6;
            colorMode = solid;
            imgui_x = 100.0f;
            imgui_y = 100.0f;
            scale = 0.0125f; // this scale fits 80 chars on one row
            x = 0.0f;
            y = 0.0f;
            z = 0.0f; 
            solidcolor = 14;
        }
        
        // grid is 640 x 480 starting in topleft corner (for easy placement)
        void setPos_grid(float x, float y, float z)
        {
            this->x = 0.0f - 2.6f + (x * scale); // / 130.0f + 0.5f;
            this->y = 0.0f + 2.4444f - (y * scale);// - (y - 320.0) / 156.0f - 0.5f;
            this->z = 0.0f + (z * scale);// + (z / 100.0f);
        }
        
        // use this one for passing raw character-codes
        void PutString(int char_index, float xx, float yy, float sc, float time)
        {
            //unsigned char normalized_charindex = static_cast<char>(char_index % 256);
            float char_width = sc * spacesize * 10;
            if (centered) {
                float string_width = char_width;
                xx = (char_width+(char_width/2.0f)) - (string_width / 2.0f);
            }
            xx = xx + x_move_offset; // used in submenu movement

            int i = 0;
            this->processChar(char_index, char_width, xx, yy, sc, time, i);
            
        }

        // use this for passing strings
        void PutString(std::string str, float xx, float yy, float sc, float time)
        {
            float char_width = sc * spacesize * 10;
            if (centered) {
                float string_width = str.size()* char_width;
                xx = (char_width+(char_width/2.0f)) - (string_width / 2.0f);
            }
            xx = xx + x_move_offset; // used in submenu movement

            int i = 0;
            for(char& c : str) {
                int ch = this->fontObject->charmapper(c);
                this->processChar(ch, char_width, xx, yy, sc, time, i);
                i++;
            }
        }

        
    private:
        void processChar(int c, float char_width, float xx, float yy, float sc, float time, int i)
        {
            float yoffset = 0;
            Sprite txtSprite = fontObject->charSprites[c]; // copy of the charsprite
            
            // drBibber
            if (this->funky) txtSprite.funky(time-float(i)/2.5); 

            if (blink) {
                float fm = fmod(time, blink_interval);
                float hlb = blink_interval / 2.0f;
                if (fm > hlb && fm <= blink_interval) txtSprite.scale(glm::vec3(0.0f));
            }

            if (forwardanim) {
                float distance = forwardAnim->countup_check(time);
                //txtSprite.move(glm::vec3(0.0, 0.0, std::pow(distance, 2)));
                txtSprite.move(glm::vec3(0.0, 0.0-distance/2.0, distance*2));
            }
           
            //per sprite animations
            if (wave) yoffset = glm::sin((time - float(i)/4.5f)*5.0)/60.0f;

            float charoffset = static_cast<float>(i) * char_width;
            txtSprite.move(glm::vec3(xx + charoffset, yy + yoffset, this->z));
            txtSprite.scale(glm::vec3(sc));
            
            if (rotate) {
                txtSprite.move(glm::vec3(-5, 0,0));
                txtSprite.rotate(time-(float(i)/2.5), glm::vec3(0.0, 1.0, 0.0));
                txtSprite.move(glm::vec3(5, 0,0));
            }

            if (this->colorMode == solid) {
                txtSprite.setIndexColor(this->solidcolor);
            }
            
            if (this->verticalPalRot)
                txtSprite.verticalPalRotate(time);
            
            if (explode) {
		        txtSprite.Z_explode(explodeOffset, *explodeLut);
            }

            if (this->stretch == true) {
                // a bit hackish solution to draw a rectangle
                bloxelObject->bloxelscale = glm::vec3(1.0f);
                txtSprite.move(glm::vec3(0 , 0, -0.2));
                txtSprite.setOpacity(0.4f);
                txtSprite.move(glm::vec3(0.5, 0,0));
                txtSprite.scale(stretchfactor);
                txtSprite.move(glm::vec3(-0.5, 0,0));
            }
            if (shadow && flat) {
                txtSprite.setIndexColor(this->shadowcolor);
                txtSprite.move(glm::vec3(-1.0, -1.0, -0.1));
                bloxelObject->drawSprite(&txtSprite, this->flat);
                if (this->colorMode == solid) txtSprite.setIndexColor(this->solidcolor);
                txtSprite.move(glm::vec3(1.0, 1.0, 0.1));
            }

            if (outline) {
                bloxelObject->outlinecolor = outlinecolor;
                bloxelObject->outlinescale = outlinescale;
                bloxelObject->drawOutlinedSprite(&txtSprite, this->flat);
            } else {
                bloxelObject->drawSprite(&txtSprite, this->flat);
            }

            if (this->stretch == true) {
                // a bit hackish solution to draw a rectangle pt2
                // set back the bloxelscale to 0.9f
                bloxelObject->bloxelscale = glm::vec3(0.9f);
            }
        }
};

#endif
