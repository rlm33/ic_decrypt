#include <iostream>
#include <stdlib.h>
#include <vector>
#include "md5.h"
#include "Decrypter.h"
#include "fstream"



using namespace std;

//funcion de encriptacion md5

string encrypt(string cadena) {
    string ret = "";
    MD5 md5;

    ret = md5.digestString(cadena.c_str()); //encriptacion md5

    return ret;
}

//funcion de desencriptacion md5

string decrypt(string result_md5, bool modo_paralelo = false, int nthreads = 2, string dom = "DOWNCASE") {
    string ret = "";
    Decrypter decrypter(dom);
    ret = decrypter.decrypt(result_md5, -1, modo_paralelo, nthreads);

    return ret;
}

//funcion para leer fichero de texto

vector <string> leerFichero(string fichero) {

    fstream ficheroe;
    string nombrefichero = fichero;
    string linea_aux = "";
    vector <string> cadenas;
    char nomfichero[nombrefichero.length() - 1];

    strcpy(nomfichero, nombrefichero.c_str());
    ficheroe.open(nomfichero, ios::in);

    if (ficheroe.is_open()) {

        getline(ficheroe, linea_aux);
        cadenas.push_back(linea_aux);

        while (!ficheroe.eof() && linea_aux.length() != 0) {
            getline(ficheroe, linea_aux);
            cadenas.push_back(linea_aux);
        }
        ficheroe.close();
    } else
        cout << "Error de apertura de fichero de lectura." << endl;

    return cadenas;

}

//funcion para escribir fichero de texto

void escribirFichero(string fichero, vector <string> cadenas) {

    fstream ficheroe;
    string nombrefichero = fichero;
    int i = 0;

    char nomfichero[nombrefichero.length() - 1];

    strcpy(nomfichero, nombrefichero.c_str());
    ficheroe.open(nomfichero, ios::out);

    if (ficheroe.is_open()) {
        while (i < cadenas.size()) {
            ficheroe << cadenas[i] << endl; //escribo y borro elemento
            i++;
        }
        ficheroe.close();
    } else
        cout << "Error de apertura de fichero de escritura." << endl;
}

//funcion para modo interactivo

void interactivo() {
    string menu = "1- Encrypt\n2- Decrypt\n3- Change domain\n4- Activate parallel\n5- Deactivate parallel\n99- Salir";
    string cadena = "", domain = "DOWNCASE";
    bool modo_paralelo = false;
    int ntrheads = 2; //2 hilos por defecto
    int opcion = 0, dom = 0;
    while (opcion != 99) {
        system("clear");
        if (opcion == 1) {
                cout << "Encriptacion MD5 = " << cadena << endl << endl;
        }
        else if (opcion == 2) {
            cout << "Cadena original = " << cadena << endl << endl;
        }
        cout << menu << endl;
        getline(cin, cadena);
        opcion = atoi(cadena.c_str());
        switch (opcion) {
            case 1:
                cout << "Cadena a encriptar: ";
                getline(cin, cadena);
                cadena = encrypt(cadena);
                cout << "Encriptacion MD5 = " << cadena << endl;
                break;
            case 2:
                cout << "Cadena MD5 a desencriptar: ";
                getline(cin, cadena);
                cadena = decrypt(cadena, modo_paralelo, ntrheads, domain);
                cout << "Cadena original = " << cadena << endl;
                break;
            case 3:
                cout << "1- DOWNCASE\n2- ALPHANUMERIC\n3- COMPLETE ASCII" << endl;
                getline(cin, cadena);
                dom = atoi(cadena.c_str());
                switch (dom) {
                    case 1:
                        domain = "DOWNCASE";
                        break;
                    case 2:
                        domain = "ALPHANUMERIC";
                        break;
                    case 3:
                        domain = "COMPLETE";
                        break;
                    default:
                        cout << "Wrong option, last domain " << domain << endl;
                }
                break;
            case 4:
                modo_paralelo = true;
                cout << "Numero de hilos a utilizar: ";
                getline(cin, cadena);
                ntrheads = atoi(cadena.c_str());
                break;
            case 5:
                modo_paralelo = false;
                break;
            case 99:
                cout << endl << "Bye bye..." << endl << endl;
                break;
            default:
                cout << "Opcion no soportada." << endl;
        }
    }
}

