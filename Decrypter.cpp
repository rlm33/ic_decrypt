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

string Decrypter::decrypt(string clave, int tam_original) {
    MD5 md5;
    string solucion = "";
    int i, j;
    //Por defecto, tam_original es menor que 0, por lo que realizamos todas las cadenas
    //desde tamaño 1 hasta 8 caracteres.
    if (tam_original <= 0) {
        for (int k = 1; k < 8; k++) {
            this->tam = k;
            for (i = 0; i<this->tam; i++) {
                solucion += '0'; //Inicializamos la cadena solución a 'tam' ceros.
            }
            if (this->expand(clave, md5, solucion, 0)) {
                return solucion;
            } else {
                solucion = "";
            }
        }
        return "Solucion no encontrada";

    } else {
        //Si conocemos el tamaño de la cadena original,
        //buscamos directamente la cadena de ese tamaño.
        this->tam = tam_original; 
        for (i = 0; i<this->tam; i++) {
            solucion += '0';
        }
        if (this->expand(clave, md5, solucion, 0)) {
            return solucion;
        } else {
            return "Solucion no encontrada";
        }
    }
}

bool Decrypter::expand(string clave, MD5 md5, string &solucion, int k) {
    int i;
    //Para cada caracter del dominio.
    for (i = 0; i < this->dominio.size(); i++) {
        solucion[k] = this->dominio[i];
        //Si ya es el último caracter de la cadena.
        if (k == this->tam - 1) {
            //cout << solucion << endl; //Mostrar solución intermedia (DEBUG).
            //Solución correcta comparada con su encriptación MD5.
            if (clave == md5.digestString(solucion.c_str())) {
                return true;
            }
        } else {
            //Llamada recursiva para la siguiente posición de la cadena.
            if (expand(clave, md5, solucion, k + 1)) {
                return true;
            }
        }
    }
    //Ninguna solución encontrada para este dominio y este tamaño de cadena.
    return false;
}