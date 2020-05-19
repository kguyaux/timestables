#include "bloxel.hpp"

Bloxel::Bloxel() //constructor
{
    _initBloxel();
    _initStarfield();
	loadPalette();
	outlineShader = new Shader((const GLchar*)"shaders/things.vs", "shaders/outlineshader.fs");
}

// convert hexcolor to vec3
glm::vec3 Bloxel::getColor(std::string hexcolorstring)
{
    unsigned long value = std::stoul(hexcolorstring, nullptr, 16);
    float r = static_cast<float>((value >> 16) & 0xff) / 255.0f;
    float g = static_cast<float>((value >> 8) & 0xff) / 255.0f;
    float b = static_cast<float>((value >> 0) & 0xff) / 255.0f;
	return glm::vec3(r, g, b);
}


void Bloxel::loadPalette()
{
	for (int i=0; i < 15; i++) {
        for (int j=0; j < 6; j++) {
		   palette[i][j] = getColor(msxpalette[i]);
        }
	};
}

void Bloxel::_initBloxel()
{
    // load shaders 
	_instancesShader = new Shader((const GLchar*)"shaders/things.vs", (const GLchar*)"shaders/things.fs");
	_instancesMonoShader = new Shader((const GLchar*)"shaders/things_monochrome.vs", (const GLchar*)"shaders/things.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    glGenVertexArrays(1, &_VAO); // Generate vertexarray
    glGenBuffers(1, &_VBO); // vbo for vertexcoords & normals

    // setup cube-instances-VAO
    glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), &Cube[0], GL_DYNAMIC_DRAW); // put vertices in VAO->VBO
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // vertices
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3)); // Normals
            glEnableVertexAttribArray(0); // aPos
            glEnableVertexAttribArray(1); // aNormal
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind to be sure
} 

void Bloxel::_initStarfield()
{
    // shader laden
	_starfieldShader = new Shader((const GLchar*)"shaders/starfield.vs", (const GLchar*)"shaders/starfield.fs");
    glGenVertexArrays(1, &_sfVAO); // Generate vertexarray
    glGenBuffers(1, &_starfieldVBO); // vbo for vertexcoords & normals
    glGenBuffers(1, &_starfieldBuffer); // modelmatrices

    std::srand(glfwGetTime()); // initialize random seed 

    modelMatrices = new glm::mat4[star_amount];
	starPositions = new glm::vec3[star_amount];
    for (unsigned int i = 0; i < star_amount; i++) {
        float x = (float)(std::rand() % 320 - 160); 
        float y = (float)(std::rand() % 320 - 160); 
        float z = (float)(std::rand() % 640 -100);
        starPositions[i] = glm::vec3(x / 100.0,y/100.0,z/100.0);
    }

    glBindVertexArray(_sfVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VAO); 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // vertices
        glEnableVertexAttribArray(0); // aPos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3)); // Normals
        glEnableVertexAttribArray(1); // aNormal


	glBindBuffer(GL_ARRAY_BUFFER, _starfieldBuffer); 
	glBufferData(GL_ARRAY_BUFFER, star_amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
        // setup those attribute-pointers so that the buffers will get read out correctly,
        // more work now because they hold model-matrices of all the stars(4x4)
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0); // vec4 number 1, for maxtrix-row 1
        glEnableVertexAttribArray(3); 

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))); //vec4 number 2, for row2
        glEnableVertexAttribArray(4); 

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))); //vec4 number 3, etc
        glEnableVertexAttribArray(5); 

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));  // etcc..
        glEnableVertexAttribArray(6); 

        // Setup the divisor, yes that has to be done also, otherwise.. uuuuhhhhhmmm 
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind to be sure

} 



void Bloxel::drawStars(float time)
{
	// update starfield
	starfieldTimeAdder += starfieldBooster;
    for (unsigned int i = 0; i < star_amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
       	float newz = std::fmod((starPositions[i].z + time/8.0+ starfieldTimeAdder)+1, 6.40) -1.0;
        // transformations:
		model = glm::translate(model, glm::vec3(starPositions[i].x, starPositions[i].y, newz));
		model = glm::scale(model, glm::vec3(0.005,0.005,0.005 * (1.0 + starfieldBooster*1000)));
        modelMatrices[i] = model;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _starfieldBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, star_amount*sizeof(glm::mat4), &modelMatrices[0]);

	// draw starfield
	_starfieldShader->use();
	_starfieldShader->setFloat("iTime", time);
	_starfieldShader->setMat4("view", glm::rotate(view, starfieldBooster*2, glm::vec3(0.0, 0.0, 1.0)));
	_starfieldShader->setMat4("projection", projection);
	glBindVertexArray(_sfVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, star_amount); 
	glBindVertexArray(0);
}


