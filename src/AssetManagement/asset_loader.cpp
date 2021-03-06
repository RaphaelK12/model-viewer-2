#include "asset_loader.h"
#include <cstring>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <stb_image.h>

Shader LoadShadersFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    FILE* vsRaw = fopen(vertexShaderPath, "rb");
    if(!vsRaw)
    {
        printf("Failed to open file at path: %s\n", vertexShaderPath);
        exit(-1);
    }

    // Get file size
    fseek(vsRaw, 0, SEEK_END);
    size_t size = (size_t)ftell(vsRaw);
    rewind(vsRaw);

    char* vsBuffer = new char[size + 1];
    size_t readSize = fread(vsBuffer, 1, size, vsRaw);
    if(readSize != size)
        printf("Bytes needed to be read: %zu\nBytes successfully read: %zu\n", size, readSize);
    vsBuffer[size] = '\0';
    rewind(vsRaw);

    // Create vertex shader object
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsBuffer, 0);
    glCompileShader(vertex);

    // Check compilation errors
    GLint compiled;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {
        GLsizei length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertex, 1024, &length, message);
        printf("Vertex Shader Compilation Errors:\n%s\n", message);
    }

    FILE* fsRaw = fopen(fragmentShaderPath, "rb");
    if(!fsRaw)
    {
        printf("Failed to open file at path: %s\n", fragmentShaderPath);
        exit(-1);
    }

    fseek(fsRaw, 0, SEEK_END);
    size = ftell(fsRaw);
    rewind(fsRaw);

    char* fsBuffer = new char[size + 1];
    readSize = fread(fsBuffer, 1, size, fsRaw);
    if(readSize != size)
        printf("Bytes needed to be read: %zu\nBytes successfully read: %zu\n", size, readSize);
    fsBuffer[size] = '\0';
    rewind(fsRaw);

    // Create fragment shader object
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsBuffer, 0);
    glCompileShader(fragment);

    // Check compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {
        GLsizei length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragment, 1024, &length, message);
        printf("Fragment Shader Compilation Errors:\n%s\n", message);
    }

    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Check the linking status
    int infoLength = 0;
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLength);
    glGetProgramiv(ID, GL_LINK_STATUS, &compiled);
    if(infoLength > 0)
    {
        GLchar message[1024];
        glGetProgramInfoLog(ID, infoLength, NULL, message);
        printf("%s\n", message);
    }

    // Get uniform names and locations from program 
    std::unordered_map<std::string, int> uniforms;

    int uniformCount = 0;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);

    if(uniformCount)
    {
        int maxNameLength = 0;
        GLsizei length = 0, count = 0;
        GLenum type = GL_NONE;
        glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

        char* buffer = new char[maxNameLength];
        for(int i = 0; i < uniformCount; i++)
        {
            glGetActiveUniform(ID, i, maxNameLength, &length, &count, &type, buffer);
            uniforms[std::string(buffer)] = glGetUniformLocation(ID, buffer);
        }

        delete[] buffer;
    }

    // Clean up
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    fclose(vsRaw);
    fclose(fsRaw);
    delete[] vsBuffer;
    delete[] fsBuffer;

    return { ID, uniforms };
}

struct ImageData
{
    unsigned char* data;
    int width, height, channels;
};

