#ifndef __BLOXEL_HPP__
#define __BLOXEL_HPP__

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "anim_timer.hpp"
#include "shader.hpp"
#include "audio.hpp"
#include <glm/gtx/string_cast.hpp>

enum AnimStates {FALLING, EXPLODE, STATIC};

const std::string msxpalette[15] = {
    "000000", //black
    "cacaca", //grey
    "ffffff", //white
    "b75e51",
    "d96459",
    "fe877c",
    "cac15e",
    "ddce85",
    "3ca042",
    "40b64a",
    "73ce7c",
    "5955df",
    "7e75f0",
    "64daee", //lightblue
    "b565b3"  //pink
};

// a sprite contains a vector of bloxelpositions
// a sprite contains form and animation, and position
class Sprite {
    private:
    public:
        Sprite(std::vector<glm::vec3> orig_bloxelpositions, std::string name = "noname");
        ~Sprite();
        float x,y;
        std::string name;
        float opacity = 1.0f;
        float time = 0.0f; //for animations
        float animTimeOffset = 0.0f;
		int colorindex = 4;
        SoundEffect* soundeffect;
		bool monochrome = false; // if monochrome then a shader is used that can handle larger arrays;
        AnimStates animState;
        void move(float _x, float _y, float _z);

		// 3 bloxel-attributes, pos, color & opactity			
		std::vector<glm::vec3> bloxels_pos, bloxels_origpos;
		std::vector<unsigned int> bloxels_indexcolor;
		std::vector<float> bloxels_opacity;

        glm::mat4 model = glm::mat4(1.0f); // a clean model for each bloxel (for scaling, pos, rot)
        glm::mat4 getModel();
		void init_animtimer(float duration, float starttime = -1.0f );
         
        void setIndexColor(unsigned int colindex);
        void setOpacity(float op);
        void scale(glm::vec3 coords);
        void rotate(float degrees, glm::vec3 coords );
        void move(glm::vec3 coords);
        void funky(float time);
        void desintegrate(float time);
		void Z_explode(float time, std::vector<float>& fLut);
		void Z_explodeDie(float time, std::vector<float>& fLut);
        void explode_vertical(float time);
        void verticalPalRotate(float time);
        float animFallforwardOffset = 0.0f;
		Anim anim = 0.0;
};

class Bloxel {
    public:
        Bloxel();
        ~Bloxel();
        void drawSprite(Sprite* sprite, GLboolean flat = GL_FALSE, GLboolean drawOutlined = GL_FALSE);
        void setWorldStuff(glm::mat4 _view, glm::mat4 _projection);
        void drawOutlinedSprite(Sprite* sprite, GLboolean flat = GL_FALSE);
		void drawStars(float time);
		float starfieldBooster = 0.0;
		float starfieldTimeAdder = 0.0;
        glm::vec3 bloxelscale = glm::vec3(0.9f);
        glm::vec3 outlinecolor = glm::vec3(1.0f,1.0f,1.0f);
        glm::vec3 outlinescale = glm::vec3(1.0f);

    private:
		void loadPalette();
        void _initBloxel();
        void _initStarfield();
		glm::vec3 getColor(std::string hexcolorstring);

        glm::mat4 view, projection;
    	glm::mat4* modelMatrices;
		glm::vec3* starPositions;
    	unsigned int star_amount = 2000;
        unsigned int _VAO, _VBO, _sfVAO, _starfieldBuffer, _starfieldVBO;
        Shader* _instancesShader;
        Shader* _instancesMonoShader;
		Shader* _starfieldShader;
        Shader* outlineShader;

        // `palette` holds the colors of each side(x6) of the bloxel
        // the msxpalette has only 16(or 15) colors
        glm::vec3 palette[32][6] = {
            glm::vec3(0.0f, 1.0f, 0.5f),
            glm::vec3(0.5f, 1.0f, 1.0f),
            glm::vec3(0.7f, 0.0f, 0.5f),
            glm::vec3(0.0f, 0.4f, 0.5f),
            glm::vec3(0.2f, 1.0f, 0.6f),
            glm::vec3(0.0f, 0.3f, 0.5f)
        }; 

        //Cube with normals:
        float Cube[6*6*6] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
};



#endif
