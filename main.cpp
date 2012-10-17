#include <iostream>
#include "md5.h"
#include "Decrypter.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    MD5 md5;
    string cadena = "alfa";
    string result_md5;
    bool interactivo=false, fichero=false; //opciones
    string fichero_a_desencriptar="";
    vector<string> cadenas_a_resolver;

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i],"-i")==0)
        {
            interactivo = true;
        }
        else if(strcmp(argv[i],"-f")==0)
        {
            if(i+1<argc)
            {
                fichero = true;
                fichero_a_desencriptar = argv[i+1];
                i++;
            }
            else
            {
                cout << "Falta especificar el fichero tras -f" << endl;
            }
        }
        else
        {
            cadenas_a_resolver.push_back(argv[i]);
        }
    }

    /*cout << "Cadena " << cadena << " es en md5:" << endl;
    cout << result_md5 << endl;

    cout << "Probando decrypter..." << endl;*/
    vector<char> dominio;
    char a = 'a';
    char z = 'z';
    while (a < z) {
        dominio.push_back(a);
        a++;
    }
    dominio.push_back(z);
    /*cout << "Dominio size: " << dominio.size() << endl;
    cout << "Dominio values: ";
    string values = "";
    for (int i = 0; i < dominio.size(); i++) {
        values += dominio[i];
    }
    cout << values << endl;*/

    string solucion;
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
    return 0;
}

