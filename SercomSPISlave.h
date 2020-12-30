#ifndef SercomSPISlave_h
#define SercomSPISlave_h

#include <Arduino.h>

class SercomSPISlave
{
    public:
    // Constructors //
    SercomSPISlave();
    // Public Methods //
    void Sercom0init();
    void Sercom1init();
    void Sercom4init();
};

#endif