void CheckForCache(const char* path, const char* binPath, bool& stb_free, ImageData& idata)
{
    FILE* cachedFile = fopen(binPath, "rb");
    
    // No cache file for texture exists. Load and save it
    if(cachedFile == nullptr)
    {
        idata.data = stbi_load(path, &idata.width, &idata.height, &idata.channels, 0);
        if(idata.data == nullptr)
        {
            printf("Failed to open texture at path: %s\n", path);
            exit(-1);
        }

        // Write the metadata and the actual image's data to the cache
        FILE* outFile = fopen(binPath, "wb");
        if(outFile == nullptr)
        {
            printf("Failed to create output binary file at path: %s\n", binPath);
            exit(-1);
        }
        fprintf(outFile, "%d %d %d\n", idata.width, idata.height, idata.channels);
        fwrite(idata.data, sizeof(unsigned char) * (size_t)idata.width * idata.height * idata.channels, 1, outFile);

        // Cleanup
        fclose(outFile);
        stb_free = true;
        printf("Created cache for image at path: %s\n", path);
    }
    // Cache file exists, load that instead
    else 
    {
        // Get image metadata
        if(fscanf(cachedFile, "%d %d %d\n", &idata.width, &idata.height, &idata.channels) == EOF)
            printf("Invalid image metadata contained in file: %s\n", binPath);

        // Get the image's actual data
        size_t dataSize = (size_t)idata.width * idata.height * idata.channels;
        idata.data = new unsigned char[dataSize + 1];
        fread(idata.data, sizeof(unsigned char) * dataSize, 1, cachedFile);
        idata.data[dataSize] = '\0';

        // Cleanup
        fclose(cachedFile);
        stb_free = false;
    }
}

Texture LoadTextureFromFile(const char* path)
{
    // OpenGL textures start from lower left corner
    stbi_set_flip_vertically_on_load(true); 

    // Texture metadata
    GLuint ID;
    ImageData idata = {};
    bool stb_free = false;
    unsigned int index = Texture::GlobalTextureIndex++;

    // The cached version's path
    std::string binPath(path);
    binPath = binPath.substr(0, binPath.find_last_of('.')) + ".bin";
    
    CheckForCache(path, binPath.c_str(), stb_free, idata);

    // Generate texture from loaded data
    glGenTextures(1, &ID);

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if(idata.channels == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, idata.width, idata.height, 0, GL_RED, GL_UNSIGNED_BYTE, idata.data);
    else if(idata.channels == 2)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, idata.width, idata.height, 0, GL_RG, GL_UNSIGNED_BYTE, idata.data);
    else if(idata.channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, idata.width, idata.height, 0, GL_RGB, GL_UNSIGNED_BYTE, idata.data);
    else if(idata.channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, idata.width, idata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, idata.data);
    else
    {
        printf("Image loaded isn't in any of the supported formats!\n(Supported: RGB, RGBA)\n");
        stbi_image_free(idata.data);

        glDeleteTextures(1, &ID);
        exit(-1);
    }

    glGenerateMipmap(ID);

    if(idata.data != nullptr)
    {
        if(stb_free)
            stbi_image_free(idata.data);
        else
            delete[] idata.data;
    }

    printf("Loaded texture file at: %s\n", path);
    return { (unsigned int)idata.width, (unsigned int)idata.height, (unsigned int)idata.channels, ID, index, String("") };
}

Texture LoadCubemapFromFiles(const char* folderPath)
{
    stbi_set_flip_vertically_on_load(false);

    std::string path;
    ImageData idata = {};
    bool stb_free = false;
    unsigned int index = Texture::GlobalTextureIndex++;

    std::string paths[]
    {
        "posx",
        "negx",
        "posy",
        "negy",
        "posz",
        "negz"
    };

    GLuint ID;
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for(unsigned int i = 0; i < 6; i++)
    {
        // The cached version's path
        path = std::string(folderPath) + "/" + paths[i] + ".jpg";
        std::string binPath = std::string(folderPath) + "/" + paths[i] + ".bin";
        
        CheckForCache(path.c_str(), binPath.c_str(), stb_free, idata);

        if(idata.data == nullptr)
        {
            printf("Failed to load part of cubemap at path: %s\n", path.c_str());
            exit(-1);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, idata.width, idata.height, 0, GL_RGB, GL_UNSIGNED_BYTE, idata.data);

        if(idata.data != nullptr)
        {
            if(stb_free)
                stbi_image_free(idata.data);
            else
                delete[] idata.data;
        }
    }

    printf("Loaded cubemap from folder: %s\n", folderPath);
    return { (unsigned)idata.width, (unsigned)idata.height, (unsigned)idata.channels, ID, index, String(folderPath) };
}

