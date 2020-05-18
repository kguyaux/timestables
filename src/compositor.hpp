
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"

#pragma once

//#define SCREEN_WIDTH 800
//#define SCREEN_HEIGHT 600


class Compositor {
	public:
        Compositor(const GLchar* compshaderfile);
		~Compositor();
        void destroy();
        void composit(GLuint bgtexture);
	private:
        Shader* _shader;
		float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		unsigned int quadVAO, quadVBO;
};


Compositor::Compositor(const GLchar* compshaderfile) {
	_shader = new Shader((const GLchar*)"shaders/screen.vs", compshaderfile);
	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    _shader->setInt("bgtexture", 0);
}


void Compositor::composit(GLuint bgtexture)
{
    glDisable(GL_DEPTH_TEST); // disable depth test so comp-quad isn't discarded due to depth test.
    _shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bgtexture);
	glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0); // for cleanliness
}

Compositor::~Compositor() {}

void Compositor::destroy() // destroy has to be called manually, before the termination of glfw
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}


