//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================

#include "DataFrame.h"
#include <iostream>
#include <stdio.h>
char author[255];
int line=0;

//Default constructor
DataFrame::DataFrame() {
    S  = 22;
    D  = 'T';
    C  = 02;
    NT = '0';
    L = 0;
   // Data[]=NULL;
    BCE = 0; }


//This topic will divide the message in a little ones which its length is 254
void DataFrame::manageFrame(HANDLE &portCOM,char msg[],int tamanio){
    int tamanioAux=0;
	int cutPoint = 0;

	while (tamanio > 0) {
		if (tamanio > 254) {
			tamanio -= 254;
			tamanioAux = 254;
		} else {
			tamanioAux = tamanio;
			tamanio = 0;
		}
		L = (unsigned char) tamanioAux;
		for (int j = 0; j < tamanioAux; j++) {
			Data[j] = msg[j + cutPoint];
		}
		if(tamanio!=0){
		cutPoint += 254;
        //Calculate the bce associated to the new little frame
        BCE = calculateBCE();
		//Send the little frame
        sendDataFrame(portCOM);

		}
	}

    //Last frame char adding
	L=(unsigned char) tamanioAux;
    BCE=calculateBCE();
	sendDataFrame(portCOM);

}

//This procedure will send a control frame when the user press the F2 key
void DataFrame::sendDataFrame(HANDLE &portCOM) {
            EnviarCaracter(portCOM,S);
            EnviarCaracter(portCOM,D);
            EnviarCaracter(portCOM,C);
            EnviarCaracter(portCOM,NT);
            EnviarCaracter(portCOM,L);
            EnviarCadena(portCOM,Data,strlen(Data));
            EnviarCaracter(portCOM,BCE);
}

void DataFrame::sendDataFrame2(HANDLE &portCOM,char cadena[]){
    EnviarCaracter(portCOM, S);
	EnviarCaracter(portCOM, D);
	EnviarCaracter(portCOM, C);
	EnviarCaracter(portCOM, NT);
	EnviarCaracter(portCOM, L);
	EnviarCadena(portCOM, cadena, (int) L);
	EnviarCaracter(portCOM, BCE);
}

unsigned char DataFrame::getS() {
    return this->S; }

unsigned char DataFrame::getD() {
    return this->D; }

unsigned char DataFrame::getC() {
    return this->C; }

unsigned char DataFrame::getNT() {
    return this->NT; }

unsigned char DataFrame::getL() {
    return this->L; }

//Data vector size is undefined until the message is received
char *DataFrame::getData() {
    return this->Data;
}

unsigned char DataFrame::getBCE() {
    return this->BCE;
}

//After a given formula, we will calculate the bce associated to data.
unsigned char DataFrame::calculateBCE() {
    unsigned char BCE = Data[0] ;
    for(int i=1 ; i< L; i++) {
        BCE = BCE ^ Data[i];
    }
    if(BCE ==255 || BCE == 0) {
        BCE = 1;
    }
    return BCE;
}

//Second topic to calculate the bce of the frame
unsigned char DataFrame::calcularBCE_2(char cadena[]){
	unsigned char BCE = cadena[0];
		for (int i = 1; i < L; i++) {
			BCE = BCE ^ cadena[i];
		}
		if (BCE == 0 || BCE == 255) {
			BCE = 1;
		}
		return BCE;
}

//Show in terminal the message in the frame
void DataFrame::showData(HANDLE pantalla,int colour){
    colour =9 + 14*16;
    pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute (pantalla, colour);
    for(int x=0;x<this->L;x++){
            printf("%c",Data[x]);
        }
}

//Write data in the output file with a of stream
void DataFrame::writeFile(ofstream &of,int colour,HANDLE pantalla,bool log,ofstream &logStream){
    if(calculateBCE() == BCE){
        switch(line){
            case 0:
                strcpy(author,Data);
                line++;
                break;
            case 1:

                colour =atoi(Data) + 0*16;
                pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute (pantalla, colour);
                printf("Recibiendo fichero de %s \n",author);

                if(log){logStream<<"Recibiendo fichero de "<< author<<"\n";}

                line++;
                break;

            case 2:

                of.open(Data,ios::app);
                line++;
                break;

            case 3:

                of.write(Data,strlen(Data));

                break;

        }
    }
    else{
        printf("No se pudo escribir en el fichero\n");
        if(log){logStream<<"No se pudo escribir en el fichero\n";}
    }
}

//Set C attribute
void DataFrame::setC(unsigned char value)
{
    this->C = value;
}

//Set D attribute
void DataFrame::setD(unsigned char value)
{
    this->D = value;
}

//Set S attribute
void DataFrame::setS(unsigned char value)
{
    this->S = value;
}

//Set NT attribute
void DataFrame::setNT(unsigned char value)
{
    this->NT = value;
}
//Set L attribute
void DataFrame::setL(unsigned char value){
    this->L=value;
}

void DataFrame::insertData(unsigned char value){
    this->Data[this->L]= value;
}

void DataFrame::setBCE(unsigned char value){
    this->BCE = value;
}

void DataFrame::setData(char msg[]){
    strcpy(Data,msg);
}

