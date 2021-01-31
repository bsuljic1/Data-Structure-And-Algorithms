#include <iostream>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti &operator [](TipKljuca k) = 0;
    virtual TipVrijednosti operator [](TipKljuca k) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;

};




template <typename TipKljuca, typename TipVrijednosti>
class AVLCvor
{
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    AVLCvor<TipKljuca, TipVrijednosti> *lijevo = nullptr;
    AVLCvor<TipKljuca, TipVrijednosti> *desno = nullptr;
    AVLCvor<TipKljuca, TipVrijednosti> *roditelj = nullptr;
    int balans = 0;
    AVLCvor(const TipKljuca &k, const TipVrijednosti &v)
    {
        kljuc = k;
        vrijednost = v;
    }

    AVLCvor(const TipKljuca &k, const TipVrijednosti &v, AVLCvor<TipKljuca, TipVrijednosti> *l, AVLCvor<TipKljuca,TipVrijednosti> *d, AVLCvor<TipKljuca,TipVrijednosti> *r)
    {
        kljuc = k;
        vrijednost = v;
        lijevo = l;
        desno = d;
        roditelj = r;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    AVLCvor<TipKljuca, TipVrijednosti> *korijen = nullptr;
    int broj_elemenata = 0;


    void obrisiSve(AVLCvor<TipKljuca, TipVrijednosti> *cvor)
    {
        if(cvor != nullptr) {
            obrisiSve(cvor->lijevo);
            obrisiSve(cvor->desno);
            delete cvor;
        }
        korijen = nullptr;
    }


    void rotacijaUdesno(AVLCvor<TipKljuca, TipVrijednosti> *cvor)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *pom = cvor->lijevo;
        bool postojiLijevo = false;

        if(cvor->roditelj && cvor == cvor->roditelj->desno) cvor->roditelj->desno = cvor->lijevo;
        else if(cvor->roditelj) cvor->roditelj->lijevo = cvor->lijevo;

        if(cvor->lijevo->desno != nullptr) {
            postojiLijevo = true;
            cvor->lijevo = pom->desno;
            pom->desno->roditelj = cvor;
            pom->desno = nullptr;
        }

        pom->roditelj = cvor->roditelj;
        if(cvor->roditelj != nullptr) cvor->roditelj = pom;
        else {
            cvor->roditelj = pom;
            korijen = pom;
        }

        cvor->balans = 0;
        pom->balans = 0;
        pom->desno = cvor;
        if(postojiLijevo == false) cvor->lijevo == nullptr;
    }

    void rotacijaUlijevo(AVLCvor<TipKljuca, TipVrijednosti> *cvor)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *pom = cvor->desno;
        bool postojiDesno = false;

        if(cvor->roditelj && cvor == cvor->roditelj->lijevo) cvor->roditelj->lijevo = cvor->desno;
        else if(cvor->roditelj != nullptr) cvor->roditelj->desno = cvor->desno;

        if(cvor->desno->lijevo) {
            postojiDesno = true;
            cvor->desno = pom->lijevo;
            pom->lijevo->roditelj = cvor;
            pom->lijevo = nullptr;
        }

        pom->roditelj = cvor->roditelj;
        if(cvor->roditelj != nullptr) cvor->roditelj = pom;
        else {
            cvor->roditelj = pom;
            korijen = pom;
        }

        cvor->balans = 0;
        pom->balans = 0;
        pom->lijevo = cvor;
        if(postojiDesno == false) cvor->desno = nullptr;
    }


    void azurirajBalans(AVLCvor<TipKljuca, TipVrijednosti>* &cvor)
    {
        if(cvor->roditelj == nullptr) return;
        if(cvor->lijevo == nullptr && cvor->desno == nullptr && cvor->balans != 0) {
            if(cvor == cvor->roditelj->desno) cvor->roditelj->balans--;
            else cvor->roditelj->balans++;
        }

        AVLCvor<TipKljuca, TipVrijednosti> *rod = cvor->roditelj;
        AVLCvor<TipKljuca, TipVrijednosti> *tmp = cvor;
        bool azuriraj = true;

        if(cvor->balans < 0 && cvor->roditelj->balans > 1) {
            rotacijaUlijevo(tmp);
            rotacijaUdesno(rod);
            azuriraj = false;
        } else if(cvor->balans > 0 && cvor->roditelj->balans < -1) {
            rotacijaUdesno(tmp);
            rotacijaUlijevo(rod);
            azuriraj = false;
        } else if(cvor->roditelj->balans > 1) {
            rotacijaUdesno(rod);
            azuriraj = false;
        } else if(cvor->roditelj->balans < -1) {
            rotacijaUlijevo(rod);
            azuriraj = false;
        }

        if(azuriraj == true) azurirajBalans(cvor->roditelj);
    }

    void umetni(AVLCvor<TipKljuca, TipVrijednosti>* &cvor)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *tmp1 = korijen;
        bool exist = false;
        AVLCvor<TipKljuca, TipVrijednosti> *tmp2 = nullptr;

        while(tmp1 != nullptr) {
            tmp2 = tmp1;
            if(cvor->kljuc < tmp1->kljuc)
                tmp1 = tmp1->lijevo;
            else tmp1 = tmp1->desno;
        }

        if(tmp2 == nullptr) {
            korijen = cvor;
            exist = true;
            broj_elemenata++;
        } else {
            if(cvor->kljuc < tmp2->kljuc) {
                tmp2->lijevo = cvor;
                cvor->roditelj = tmp2;
                broj_elemenata++;
                if(tmp2->desno != nullptr) {
                    tmp2->balans = 0;
                    exist = true;
                }
            } else {
                tmp2->desno = cvor;
                cvor->roditelj = tmp2;
                broj_elemenata++;
                if(tmp2->lijevo != nullptr) {
                    tmp2->balans = 0;
                    exist = true;
                }
            }
        }

        if(exist == false) azurirajBalans(cvor);

    }

    TipVrijednosti &dodaj(TipKljuca k)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *novi = new AVLCvor<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
        umetni(novi);
        return novi->vrijednost;
    }

    void preorder(AVLCvor<TipKljuca, TipVrijednosti> *cvor)
    {
        if(cvor != nullptr) {
            dodaj(cvor->kljuc) = cvor->vrijednost;
            preorder(cvor->lijevo);
            preorder(cvor->desno);
        }
    }