Mesh LoadMeshFromOBJ(const char* path)
{
    FILE* objRaw = fopen(path, "r");
    if(!objRaw)
    {
        printf("Failed to open OBJ file at path: %s\n", path);
        exit(-1);
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;

    char buffer[100];
    while(fscanf(objRaw, "%s ", buffer) != EOF)
    {
        buffer[strlen(buffer)] = '\0';
        if(!strcmp(buffer, "v"))
        {
            float x, y, z;
            if(fscanf(objRaw, "%f %f %f\n", &x, &y, &z) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            vertices.emplace_back(x, y, z);
        }
        else if(!strcmp(buffer, "vt"))
        {
            float u, v;
            if(fscanf(objRaw, "%f %f\n", &u, &v) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            uvs.emplace_back(u, v);
        }
        else if(!strcmp(buffer, "vn"))
        {
            float x, y, z;
            if(fscanf(objRaw, "%f %f %f\n", &x, &y, &z) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            normals.emplace_back(x, y, z);
        }
        else if(!strcmp(buffer, "f"))
        {
            int v1, v2, v3, t1, t2, t3, n1, n2, n3;
            if(fscanf(objRaw, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);

            textureIndices.push_back(t1 - 1);
            textureIndices.push_back(t2 - 1);
            textureIndices.push_back(t3 - 1);

            normalIndices.push_back(n1 - 1);
            normalIndices.push_back(n2 - 1);
            normalIndices.push_back(n3 - 1);
        }
    }

    fclose(objRaw);

    // A big thank you to this answer for the algorithm.
    // https://stackoverflow.com/a/23356738
    std::vector<glm::vec3> finalVertices;
    std::vector<glm::vec2> finalUVs;
    std::vector<glm::vec3> finalNormals;
    std::vector<unsigned int> finalIndices;

    for(size_t i = 0; i < vertexIndices.size(); i++)
    {
        int vindex = vertexIndices[i];
        int uvindex = textureIndices[i];
        int nindex = normalIndices[i];
        
        finalVertices.push_back(vertices[vindex]);
        finalUVs.push_back(uvs[uvindex]);
        finalNormals.push_back(normals[nindex]);
    }

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    for(size_t i = 0; i < finalVertices.size() - 2; i += 3)
    {
        glm::vec3& P1 = finalVertices[i];
        glm::vec3& P2 = finalVertices[i + 1];
        glm::vec3& P3 = finalVertices[i + 2];

        // Calculate tangent and bitangent vectors
        glm::vec2& uv1 = finalUVs[i + 0];
        glm::vec2& uv2 = finalUVs[i + 1];
        glm::vec2& uv3 = finalUVs[i + 2];

        glm::vec3 edge1 = P2 - P1;
        glm::vec3 edge2 = P3 - P1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        /*  Resolve the following:
                Q1 = uv1.x * T + uv1.y * B
                Q2 = uv2.x * T + uv2.y * B

            Rewrite as a matrix multiplication:
                | Q1.x Q1.y Q1.z |   | uv1.x uv1.y |   | T.x T.y T.z |
                | Q2.x Q2.y Q2.z | = | uv2.x uv2.y | * | B.x B.y B.z |

            Multiply both sides with inverse uv matrix
                | T.x T.y T.z |       | uv2.y -uv1.y |   | Q1.x Q1.y Q1.z |
                | B.x B.y B.z | = f * | -uv2.x uv1.x | * | Q2.x Q2.y Q2.z |
            for f = 1 / (uv1.x * uv2.y - uv2.x * uv1.y)
        */

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent, bitangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    printf("Loaded indexed mesh from .obj file at: %s\n", path);

    Mesh result = GenerateMesh(finalVertices, finalUVs, finalNormals, tangents, bitangents);
    strncpy(result.name, path, 127);
    return result;
}