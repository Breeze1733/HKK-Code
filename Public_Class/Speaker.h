#ifndef SPEAKER_H
#define SPEAKER_H

class Speaker {
    private:
        int x;
        int y;       
        int sensitivity;
        int ratedPower; 
    public:
        Speaker() : x(0), y(0), sensitivity(0), ratedPower(0) {}
        Speaker(int xPos, int yPos, int sens, int power) :
                x(xPos), y(yPos), sensitivity(sens), ratedPower(power) {}
        int getX() const { return x; }
        void setX(int xPos) { x = xPos; }
        int getY() const { return y; }
        void setY(int yPos) { y = yPos; }
        int getSensitivity() const { return sensitivity; }
        void setSensitivity(int sens) { sensitivity = sens; }
        int getRatedPower() const { return ratedPower; }
        void setRatedPower(int power) { ratedPower = power; }
};

#endif // SPEAKER_H