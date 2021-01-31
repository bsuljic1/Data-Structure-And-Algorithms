#include <iostream>
#include <vector>
#include <algorythm>

using namespace std;

class Cvor
{
public:
    int vrijednost = 0;
    Cvor *lijevo = nullptr;
    Cvor *desno = nullptr;
    Cvor() {}
};


void obrisi( std::vector<int> &stablo, int indeksElementa)
{
    obrisi(2*indeksElementa + 1);
    obrisi(2*indeksElementa + 2);
    stablo.at(indeksElementa) = -1;
}

int visina(Cvor *cvor)
{
    if(cvor == nullptr) return 0;
    if(cvor->desno == nullptr && cvor->lijevo == nullptr) return 1;
    int lijevo = visina(cvor->lijevo);
    int desno = visina(cvor->desno);
    if(lijevo > desno) return lijevo+ 1;
    else return desno + 1;
}


int funkcija(vector<int> stabloA, Cvor* stabloB)
{
    //pronaci drugi element po velicini u stabluB
    //najveci element je onaj koji je najdesniji u stablu
    //drugi po velicini je roditelj najdesnijeg cvora u stablu
    Cvor *pom = stabloB;//pokazivac na korijen stabla
    while(pom->desni) {
        pom = pom->desno;
    }

    //pom sada pokazuje na najdesniji cvor i to je najveci element u stablu
    if(pom->lijevo == nullptr) //ako ima lijevo dijete, pa je zapravo pom najveci
        pom = pom->roditelj; // ovo je roditelj najdesnijeg cvora i on je drugi po velicini najveci
    int drugiPoVelicni = pom->vrijednost; // drugi po velicini element

    //sada trazimo najblizi cvor po velicini u stabluA, ovom cvoru pom
    int brojCvorova = stabloA.size(); // broj cvorova u stabluA
    int korijen = stabloA.at(0);
    //stablo preko niza je implementirano tako da je na prvom mjestu uvijek korijen
    //lijevo dijete svakog elementa je na indeksu 2*indeksRoditelja+1, a desno dijete na indeksu 2*indeksRoditelja+2

    int najblizi = korijen;
    int indeksNajblizeg = 0;
    //u ovoj petlji trazimo najblizi
    for(int i = 0; i < brojCvorova; i++)
        if(stabloA.at(i) ! = -1 && (abs(stabloA.at(i) - drugiPoVelicni) < abs(najblizi - drugiPoVelicni))) {
            najblizi = stabloA.at(i);
            indeksNajblizeg = i;
        }


    //pronasli smo najblizi, i sada ga trebamo obrisati zajedno sa njegovim lijevim i desnim podstablom
    //to cemo uraditi pomocnom funkcijom obrisi
    obrisi(stabloA, indeksElementa);

    //jos trebamo izracunati balans korijenskog cvora stablaB i vratiti ga kao rezultat

    int balans = visina(stabloB->lijevo) - visina(stabloB->desno);
    return balans;
}

int main()
{
    return 0;
}
