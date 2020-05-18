#ifndef __BUFFERS_HPP__
#define __BUFFERS_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"


// This looked handy, to have multiple buffers to draw things to, and composit/combine them later
// but blitting was so slow, that I do things now with only one buffer
#define NUM_MULTISAMPLE_BUFFERS 1

class Buffers {
	public:
		Buffers(unsigned int w, unsigned int h);
		~Buffers();
        
        // our initial buffer to which the 3d is rendered to
		GLuint multisample_count = 8;

		GLuint ms_FBO[NUM_MULTISAMPLE_BUFFERS]; // multisample-framebuffer
		GLuint ms_rbo[NUM_MULTISAMPLE_BUFFERS]; // for depth and stencil
		GLuint ms_renderBufferTexture[NUM_MULTISAMPLE_BUFFERS];  // this texture will be blitted to intermediate buffer
        GLuint intermediate_FBO[NUM_MULTISAMPLE_BUFFERS]; //intermediate
	    GLuint screenTexture[NUM_MULTISAMPLE_BUFFERS]; // from here the result will be accesible

		void re_init(unsigned int w, unsigned int h);
        void MStoIntermediate_Blit(GLuint bufferID);
		void destroy();
		void draw();
        void resizeblurtexture(float scalefactor);

	private:
		unsigned int _width;
		unsigned int _height;
		void _init();
};


#endif
