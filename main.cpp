#include <iostream>
#include "md5.h"
#include "Decrypter.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Not implemented yet." << endl;

    MD5 md5;
    string cadena = "alfa";
    string result_md5 = md5.digestString(cadena.c_str());
    cout << "Cadena " << cadena << " es en md5:" << endl;
    cout << result_md5 << endl;

    cout << "Probando decrypter..." << endl;
    vector<char> dominio;
    char a = 'a';
    char z = 'z';
    while (a < z) {
        dominio.push_back(a);
        a++;
    }
    dominio.push_back(z);
    cout << "Dominio size: " << dominio.size() << endl;
    cout << "Dominio values: ";
    string values = "";
    for (int i = 0; i < dominio.size(); i++) {
        values += dominio[i];
    }
    cout << values << endl;

    Decrypter decrypter(dominio);
    decrypter.setTam(cadena.length());
    string solucion = "";
    solucion = decrypter.decrypt(result_md5);
    cout << "Solucion encontrada: " << solucion << endl;

    return 0;
}

