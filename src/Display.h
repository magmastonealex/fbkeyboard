#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ios>
#define CHAR_X_START(margin, i, stepSizePx, scale) (margin + ((i)*(stepSizePx + (8*scale))) - scale)
#define CHAR_Y_START(entryWidth, scale, i, rowWidth)  (entryWidth + (4*scale) + (i*(rowWidth)))
typedef struct RGB
{
    unsigned char r, g, b;
} RGB;

class ImageRGB
{
	public:
    	int w, h;
    	RGB* data;
};

class Display
{
public:
	void refresh();
	void write(std::string filename);
	void setString(std::string text);
	//Get character at screen coordinates4.
	char keyAt(int x, int y);

	void toggleUpper() {
		if(currentKeyboard == &UCkeyboard) {
			currentKeyboard = &LCkeyboard;
		}
		else {
			currentKeyboard = &UCkeyboard;
		}
	}

	void toLower() {
		currentKeyboard = &LCkeyboard;
	}

	void toggleSym1() {
		if(currentKeyboard == &SYkeyboard) {
			currentKeyboard = &LCkeyboard;
		}
		else {
			currentKeyboard = &SYkeyboard;
		}
	}

	void toggleSym2() {
		if(currentKeyboard == &SY2keyboard) {
			currentKeyboard = &LCkeyboard;
		}
		else {
			currentKeyboard = &SY2keyboard;
		}
	}

	Display(int width, int height, int dpi);
	~Display();

private:
	int width;
	int height;
	int margin;
	int scale;
	int dpi;
	int entryWidth;
	int keyboardWidth;
	int rowWidth;
	int stepSizePx;
    int kbScale;
    int maxlen;

	ImageRGB* keyboardImage;

	RGB textColor;
	std::string currentText;

	char (*currentKeyboard)[3][11];

	char LCkeyboard[3][11] = {
		{'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0x08},
		{0x00, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0x0A},
		{0x0F, 0x0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', 0x0, 0x0},
	};

	char UCkeyboard[3][11] = {
		{'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0x08},
		{0x00, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0x0A},
		{0x0F, 0x0, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', 0x0, 0x0},
	};

	char SYkeyboard[3][11] = {
		{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0x08},
		{'@', '#', '$', '_', '&', '-', '+', '(', ')', '/', 0x0A},
		{0x0, '*', '"', '\'', ':', ';', '!', '?', '~', '`', '|'},
	};

	char SY2keyboard[3][11] = {
		{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x08},
		{0x0, '=', '<', '>', '[', ']', '\\', '^', '{', '}', 0x0A},
		{0x0, '.', ',', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
	};

	void set_px (RGB value, int x, int y, int scale);
	void print_letter(char letter, int atX, int atY, int scale, bool inverted = false);
	int kb_putc(char character);
	void fastRect(int atX, int atY, int width, int height, RGB color);
	void xLine(int startX, int y, RGB color);
	void yLine(int x, int startY, RGB color);
};

#endif