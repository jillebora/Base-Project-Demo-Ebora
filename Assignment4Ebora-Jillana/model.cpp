#include "model.h"
#include <iostream>

#include "tiny_obj_loader.h"

Model::Model()
	: position(0.f, 0.f, 0.f)
	, rotation(0.f, 0.f, 0.f)
	, scale(1.f, 1.f, 1.f)
	, VAO(0)
	, VBO(0)
	, EBO(0)
	, indexCount(0)
	, meshLoaded(false)
{}

Model::~Model()
{
	if (meshLoaded)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}

void Model::loadMesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
{
    // Clean up existing buffers if any
    if (meshLoaded) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    indexCount = (GLsizei)indices.size();

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Setup VBO (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(GLfloat),
        vertices.data(),
        GL_STATIC_DRAW);

    // Setup EBO (indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(GLuint),
        indices.data(),
        GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    meshLoaded = true;

    // Keep the RenderObject's VAO and indexCount same
    renderObj.VAO = &VAO;
    renderObj.indexCount = indexCount;
}

bool Model::loadFromObj(const char* path)
{
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string warning, error;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials,
        &warning, &error, path);

    std::vector<GLfloat> vertices(attrib.vertices.begin(), attrib.vertices.end());

    std::vector<GLuint> indices;
    for (const auto& shape : shapes)
        for (const auto& idx : shape.mesh.indices)
            indices.push_back((GLuint)idx.vertex_index);

    loadMesh(vertices, indices);
    return true;


}

void Model::setShader(GLuint* shaderProg)
{
    renderObj.shaderProg = shaderProg;
}

void Model::setCamera(glm::mat4* projection, glm::mat4* view)
{
    renderObj.projection = projection;
    renderObj.view = view;
}

RenderObject* Model::getRenderObject()
{
    return &renderObj;
}

// ---- Transformation setters ------------------------------------------------
void Model::setPosition(const glm::vec3& pos)
{
    position = pos;
}


void Model::setRotation(const glm::vec3& rot)
{
    rotation = rot;
}


void Model::setScale(const glm::vec3& scl)
{
    scale = scl;
	renderObj.baseScale = scl; // Update the RenderObject's base scale as well
}


glm::mat4 Model::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);

    //apply translation
    model = glm::translate(model, position);

    //apply rotation
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

void Model::destroy()
{
    if (meshLoaded)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        meshLoaded = false;
    }
}

void Model::draw(GLuint shaderProgram) const
{
    if (!meshLoaded)
    {
        std::cerr << "Warning: Attempting draw without loaded mesh" << std::endl;
        return;
    }

    glm::mat4 model = getModelMatrix();

    GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}