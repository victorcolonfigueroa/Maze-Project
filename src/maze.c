#include "./headers/maze.h"
#include "structs.h"

#define NUM_RAIN_PARTICLES 100 // Define the number of rain particles

/* Global Variables */
uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
uint32_t tiles[TEX_COUNT][TEX_HEIGHT][TEX_WIDTH];
point pos, dir, plane;
double time;

typedef struct {
    int width;
    int height;
    int **map_data;
} MiniMap;

MiniMap mini_map;
RainParticle rain_particle[NUM_RAIN_PARTICLES];

/**
 * Function Declarations
 */
void initialize_mini_map(int width, int height);
void update_mini_map(int player_x, int player_y);
void render_mini_map(SDL_Instance *instance);

/**
 * main - Program entry point
 * @argc: Argument count
 * @argv: Pointer to an array of arguments
 *
 * Return: 0 on success
 */
int main(int argc, char **argv) {
    int *maze_map;
    char *map_dir = "maps/", *map_path = NULL;
    SDL_Instance instance;

    /* Initialize key variables */
    instance.textured = 1;
    instance.show_mini_map = 1; // Enable minimap
    pos.x = 1; pos.y = 12; // Initialize player position
    dir.x = 1; dir.y = -0.66; 
    plane.x = 0; plane.y = 0.66;
    time = 0;

    // Initialize maze and mini-map dimensions
    initialize_maze(&instance);
    initialize_mini_map(10, 10); // Initialize mini_map with appropriate dimensions

    // Read maze map from file
    if (argc > 1) {
        map_path = strcat(map_dir, argv[1]);
        maze_map = read_map(map_path);
        if (!maze_map) return (1);
    } else {
        fprintf(stderr, "No map provided.\n");
        return (1);
    }

    // Initialize rain particles
    for (int i = 0; i < NUM_RAIN_PARTICLES; i++) {
        rain_particle[i].x = rand() % SCREEN_WIDTH;
        rain_particle[i].y = rand() % SCREEN_HEIGHT;
        rain_particle[i].speed = 1 + rand() % 5;
    }

    while (maze_loop(&instance) == 0) {
        if (!instance.textured)
            render_untextured_floor_ceiling(&instance);

        ray_cast(&instance, maze_map);

        /* Handle events */
        update_position(maze_map);

        // Update and render the mini-map
        update_mini_map(pos.x, pos.y);

        // Render the frame
        render(&instance);
    }

    destroy_maze(&instance);
    free(maze_map);

    return (0);
}

/**
 * initialize_mini_map - Initialize the minimap structure
 * @width: Width of the minimap
 * @height: Height of the minimap
 */
void initialize_mini_map(int width, int height) {
    mini_map.width = width;
    mini_map.height = height;

    mini_map.map_data = malloc(mini_map.width * sizeof(int *));

    for (int i = 0; i < mini_map.width; i++) {
        mini_map.map_data[i] = malloc(mini_map.height * sizeof(int));
        for (int j = 0; j < mini_map.height; j++) {
            mini_map.map_data[i][j] = 0; // Initialize all cells to zero (no walls)
        }
    }
}

/**
 * update_mini_map - Update the minimap with the player's position
 * @player_x: Player's x position in the maze
 * @player_y: Player's y position in the maze
 */
void update_mini_map(int player_x, int player_y) {
    // Clear the mini-map
    for (int i = 0; i < mini_map.width; i++)
        for (int j = 0; j < mini_map.height; j++)
            mini_map.map_data[i][j] = 0;

    // Update player's position on the mini-map
    if (player_x >= 0 && player_x < mini_map.width && player_y >= 0 && player_y < mini_map.height) {
        mini_map.map_data[player_x][player_y] = 1; // Mark player's position
    }
}

/**
 * render_mini_map - Render the minimap on the screen
 * @instance: Pointer to an instance of SDL
 */
void render_mini_map(SDL_Instance *instance) {
    int mini_map_scale = 5; // Scale factor for the minimap

    SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 255); // White color for walls

    for (int i = 0; i < mini_map.width; i++)
        for (int j = 0; j < mini_map.height; j++)
            if (mini_map.map_data[i][j] == 1) { // Player position
                SDL_Rect rect = {i * mini_map_scale, j * mini_map_scale, mini_map_scale, mini_map_scale};
                SDL_RenderFillRect(instance->renderer, &rect);
            }

    SDL_SetRenderDrawColor(instance->renderer, 255, 0, 0, 255); // Red color for player
    SDL_Rect player_rect = {pos.x * mini_map_scale, pos.y * mini_map_scale, mini_map_scale, mini_map_scale};

    SDL_RenderFillRect(instance->renderer, &player_rect); // Draw player on minimap
}