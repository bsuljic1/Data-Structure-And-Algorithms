#include <iostream>
#include <vector>

using namespace std;

class Cvor
{
public:
    int vrijednost;
    Cvor *lijevo = nullptr;
    Cvor *desno = nullptr;
};

int visina(Cvor *cvor)
{
    if(cvor == nullptr) return 0;
    if(cvor->desno == nullptr && cvor->lijevo == nullptr) return 1;
    int lijevo = visina(cvor->lijevo);
    int desno = visina(cvor->desno);
    if(lijevo > desno) return lijevo + 1;
    else return desno + 1;
}

int visina(vector<int> A, int indeks)
{
    if(A[indeks] == -1) return 0;
    if(A[2*indeks+1] == -1 && A[2*indeks + 2] == -1) return 1;
    int lijevo = visina(A, 2*indeks + 1);
    int desno = visina(A, 2*indeks + 2);
    if(lijevo > desno) return lijevo + 1;
    else return desno + 1;
}

void obrisi(Cvor *cvor)
{
    if(cvor) {
        obrisi(cvor->lijevo);
        obrisi(cvor->desno);
        cvor = nullptr;
    }
}

void dodaj(Cvor *roditelj, bool desnoDijete, vector<int> clanovi, int indeks)
{
    if(clanovi[indeks] != -1) {
        Cvor *novi;
        novi->vrijednost = clanovi[indeks];
        if(desnoDijete) roditelj->desno = novi;
        else roditelj->lijevo = novi;
        dodaj(novi, false, clanovi, 2*indeks+1);
        dodaj(novi, true, clanovi, 2*indeks+2);
    }
}

void prebaciStabloAB(vector<int> A, Cvor *B)
{
    //izracunati balans stabla A i stabla B

    //racunamo balans stabla B
    int balansB = visina(B->lijevo) - visina(B->desno);

    //racunamo balans stabla A
    int balansA = visina(A, 1) - visina(A, 2);

    //ako je balans A manji od balansa B potrebno je prebaciti citavo stablo A kao podstablo jednog od cvorova stabla B
    //tako da struktura stabla A ostane ocuvana u stablu B i da i A i B ostanu stabla binarne pretrage
    if(balansA < balansB) {
        int korijenA = A[0];
        Cvor *pom = B;
        while(pom) {
            if(korijenA > pom->vrijednost)
                pom = pom->desno;
            else
                pom = pom->lijevo;
        }
        if(korijenA > pom->vrijednost) { //stavljamo stablo A u desno podstablo
            dodaj(pom, true, A, 0);
        } else { // stavljamo stablo A u lijevo podstablo
            dodaj(pom, false, A, 0);
        }
    }



    //ako je balans A veci od balansa B potrebno je obrisati sve cvorove stabla B
    else if(balansA > balans B)
        obrisi(B); // pomocna funkcija obrisi

    //ako im je balans isti potrebno je pronaci minimalni element  stabla B i minimalni element stabla A i ispisati ih na ekran

    else {
        //minimalni element je najljevlji element
        int minA = A[0];
        for(int i = 0; i < A.size(); i = 2*i+1)
            if(A[i] < minA) minA = A[i];

        int minB = B->vrijednost;
        Cvor *pom = cvor;
        while(pom)
            pom = pom->lijevo;
        minB = pom->vrijednost;

        cout<< "Minimalni element stabla A je "<< minA <<", a minimalni element stabla B je " << minB << endl;

    }

}

}

int main()
{
    return 0;
}
