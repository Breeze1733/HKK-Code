#ifndef Field_h
#define Field_h

class Field {
    private:
        int width;
        int length;

    public:
        Field(int w, int l) : width(w), length(l) {}

        int getWidth() const { return width; }
        int getLength() const { return length; }

        void setWidth(int w) { width = w; }
        void setLength(int l) { length = l; }
};

#endif // Field_h