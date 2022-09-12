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
uint32_t separateSeparation = 50 * 50;
static const uint32_t boidSize = 15;
static const uint32_t boidSpeed = 5;

bool blahaj = false;
bool separate = true;

enum {
	MAX_BOIDS = 30,
};

static float32_t distsqr2(Vector2 v1, Vector2 v2)
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

static Image boidImage;
static Texture2D boidTexture;

static inline void drawBoids(vec2 pos[], vec2 vel[])
{
	for (size_t i = 0; i < MAX_BOIDS; i++) {
		if (blahaj) DrawTextureEx(boidTexture, pos[i], angle(vel[i]), 0.08, WHITE);
		else {
			DrawPoly(pos[i], 3, boidSize, angle(vel[i]) + 90, RED);
			DrawLineV(pos[i], add2(pos[i], scl2(unit2(vel[i]), 20)), BLUE);
		}
	}
}

// adjusts the direction vectors of boids i and j, to align graudually to the their average,
// with the suddenness of the turn determined by the tightness, if already aligned then
// don't bother doing the calculations
static void alignBoids(vec2 pos[], vec2 dir[], size_t i, size_t j)
{
	static const float32_t tightness = 1.0f / 5000.0f;  // dictates severity of turn based on separation
	float32_t distanceSquared = distsqr2(pos[i], pos[j]);

	if (lensqr2(sub2(dir[i], dir[j])) > 0.001 && distanceSquared < alingmentSeparation) {
		vec2 final = unit2(add2(dir[i], dir[j]));
		vec2 softener = scl2(final, expf(-distanceSquared * tightness));
		dir[i] = unit2(add2(dir[i], softener));
		dir[j] = unit2(add2(dir[j], softener));
	}
}

static void separateBoids(vec2 pos[], vec2 dir[], size_t i, size_t j)
{
	float32_t distanceSquared = distsqr2(pos[i], pos[j]);
	static const float tightness = 1.0f / 500.0f;

	if (distanceSquared < (separateSeparation)) {
		vec2 mid = {(pos[i].x + pos[j].x)/2, (pos[i].y + pos[j].y)/2};
		float scaler = expf(-distanceSquared * tightness); // scale the dirs by this
		vec2 difi = sub2(pos[i], mid);
		vec2 difj = sub2(pos[j], mid);
		//printf("%f\n", scaler);
		dir[i] = unit2(add2(dir[i], scl2(difi, scaler)));
		dir[j] = unit2(add2(dir[j], scl2(difj, scaler)));
	}
}

static inline void moveBoids(vec2 pos[], vec2 vel[])
{
	for (size_t i = 0; i < MAX_BOIDS; i++) {
		pos[i] = add2(pos[i], scl2(vel[i], boidSpeed));
		pos[i].x = fmodf(fmodf(pos[i].x, winW) + winW, winW);
		pos[i].y = fmodf(fmodf(pos[i].y, winH) + winH, winH);
		for (size_t j = i + 1; j < MAX_BOIDS; j++) {
			if (separate) separateBoids(pos, vel, i, j);
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


	// Init positions & velocities
	initBoids(pos, vel);
	// Init textures
	boidImage = LoadImage("crappycutout.png");
	ImageFormat(&boidImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	boidTexture = LoadTextureFromImage(boidImage);


	while (!WindowShouldClose()) {
		// Update
		if (!IsKeyDown(KEY_M)) moveBoids(pos, vel);
		if (IsKeyPressed(KEY_B)) blahaj = !blahaj;
		if (IsKeyPressed(KEY_S)) {
			separate = !separate;
			printf("separate is now %d\n", separate);
		}
		if (IsKeyDown(KEY_W)) {
			separateSeparation += 100;
			printf("square separation distance is %d\n", separateSeparation);
		}

		// Draw
		BeginDrawing();
			ClearBackground(RAYWHITE);
			drawBoids(pos, vel);
		EndDrawing();
	}

	CloseWindow();
	return EXIT_SUCCESS;
}
