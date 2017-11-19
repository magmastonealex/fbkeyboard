#include "Display.h"

#include "font.h"

Display::Display(int width, int height, int dpi) : width{width}, height{height}, dpi{dpi} {
	keyboardImage = new ImageRGB();
	keyboardImage->w = width;
	keyboardImage->h = height;
	keyboardImage->data = new RGB[width*height];

	//Initialize background to grey.
	//Doing not-grey would be much slower.
	memset((unsigned char*) keyboardImage->data, 0x70, width*height*3);

	margin = width / 33;
	scale = dpi / 34;

	entryWidth = dpi/3;

	keyboardWidth = width - entryWidth;

	rowWidth = keyboardWidth/4;

	stepSizePx = (height-(2*margin) - (11*8*scale))/10;

	kbScale = scale-2;

	if(kbScale < 0) {
    	kbScale = 1;
    }

    maxlen = (height-(2*margin)) / ((scale*8) + 4);

    currentKeyboard = &LCkeyboard;
}

Display::~Display() {
	delete keyboardImage->data;
	delete keyboardImage;
}

void Display::write(std::string filename){
	std::ofstream f2(filename, std::ios::binary);
    f2 << "P6" << std::endl;
    f2 << std::to_string(width) << " " << std::to_string(height) << std::endl;
    f2 << "255" << std::endl;
    f2.write((char*)keyboardImage->data, width*height*3);
}

void Display::set_px (RGB value, int x, int y, int scale) {
	for(int xscale = 0; xscale < scale; xscale++) {
		for(int yscale=0; yscale < scale; yscale++) {
			keyboardImage->data[keyboardImage->w*(y+yscale) + x+xscale].r = value.r;
			keyboardImage->data[keyboardImage->w*(y+yscale) + x+xscale].g = value.g;
			keyboardImage->data[keyboardImage->w*(y+yscale) + x+xscale].b = value.b;				
		}
	}
}

void Display::print_letter(char letter, int atX, int atY, int scale, bool inverted) {
	RGB white = {0xFF, 0xFF, 0xFF};
	RGB black = {0x10, 0x10, 0x10};
	for (int y=0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if(font8x8_basic[(uint8_t) letter][x] & (uint8_t)(1 << (7-(y)))) {
				set_px(inverted ? black : white, atX + (x*scale), atY + (y*scale), scale);
			}
		}
	}
}

void Display::fastRect(int atX, int atY, int width, int height, RGB color) {
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			keyboardImage->data[((atY+y)*keyboardImage->w) + (atX + x)].r = color.r;
			keyboardImage->data[((atY+y)*keyboardImage->w) + (atX + x)].g = color.g;
			keyboardImage->data[((atY+y)*keyboardImage->w) + (atX + x)].b = color.b;
		}
	}
}

void Display::xLine(int startX, int y, RGB color) {
	for(int x = startX; x < keyboardImage->w; x++) {
		keyboardImage->data[((y)*keyboardImage->w) + x].r = color.r;
		keyboardImage->data[((y)*keyboardImage->w) + x].g = color.g;
		keyboardImage->data[((y)*keyboardImage->w) + x].b = color.b;
	}
}

void Display::yLine(int x, int startY, RGB color) {
	for(int y = startY; y < keyboardImage->h; y++) {
		keyboardImage->data[((y)*keyboardImage->w) + x].r = color.r;
		keyboardImage->data[((y)*keyboardImage->w) + x].g = color.g;
		keyboardImage->data[((y)*keyboardImage->w) + x].b = color.b;
	}
}


char Display::keyAt(int x, int y) {
	if(x < entryWidth) {
		return 0;
	}

	for(int xCheck = 1; xCheck < 5; xCheck++) {
		//determine row of touch.

		int lastRowStart = CHAR_Y_START(entryWidth, scale, xCheck - 1, rowWidth);
		int rowStart = CHAR_Y_START(entryWidth, scale, xCheck, rowWidth);

		if(x > lastRowStart && x < rowStart) {
			//std::cout << "row: " << xCheck << std::endl;
			for(int yCheck = 1; yCheck < 12; yCheck++) {
				int lastCharStart = CHAR_X_START(margin, yCheck-1, stepSizePx, scale);
				int thisCharStart = CHAR_X_START(margin, yCheck, stepSizePx, scale);
				if(y > lastCharStart && y < thisCharStart) {
					//std::cout << "col: " << yCheck << std::endl;
					if(xCheck > 3) {
						std::cout << "col: " << yCheck << std::endl;
						switch(yCheck) {
							case 10:
								toggleSym2();
								return 0;
							case 11:
								toggleSym1();
								return 0;
							default:
								toLower();
								return ' ';
						}
					}
					char touched = (*currentKeyboard)[(xCheck-1)][10-(yCheck-1)];

					if(touched == 0x0F) {
						toggleUpper();
						return 0;
					}
					else if(touched == 0x00) {
						toLower();
					}

					return touched;
				}
			}
		}
	}
	return 0;
}

void Display::refresh() {
	memset((unsigned char*) keyboardImage->data, 0x70, width*height*3);

	for (int i = 0; i < 3; i++) {
    	for (int z = 0; z < 11; z++) {
    		print_letter((*currentKeyboard)[i][10-z], CHAR_Y_START(entryWidth, scale, i, rowWidth), CHAR_X_START(margin, z, stepSizePx, scale), scale);
    	}
    }

    print_letter(0x01, CHAR_Y_START(entryWidth, scale, 3, rowWidth) , CHAR_X_START(margin, 9, stepSizePx, scale), scale);
    print_letter(0x03, CHAR_Y_START(entryWidth, scale, 3, rowWidth), CHAR_X_START(margin, 10, stepSizePx, scale), scale);


    //event: 4 code 4 value: 42
    RGB color = {0xFF, 0xFF, 0xFF};
	fastRect(0, 0, entryWidth, height, color);
	for(int i=1; i < 11; i++) {
		int lastCharEnd = (CHAR_X_START(margin, i-1, stepSizePx, scale) + (8*scale));
		int thisCharStart = CHAR_X_START(margin, i, stepSizePx, scale);
		xLine(0, (lastCharEnd + thisCharStart) / 2 , color);
	}


	int lastLine = entryWidth;
    for(; lastLine <= width; lastLine+=rowWidth) {
    	yLine(lastLine, 0, color);
    }
    lastLine-=rowWidth*2;

	int lastCharEnd = (CHAR_X_START(margin, 3-1, stepSizePx, scale)+(8*scale));
	int thisCharStart = CHAR_X_START(margin, 3, stepSizePx, scale);

    fastRect(lastLine, (lastCharEnd + thisCharStart) / 2, rowWidth, (stepSizePx + (8*scale))*5, color );

    for(uint8_t i = 0; i < currentText.length() && i < maxlen; i++) {
    	print_letter(currentText.at(i), (entryWidth/2) - ((kbScale*8)/2), height - margin - (kbScale*8) - (i*((kbScale*8) + 4)), kbScale, true);
    }
}

void Display::setString(std::string text){
	currentText = text;
}