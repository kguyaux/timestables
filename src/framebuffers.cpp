#include "framebuffers.hpp"

Buffers::Buffers(unsigned int w, unsigned int h) {
	_width = w;
	_height = h;
	_init();
}


void Buffers::_init()
{
    // init the buffers and the textures- ID's
    glGenFramebuffers(NUM_MULTISAMPLE_BUFFERS, ms_FBO);
    glGenTextures(NUM_MULTISAMPLE_BUFFERS, ms_renderBufferTexture);
    glGenRenderbuffers(NUM_MULTISAMPLE_BUFFERS, ms_rbo);
    glGenFramebuffers(NUM_MULTISAMPLE_BUFFERS, intermediate_FBO);
    glGenTextures(NUM_MULTISAMPLE_BUFFERS, screenTexture);
    
    for (int i=0; i< NUM_MULTISAMPLE_BUFFERS; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, ms_FBO[i]);

            // first we create the texture(colorbuffer), that is multisampled
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ms_renderBufferTexture[i]);
                
                if (i==0)
                    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisample_count, GL_RGB, _width, _height, GL_TRUE);
                // .. because I don't need an alphalayer with the first buffer I thought it was faster to leave that out in the first one..
                //
                if (i>0)
                    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisample_count, GL_RGBA, _width, _height, GL_TRUE);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, ms_renderBufferTexture[i], 0); 
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);  // unbind, just to be sure, 

            // overige buffers:(depth & stencil)
            // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
            glBindRenderbuffer(GL_RENDERBUFFER, ms_rbo[i]);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample_count, GL_DEPTH24_STENCIL8, _width, _height);  
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ms_rbo[i]);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


        // create intermediateFBO, 
        // to this buffer the multisamplerender will be blitted, so we can use it's
        // attached texture as a sampler for later postprocessing
        glBindFramebuffer(GL_FRAMEBUFFER, intermediate_FBO[i]);
            // create a color attachment texture
            glBindTexture(GL_TEXTURE_2D, screenTexture[i]);
                if (i==0)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                if (i==1)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture[i], 0);	// we only need a color buffer

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    } // end of for-loop to create all the buffers
}


void Buffers::re_init(unsigned int w, unsigned int h)
{
	this->_width = w;
	this->_height = h;
#ifdef TT_DEBUG
	std::cout << "Re-init buffers, width: " << _width << " height: " << _height << std::endl;
#endif

    this->destroy();
	_init();
}

void Buffers::MStoIntermediate_Blit(GLuint bufferID) 
{
	// now resolve multisampled buffer(s) into intermediate FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_FBO[bufferID]);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate_FBO[bufferID]);
    glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


// buffers need to be destroyed manually so I put this in a function and
// not in the destructor
void Buffers::destroy()
{
	glDeleteFramebuffers(NUM_MULTISAMPLE_BUFFERS, ms_FBO);
	glDeleteFramebuffers(NUM_MULTISAMPLE_BUFFERS, intermediate_FBO);
    glDeleteFramebuffers(NUM_MULTISAMPLE_BUFFERS, ms_rbo);
    glDeleteTextures(NUM_MULTISAMPLE_BUFFERS, ms_renderBufferTexture);
    glDeleteTextures(NUM_MULTISAMPLE_BUFFERS, screenTexture); 
}

Buffers::~Buffers() {
}

