#include <iostream>
#include "Window.h"

using namespace std;

int main() {
	cout << "Creating Window...\n";

	Window* pWindow = new Window();

	bool running = true;
	while (running) {
		if (!pWindow->ProcessMessage()) {
			cout << "Closing Window...\n";
			running = false;
		}
		Sleep(10);
	}

	delete pWindow;	
	return 0;
}
