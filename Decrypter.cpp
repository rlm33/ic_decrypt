#include "Decrypter.h"
#include <iostream>
using namespace std;

vector<char> Decrypter::build_downcase() {
    vector<char> dom;
    char a = 'a';
    char z = 'z';
    while (a < z) {
        dom.push_back(a);
        a++;
    }
    dom.push_back(z);

    return dom;
}

vector<char> Decrypter::build_alphanumeric() {
    vector<char> dom = build_downcase();
    char A = 'A';
    char Z = 'Z';
    while (A < Z) {
        dom.push_back(A);
        A++;
    }
    dom.push_back(Z);
    char cero = '0';
    char nueve = '9';
    while (cero < nueve) {
        dom.push_back(cero);
        cero++;
    }
    dom.push_back(nueve);

    return dom;
}

vector<char> Decrypter::build_complete() {
    vector<char> dom;
    char first_ascii = ' '; //Decimal = 32.
    char last_ascii = '~'; //Decimal = 126.
    while(first_ascii < last_ascii)
    {
        dom.push_back(first_ascii);
        first_ascii++;
    }
    dom.push_back(last_ascii);
    
    return dom;
}

Decrypter::Decrypter() {
    this->dominio = build_downcase();
    this->tam = 4;
    this->rank_solver = -1;
}

Decrypter::Decrypter(string tipo_dominio) {
    if (tipo_dominio == "downcase" || tipo_dominio == "DOWNCASE") {
        this->dominio = build_downcase();
    } else if (tipo_dominio == "alphanumeric" || tipo_dominio == "ALPHANUMERIC") {
        this->dominio = build_alphanumeric();
    } else if (tipo_dominio == "complete" || tipo_dominio == "COMPLETE") {
        this->dominio = build_complete();
    } else {
        cout << "Fallo al especificar dominio. Tomando minusculas por defecto." << endl;
        this->dominio = build_downcase();
    }

    this->tam = 4;
}

Decrypter::Decrypter(vector<char> dominio) {
    this->dominio = dominio;
    this->tam = 4;
}

void Decrypter::setDominio(vector<char> dominio) {
    this->dominio = dominio;
}

void Decrypter::setTam(int tam) {
    this->tam = tam;
}

int Decrypter::getRankSolver() {
    return this->rank_solver;
}

string Decrypter::decrypt(string clave, int tam_original, bool parallel, int nthreads, bool mpi, int me, int nprocs) {
    MD5 md5;
    string solucion = "";
    int i, j;
    this->continue_parallel = nprocs;
    this->rank_solver = -1;
    //Por defecto, tam_original es menor que 0, por lo que realizamos todas las cadenas
    //desde tamaño 1 hasta 8 caracteres.
    if (tam_original <= 0) {
        for (int k = 1; k < 8; k++) {
            this->tam = k;
            for (i = 0; i<this->tam; i++) {
                solucion += '0'; //Inicializamos la cadena solución a 'tam' ceros.
            }
            if (mpi) {
                this->rank_solver = this->expand_mpi(clave, md5, solucion, 0, me, nprocs);
                return solucion;
            }
            else if(parallel){
                if (this->expand_parallel(clave, md5, solucion, 0, nthreads)) {
                    return solucion;
                }
            }
            else{
                if (this->expand(clave, md5, solucion, 0)) {
                    return solucion;
                } else {
                    solucion = "";
                }
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
        if (mpi) {
            //cout << "MPI con continue = " << this->continue_parallel << endl;
            this->rank_solver = this->expand_mpi(clave, md5, solucion, 0, me, nprocs);
            return solucion;
        }
        else if(parallel) {
            this->expand_parallel(clave, md5, solucion, 0, nthreads);
                return solucion;
        }
        else{
            if (this->expand(clave, md5, solucion, 0)) {
                return solucion;
            } else {
                return "Solucion no encontrada";
            }
        }
    }
}

bool Decrypter::expand(string clave, MD5 md5, string &solucion, int k) {
    int i;
    //Para cada caracter del dominio.
    for (i = 0; i < this->dominio.size() && this->continue_parallel; i++) {
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

bool Decrypter::expand_parallel(string clave, MD5 md5, string &solucion, int k, int nthreads) {
    int i,j;
    int chunk;
    //int nthreads = 2;
    chunk = dominio.size()/nthreads;
    if (dominio.size()%nthreads != 0) {
        chunk++; //Por si no es multiplo del numero de threads
    }
    omp_set_num_threads(nthreads);
    bool encontrada = false;
    string solucionReal = "Solucion no encontrada";
    string aux = "";
    
    cout << "tenemos " << nthreads << " threads, a " << chunk << " letras cada uno de " << dominio.size() << " en total. " << endl;
    
     
        #pragma omp parallel for shared(chunk) private(i,j,aux) schedule(dynamic,chunk)
                for (i = 0; i < this->dominio.size(); i++) {
                    aux = "";
                    for (j = 0; j<this->tam && aux.length()<this->tam; j++) {
                        aux += '0'; //Inicializamos la cadena solución a 'tam' ceros.
                    }
                    aux[k] = this->dominio[i];
                    //Si ya es el último caracter de la cadena.
                    if (k == this->tam - 1) {
                        //cout << aux << endl; //Mostrar solución intermedia (DEBUG).
                        //Solución correcta comparada con su encriptación MD5.
                        if (clave == md5.digestString(aux.c_str())) {
                            encontrada = true;
                            solucionReal = aux;
                            this->continue_parallel = 0;
                            i = this->tam;
                        }
                    } else {
                        //Llamada recursiva para la siguiente posición de la cadena.
                        if (expand(clave, md5, aux, k + 1)) {
                            encontrada = true;
                            solucionReal = aux;
                            this->continue_parallel = 0;
                            i = this->tam;
                        }
                    }
                }
    
    solucion = solucionReal;
    //Ninguna solución encontrada para este dominio y este tamaño de cadena.
    return encontrada;
}

int Decrypter::expand_mpi(string clave, MD5 md5, string &solucion, int k, int me, int nprocs) {
    int i,sum,rank_solver=-1;
    //Para cada caracter del dominio.
    for (i = me; i < this->dominio.size(); i+=nprocs) {
        MPI_Allreduce (&this->continue_parallel, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        if (sum < nprocs*nprocs) { //Si la suma es menor que el cuadrado de nprocs
            rank_solver = nprocs - (nprocs*nprocs - sum); //El resolutor es rank_solver
            break; //Y salimos.
        }
        solucion[k] = this->dominio[i];
        //Si ya es el último caracter de la cadena.
        if (k == this->tam - 1) {
            //cout << solucion << endl; //Mostrar solución intermedia (DEBUG).
            //Solución correcta comparada con su encriptación MD5.
            if (clave == md5.digestString(solucion.c_str())) {
                this->continue_parallel = me;
                rank_solver = me;
            }
        } else {
            //Llamada recursiva para la siguiente posición de la cadena.
            if(expand(clave, md5, solucion, k + 1)) {
                this->continue_parallel = me;
                rank_solver = me;
            }
        }
    }
    
    return rank_solver;
}
