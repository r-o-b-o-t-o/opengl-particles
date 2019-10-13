#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

class Application;

class DebugWindow {
public:
	DebugWindow(Application* app);
	void draw();

	int framerate;

private:
	Application* app;
	bool vsync;
	bool wasVSyncEnabled;

	void applyVSyncValue() const;
};

#endif