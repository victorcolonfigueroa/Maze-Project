#ifndef STRUCTS_H
#define STRUCTS_H

#include "macros.h"
/**
 * struct SDL_Instance - structure to hold initialize variables for SDL2
 * @window: sdl window
 * @renderer: sdl renderer
 * @textures: sdl texture surfaces
 * @texture: SDL texture
 * @textured: 0 if not, 1 otherwise
 */
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	/* SDL_Surface *textures[TEX_COUNT]; */
	SDL_Texture *texture;
	int textured;
	int show_mini_map;
} SDL_Instance;


/**
 * struct point - structure to hold x and y coordinate points
 * @x: double x coordinate
 * @y: double y coordinate
 *
 * Description: structure holds x and y coordinate points of type double
 */
typedef struct point
{
	double x;
	double y;
} point;



typedef struct
{
	float x, y;
	float speed;
} RainParticle;

typedef struct
{
	int width, height;
	int **map_data;
} MiniMap;

struct SDL_Instance {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Texture *weapon1_texture;
	SDL_Texture *weapon2_texture;
	int textured;
	int show_mini_map;
};

#endif /* STRUCTS_H */