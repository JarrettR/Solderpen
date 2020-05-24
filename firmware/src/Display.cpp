#include "Display.h"


void Display::begin() {
    _display = U8X8_SSD1306_128X64_NONAME_HW_I2C();
    _display.begin();
    _display.setFont(u8x8_font_artossans8_r);
    clear();
    delay(50);
}

void Display::clear() {
    for(int i=0; i<DISPLAY_HEIGHT*2; i++) for(int j=0; j<=DISPLAY_WIDTH; j++) _display.drawString(j,i*5, " ");
}

void Display::clearBuffer() {
    int buff = _current?DISPLAY_HEIGHT:0;
    for(int i=0; i<DISPLAY_HEIGHT; i++) 
        for(int j=0; j<DISPLAY_WIDTH; j++) 
            _buffer[i+buff][j] = ' ';
}

void Display::draw(int x, int y, const char * output) {
    if(_current) y += 4;
    for(int i=x; output[i-x] != 0 && i<DISPLAY_WIDTH; i++) {
       _buffer[y][i] = output[i-x];
    }
}

void Display::update() {
    for(int i=0; i<DISPLAY_HEIGHT; i++) 
        for(int j=0; j<DISPLAY_WIDTH; j++) 
            if(_buffer[i][j] != _buffer[i+DISPLAY_HEIGHT][j]) {
                char out[2] = {_buffer[i+(_current?DISPLAY_HEIGHT:0)][j],0};
                _display.drawString(j, i*10, out);
            }
    _current = !_current;
    clearBuffer();
}