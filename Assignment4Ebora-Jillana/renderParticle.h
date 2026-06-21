#pragma once

#include "P6/particle.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderObject
{
    GLuint* shaderProg;
    GLuint* VAO;

    glm::mat4* projection;
    glm::mat4* view;

    GLsizei indexCount;
    glm::vec3 baseScale = glm::vec3(1.f);
};

class RenderParticle
{
public:

    // Reference for our P6 Particle
    P6::Particle* PhysicsParticle;

    // Reference for our 3D Model
    RenderObject* RenderObjectRef;

    // Color just cuz
    glm::vec3 Color;

    glm::vec3 Scale;

    // Lazy constructor to default to white
    RenderParticle(
        P6::Particle* p,
        RenderObject* obj
    )
        : PhysicsParticle(p),
        RenderObjectRef(obj)
    {
        Color = glm::vec3(1.f, 1.f, 1.f);

        Scale = glm::vec3(1.f);
    }

    // Constructor with custom color
    RenderParticle(
        P6::Particle* p,
        RenderObject* obj,
        glm::vec3 c
    )
        : PhysicsParticle(p),
        RenderObjectRef(obj),
        Color(c)
    {
        Scale = glm::vec3(1.f);
    }

    // Draw Function
    void Draw()
    {
        if (PhysicsParticle->IsDestroyed())
            return;

        glUseProgram(
            *RenderObjectRef->shaderProg
        );

        glm::mat4 transform =
            glm::mat4(1.f);

        transform =
            glm::translate(
                transform,
                PhysicsParticle->Position
            );

        transform =
            glm::scale(
                transform,
				Scale * RenderObjectRef->baseScale
            );

        GLuint transformLoc =
            glGetUniformLocation(
                *RenderObjectRef->shaderProg,
                "transform"
            );

        GLuint projLoc =
            glGetUniformLocation(
                *RenderObjectRef->shaderProg,
                "projection"
            );

        GLuint viewLoc =
            glGetUniformLocation(
                *RenderObjectRef->shaderProg,
                "view"
            );

        glUniformMatrix4fv(
            transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transform)
        );

        glUniformMatrix4fv(
            projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(
                *RenderObjectRef->projection
            )
        );

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(
                *RenderObjectRef->view
            )
        );

        GLuint colorLoc = glGetUniformLocation(*RenderObjectRef->shaderProg, "color");
        glUniform3fv(colorLoc, 1, glm::value_ptr(Color));

        glBindVertexArray(
            *RenderObjectRef->VAO
        );

        glDrawElements(
            GL_TRIANGLES,
            RenderObjectRef->indexCount,
            GL_UNSIGNED_INT,
            0
        );
    }
};