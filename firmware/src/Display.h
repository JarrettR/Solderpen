#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <U8x8lib.h>

#define DISPLAY_WIDTH 16
#define DISPLAY_HEIGHT 4

class Display {

    public:

        void begin();

        void clear();

        void draw(int x, int y, const char * output);

        void update();

    private:

        void clearBuffer();

        U8X8 _display;
        
        char _buffer[DISPLAY_HEIGHT*2][DISPLAY_WIDTH];
        
        bool _current;
        
};

#endif