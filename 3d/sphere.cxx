#include <cstdlib>
#include <cstdio>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <glm/glm.hpp>
#include "gl-subs.hxx"
#include "sphere.hxx"

namespace renderer {

  void sphere::InitializeData() {

    int err = (posix_memalign((void **)&buffer, 16,
			      sizeof(GLfloat) * 3 * L));

    if(err) {
      fprintf(stderr, "Error allocating memory\n");
      exit(-1);
    }
  }

  sphere::~sphere() {
    free(buffer);
  }

  void sphere::setPointsize(float size) {
    pointsize = size;
    glUseProgram(theProgram);
    glUniform1f(pointsizeUniform, pointsize);
    glUseProgram(0);
  }

  void sphere::InitializeProgram() {
    std::vector<std::string> inputList = { "position" };

    std::vector<GLuint> shaderList = {
        renderer::LoadShader(GL_VERTEX_SHADER,
	                     "data/sphere.vert"),
        renderer::LoadShader(GL_FRAGMENT_SHADER,
			     "data/sphere.frag")
    };

    theProgram = renderer::CreateProgram(shaderList, inputList);

    offsetUniform = glGetUniformLocation(theProgram, "offset");

    perspectiveMatrixUnif = glGetUniformLocation(theProgram,
						 "perspectiveMatrix");

    colorUniform = glGetUniformLocation(theProgram, "Color");
    lightdirUniform = glGetUniformLocation(theProgram, "lightDir");
    pointsizeUniform = glGetUniformLocation(theProgram, "pointsize");

    glUseProgram(theProgram);
    glUniform4f(colorUniform, color[0], color[1], color[2], color[3]);
    glUniform3f(lightdirUniform, 0.0f, 0.0f, 1.0f);
    glUniform1f(pointsizeUniform, pointsize);
    glUseProgram(0);

    update_global_uniforms();

    //Create vao and vbo stuff
    glGenVertexArrays(1, &vaoObject);
    glGenBuffers (1, &vertexBufferObject);

    glBindVertexArray(vaoObject);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);
    glBindVertexArray(0);

  }

  void sphere::draw() {
    bool psize = glIsEnabled(GL_VERTEX_PROGRAM_POINT_SIZE);
    if (!psize)
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    /*
    GLfloat psize;
    glGetFloatv(GL_POINT_SIZE, &psize);
    glPointSize(16);
    */

    glUseProgram(theProgram);
    glBindVertexArray(vaoObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
    		 3 * L * sizeof(float),
    		 buffer, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(GLfloat)*3, 0);

    glDrawArrays(GL_POINTS, 0, L);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    if (!psize)
      glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);

//    GLenum err;
//    while ((err = glGetError()) != GL_NO_ERROR) {
//	fprintf(stderr, "%s: %d\n", "OpenGL error drawing sphere", err);
//    }


    /*
    glPointSize(psize);
    */
  }
  
}
