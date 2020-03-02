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
