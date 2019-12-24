#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <fstream>
#include <iostream>
#include <vector>


GLuint loadTexture(char const *path, bool gammaCorrection) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1) {
            internalFormat = dataFormat = GL_RED;
        } else if (nrComponents == 3) {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        } else if (nrComponents == 4) {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    return textureID;
}

GLuint loadBMP_custom(const char *imagepath) {

    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char *data;

    // Open the file
    FILE *file = fopen(imagepath, "rb");
    if (!file) {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
        getchar();
        return 0;
    }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    // A BMP files always begins with "BM"
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    // Make sure this is a 24bpp file
    if (*(int *) &(header[0x1E]) != 0) {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (*(int *) &(header[0x1C]) != 24) {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }

    // Read the information about the image
    dataPos = *(int *) &(header[0x0A]);
    imageSize = *(int *) &(header[0x22]);
    width = *(int *) &(header[0x12]);
    height = *(int *) &(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0) dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // Everything is in memory now, the file can be closed.
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete[] data;

    // Poor filtering, or ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // ... nice trilinear filtering ...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // ... which requires mipmaps. Generate them automatically.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Return the ID of the texture we just created
    return textureID;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char *imagepath) {

    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(imagepath, "rb");
    if (fp == NULL) {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
        getchar();
        return 0;
    }

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int *) &(header[8]);
    unsigned int width = *(unsigned int *) &(header[12]);
    unsigned int linearSize = *(unsigned int *) &(header[16]);
    unsigned int mipMapCount = *(unsigned int *) &(header[24]);
    unsigned int fourCC = *(unsigned int *) &(header[80]);

    unsigned char *buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char *) malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;

        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if (width < 1) width = 1;
        if (height < 1) height = 1;

    }
    free(buffer);
    return textureID;
}


typedef union PixelInfo {
    std::uint32_t Colour;
    struct {
        std::uint8_t R, G, B, A;
    };
} *PPixelInfo;

class Tga {
private:
    std::vector<std::uint8_t> Pixels;
    bool ImageCompressed;
    std::uint32_t width, height, size, BitsPerPixel;

public:
    Tga(const char *FilePath);

    std::vector<std::uint8_t> GetPixels() { return this->Pixels; }

    std::uint32_t GetWidth() const { return this->width; }

    std::uint32_t GetHeight() const { return this->height; }

    bool HasAlphaChannel() { return BitsPerPixel == 32; }
};

Tga::Tga(const char *FilePath) {
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) { throw std::invalid_argument("File Not Found."); }

    std::uint8_t Header[18] = {0};
    std::vector<std::uint8_t> ImageData;
    static std::uint8_t DeCompressed[12] = {0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    static std::uint8_t IsCompressed[12] = {0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    hFile.read(reinterpret_cast<char *>(&Header), sizeof(Header));

    if (!memcmp(DeCompressed, &Header, sizeof(DeCompressed))) {
        BitsPerPixel = Header[16];
        width = Header[13] * 256 + Header[12];
        height = Header[15] * 256 + Header[14];
        size = ((width * BitsPerPixel + 31) / 32) * 4 * height;

        if ((BitsPerPixel != 24) && (BitsPerPixel != 32)) {
            hFile.close();
            throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        ImageData.resize(size);
        ImageCompressed = false;
        hFile.read(reinterpret_cast<char *>(ImageData.data()), size);
    } else if (!memcmp(IsCompressed, &Header, sizeof(IsCompressed))) {
        BitsPerPixel = Header[16];
        width = Header[13] * 256 + Header[12];
        height = Header[15] * 256 + Header[14];
        size = ((width * BitsPerPixel + 31) / 32) * 4 * height;

        if ((BitsPerPixel != 24) && (BitsPerPixel != 32)) {
            hFile.close();
            throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        PixelInfo Pixel = {0};
        int CurrentByte = 0;
        std::size_t CurrentPixel = 0;
        ImageCompressed = true;
        std::uint8_t ChunkHeader = {0};
        int BytesPerPixel = (BitsPerPixel / 8);
        ImageData.resize(width * height * sizeof(PixelInfo));

        do {
            hFile.read(reinterpret_cast<char *>(&ChunkHeader), sizeof(ChunkHeader));

            if (ChunkHeader < 128) {
                ++ChunkHeader;
                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel) {
                    hFile.read(reinterpret_cast<char *>(&Pixel), BytesPerPixel);

                    ImageData[CurrentByte++] = Pixel.B;
                    ImageData[CurrentByte++] = Pixel.G;
                    ImageData[CurrentByte++] = Pixel.R;
                    if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
                }
            } else {
                ChunkHeader -= 127;
                hFile.read(reinterpret_cast<char *>(&Pixel), BytesPerPixel);

                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel) {
                    ImageData[CurrentByte++] = Pixel.B;
                    ImageData[CurrentByte++] = Pixel.G;
                    ImageData[CurrentByte++] = Pixel.R;
                    if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
                }
            }
        } while (CurrentPixel < (width * height));
    } else {
        hFile.close();
        throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit TGA File.");
    }

    hFile.close();
    this->Pixels = ImageData;
}

GLuint loadTGA(const char *path) {
    Tga tga(path);
    GLenum dataFormat = tga.HasAlphaChannel() ? GL_RGBA : GL_RGB;
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, tga.GetWidth(), tga.GetHeight(), 0, dataFormat, GL_UNSIGNED_BYTE, tga.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return textureID;
}