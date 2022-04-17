#ifndef IO_H
#define IO_H

void out (unsigned short port, unsigned char value);
unsigned char in(unsigned short port);
void wait();

#endif