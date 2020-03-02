//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================

#ifndef CONTROLFRAME_H
#define CONTROLFRAME_H

//This class contains four attributes:
// S: Synchronism
// D: Direction
// C: Control
// NT: Frame number
//It's used to send a control frame from an user to another one
class ControlFrame
{
protected:
    unsigned char S;
    unsigned char D;
    unsigned char C;
    unsigned char NT;

public:
    ControlFrame();
    ControlFrame(int value);

    //Setters
    void setC(unsigned char value);
    void setD(unsigned char value);
    void setS(unsigned char value);
    void setNT(unsigned char value);

    //Getters
    unsigned char getC();
    unsigned char getD();
    unsigned char getS();
    unsigned char getNT();


};

#endif // CONTROLFRAME_H
