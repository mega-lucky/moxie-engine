#ifndef __GAMEHANDLER_H
#define __GAMEHANDLER_H

#include "gamemath.h"
#include "world.h"
#include "render.h"
#include "inputs.h"
#include "platform.h"

int InitGame();
void TerminateGame();
void UpdateGame();
double GetDeltaTime();
double GetFPS();
void LockFPS(double FPS);
void UnlockFPS();
void StartGameLoop();

#endif