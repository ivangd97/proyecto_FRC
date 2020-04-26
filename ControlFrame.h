//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================

#ifndef CONTROLFRAME_H
#define CONTROLFRAME_H
#include "PuertoSerie.h"
#include <fstream>



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
    void imprimirTramaControl(int opcion,ofstream &pStream);
    void imprimirTramaControl2(int opcion,ofstream &pStream);


    //Setters
    void setC(unsigned char value);
    void setD(unsigned char value);
    void setS(unsigned char value);
    void setNT(unsigned char value);
    void sendControlFrame(HANDLE &portCOM,bool log,ofstream &logStream,HANDLE pantalla);
    void sendControl(HANDLE &portCOM);
    void changeNT();

    //Getters
    unsigned char getC();
    unsigned char getD();
    unsigned char getS();
    unsigned char getNT();


};

#endif // CONTROLFRAME_H
