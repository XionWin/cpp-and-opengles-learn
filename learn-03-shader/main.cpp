#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "loadShader/loadShader.h"
#include "sdl2/sdl2.h"

#define FULLSCREEN 1
#define TRIANGLE_SIZE 0.5f
#define INFO_LOG_BUFFER_SIZE 1024

// struct representing a colored 2D vertex
typedef struct
{
	float x;
	float y;
	float r;
	float g;
	float b;
	float a;
} Vertex;

//  adjusting viewport and projection matrix to fit current window dimensions
void resize(int w, int h, GLint proj_mat_location);

// create rotation matrix (z axis) and upload to shader at given uniform location
void setRotationMatrix(float rad, GLint model_mat_location);

// create orthogonal matrix and upload to shader at given uniform location
void setOrthoMatrix(float left, float right, float bottom,
					float top, float n, float f, GLint proj_mat_location);

int main()
{
	SDL2 sdl2 = SDL2(true);

	SDL_Window *window = sdl2.createWindow();
	SDL_GLContext context = sdl2.createGLContext();

	// create RGB-triangle data and copy to vertex buffer
	unsigned int size = 1;
	Vertex vertices[size];

	vertices[0].x = .0f;
	vertices[0].y = .0f;
	vertices[0].r = 1.0f;
	vertices[0].g = .0f;
	vertices[0].b = .0f;
	vertices[0].a = 1.0f;


	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint shader_program = LoadShaders("shader/simplevertshader.glsl", "shader/simplefragshader.glsl");

	// linking successfull?
	int status;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// use custom shader
	glUseProgram(shader_program);

	// feed vertex attributes with triangle-data
	// position
	GLint posAttrib = glGetAttribLocation(shader_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// color
	GLint colorAttrib = glGetAttribLocation(shader_program, "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(2 * sizeof(float)));
	
	// retrieve uniform locations
	GLint proj_mat_location = glGetUniformLocation(shader_program, "proj_mat");
	GLint model_mat_location = glGetUniformLocation(shader_program, "model_mat");

	// inital resize to set viewport and projection mastrix
	SDL_DisplayMode displayMode = sdl2.getDisplayMode();
	resize(displayMode.w, displayMode.h, proj_mat_location);

	// run main loop
	printf("Rendering triangle...\n");

	sdl2.update(
		[proj_mat_location, model_mat_location, size] () {
			int quit = 0;
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN)
					quit = 1;
				else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
					resize(e.window.data1, e.window.data2, proj_mat_location);
			}
			if (quit)
			{
				printf("Program was quit by user!\n");
				exit(0);
			}
			glClearColor(.2f, .2f, .2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			setRotationMatrix(SDL_GetTicks() / 10000.f * M_PI / 2.f, model_mat_location);
			glDrawArrays(GL_TRIANGLE_FAN, 0, size);
			// glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, indices);
		}
	);
	

	// clean up
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void setRotationMatrix(float rad, GLint model_mat_location)
{
	// rotation around z axis
	float sin_angle = sin(rad);
	float cos_angle = cos(rad);
	float mat[16];
	mat[0] = cos_angle;
	mat[1] = sin_angle;
	mat[2] = 0;
	mat[3] = 0;

	mat[4] = -sin_angle;
	mat[5] = cos_angle;
	mat[6] = 0;
	mat[7] = 0;

	mat[8] = 0;
	mat[9] = 0;
	mat[10] = 1;
	mat[11] = 0;

	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 0;
	mat[15] = 1;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, mat);
}

void resize(int w, int h, GLint proj_mat_location)
{
	glViewport(0, 0, w, h);
	// set orthogonal view so that coordinates [-1, 1] area always visible and proportional on x and y axis
	if (w > h)
	{
		float f = w / (float)h;
		setOrthoMatrix(-f, f, -1, 1, -1, 1, proj_mat_location);
	}
	else
	{
		float f = h / (float)w;
		setOrthoMatrix(-1, 1, -f, f, -1, 1, proj_mat_location);
	}
}

void setOrthoMatrix(float left, float right, float bottom,
					float top, float n, float f, GLint proj_mat_location)
{
	// set orthogonal matrix
	float mat[16];
	mat[0] = 2.0f / (right - left);
	mat[1] = 0.f;
	mat[2] = 0.f;
	mat[3] = 0.f;

	mat[4] = 0.f;
	mat[5] = 2.0f / (top - bottom);
	mat[6] = 0.f;
	mat[7] = 0.f;

	mat[8] = 0.f;
	mat[9] = 0.f;
	mat[10] = -2.f / (f - n);
	mat[11] = 0.f;

	mat[12] = -(right + left) / (right - left);
	mat[13] = -(top + bottom) / (top - bottom);
	mat[14] = -(f + n) / (f - n);
	mat[15] = 1.f;
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, mat);
}
