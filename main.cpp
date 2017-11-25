#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <list>
#include <iomanip>

using namespace std;

list<char> quitarArroba(string a)
{
    list<char> lista;
    lista.push_back(a[0]);
    for(unsigned int i=1; i<a.length(); i++)
    {
        if(lista.back()!='@' || a[i]!='@')
        {
            lista.push_back(a[i]);
        }
        else{

        }
    }
    return lista;
}

bool estaEnLista(list<char> lista, char elemento)
{
    bool resultado=false;
    for (std::list<char>::iterator it=lista.begin(); it!=lista.end() ; ++it)
    if(*it==elemento) resultado=true;

    return resultado;
}

int obtenerVecesQueSeRepiteUnElemento(list<char> lista ,char elemento)
{
    int contador=0;
    for (std::list<char>::iterator it=lista.begin(); it!=lista.end() ; ++it)
        if(*it==elemento)
        contador++;
    return contador;
}

string reemplazarSubstrings(string str, const string from, const string to) {
    if(from.empty())
        return str;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    //cout<<str<<endl;
    return str;
}


list<char> obtenerListaDeElementosSinRepeticion(list<char> lista)
{
    list<char> elementosSinRepeticion;
    for (std::list<char>::iterator it=lista.begin(); it!=lista.end() ; ++it)
    {
         if(obtenerVecesQueSeRepiteUnElemento(elementosSinRepeticion,*it)==0 && *it!='(' && *it !=')' && *it!='[' && *it !=']' && *it!='=' && *it !='-' && *it !='+' && !isdigit(*it) && *it !='n' )//&& (*it=='C' || *it=='c' || *it=='@'))
         {
             elementosSinRepeticion.push_back(*it);
         }
    }

    /*for (std::list<char>::iterator it=elementosSinRepeticion.begin(); it!=elementosSinRepeticion.end() ; ++it)
    cout<<*it<<endl;
    cout<<"-----"<<endl;*/
    return elementosSinRepeticion;

}

int hallarNroDeElementos(list<char> lista)
{
    int nroElementos=0;
    list<char> elementosSinRepeticion=obtenerListaDeElementosSinRepeticion(lista);
    for (std::list<char>::iterator it=elementosSinRepeticion.begin(); it!=elementosSinRepeticion.end() ; ++it)
    nroElementos+=obtenerVecesQueSeRepiteUnElemento(lista,*it);
    return nroElementos;
}

int hallarNroDeElementosComunes(list<char> lista1,list<char> lista2)
{
    list<char> elementosSinRepeticion=obtenerListaDeElementosSinRepeticion(lista1);
    int cont=0;
    for (std::list<char>::iterator it1=elementosSinRepeticion.begin(); it1!=elementosSinRepeticion.end() ; ++it1)
    cont+=min(obtenerVecesQueSeRepiteUnElemento(lista1,*it1),obtenerVecesQueSeRepiteUnElemento(lista2,*it1));
    return cont;
}

double hallarIndiceJaccardTanimoto(string cadena1, string cadena2)
{
    double indice=0;
    cadena1=reemplazarSubstrings(reemplazarSubstrings(cadena1,"Br","$"),"Cl","*");
    cadena2=reemplazarSubstrings(reemplazarSubstrings(cadena2,"Br","$"),"Cl","*");
    int Na=hallarNroDeElementos(quitarArroba(cadena1));
    int Nb=hallarNroDeElementos(quitarArroba(cadena2));
    int Nc=hallarNroDeElementosComunes(quitarArroba(cadena1),quitarArroba(cadena2));
    indice=(double)Nc/(Na+Nb-Nc);
   // cout<<"Na: "<<Na <<"Nb: "<<Nb <<"Nc: "<<Nc <<endl;
    return indice;
}


int main()
{
    /*string c1="O=C(N[C@H](c1cc(Cl)c2c(c1O)nccc2)c1ccc(cc1)C)COc1ccccc1";
    string c2="COc1ccc(cc1)[C@H](c1cc(Cl)c2c(c1O)nccc2)NC(=O)COc1ccccc1";
     cout << fixed;
    cout << setprecision(2);
    cout<<hallarIndiceJaccardTanimoto(c1,c2);*/

    string lineaDeEntrada;
    ifstream archivoEntrada;
    string identificadores[12423];
    string keys[12423];
    archivoEntrada.open("ZINC_chemicals.tsv");
    int cont=0;
    getline(archivoEntrada,lineaDeEntrada);
    //carga e variables
    while(archivoEntrada)
    {
        getline(archivoEntrada,lineaDeEntrada);
        int pos = lineaDeEntrada.find_last_of("\t");
        int pos1=lineaDeEntrada.find_first_of("\t");
        identificadores[cont]=lineaDeEntrada.substr(pos1+1,13);
        keys[cont]=lineaDeEntrada.substr(pos+1,*lineaDeEntrada.end()-pos);
        cont++;
    }
    archivoEntrada.close();
    //calculo del indice de JAccardTanimoto
    ofstream archivoSalida;
    archivoSalida.open("datos.txt");
    archivoSalida << fixed;
    archivoSalida << setprecision(2);
    //12422
    //#pragma omp parallel for
    //{
    #pragma omp parallel for
    //{ 
   // #pragma omp for
    for(int i=0; i<100; i++)
    {
        for (int j=0; j<100; j++)
        {
            if(i<j)
            {
                archivoSalida<<identificadores[i]<<"\t"<<identificadores[j]<<"\t"<<hallarIndiceJaccardTanimoto(keys[i],keys[j])<<endl;

            }
        }
        //cout<<identificadores[i]<<" "<<keys[i]<<endl;
    }
  //  }
    return 0;
}
