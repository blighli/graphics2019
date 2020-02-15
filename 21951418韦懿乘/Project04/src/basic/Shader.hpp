#ifndef shader_hpp
#define shader_hpp

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>

// TODO: separate vertex and fragment shader
//       VertexShader provide interface to update model/view/projection
//       FragmentShader closely related with Material to set visual effect
//       Shader organize VertexShader and FragmentShader
//       but... is it necessary?
class Shader {
public:
    // constructor reads and builds the shader
    Shader(const GLchar *vertexPath,
           const GLchar *fragmentPath,
           const GLchar *geometryPath = nullptr);
    Shader() = default;
    // use/activate the shader
    unsigned int getID() const;
    void use() const;

    // utility uniform functions
    void set(const std::string &name, bool value) const;
    void set(const std::string &name, int value) const;
    void set(const std::string &name, unsigned value) const;
    void set(const std::string &name, float value) const;
    void set(const std::string &name, const glm::vec2 &value) const;
    void set(const std::string &name, float x, float y) const;
    void set(const std::string &name, const glm::vec3 &value) const;
    void set(const std::string &name, float x, float y, float z) const;
    void set(const std::string &name, const glm::vec4 &value) const;
    void set(const std::string &name, float x, float y, float z, float w) const;
    void set(const std::string &name, const glm::mat2 &mat) const;
    void set(const std::string &name, const glm::mat3 &mat) const;
    void set(const std::string &name, const glm::mat4 &mat) const;

private:

    // the program id
    unsigned int id = 0;
    // checking error
    void checkCompileErrors(GLuint shader, const std::string &type,
                                           std::string path);
};

#endif /* shader_hpp */
