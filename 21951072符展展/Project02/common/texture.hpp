#ifndef TEXTURE_HPP
#define TEXTURE_HPP

//stbi_image
GLuint loadTexture(char const* path, bool gammaCorrection = false);

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

//Load TGA
GLuint loadTGA(const char* path);

#endif