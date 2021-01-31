#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int> &A, int pocetak, int sredina, int kraj, int *t)
{
    int duzinaLijevo = sredina - pocetak + 1; // duzina lijevog niza
    int duzinaDesno = kraj - sredina; // duzina desnog niza
    vector<int> desno(duzinaLijevo);
    vector<int> lijevo(duzinaDesno);


    for(int i = 0; i < duzinaLijevo; i++) //pravimo lijevi niz
        lijevo[i] = A[pocetak + i];

    for(int = 0; i < duzinaDesno; i++) //pravimo desni niz
        desno[i] = A[sredina + 1 + i];

    if(duzinaLijevo == 2){
        for(int i = 0; i < velT; i++){
            if(t[hash([1], vel) + i] == 0) {
                t[hash(lijevo[1], vel) + i] = lijevo[1];
                break;
        }
    }
        t[hash(lijevo[1], vel)] = lijevo[1];
    }
    
    if(duzinaDesno == 2){
        for(int i = 0; i < velT; i++){
            if(t[hash(desno[1], vel) + i] == 0) {
                t[hash(desno[1], vel) + i] = desno[1];
                break;
        }
    }
    //sada sortiramo..

    int i, j;
    for(int k = pocetak; i <duzinaLijevo && j < duzinaDesno; k++) {
        if(lijevo[i] < desno[j]) A[k] = lijevo[i++];
        else A[k] = desno[j++];
    }

    while(i < duzinaLijevo) A[k] = lijevo[i++];
    while(j < duzinaDesno) A[k] = desno[j++];

}

void mergeSort(vector<int> A, int pocetak, int kraj, int *t)
{
    int sredina;
    if(pocetak < kraj) {
        sredina = (pocetak + kraj) / 2;
        mergeSort(A, pocetak, sredina);
        mergeSort(A, sredina+1, kraj, t);
        merge(A, pocetak, sredina, kraj, t);
    }
}

void VekHash(vector<int> A, int *t, int velT)
{
    mergeSort(A, 0, A.size(), t);
}

int main()
{
    return 0;
}
