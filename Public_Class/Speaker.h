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
        double getSensitivity_dBWm() const {
            if (impedance <= 0) {
                // 已是 dB/W/m
                return sensitivity;
            } else {
                // dB/2.83V/m 转 dB/W/m
                // dB(W/m) = dB(2.83V/m) - 10*log10(8/Z)
                // 公式参考：https://www.audiosciencereview.com/forum/index.php?threads/speaker-sensitivity-db-w-m-vs-db-2-83v-m.51385/
                return sensitivity - 10.0 * log10(8.0 / impedance);
            }
        }
};

#endif // SPEAKER_H