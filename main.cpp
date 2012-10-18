#include <iostream>
#include <stdlib.h>
#include "md5.h"
#include "Decrypter.h"

using namespace std;

string encrypt(string cadena) {
    string ret = "";
    MD5 md5;

    ret = md5.digestString(cadena.c_str());

    return ret;
}

string decrypt(string result_md5, int tam = 4) {
    string ret = "";
    Decrypter decrypter("DOWNCASE");
    decrypter.setTam(tam);
    ret = decrypter.decrypt(result_md5);

    return ret;
}

void interactivo() {
    string menu = "1- Encrypt\n2- Decrypt\n99- Salir";
    string cadena = "";
    int opcion = 0;
    while (opcion != 99) {
        cout << menu << endl;
        getline(cin, cadena);
        opcion = atoi(cadena.c_str());
        switch (opcion) {
            case 1:
                cout << "Cadena a encriptar: ";
                getline(cin, cadena);
                cout << "Encriptacion MD5 = " << encrypt(cadena) << endl;
                break;
            case 2:
                cout << "Cadena MD5 a desencriptar: ";
                getline(cin, cadena);
                cout << "Cadena original = " << decrypt(cadena) << endl;
                break;
            case 99:
                cout << endl << "Bye bye..." << endl << endl;
                break;
            default:
                cout << "Opcion no soportada." << endl;
        }
    }
}

void resolver_cadenas(vector<string> cadenas_a_resolver, string dominio) {
    MD5 md5;
    string cadena, result_md5, solucion;
    Decrypter decrypter(dominio);
    for (int i = 0; i < cadenas_a_resolver.size(); i++) {
        cadena = cadenas_a_resolver[i];
        decrypter.setTam(cadena.length());
        solucion = "";
        result_md5 = md5.digestString(cadena.c_str());
        solucion = decrypter.decrypt(result_md5);
        cout << "Solucion encontrada: " << solucion;
        if (solucion == cadena) {
            cout << " OK." << endl;
        } else {
            cout << " FAIL!" << endl;
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    bool modo_interactivo = false, modo_fichero = false; //opciones
    string fichero_a_desencriptar = "";
    vector<string> cadenas_a_resolver;
    //Procesar opciones:
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            modo_interactivo = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                modo_fichero = true;
                fichero_a_desencriptar = argv[i + 1];
                i++;
            } else {
                cout << "Falta especificar el fichero tras -f" << endl;
            }
        } else {
            cadenas_a_resolver.push_back(argv[i]);
        }
    }
    if (modo_interactivo) {
        interactivo();
    } else if (modo_fichero) {
        cout << "TODO: FICHERO" << endl;
    } else {
        resolver_cadenas(cadenas_a_resolver, "DOWNCASE");
    }

    return 0;
}

