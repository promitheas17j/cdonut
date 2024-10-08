#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "util.h"

/* ========= Constants ========== */
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 35
const float PI = 3.14159;

// Rotation value
float rotation_x = 0.0;
float rotation_z = 0.0;

// 3D donut size parameters
const float R1 = 0.7; // Radius of the circle spinning around the torus
const float R2 = 1.7; // Radius of the torus (distance from the centre of the hole)

// Light source position
const float LIGHT_SOURCE_Z = 5.0;

// Buffersfor storing the frame and z values
char screen[SCREEN_WIDTH][SCREEN_HEIGHT];
float z_buffer[SCREEN_WIDTH][SCREEN_HEIGHT];

// Torus resolution
const int THETA_STEPS = 200;
const int PHI_STEPS = 200;

// Scaling and viewer distance
const int SCALING_FACTOR = 20; // Adjust to fit the donut size on the screen
const int VIEWER_DISTANCE = 4; // Distance of viewer from the donut in z-axis


// Clear the screen buffer
void clear_buffers(){
	for (int i = 0; i < SCREEN_WIDTH; i++){
		for (int j = 0; j < SCREEN_HEIGHT; j++){
			screen[i][j] = ' ';
			z_buffer[i][j] = 0;
		}
	}
}

// Print the buffer to the terminal
void render_screen(){
	printf("\x1b[H"); // Move cursor to top of screen
	for (int j = 0; j < SCREEN_HEIGHT; j++){
		for (int i = 0; i < SCREEN_WIDTH; i++){
			putchar(screen[i][j]);
		}
		putchar('\n');
	}
}

void rotate(float* x, float* y, float* z){
	// Rotate around the x-axis
	float y_rot = (*y * cos(rotation_x)) - (*z * sin(rotation_x));
	float z_rot = (*y * sin(rotation_x)) + (*z * cos(rotation_x));

	// Rotate around the z-axis
	float x_rot = (*x * cos(rotation_z)) - (y_rot * sin(rotation_z));
	float new_y = (*x * sin(rotation_z)) + (y_rot * cos(rotation_z));

	*x = x_rot;
	*y = new_y;
	*z = z_rot;
}

void calculate_torus_points(){
	for (int theta_step = 0; theta_step < THETA_STEPS; theta_step++) {
		// Convert theta_step to an angle theta
		float theta = (float)theta_step * 2 * PI / THETA_STEPS;

		for (int phi_step = 0; phi_step < PHI_STEPS; phi_step++){
			// Convert phi_step to angle phi
			float phi = (float)phi_step * 2 * PI / PHI_STEPS;

			// Parametric equations for a torus
			float x = (R2 + R1 * cos(theta)) * cos(phi);
			float y = (R2 + R1 * cos(theta)) * sin(phi);
			float z = R1 * sin(theta);

			// Apply rotation to the point
			rotate(&x, &y, &z);

			// Project x and y to a 2D plane using a simple perspective projection
			float x_proj = ((float)SCREEN_WIDTH / 2) + SCALING_FACTOR * x / (VIEWER_DISTANCE + z);
			float y_proj = ((float)SCREEN_HEIGHT / 2) + SCALING_FACTOR * y / (VIEWER_DISTANCE + z);

			// Calculate 1/z for depth buffering (closer points should overwrite farther ones)
			float z_inverse = 1 / (VIEWER_DISTANCE + z);

			int x_pos = (int)x_proj;
			int y_pos = (int)y_proj;

			if (x_pos >= 0 && x_pos < SCREEN_WIDTH && y_pos >= 0 && y_pos < SCREEN_HEIGHT){
				// Depth test: check if this point is closer than previous points
				if (z_inverse > z_buffer[x_pos][y_pos]){
					// Update the z_buffer with the closer point
					z_buffer[x_pos][y_pos] = z_inverse;
					/* log_message("info", "z_inverse value", "float", z_inverse); */

					/*
						Choose an ASCII character to represent the donut surface
						e.g. From brightest to least bright, with 4 total brightness values:
						@, O, o, .
					*/
					// TODO: determine how I should separate my z_inverse values so that the depth looks good and it is independent from scale, and the radii
					if (z_inverse < 0.1) {
						screen[x_pos][y_pos] = '@';
					}
					else if (z_inverse >= 0.1 && z < 0.3) {
						screen[x_pos][y_pos] = 'O';
					}
					else if (z_inverse >= 0.3 && z < 0.5) {
						screen[x_pos][y_pos] = 'o';
					}
					else { // (z >= 0.6)
						screen[x_pos][y_pos] = '.';
					}
				}
			}
		}
	}
}

int main(){
	printf("\x1b[2J"); // ANSI escape code to clear the screen

	while (1){
		clear_buffers();
		calculate_torus_points();
		render_screen();

		// Increment rotaion angles
		rotation_x += 0.05;
		rotation_z += 0.03;
		
		usleep(30000); // Delay for smooth animation (30ms)
	}

	return 0;
}
