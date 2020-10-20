#include "loadShader.h"
#include <assert.h>

GfxShader vertexShader;
GfxShader fragmentShader;
GfxProgram program;

#define check() assert(glGetError() == 0)

//void LoadShaders(){
GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
{
        vertexShader.loadVertexShader(vertex_file_path);       //"simplevertshader.glsl");
        fragmentShader.loadFragmentShader(fragment_file_path); //"simplefragshader.glsl");
        program.create(&vertexShader, &fragmentShader);
        check();
        glLinkProgram(program.getId());
        check();
        glDeleteShader(vertexShader.getId());
        glDeleteShader(fragmentShader.getId());
        return program.getId();
}

bool GfxShader::loadVertexShader(const char *filename)
{
        //cheeky bit of code to read the whole file into memory
        assert(!this->Src);
        FILE *file = fopen(filename, "rb");
        assert(file);
        fseek(file, 0, SEEK_END);
        int sz = ftell(file);
        fseek(file, 0, SEEK_SET);
        this->Src = new GLchar[sz + 1];
        fread(Src, 1, sz, file);
        this->Src[sz] = 0; //null terminate it!
        fclose(file);

        //now create and compile the shader
        this->ShaderType = GL_VERTEX_SHADER;
        this->Id = glCreateShader(this->ShaderType);
        glShaderSource(this->Id, 1, (const GLchar **)&this->Src, 0);
        glCompileShader(this->Id);
        check();

        return true;
}

bool GfxShader::loadFragmentShader(const char *filename)
{
        //cheeky bit of code to read the whole file into memory
        assert(!this->Src);
        FILE *file = fopen(filename, "rb");
        assert(file);
        fseek(file, 0, SEEK_END);
        int sz = ftell(file);
        fseek(file, 0, SEEK_SET);
        this->Src = new GLchar[sz + 1];
        fread(Src, 1, sz, file);
        this->Src[sz] = 0; //null terminate it!
        fclose(file);

        //now create and compile the shader
        this->ShaderType = GL_FRAGMENT_SHADER;
        this->Id = glCreateShader(this->ShaderType);
        glShaderSource(this->Id, 1, (const GLchar **)&this->Src, 0);
        glCompileShader(this->Id);
        check();
        return true;
}

bool GfxProgram::create(GfxShader *vertex_shader, GfxShader *fragment_shader)
{
        VertexShader = vertex_shader;
        FragmentShader = fragment_shader;
        this->Id = glCreateProgram();
        glAttachShader(this->Id, VertexShader->getId());
        glAttachShader(this->Id, FragmentShader->getId());
        glLinkProgram(this->Id);
        check();

        return true;
}