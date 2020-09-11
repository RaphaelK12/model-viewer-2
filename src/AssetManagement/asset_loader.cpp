#include "asset_loader.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <stb_image.h>
#include <fstream>

Shader LoadShadersFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    // Load files into memory
    FILE* vsRaw = fopen(vertexShaderPath, "rb");
    if(!vsRaw)
    {
        printf("Failed to open file at path: %s\n", vertexShaderPath);
        exit(-1);
    }

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
    glGetProgramiv(ID, GL_LINK_STATUS, &compiled);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLength);
    if(infoLength > 0)
    {
        std::vector<char> ProgramErrorMessage(infoLength + 1);
        glGetProgramInfoLog(ID, infoLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
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

            uniforms[std::string(buffer, length)] = glGetUniformLocation(ID, buffer);
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

Texture LoadTextureFromFile(const char* path)
{
    unsigned int index = Texture::GlobalTextureIndex++;
    
    GLuint ID;
    int width, height, channels;
    unsigned char* data = nullptr;
    bool stb_free = false;

    // Load image from file
    stbi_set_flip_vertically_on_load(true);

    std::string binPath(path);
    binPath = binPath.substr(0, binPath.find_last_of('.')) + ".bin";
    FILE* cachedFile = fopen(binPath.c_str(), "rb");
    
    // No cache file for texture exists. Load and save it
    if(cachedFile == nullptr)
    {
        data = stbi_load(path, &width, &height, &channels, 0);
        if(data == nullptr)
        {
            printf("Failed to open texture at path: %s\n", path);
            exit(-1);
        }

        // Make a cache
        FILE* outFile = fopen(binPath.c_str(), "wb");
        if(outFile == nullptr)
        {
            printf("Failed to create output binary file at path: %s\n", binPath.c_str());
            exit(-1);
        }
        fprintf(outFile, "%d %d %d\n", width, height, channels);
        fwrite(data, sizeof(unsigned char) * size_t(width * height * channels), 1, outFile);

        // Cleanup
        fclose(outFile);
        stb_free = true;
    }
    else // Cache file exists, load that instead
    {
        FILE* inFile = fopen(binPath.c_str(), "rb");
        if(inFile == nullptr)
        {
            printf("Failed to open input binary file at path: %s\n", binPath.c_str());
            exit(-1);
        }

        // Get image metadata
        fscanf(inFile, "%d %d %d\n", &width, &height, &channels);

        // Read remaining unsigned chars
        size_t dataSize = width * height * channels;
        data = new unsigned char[dataSize + 1];
        fread(data, sizeof(unsigned char) * dataSize, 1, inFile);
        data[dataSize] = '\0';

        // Cleanup
        fclose(inFile);
        stb_free = false;
    }

    glGenTextures(1, &ID);

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(channels == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    else if(channels == 2)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
    else if(channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
    {
        printf("Image loaded isn't in any of the supported formats!\n(Supported: RGB, RGBA)\n");
        stbi_image_free(data);

        glDeleteTextures(1, &ID);
        exit(-1);
    }

    glGenerateMipmap(ID);

    if(data != nullptr)
        if(stb_free)
            stbi_image_free(data);
        else
            delete[] data;

    printf("Loaded texture file at: %s\n", path);
    return { (unsigned int)width, (unsigned int)height, (unsigned int)channels, ID, index };
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
    strcpy(result.name, path);
    return result;
}