#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <vector>
#include <string>

#include "renderParticle.h"

using namespace std;

class Model
{
    private:
        glm::vec3 position;
        glm::vec3 rotation;     // rotation in degrees (X, Y, Z)
        glm::vec3 scale;

        // OpenGL buffer objects
        GLuint VAO, VBO, EBO;

        // Mesh data
        GLsizei indexCount;
        bool meshLoaded;

        // Bridges this model to RenderParticle
        RenderObject renderObj;

    public:
        // Constructors
        Model();
        ~Model();

        void loadMesh(const vector<GLfloat>& vertices, const vector<GLuint>& indices);

        bool loadFromObj(const char* path);

        void setShader(GLuint* shaderProg);

        void setCamera(glm::mat4* projection, glm::mat4* view);

        RenderObject* getRenderObject();

        glm::vec3 getPosition() const { return position; };
        glm::vec3 getRotation() const { return rotation; };
        glm::vec3 getScale() const { return scale; };

        // Setters for transformations
        void setPosition(const glm::vec3& pos);
        void setRotation(const glm::vec3& rot);
        void setScale(const glm::vec3& scl);

        bool isMeshLoaded() const { return meshLoaded; };

        //Generate the model transformation matrix
        glm::mat4 getModelMatrix() const;

        void destroy();

        //Draw the model
        void draw(GLuint shaderProgram) const;

};
