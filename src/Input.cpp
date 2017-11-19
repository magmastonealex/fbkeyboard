#include "Input.h"
#include <iostream>

Input::Input(std::string eventDevice) {
	eventStream = std::ifstream(eventDevice, std::ios::binary);
}

Input::~Input() {
	eventStream.close();
}

Touch::Touch(int X, int Y) : x{X}, y{Y} {}

Touch Input::getNextEvent() {
	inputEvent event;
	eventStream.read((char*)&event, sizeof(event));
	int keepLooping = 1;
	int touchX;
	int touchY;
	int isDown = 0;
	while(isDown == 0) {
		while(keepLooping) {
			if(event.code == 57 && ((int)event.value != -1)) {
				isDown = 1;
			}
			if(event.code == 0x35) {
				touchX = event.value;
			}
			if(event.code == 0x36) {
				touchY = event.value;
			}
			eventStream.read((char*)&event, sizeof(event));
			if(event.type == 0 && event.code == 0 && event.value == 0) {
				keepLooping = 0;
			}
		}
		keepLooping = 1;
	}
	return Touch(touchX, touchY);
}