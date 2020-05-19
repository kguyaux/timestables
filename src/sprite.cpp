#include <cmath>
#include <functional>
#include "shader.hpp"
#include <math.h>
#include "bloxel.hpp"
#include "debug.hpp"


//Constructor
Sprite::Sprite(std::vector<glm::vec3> _bloxels_pos, std::string _name )
    :bloxels_pos(_bloxels_pos), bloxels_origpos(_bloxels_pos)
{
    name = _name;
	for (unsigned int i = 0; i < bloxels_pos.size(); i++) {
		this->bloxels_opacity.push_back(this->opacity);
		this->bloxels_indexcolor.push_back(colorindex);
	}
}


void Sprite::setIndexColor(unsigned int colindex)
{
	for (unsigned int i = 0; i < bloxels_pos.size(); i++) {
		this->bloxels_indexcolor[i] = colindex;
	}
}

void Sprite::init_animtimer(float duration, float starttime)
{
    if (this->anim.awaiting) {
		this->anim.duration = duration;
		this->anim.start_time = starttime;
	}
}


void Sprite::rotate(float radians, glm::vec3 coords )
{
    model = glm::rotate(model, radians, coords); 
}


void Sprite::scale(glm::vec3 coords)
{
    model = glm::scale(model, coords); 
}


void Sprite::move(glm::vec3 coords)
{
    model = glm::translate(model, coords); 
}


void Sprite::setOpacity(float op)
{
    for (unsigned int i=0; i < bloxels_pos.size(); i++) {
		bloxels_opacity[i] = op;
    }
}


void Sprite::explode_vertical(float time)
{
    for (unsigned int i=0; i < bloxels_pos.size(); i++) {
        float x = this->anim.countup_check(time) * 4.0;
        float jaja = x < 2.0 ? -1 * std::pow(x - 2.0, 2.0) + 4.0f : 4.0f;
        float factor = jaja;     
        bloxels_pos[i].y = ((bloxels_origpos[i].y -3.0) * (factor + 1.0)) + 3.0f;
		bloxels_opacity[i] = 1.0f - jaja / 4.0f;
    }
}


void Sprite::verticalPalRotate(float time)
{
    float rate =20.0f; // framerate
    for (unsigned int i=0; i < bloxels_pos.size(); i++) {
        int col = static_cast<int>(fmod(time*rate, 14.0f));
        // rotate palette vertically
        int yy = static_cast<int>(bloxels_origpos[i].y);
        col = ((col + yy) % 14) + 1;
        bloxels_indexcolor[i] = col;
    }
}


void Sprite::desintegrate(float time)
{
    for (unsigned int i=0; i < bloxels_pos.size(); i++) {
        bloxels_indexcolor[i] = std::rand() % 2;
		bloxels_opacity[i] = static_cast<float>(std::rand() % 2 ) / 2.0f * time;
    }
}


// drBibber
void Sprite::funky(float time)
{
    for (unsigned int i=0; i < bloxels_pos.size(); i++) {
        bloxels_pos[i].z = bloxels_origpos[i].z + (std::rand() % 20 + 1)/20.0 - 0.5;
    }
}


void Sprite::Z_explode(float time, std::vector<float>& fLut)
{
    size_t amount = 0;
	if (bloxels_pos.size() > fLut.size()) {
        amount = fLut.size();
    } else {
        amount = bloxels_pos.size();
    }
    float speed = 80.0f;
    float power = 2.0f;
    float spread = 2.0f;
    for (unsigned int i=0; i < amount; i++) {
        bloxels_pos[i].z = bloxels_origpos[i].z - ((1.0 - fLut[i]/spread) * std::pow(time, power) * speed);
    }
}


void Sprite::Z_explodeDie(float time, std::vector<float>& fLut)
{
    size_t amount = 0;
	if (bloxels_pos.size() > fLut.size()) {
        amount = fLut.size();
    } else {
        amount = bloxels_pos.size();
    }
    for (size_t i=0; i < amount; i++) {
		float value = ((1.0 - fLut[i]/5.0) * time * 20.0);
        bloxels_pos[i].y = bloxels_origpos[i].y - value;
        bloxels_pos[i].z = bloxels_origpos[i].z - value;
    }
}

// Destructor
Sprite::~Sprite() {}