public:
    ~AVLStabloMapa()
    {
        obrisiSve(korijen);
    }

    AVLStabloMapa() {}

    AVLStabloMapa(const AVLStabloMapa<TipKljuca, TipVrijednosti> &as)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *pom = as.korijen;
        preorder(pom);
    }

    AVLStabloMapa<TipKljuca, TipVrijednosti> &operator =(const AVLStabloMapa<TipKljuca, TipVrijednosti> &as)
    {

        if(this == &as) return *this;
        obrisiSve(korijen);
        korijen = nullptr;
        broj_elemenata = 0;
        AVLCvor<TipKljuca, TipVrijednosti> *pom = as.korijen;
        preorder(pom);
        return *this;
    }

    TipVrijednosti &operator [](TipKljuca k)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *rod = korijen;
        while(rod != nullptr && rod->kljuc != k) {
            if(k > rod->kljuc) rod = rod->desno;
            else rod = rod->lijevo;
        }

        if(rod) return rod->vrijednost;

        AVLCvor<TipKljuca, TipVrijednosti> *pom = new AVLCvor<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
        umetni(pom);
        return pom->vrijednost;
    }

    TipVrijednosti operator [](TipKljuca k) const
    {
        AVLCvor<TipKljuca, TipVrijednosti> *cvor = korijen;
        while(cvor != nullptr && cvor->kljuc != k) {
            if(k > cvor->kljuc) cvor = cvor->desno;
            else cvor = cvor->lijevo;
        }

        if(cvor) return cvor->vrijednost;

        TipVrijednosti pom = TipVrijednosti();
        return pom;
    }


    int brojElemenata() const
    {
        return broj_elemenata;
    }

    void obrisi()
    {
        obrisiSve(korijen);
        korijen = nullptr;
        broj_elemenata = 0;
    }

    void obrisi(const TipKljuca& k)
    {
        AVLCvor<TipKljuca, TipVrijednosti> *c1 = korijen;
        AVLCvor<TipKljuca, TipVrijednosti> *c2 = nullptr;
        AVLCvor<TipKljuca, TipVrijednosti> *pom = nullptr;
        AVLCvor<TipKljuca, TipVrijednosti> *pom2 = nullptr;
        AVLCvor<TipKljuca, TipVrijednosti> *pom3 = nullptr;
        while(c1 != nullptr && c1->kljuc != k) {
            c2 = c1;
            if(k < c1->kljuc) c1 = c1->lijevo;
            else c1 = c1->desno;
        }

        if(!c1) throw "Greska";
        if(c1->lijevo == nullptr) pom3 = c1->desno;
        else if(c1->desno == nullptr) pom3 = c1->lijevo;
        else {
            pom2 = c1;
            pom3 = c1->lijevo;
            pom = pom3->desno;
            while(pom != nullptr) {
                pom2 = pom3;
                pom3 = pom;
                pom = pom3->desno;
            }
            if(pom2 != c1) {
                pom2->desno = pom3->lijevo;
                pom3->lijevo = c1->lijevo;
                pom2->roditelj = pom3;
                pom3->roditelj = c2;
                if(pom2->desno) pom2->desno->roditelj = pom2;
            }
            pom3->desno = c1->desno;
            c1->desno->roditelj = pom3;
        }
        if(c2 == nullptr) {
            korijen = pom3;
            if(korijen != nullptr) korijen->roditelj = nullptr;
        } else if(c1 == c2->lijevo) {
            c2->lijevo = pom3;
            if(pom3 != nullptr) pom3->roditelj = c2;
        } else {
            c2->desno = pom3;
            if(pom3 != nullptr) pom3->roditelj = c2;
        }
        if(c1->roditelj && c1 == c1->roditelj->lijevo) c1->roditelj->balans--;
        else if(c1->roditelj != nullptr) c1->roditelj->balans++;
        delete c1;
        broj_elemenata--;
    }

};

int main()
{
    std::cout << "Zadaća 4, Zadatak 1";
    AVLStabloMapa <int, int> m;
    int vel(500000);
    int progress(1000);
    for (int i(0); i<vel; i++) {
        m[i] = i;
        if (i==progress) {
            cout<<"Dodano "<<i<<endl;
            progress *= 2;
        }
    }
// Obicno bin. stablo ce uspjeti dodati oko 30000-70000 el. za 10 sekundi
// Korektno AVL stablo ce zavrsiti kompletan test za <1 sekundu

    for (int i(0); i>=-vel; i--)
        m[i] = i;
    for (int i(-vel); i<vel; i++)
        if (m[i] != i) {
            cout << i << " NOT OK";
            break;
        }
    for (int i(-vel); i<vel; i++)
        m.obrisi(i);
    cout << "OK";
    return 0;
}
