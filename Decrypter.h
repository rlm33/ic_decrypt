#ifndef DECRYPTER_H
#define	DECRYPTER_H

#include <vector>
#include <string>
#include "md5.h"

using namespace std;

class Decrypter {
private:
    vector<char> dominio;
    int tam;
    string expand(string clave, MD5 md5, string solucion, int k);
public:
    Decrypter(vector<char> dominio);
    void setDominio(vector<char> dominio);
    void setTam(int tam);
    string decrypt(string clave);
};

#endif	/* DECRYPTER_H */

