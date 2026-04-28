#ifndef PLATFORM_H
#define PLATFORM_H

typedef void (*WindowResizeCallback)(int Width, int Height);

int InitWindow();
int ShouldWindowClose();
int GetWindowWidth();
int GetWindowHeight();
float GetWindowAspectRatio();
void TerminateWindow();
void SwapFrameBuffers();
double GetTime();
void BindToWindowResize(WindowResizeCallback cb);

#endif