void Bloxel::setWorldStuff(glm::mat4 _view, glm::mat4 _projection)
{
    view = _view;
    projection = _projection;    
}


void Bloxel::drawOutlinedSprite(Sprite* sprite, GLboolean flat)
{
    glDisable(GL_DEPTH_TEST);
    drawSprite(sprite, flat, true);
    glEnable(GL_DEPTH_TEST); 
    drawSprite(sprite, flat, false);
}

void Bloxel::drawSprite(Sprite* sprite, GLboolean flat, GLboolean drawOutlined)
{
    if (drawOutlined) {
            outlineShader->use();
            outlineShader->setMat4("view", view);
            outlineShader->setMat4("projection", projection);
            outlineShader->setVec3("outlinecolor", outlinecolor);
    } else {
        if (!sprite->monochrome) {
            _instancesShader->use();
            _instancesShader->setMat4("view", view);
            _instancesShader->setMat4("projection", projection);
        } else {
            _instancesMonoShader->use();
            _instancesMonoShader->setMat4("view", view);
            _instancesMonoShader->setMat4("projection", projection);

        }
    }

    for(unsigned int vi = 0; vi < sprite->bloxels_pos.size(); vi++)
    {
        std::stringstream ss;
        std::string index;
        ss << vi; 
        index = ss.str();

        // do funny stuff with with the instancemodel 
        glm::mat4 instanceModel = sprite->model;
        // reminder: with the model passed from cpu we can now do things like explosions & stuff.. if we want
        instanceModel =  glm::translate(instanceModel, glm::vec3(-1.0f) * sprite->bloxels_pos[vi]);

        if (drawOutlined)
            instanceModel = glm::scale(instanceModel, glm::vec3(outlinescale));
        else
            instanceModel = glm::scale(instanceModel, bloxelscale);
        if (drawOutlined) {
            outlineShader->setMat4(("models[" + index + "]").c_str(), instanceModel);
            outlineShader->setFloat(("opacity[" + index + "]").c_str(), 1.0f);
        
        } else {
            if (!sprite->monochrome) {
                _instancesShader->setMat4(("models[" + index + "]").c_str(), instanceModel);
                _instancesShader->setFloat(("opacity[" + index + "]").c_str(), sprite->bloxels_opacity[vi]);
            } else {
                _instancesMonoShader->setMat4(("models[" + index + "]").c_str(), instanceModel);
                _instancesMonoShader->setFloat(("opacity[" + index + "]").c_str(), sprite->bloxels_opacity[vi]);
            }
        }
        
        // set colors of all sides
        for(unsigned int side = 0; side < 6; side++) {
            std::stringstream s_ss;
            std::string s_index;
            if (drawOutlined) {
                //pass
            } else {
                if (!sprite->monochrome) {
                    s_ss << side + (vi * 6); 
                    s_index = s_ss.str();
                    _instancesShader->setVec3(("colors[" + s_index + "]").c_str(),
                                              palette[sprite->bloxels_indexcolor[vi]][side]);
                } else {
                    s_ss << side; 
                    s_index = s_ss.str();
                    unsigned int palette_index = sprite->bloxels_indexcolor[vi];
                    glm::vec3 rgbvalue = palette[palette_index][side];
                    _instancesMonoShader->setVec3(("colors[" + s_index + "]").c_str(), rgbvalue);
                }
            }
        }
    }  
    glBindVertexArray(_VAO);
    if (flat)
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, sprite->bloxels_pos.size()); 
    else
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, sprite->bloxels_pos.size()); 
    glBindVertexArray(0); // for cleanliness
}


// destructor
Bloxel::~Bloxel()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteVertexArrays(1, &_sfVAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_starfieldBuffer);
}


