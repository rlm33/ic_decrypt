#ifndef DECRYPTER_H
#define	DECRYPTER_H

#include <vector>
#include <string>
#include <omp.h>
#include "mpi.h"
#include "md5.h"

using namespace std;

class Decrypter {
private:
    //Almacena los caracteres posibles en la cadena original.
    vector<char> dominio;
    //El tamaño de la cadena original.
    int tam;
    //La sucia
    int continue_parallel;
    //Y la otra
    int rank_solver;
    //Método para llamada recursiva de 'decrypt'.
    //Devuelve true si encuentra solución y false en caso contrario.
    bool expand(string clave, MD5 md5, string &solucion, int k);
    bool expand_parallel(string clave, MD5 md5, string &solucion, int k, int nthreads = 2);
    int expand_mpi(string clave, MD5 md5, string &solucion, int k, int me, int nprocs);
    
public:
    //Constructor por defecto, dominio = minúsculas y tam = 4
    Decrypter();
    //Constructor con opciones para el dominio. Son:
    //"downcase" para minúsculas.
    //"alphanumeric" para letras y números.
    //"complete" incluye todo caracter ascii.
    Decrypter(string tipo_dominio);
    //Constructor para un dominio conocido.
    Decrypter(vector<char> dominio);
    void setDominio(vector<char> dominio);
    void setTam(int tam);
    int getRankSolver();
    //Helpers para construir los dominios.
    vector<char> build_downcase();
    vector<char> build_alphanumeric();
    vector<char> build_complete();
    //Método de desencriptación MD5 por fuerza bruta.
    //Sus parámetros son:
    //clave -> cadena MD5 a desencriptar
    //tam_original -> tamaño de la cadena original si es conocido, 0 o negativo si no,
    //en cuyo caso prueba todas las cadenas de tamaño entre 1 y 8 caracteres.
    //Devuelve la cadena original o la cadena "Solucion no encontrada" si no consigue
    //desencriptar bajo el dominio activo y/o no coincide tam_original con el tamaño
    //de la cadena desencriptada.
    string decrypt(string clave, int tam_original=-1, bool parallel = false, int nthreads = 2, bool mpi = false, int me = 0, int nprocs = 1);
};

#endif	/* DECRYPTER_H */

