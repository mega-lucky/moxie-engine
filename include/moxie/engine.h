#ifndef __GAMEHANDLER_H
#define __GAMEHANDLER_H

#include "moxmath/moxmath.h"
#include "moxie/world.h"
#include "moxie/render.h"
#include "moxie/inputs.h"
#include "moxie/window.h"

int InitGame();
void TerminateGame();
void UpdateGame();
double GetDeltaTime();
double GetFPS();
void LockFPS(double FPS);
void UnlockFPS();
void StartGameLoop();

#endif