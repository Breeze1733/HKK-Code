#ifndef SPEAKER_H
#define SPEAKER_H

#include <string>
using std::string;

class Speaker {
    private:
        int x;
        int y;       
        int decibel; 
    public:
        Speaker() : x(0), y(0), decibel(0) {} 
        Speaker(int xPos, int yPos, int db) : x(xPos), y(yPos), decibel(db) {}
        int getX() const { return x; }
        void setX(int xPos) { x = xPos; }
        int getY() const { return y; }
        void setY(int yPos) { y = yPos; }
        int getDecibel() const { return decibel; } 
        void setDecibel(int db) { decibel = db; } 
};

#endif // SPEAKER_H