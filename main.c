#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define vec2sub Vector2Subtract
#define vec2add Vector2Add
#define vec2len Vector2Length
#define vec2scl Vector2Scale
#define vec2ang Vector2Angle
#define vec2lensqr Vector2LengthSqr
#define vec2 Vector2
#define vec2dot Vector2DotProduct
#define vec2norm Vector2Normalize
#define scrnH 800
#define scrnW 1200
#define colour Color
#define boidSize 20
#define SPEED 4

void initBoids(vec2 **Positions, vec2 **Velocities, int boids, int *size) {
	free(*Positions);
	free(*Velocities);

	*Positions = calloc(*size * 1.5, sizeof(Vector2));
	*Velocities = calloc(*size * 1.5, sizeof(Vector2));
	*size = *size * 1.5;

	for (int i=0; i < boids; i++) {
		(*Positions)[i] = (vec2){(rand() % scrnW - boidSize) + boidSize*2, (rand() % scrnH-boidSize) + boidSize*2};
		(*Velocities)[i] = (vec2){(rand() % SPEED) - (rand() % SPEED), (rand() % SPEED) - (rand() % SPEED)};
	}
}

float angle(vec2 input) {
	vec2 base = {0, 1};
	input = vec2norm(input);
	float dot = vec2dot(base, input) / (vec2lensqr(base) * vec2lensqr(input));
	dot = acos(dot) * 57.2957795131;

	if (input.x < 0) dot = dot + 180;
	if (input.x >= 0) dot = 180 - dot;

	printf("%g\n", dot);
	return dot;
}

int main(void)
{
    InitWindow(scrnW, scrnH, "birdies :3");
    SetTargetFPS(60);

	int boids = 10;
	int size;
	size = boids;

	vec2 *Positions = NULL;
	vec2 *Velocities = NULL;

	initBoids(&Positions, &Velocities, boids, &size);


    while (!WindowShouldClose())
    {
		// GAME LOGIC HERE
		for (int i = 0; i < boids; i++) {
			Positions[i] = vec2add(Positions[i], Velocities[i]);
		}

		// DRAWING HERE
		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (int i = 0; i < boids; i++) {
			DrawPoly(Positions[i], 3, boidSize, angle(Velocities[i]) + 180, RED);
			DrawLineV(Positions[i], vec2add(Positions[i], vec2scl(vec2norm(Velocities[i]), 30)), BLUE);
		}

		EndDrawing();
    }

    CloseWindow();

    return 0;
}

