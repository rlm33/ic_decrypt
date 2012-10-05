#include "Decrypter.h"
#include <iostream>
using namespace std;

Decrypter::Decrypter(vector<char> dominio) {
    this->dominio = dominio;
    this->tam = 2;
}

void Decrypter::setDominio(vector<char> dominio) {
    this->dominio = dominio;
}

void Decrypter::setTam(int tam) {
    this->tam = tam;
}

string Decrypter::decrypt(string clave) {
    MD5 md5;
    string solucion = "";
    int i, j;
    for (i = 0; i<this->tam; i++) {
        solucion += '0';
    }
    return this->expand(clave,md5,solucion,0);
}

string Decrypter::expand(string clave, MD5 md5, string solucion, int k) {
    int i;
    
    for (i = 0; i < this->dominio.size(); i++) {
        solucion[k] = dominio[i];
        if(k == this->tam-1)
        {
            cout << solucion << endl;
            if (clave == md5.digestString(solucion.c_str())) {
                    return solucion;
            } 
        }
        else
        {
            return expand(clave,md5,solucion,k+1);
        }
    }
    return "";
}