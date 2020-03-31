//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================

#ifndef DATAFRAME_H
#define DATAFRAME_H
#include "PuertoSerie.h"
#include <fstream>
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



        //Getters
        unsigned char getS();
        unsigned char getD();
        unsigned char getC();
        unsigned char getNT();
        unsigned char getL();
        char *getData();
        unsigned char getBCE();

        //This procedure will send a control frame when the user press the F2 key
        void sendDataFrame(HANDLE &portCOM);
        void sendDataFrame2(HANDLE &portCOM,char cadena[]);
        unsigned char calcularBCE_2(char cadena[]);

        //This topic will divide the message in a little ones which its length is 254
        void manageFrame(HANDLE &portCOM,char msg[],int tamanio);
        //After a given formula, we will calculate the bce associated to data.
        unsigned char calculateBCE();
        //Show in terminal the message in the frame
        void showData(HANDLE pantalla,int colour);

        //Write Data in a new file
        void writeFile(ofstream& of,int colour,HANDLE pantalla,bool log,ofstream &logStream);

         //Setters
        void setC(unsigned char value);
        void setD(unsigned char value);
        void setS(unsigned char value);
        void setNT(unsigned char value);
        void setL(unsigned char value);
        void setBCE(unsigned char value);
        void setData(char msg[]);

        void insertData(unsigned char value);



};

#endif // DATAFRAME_H
