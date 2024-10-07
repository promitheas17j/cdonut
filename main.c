#include <stdio.h>
#include <math.h>
#include <unistd.h>

/* ========= Constants ========== */
/* const int SCREEN_WIDTH = 100; */
#define SCREEN_WIDTH 100
/* const int SCREEN_HEIGHT = 30; */
#define SCREEN_HEIGHT 30
const float PI = 3.14159;

// Rotation speed
const float ROTATION_SPEED = 0.04;

// 3D donut size parameters
const float R1 = 1.0; // Radius of the circle spinning around the torus
const float R2 = 2.0; // Radius of the torus (distance from the centre of the hole)

// Light source position
const float LIGHT_SOURCE_Z = 5.0;

// Buffersfor storing the frame and z values
char screen[SCREEN_WIDTH][SCREEN_HEIGHT];
float z_buffer[SCREEN_WIDTH][SCREEN_HEIGHT];

// Torus resolution
const int THETA_STEPS = 100;
const int PHI_STEPS = 100;

// Scaling and viewer distance
const int SCALING_FACTOR = 2; // Adjust to fit the donut size on the screen
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

void calculate_torus_points(){
	for (int theta_step = 0; theta_step < THETA_STEPS; theta_step++) {
		// Convert theta_step to an angle theta
		float theta = (float)theta_step * 2 * PI / THETA_STEPS;

		for (int phi_step = 0; phi_step < PHI_STEPS; phi_step++){
			// Convert phi_step to angle phi
			float phi = (float)phi_step * 2 * PI / PHI_STEPS;

			float x = (R2 + R1 * cos(theta)) * cos(phi);
			float y = (R2 + R1 * cos(theta)) * sin(phi);
			float z = R1 * sin(theta);

			// Project x and y to a 2D plane using a simple perspective projection
			float x_proj = ((float)SCREEN_WIDTH / 2) + SCALING_FACTOR * x / (VIEWER_DISTANCE + z);
			float y_proj = ((float)SCREEN_HEIGHT / 2) + SCALING_FACTOR * y / (VIEWER_DISTANCE + z);

			// Calculate 1/z for depth buffering (closer points should overwrite farther ones)
			float z_inverse = 1 / (VIEWER_DISTANCE + z);
			printf("Point (%.2f, %.2f, %.2f)\n", x, y, z);

			// TODO: Store projected coordinates in buffer
		}
	}
}

void spin_donut(){
	float A = 0; // Rotation angle around x-axis
	float B = 0; // Rotation angle around z-axis

	while (1){
		clear_buffers();

		// TODO: PERFORM 3D CALCULATIONS AND PROJECTION HERE
		printf("%f\t%f", A, B);

		render_screen();

		// Increment rotaion angles
		A += ROTATION_SPEED;
		B += ROTATION_SPEED;
		
		usleep(30000); // Delay for smooth animation (30ms)
	}
}

int main(){
	printf("\x1b[2J"); // ANSI escape code to clear the screen

	calculate_torus_points();
	/* spin_donut(); */

	return 0;
}
