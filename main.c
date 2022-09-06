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
#define vec2dist Vector2Distance
#define vec2mul Vector2Multiply
#define colour Color
#define boidSize 20
#define SPEED 4
#define scrnH 800
#define scrnW 1200

float randZero() {
	return rand() / (float)RAND_MAX;
}

void initBoids(vec2 **Positions, vec2 **Velocities, int boids, int *size) {
	free(*Positions);
	free(*Velocities);

	*Positions = calloc(*size * 1.5, sizeof(Vector2));
	*Velocities = calloc(*size * 1.5, sizeof(Vector2));
	*size = *size * 1.5;

	for (int i=0; i < boids; i++) {
		(*Positions)[i] = (vec2){(rand() % scrnW - boidSize) + boidSize*2, (rand() % scrnH-boidSize) + boidSize*2};
		(*Velocities)[i] = vec2norm((vec2){randZero() - randZero(), randZero() - randZero()});
		if (vec2lensqr((*Velocities)[i]) == 0) {
			(*Velocities)[i] = (vec2){4,4};
		}
		(*Velocities)[i] = vec2scl((*Velocities)[i], SPEED);
	}
}

float angle(vec2 input) {
	vec2 base = {0, 1};
	input = vec2norm(input);
	float dot = vec2dot(base, input) / (vec2lensqr(base) * vec2lensqr(input));
	dot = acos(dot) * 57.2957795131;

	if (input.x < 0) dot = dot + 180;
	if (input.x >= 0) dot = 180 - dot;

	return dot;
}

int main(void)
{
    InitWindow(scrnW, scrnH, "birdies :3");
    SetTargetFPS(60);

	int boids = 20;
	int size;
	size = boids;

	vec2 *Positions = NULL;
	vec2 *Velocities = NULL;
	vec2 tempV[boids];

	initBoids(&Positions, &Velocities, boids, &size);


    while (!WindowShouldClose())
    {
		// GAME LOGIC HERE
		for (int i = 0; i < boids; i++) {
			vec2 vel = Velocities[i];
			Positions[i] = vec2add(Positions[i], Velocities[i]);
			// looping boids back round
			Positions[i].x = fmod(scrnW - (fmod((scrnW - Positions[i].x), scrnW)), scrnW);
			Positions[i].y = fmod(scrnH - (fmod((scrnH - Positions[i].y), scrnH)), scrnH);
			vec2 dir = vec2norm(Velocities[i]);

			for (int j = i+1; j < boids; j++) {
				if (abs(vec2dist(Positions[i], Positions[j])) < 150) {
					dir = vec2add(dir, vec2norm(Velocities[j]));
					dir = vec2norm(dir);
					if (abs(vec2dist(Positions[i], Positions[j])) < 30) { // separation
						// should do the same as with tempV but with tempP :3
					}
				}
			}
			tempV[i] = vec2scl(dir, SPEED);
			printf("%f\n", vec2len(vec2scl(dir, SPEED)));
		}


		// DRAWING HERE
		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (int i = 0; i < boids; i++) {
			Velocities[i] = tempV[i];
			DrawPoly(Positions[i], 3, boidSize, angle(Velocities[i]) + 180, RED);
			DrawLineV(Positions[i], vec2add(Positions[i], vec2scl(vec2norm(Velocities[i]), 30)), BLUE);
		}

		EndDrawing();
    }

    CloseWindow();

    return 0;
}

