#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <ios>
#include "Display.h"
#include "Input.h"

extern "C" {
	#include <unistd.h>
}

int run(int width, int height, int dpi, std::string fbdev, std::string tsdev, std::string prompt, bool password);

int main(int argc, char** argv) {

	opterr = 0;
	int c;
	
	int width = 0;
	int height = 0;
	int dpi = 0;
	char* tsdev = "";
	char* fbdev = "";
	char* prompt = "";
	bool password = false;

	while ((c = getopt (argc, argv, "w:h:d:t:f:pP:")) != -1)
	switch (c)
	{
		case 'p':
			password = true;
			break;
		case 'P':
			prompt = optarg;
			break;
		case 'f':
			fbdev = optarg;
			break;
		case 't':
			tsdev = optarg;
			break;
		case 'd':
			dpi = std::stoi(std::string(optarg));
			break;
		case 'h':
			height = std::stoi(std::string(optarg));
			break;
		case 'w':
			width = std::stoi(std::string(optarg));
			break;
		case '?':
			std::cerr << "Unknown Arguments" << std::endl;
			return 1;
		default:
			return 3;
	}

	if(width == 0 || height == 0 || dpi == 0 || strlen(tsdev) == 0 || strlen(fbdev) == 0) {
		std::cerr << "Invalid Arguments" << std::endl;
		return 4;
	}

	std::cout << width << height << dpi << tsdev << fbdev << std::endl;

	return run(width, height, dpi, std::string(fbdev), std::string(tsdev), std::string(prompt), password);
}

int run(int width, int height, int dpi, std::string fbdev, std::string tsdev, std::string prompt, bool password) {

	std::string kbfile = "tst.ppm";

  	Display screen(width, height, dpi);
  	screen.setString(prompt);
  	screen.refresh();
  	screen.write("out.ppm");
  	system((std::string("fbsplash -s out.ppm -d ") + fbdev).c_str());
  	std::string todisplay = "";
  	std::string toreturn = "";
  	Input inputSource(tsdev);
	while(1) {
		Touch t = inputSource.getNextEvent();
		char touched = screen.keyAt(t.x, t.y);
		switch(touched) {
			case 0:
				break;
			case 0x08:
				if(toreturn.length() > 0)
					toreturn.pop_back();
				break;
			case 0x0a:
				std::cout << toreturn << std::endl;
				return 0;
				break;
			default:
				toreturn += touched;
				if(password)
					todisplay += (char)0x05;
				else
					todisplay += touched;
				break;
		}
	  	screen.setString(toreturn);
	  	screen.refresh();
	  	screen.write("out.ppm");
	  	system("fbsplash -s out.ppm -d /dev/fb0");
	}
}