#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

template<typename Tip>
void bubble_sort(Tip *niz, int vel)
{
    for(int i  = vel-1 ; i >= 1; i--)
        for(int j = 1; j <= i; j++)
            if(niz[j-1] > niz[j]) {
                Tip temp = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = temp;
            }
}

template<typename Tip>
void selection_sort(Tip *niz, int vel)
{
    for(int i = 0; i < vel-1; i++) {
        Tip min = niz[i];
        int min_indeks = i;
        for(int j = i+1; j < vel; j++) {
            if(niz[j] < min) {
                min = niz[j];
                min_indeks = j;
            }
        }
        niz[min_indeks] = niz[i];
        niz[i] = min;
    }
}


template<typename Tip>
int particija(Tip *niz, int prvi, int zadnji)
{

    Tip pivot = niz[zadnji];
    int i = prvi - 1;

    for(int j = prvi; j <= zadnji - 1; j++) {
        if(niz[j] < pivot) {
            i++;
            Tip temp = niz[i];
            niz[i] = niz[j];
            niz[j] = temp;
        }
    }
    Tip temp = niz[i+1];
    niz[i+1] = niz[zadnji];
    niz[zadnji] = temp;
    return i+1;
}

template<typename Tip>
void quick_sort(Tip *niz, int vel)
{
    int prvi = 0;
    int zadnji = vel - 1;
    if(prvi < zadnji) {
        int j = particija(niz, prvi, zadnji);
        quick_sort(niz, j);
        quick_sort(niz+j+1, zadnji-j);
    }
}

template<typename Tip>
void merge(Tip *niz, int prvi, int p, int q, int zadnji)
{
    int i = 0;
    int j = 0;
    int k = prvi;

    int n1 = q;
    int n2 = zadnji - p;

    Tip *L = new Tip[n1];
    Tip *R = new Tip[n2];

    for(i = 0; i < n1; i++)
        L[i] = niz[prvi + i];

    for(j = 0; j < n2; j++)
        R[j] = niz[q + j];

    i = 0;
    j = 0;

    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            niz[k] = L[i];
            i++;
        } else {
            niz[k] = R[j];
            j++;
        }
        k++;
    }

    while(i < n1) {
        niz[k] = L[i];
        i++;
        k++;
    }

    while(j < n2) {
        niz[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

template<typename Tip>
void merge_sort(Tip *niz, int vel)
{
    int prvi = 0;
    int zadnji = vel-1;
    if(prvi < zadnji) {
        int p = (zadnji-prvi)/2 + prvi;
        int q = p + 1;
        merge_sort(niz, q);
        merge_sort(niz + q, zadnji - p);
        merge(niz, prvi, p, q, zadnji);
    }
}

void ucitaj(string filename, int *&niz, int &vel)
{
    ifstream op(filename);
    if(!op) std::cout << "Greska" << std::endl;
    else {
        int x;
        int br = 0;
        while(op >> x) br++;
        vel = br;
        niz = new int[br];
        op.close();
        op.open(filename);
        br = 0;
        while(op >> x) {
            niz[br] = x;
            br++;
        }
    }
    if(!op.eof()) std::cout << "Greska" << std::endl;
    op.close();
}

void generisi(string filename, int vel)
{
    ofstream output(filename);
    for(int i = 0; i < vel; i++) {
        output << rand();
        output << " ";
    }
    output.close();
}

void funkcija()
{
    // pomocu funkcije ucitaj() ucitava brojeve u niz
    int *niz;
    int vel = 10;
    generisi("funkcija.txt", 10);
    ucitaj("funkcija.txt", niz, vel);

    //omogucuje korisniku da izabere koji algoritam sortiranja ce se koristiti
    auto a = std::chrono::high_resolution_clock::now();
    cout << "Koji algoritam sortiranja zelite koristiti?\n1 - Bubble sort\n2 - Selection sort\n3 - Quick sort\n4 - Merge sort\n";
    int izbor;
    cin >> izbor;
    switch(izbor) {
    case 1:
        bubble_sort(niz, vel); // poziva odgovarajuću funkciju za sortiranje;
        break;
    case 2:
        selection_sort(niz, vel);
        break;
    case 3:
        quick_sort(niz, vel);
        break;
    case 4:
        merge_sort(niz, vel);
        break;
    }
    
    cout << "Vrijeme izvrsavanja sortiranja: "; // ispisuje vrijeme koliko se funkcija izvrsavala (u milisekundama)
        auto b = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(b-a).count() << " ms" << std::endl;

    //provjerava da li je niz zaista sortiran (prolazi kroz niz i provjerava da li je svaki clan veci ili jednak prethodnom)
    bool sortiran = true;
    for(int i = 1; i <= vel; i++)
        if(niz[i-1] > niz[i]) sortiran = false;
    if(sortiran) cout << "Niz je zaista sortiran" << endl;
    else cout << "Niz nije sortiran" << endl;

    //zapisuje sortirani niz u neku novu izlaznu datoteku
    ofstream output;
    output.open("IzlaznaDatoteka.txt");
    for(int i = 0; i < vel; i++) {
        output << niz[i];
        output << " ";
    }
    output.close();
}

int main()
{
    std::cout << "Pripremna Zadaca za Vjezbu 6, Zadatak 1";
    funkcija();
    return 0;
}