//funcion para resolver las cadenas en md5

vector <string> resolver_cadenas(vector<string> cadenas_a_resolver, string dominio, bool modo_paralelo = false, int nthreads = 2, bool modo_mpi = false, int me = 0, int nprocs = 0) {
    MD5 md5;
    string cadena, result_md5, solucion;
    vector <string> resueltas;
    Decrypter decrypter(dominio);
    for (int i = 0; i < cadenas_a_resolver.size(); i++) {
        cadena = cadenas_a_resolver[i];
        //decrypter.setTam(cadena.length());
        solucion = "";
        result_md5 = md5.digestString(cadena.c_str());
        solucion = decrypter.decrypt(result_md5, cadena.length(), modo_paralelo, nthreads, modo_mpi, me, nprocs); //booleano para usar paralelismo o mpi
        if (solucion == "Solucion no encontrada") {
            cout << solucion << " para cadena " << cadena << endl;
        } else {
            cout << "Solucion encontrada: " << solucion;
            if (solucion == cadena) {
                resueltas.push_back("cadena encriptada: " + result_md5 + " cadena original: " + solucion);
                cout << " OK." << endl;
            } else {
                cout << " FAIL!" << endl;
            }
        }
    }

    return resueltas;
}

/*
 * cuerpo principal
 */
int main(int argc, char** argv) {
    //opciones
    bool modo_interactivo = false;
    bool modo_fichero = false;
    bool modo_paralelo = false;
    bool modo_mpi = false;
    
    int nthreads = 2; //2 hilos por defecto
    string fichero_a_desencriptar = "";
    string dominio_activo = "DOWNCASE";
    vector<string> cadenas_a_resolver;

    //Procesar opciones:
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-np") == 0) {
            if (i + 1 < argc) {
                i++;
            }
        }
        else if (strcmp(argv[i], "-i") == 0) {
            modo_interactivo = true;
        } else if (strcmp(argv[i], "-f") == 0) { //seleccion de modo fichero
            if (i + 1 < argc) {
                modo_fichero = true;
                fichero_a_desencriptar = argv[i + 1];
                i++;
            } else {
                cout << "Falta especificar el fichero tras -f" << endl;
            }
        } else if (strcmp(argv[i], "-d") == 0) { //seleccion de modo activo
            if (i + 1 < argc) {
                dominio_activo = argv[i + 1];
                i++;
            } else {
                cout << "Falta especificar el tipo de dominio tras -d" << endl;
            }
        } else if (strcmp(argv[i], "-p") == 0) {
            modo_paralelo = true;
        } else if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                nthreads = atoi(argv[i + 1]);
                i++;
                modo_paralelo = true;
            } else {
                cout << "Falta especificar el numero de hilos tras -n" << endl;
            }
        } else if (strcmp(argv[i], "-mpi") == 0) {
            modo_mpi = true;
        } else {
            cadenas_a_resolver.push_back(argv[i]);
        }
    }
    if (modo_mpi) {
        int me, nprocs;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
        MPI_Comm_rank(MPI_COMM_WORLD, &me);
        
        resolver_cadenas(cadenas_a_resolver, dominio_activo, false, 0, modo_mpi, me, nprocs);
        
        MPI_Finalize();
    }
    else if (modo_interactivo) { //modo interactivo
        interactivo();
    } else if (modo_fichero) { //ejecucion de modo fichero
        vector <string> resueltas;
        cadenas_a_resolver = leerFichero(fichero_a_desencriptar);
        resueltas = resolver_cadenas(cadenas_a_resolver, dominio_activo, modo_paralelo, nthreads);
        escribirFichero(fichero_a_desencriptar + ".sal", resueltas);
    } else {
        resolver_cadenas(cadenas_a_resolver, dominio_activo, modo_paralelo, nthreads);
    }
    
    return 0;
}

