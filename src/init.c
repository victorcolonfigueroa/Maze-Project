#include "./headers/maze.h"
#include "structs.h"

#define MAZE_WIDTH 50
#define MAZE_HEIGHT 50
#define NUM_RAIN_PARTICLES 1000

// Global variables
MiniMap mini_map;
RainParticle rain_particle[NUM_RAIN_PARTICLES];

/**
 * Function Declarations
 */
int initialize_sdl(SDL_Instance *instance);
void destroy_maze(SDL_Instance *instance);
void update_renderer(SDL_Instance *instance);
void initialize_rain_particles();
void initialize_mini_map(int maze_width, int maze_height);
void render_mini_map(SDL_Instance *instance);
void render(SDL_Instance *instance);

/**
 * main - Program entry point
 * @argc: Argument count
 * @argv: Pointer to an array of arguments
 *
 * Return: 0 on success
 */
int main(int argc, char **argv) {
    SDL_Instance instance;

    // Initialize SDL and check for errors
    if (initialize_sdl(&instance) != 0) {
        return 1;
    }

    // Initialize rain particles and minimap
    initialize_rain_particles();
    initialize_mini_map(MAZE_WIDTH, MAZE_HEIGHT);

    // Main game loop (placeholder for actual game logic)
    while (1) { // Replace with actual condition for the game loop
        // Handle events and game logic here...

        // Render frame
        render(&instance);
    }

    destroy_maze(&instance); // Clean up resources before exiting
    return 0;
}

/**
 * initialize_sdl - Initialize SDL
 * @instance: Pointer to an instance of SDL
 *
 * Return: 0 on success, 1 otherwise
 */
int initialize_sdl(SDL_Instance *instance) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error Initializing SDL2: %s\n", SDL_GetError());
        return 1;
    }

    instance->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!instance->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    instance->renderer = SDL_CreateRenderer(instance->window, -1, render_flags);
    if (!instance->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(instance->window);
        SDL_Quit();
        return 1;
    }

    instance->texture = SDL_CreateTexture(instance->renderer,
                                          SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!instance->texture) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

/**
 * destroy_maze - Release resources closing the maze
 * @instance: Pointer to an instance of the maze
 *
 * Return: Nothing
 */
void destroy_maze(SDL_Instance *instance) {
    SDL_DestroyTexture(instance->texture);
    SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow(instance->window);
    SDL_Quit();
}

/**
 * update_renderer - Update maze screen with current buffer
 * @instance: An initialized sdl instance
 */
void update_renderer(SDL_Instance *instance) {
    if (instance->textured) {
        SDL_UpdateTexture(instance->texture, NULL, buffer, SCREEN_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(instance->renderer);
        SDL_RenderCopy(instance->renderer, instance->texture, NULL, NULL);

        // Clear the buffer for the next frame
        memset(buffer, 0, sizeof(buffer)); // Clear the entire buffer to zero
    }

    // Update renderer
    SDL_RenderPresent(instance->renderer);
}

/**
 * initialize_rain_particles - Initialize rain particles
 */
void initialize_rain_particles() {
    for (int i = 0; i < NUM_RAIN_PARTICLES; i++) {
        rain_particle[i].x = rand() % SCREEN_WIDTH;
        rain_particle[i].y = rand() % SCREEN_HEIGHT;
        rain_particle[i].speed = (rand() % 5) + 2; // Random speed between 2 and 6
    }
}

/**
 * initialize_mini_map - Initialize the minimap structure
 * @maze_width: Width of the maze
 * @maze_height: Height of the maze
 */
void initialize_mini_map(int maze_width, int maze_height) {
    mini_map.width = maze_width;
    mini_map.height = maze_height;

    // Allocate memory for mini_map.map_data
    mini_map.map_data = (int **)malloc(maze_width * sizeof(int *));
    
    for (int i = 0; i < maze_width; i++) {
        mini_map.map_data[i] = (int *)malloc(maze_height * sizeof(int));
        for (int j = 0; j < maze_height; j++) {
            mini_map.map_data[i][j] = 0; // Initialize all cells to zero (no walls)
        }
    }
}

/**
 * render_mini_map - Render the minimap on the screen
 * @instance: Pointer to an instance of SDL
 */
void render_mini_map(SDL_Instance *instance) {
    int mini_map_scale = 5; // Scale factor for rendering

    // Set color for walls in minimap
    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 255); // White color for walls

    for (int i = 0; i < mini_map.width; i++) {
        for (int j = 0; j < mini_map.height; j++) {
            if (mini_map.map_data[i][j] == 1) { // Player position or wall indicator
                SDL_Rect rect = {i * mini_map_scale, j * mini_map_scale, mini_map_scale, mini_map_scale};
                SDL_RenderFillRect(instance->renderer, &rect);
            }
        }
    }

    // Draw player on minimap
    SDL_SetRenderDrawColor(instance->renderer, 255, 0, 0, 255); // Red color for player
    SDL_Rect player_rect = {pos.x * mini_map_scale, pos.y * mini_map_scale, mini_map_scale, mini_map_scale};
    
    SDL_RenderFillRect(instance->renderer, &player_rect); // Draw player on minimap
}

/**
 * render - Main rendering function to display elements on screen
 * @instance: Pointer to an instance of SDL
 */
void render(SDL_Instance *instance) {
    SDL_RenderClear(instance->renderer);

    // Render main game elements here...

    // Render the minimap if enabled
    if (instance->show_mini_map) {
        render_mini_map(instance);
    }

    update_renderer(instance); // Call update renderer at the end of render function.
}