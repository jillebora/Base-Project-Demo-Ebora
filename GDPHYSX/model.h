#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <vector>
#include <string>

#include "renderParticle.h"

using namespace std;

// Represents a renderable 3D mesh
class Model
{
    private:
        // Transformation properties.
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
        // Constructors and destructor.
        Model();
        ~Model();

        // Loads mesh data directly from vertex and index arrays.
        void loadMesh(const vector<GLfloat>& vertices, const vector<GLuint>& indices);

        // Loads a mesh from an OBJ file.
        bool loadFromObj(const char* path);

        // Assigns the shader program used when rendering this model
        void setShader(GLuint* shaderProg);

        // Assigns the projection and view matrices used by the rendering system
        void setCamera(glm::mat4* projection, glm::mat4* view);

        // Returns a RenderObject that can be shared with particle renderers
        RenderObject* getRenderObject();

        // Transformation getters.
        glm::vec3 getPosition() const { return position; };
        glm::vec3 getRotation() const { return rotation; };
        glm::vec3 getScale() const { return scale; };

        // Setters for transformations
        void setPosition(const glm::vec3& pos);
        void setRotation(const glm::vec3& rot);
        void setScale(const glm::vec3& scl);

        // Returns whether a mesh has been loaded
        bool isMeshLoaded() const { return meshLoaded; };

        //Generate the model transformation matrix
        glm::mat4 getModelMatrix() const;

        // delete vertex array and buffers
        void destroy();

        //Draw the model
        void draw(GLuint shaderProgram) const;

};
