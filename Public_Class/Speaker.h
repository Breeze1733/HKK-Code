#ifndef SPEAKER_H
#define SPEAKER_H

#include <string>
using std::string;

class Speaker {
    private:
        string name; 
        int x;
        int y;       
        int decibel; 
    public:
        Speaker() : name(""), x(0), y(0), decibel(0) {} 
        Speaker(string n, int xPos, int yPos, int db) : name(n), x(xPos), y(yPos), decibel(db) {} 
        string getName() { return name; }
        void setName(string n) { name = n; }
        int getX() { return x; }
        void setX(int xPos) { x = xPos; }
        int getY() { return y; }
        void setY(int yPos) { y = yPos; }
        int getDecibel() { return decibel; } 
        void setDecibel(int db) { decibel = db; } 
};

#endif // SPEAKER_H