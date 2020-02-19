#ifndef CONTROLFRAME_H
#define CONTROLFRAME_H


class ControlFrame
{
    unsigned char S;
    unsigned char D;
	unsigned char C;
	unsigned char NT;

    public:
        ControlFrame();
        ControlFrame(char value);
        void setAtribute(char value, int attribute);
        char getAtribute(int attribute);
};

#endif // CONTROLFRAME_H
