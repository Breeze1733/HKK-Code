#ifndef SPEAKER_H
#define SPEAKER_H

class Speaker {
    private:
        int x;
        int y;       
        int sensitivity;
        int impedance;
        int ratedPower; 
    public:
        Speaker() : x(0), y(0), sensitivity(0), impedance(0), ratedPower(0) {}
        Speaker(int xPos, int yPos, int sens, int imp, int power) :
                x(xPos), y(yPos), sensitivity(sens), impedance(imp), ratedPower(power) {}
        int getX() const { return x; }
        void setX(int xPos) { x = xPos; }
        int getY() const { return y; }
        void setY(int yPos) { y = yPos; }
        int getSensitivity() const { return sensitivity; }
        void setSensitivity(int sens) { sensitivity = sens; }
        int getImpedance() const { return impedance; }
        void setImpedance(int imp) { impedance = imp; }
        int getRatedPower() const { return ratedPower; }
        void setRatedPower(int power) { ratedPower = power; }
};

#endif // SPEAKER_H