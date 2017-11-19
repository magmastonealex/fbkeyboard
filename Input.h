#pragma once
#include <fstream>
#include <string>

extern "C" {
	#include <sys/time.h>
}

class Touch {
public:
	Touch(int X, int Y);
	int x;
	int y;
};

//from kernel source
typedef struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
} inputEvent;

class Input {
public:
	Input(std::string eventDevice);
	~Input();

	//note: will block indefinitely.
	Touch getNextEvent();
private:
	std::ifstream eventStream;
};