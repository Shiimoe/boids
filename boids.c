#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef float float32_t;
typedef double float64_t;

#define sub2 Vector2Subtract
#define add2 Vector2Add
#define addval2 Vector2AddValue
#define len2 Vector2Length
#define scl2 Vector2Scale
#define ang2 Vector2Angle
#define lensqr2 Vector2LengthSqr
#define vec2 Vector2
#define dot2 Vector2DotProduct
#define unit2 Vector2Normalize
#define dist2 Vector2Distance
#define mul2 Vector2Multiply
#define div2 Vector2Divide
#define colour Color

static const uint32_t scrnW = 1920;
static const uint32_t scrnH = 1080;
static uint32_t winW;
static uint32_t winH;

static const uint32_t alingmentSeparation = 150 * 150;
static const uint32_t boidSize = 15;
static const uint32_t boidSpeed = 5;

enum {
	MAX_BOIDS = 30,
};

static float distsqr2(Vector2 v1, Vector2 v2)
{
    return ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}


void initBoids(vec2 pos[], vec2 vel[])
{
	for (size_t i = 0; i < MAX_BOIDS; i++) {
		pos[i] = (vec2){
			(rand() % winW - boidSize) + boidSize * 2,
			(rand() % winH - boidSize) + boidSize * 2
		};
		float32_t randAng = (rand() / (float)RAND_MAX) * 2 * PI;
		vel[i] = (vec2){
			cosf(randAng),
			sinf(randAng)
		};
	}
}

static float32_t angle(vec2 dir)
{
	return (atan2f(dir.y, dir.x) + PI) * (180/PI);
}

float gauss(float x, float tightness)
{
	return expf(-tightness * x);
}

static inline void drawBoids(vec2 pos[], vec2 vel[])
{
	for (size_t i = 0; i < MAX_BOIDS; i++) {
		DrawPoly(pos[i], 3, boidSize, angle(vel[i]) + 90, RED);
		DrawLineV(pos[i], add2(pos[i], scl2(unit2(vel[i]), 20)), BLUE);
	}
}

static void alignBoids(vec2 pos[], vec2 dir[], size_t i, size_t j)
{
	static const float tightness = 1.0/5000;
	float distanceSquared = distsqr2(pos[i], pos[j]);
	if (len2(sub2(dir[i], dir[j])) > 0.02 && distanceSquared < alingmentSeparation) {
		vec2 final = unit2(add2(dir[i], dir[j]));
		vec2 softener = scl2(final, gauss(distanceSquared, tightness));
		dir[i] = unit2(add2(dir[i], softener));
		dir[j] = unit2(add2(dir[j], softener));
	}
}

static inline void moveBoids(vec2 pos[], vec2 vel[])
{
	for (size_t i = 0; i < MAX_BOIDS; i++) {
		pos[i] = add2(pos[i], scl2(vel[i], boidSpeed));
		pos[i].x = fmodf(fmodf(pos[i].x, winW) + winW, winW);
		pos[i].y = fmodf(fmodf(pos[i].y, winH) + winH, winH);
		for (size_t j = i + 1; j < MAX_BOIDS; j++) {
			alignBoids(pos, vel, i, j);
		}
	}
}

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(scrnW, scrnH, "boids");
	SetTargetFPS(60);

	winW = GetRenderWidth();
	winH = GetRenderHeight();

	vec2 pos[MAX_BOIDS];
	vec2 vel[MAX_BOIDS];

	initBoids(pos, vel);

	while (!WindowShouldClose()) {
		// Update
		moveBoids(pos, vel);

		// Draw
		BeginDrawing();
			ClearBackground(RAYWHITE);
			drawBoids(pos, vel);
		EndDrawing();
	}

	CloseWindow();
	return EXIT_SUCCESS;
}
