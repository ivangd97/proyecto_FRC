#ifndef DATAFRAME_H
#define DATAFRAME_H

class DataFrame(){
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
};

#endif // DATAFRAME_H
