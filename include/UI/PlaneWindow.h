#ifndef PLANE_WINDOW_H
#define PLANE_WINDOW_H

class Application;

class PlaneWindow {
public:
	PlaneWindow(Application* app);
	void draw();

private:
	Application* app;
	int size;
};

#endif