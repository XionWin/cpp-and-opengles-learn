#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

class GfxShader
{
    GLuint Id;
    GLchar *Src;
    GLuint ShaderType;

public:
    GfxShader() : Src(NULL), Id(0), ShaderType(0) {}
    ~GfxShader()
    {
        if (Src)
            delete[] Src;
    }
    bool loadVertexShader(const char *filename);
    bool loadFragmentShader(const char *filename);
    GLuint getId() { return Id; }
};

class GfxProgram
{
    GLuint Id;
    GfxShader *VertexShader;
    GfxShader *FragmentShader;

public:
	GfxProgram() {}
	~GfxProgram() {}
	bool create(GfxShader* vertexShader, GfxShader* fragmentShader);
    GLuint getId() { return Id; }
};