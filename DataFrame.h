#ifndef DATAFRAME_H
#define DATAFRAME_H

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
        unsigned char calculateBCE(int x,char msg[]);
};

#endif // DATAFRAME_H
