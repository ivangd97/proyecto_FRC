//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================

#include "DataFrame.h"
#include <iostream>
#include <stdio.h>


//Default constructor
DataFrame::DataFrame() {
    S  = 22;
    D  = 'T';
    C  = 02;
    NT = '0';
    L = 0;
    BCE = 0; }


void manageTrame(HANDLE &portCOM,char msg[802],int tamanio){
    int tam=0;
	int corte = 0;
	while (tamanio > 0) {
		if (tamanio > 254) {
			tamanio -= 254;
			tam = 254;
		} else {
			tam = tamanio;
			tamanio = 0;
		}
		L = (unsigned char) tam;
		for (int j = 0; j < tam; j++) {
			Datos[j] = cadena[j + corte];
		}
		if(tamanio!=0){
		corte = corte + 254;
		Datos[tam] = '\0';
		BCE = calculateBCE();
		enviarTramaDatos(PuertoCOM);
		}
	}

	L=(unsigned char) tam+1;
	Datos[tam]='\n';
	Datos[tam+1]='\0';
	BCE=calcularBCE();
	enviarTramaDatos(PuertoCOM);

}




//



}


//This procedure will send a control frame when the user press the F2 key
void DataFrame::sendFrame(DataFrame &fSend,HANDLE &portCOM) {
    int controlFrame;
    bool exit = false;
    while(!exit) {
        //Interface to choose the type of control frame which the user will send
        printf("Trama de control a enviar :\n 0: Salir \n 1: Trama ENQ. \n 2: Trama EOT. \n 3: Trama ACK. \n 4: Trama NACK. \n");
        std::cin>>controlFrame;
        switch (controlFrame) {
        //0 case will exit
        case 0:
            printf("Saliendo...\n");
            //exit=true;
            break;

        case 1:
            printf("Has enviado Trama ENQ \n");
            exit = true;
            fSend.setC(05);
            EnviarCaracter(portCOM,fSend.getS());
            EnviarCaracter(portCOM,fSend.getD());
            EnviarCaracter(portCOM,fSend.getC());
            EnviarCaracter(portCOM,fSend.getNT());
            break;

        case 2:
            printf("Has enviado Trama EOT \n");
            exit = true;

            fSend.setC(04);

            EnviarCaracter(portCOM,fSend.getS());
            EnviarCaracter(portCOM,fSend.getD());
            EnviarCaracter(portCOM,fSend.getC());
            EnviarCaracter(portCOM,fSend.getNT());
            break;

        case 3:
            printf("Has enviado Trama ACK \n");
            exit = true;

            fSend.setC(06);

            EnviarCaracter(portCOM,fSend.getS());
            EnviarCaracter(portCOM,fSend.getD());
            EnviarCaracter(portCOM,fSend.getC());
            EnviarCaracter(portCOM,fSend.getNT());
            break;

        case 4:
            printf("Has enviado Trama NACK \n");
            exit = true;
            fSend.setC(21);

            EnviarCaracter(portCOM,fSend.getS());
            EnviarCaracter(portCOM,fSend.getD());
            EnviarCaracter(portCOM,fSend.getC());
            EnviarCaracter(portCOM,fSend.getNT());
            break;

        default:
            printf("Trama incorrecta, seleccione de nuevo.\n");
            break; } } }



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
char DataFrame::getData(int i) {
    return this->Data[i]; }
unsigned char DataFrame::getBCE() {


    return this->BCE; }

unsigned char calculateBCE(int x,char msg[]) {
    unsigned char BCE = msg[0] ^ msg[1];
    if(x ==1) {
        BCE = 1;

    }
    else {
        for(int i=2; i<x; i++) {
            BCE = BCE ^ msg[i];
        }
        if(BCE ==255 || BCE == 0) {
            BCE = 1; }
        }
    return BCE;

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
void DataFrame::setL(unsigned char value){
this->L=value;
}
