#include "keyboard.h"
#include "ascii.h"
#include "../kernel.h"

void handleKeypress(unsigned char key){
    static bool uppercase = false;
    if (key == 0x3A || key == 0x36 ||
    key == 0xAA || key == 0xB6 || key == 0x2A){
        uppercase = !uppercase;
        return;
    }
    
    char ascii = ASCIITable[key];
    if (uppercase){
        ascii -= 32;
    }
    if (key < 0x57){
        print(ascii);
    }
    
}