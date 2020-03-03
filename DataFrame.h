//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================

#ifndef DATAFRAME_H
#define DATAFRAME_H
#include "PuertoSerie.h"

class DataFrame{
    protected:
        unsigned char S;
        unsigned char D;
        unsigned char C;
        unsigned char NT;
        unsigned char L;
        char Data[255];
        unsigned char BCE;

    public:
        DataFrame();



        unsigned char getS();
        unsigned char getD();
        unsigned char getC();
        unsigned char getNT();
        unsigned char getL();
        char getData(int i);
        unsigned char getBCE();

        //This procedure will send a control frame when the user press the F2 key
        void sendFrameData(HANDLE &portCOM);
        void manageFrame(HANDLE &portCOM,char msg[802],int tamanio);



        unsigned char calculateBCE(int x,char msg[]);

         //Setters
        void setC(unsigned char value);
        void setD(unsigned char value);
        void setS(unsigned char value);
        void setNT(unsigned char value);
        void setL(unsigned char value);



};

#endif // DATAFRAME_H
