#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

template <typename Tip>
class Lista
{
public:
    virtual ~Lista() {};
    virtual int brojElemenata() const = 0;
    virtual Tip &trenutni() = 0;
    virtual Tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip &operator [](int i) = 0;
    virtual Tip operator [](int i) const = 0;
};


template <typename Tip>
class NizLista : public Lista<Tip>
{
private:
    int kapacitet = 50;
    int broj_elemenata = 0;
    int tekuci = 0;
    Tip **T = nullptr;
public:
    NizLista() {}

    NizLista(const NizLista &n)
    {
        kapacitet = n.kapacitet;
        broj_elemenata = n.broj_elemenata;
        tekuci = n.tekuci;
        T = new Tip*[kapacitet];
        for(int i = 0; i < broj_elemenata; i++) T[i] = new Tip(*n.T[i]);
    }

    NizLista(NizLista &&n)
    {
        kapacitet = n.kapacitet;
        broj_elemenata = n.broj_elemenata;
        tekuci = n.tekuci;
        T = n.T;
        n.broj_elemenata = 0;
        n.T = nullptr;
    }

    NizLista &operator =(const NizLista &n)
    {
        if(this == &n) return *this;
        if(T != nullptr)
            for(int i = 0; i < broj_elemenata; i++) delete T[i];
        delete[] T;
        kapacitet = n.kapacitet;
        broj_elemenata = n.broj_elemenata;
        tekuci = n.tekuci;
        T = new Tip*[kapacitet];
        for(int i = 0; i < broj_elemenata; i++) T[i] = new Tip(*n.T[i]);
        return *this;
    }

    NizLista &operator =(NizLista &&n)
    {
        if(this == &n) return *this;
        kapacitet = n.kapacitet;
        broj_elemenata = n.broj_elemenata;
        tekuci = n.tekuci;
        T = n.T;
        n.T = nullptr;
        return *this;
    }

    ~NizLista()
    {
        if(T != nullptr)
            for(int i = 0; i < broj_elemenata; i++) delete T[i];
        delete[] T;
    }

    int brojElemenata() const
    {
        return broj_elemenata;
    }

    Tip &trenutni()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        return *T[tekuci];
    }

    Tip trenutni() const
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        return *T[tekuci];
    }

    bool prethodni()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        if(tekuci == 0) return false;
        tekuci = tekuci - 1;
        return true;

    }

    bool sljedeci()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        if(tekuci == broj_elemenata - 1) return false;
        tekuci = tekuci + 1;
        return true;
    }

    void pocetak()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        tekuci = 0;
    }

    void kraj()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        tekuci = broj_elemenata - 1;
    }

    void obrisi()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        delete T[tekuci];
        for(int i = tekuci; i <= broj_elemenata - 2; i++)
            T[i] = T[i+1];
        broj_elemenata--;
        if(tekuci == broj_elemenata) tekuci--;
        if(broj_elemenata == 0) {
            delete[] T;
            T = nullptr;
        }
    }

    void dodajIspred(const Tip& el)
    {
        if(broj_elemenata == 0) {
            T = new Tip*[kapacitet];
            T[0] = new Tip(el);
            tekuci = 0;
        } else {
            if(broj_elemenata == kapacitet) {
                Tip **pom = T;
                kapacitet += 50;
                T = new Tip*[kapacitet];
                for(int i = 0; i < broj_elemenata; i++)
                    T[i] = pom[i];
                delete[] pom;
            }
            for(int i = broj_elemenata - 1; i >= tekuci; i--)
                T[i+1] = T[i];

            T[tekuci] = new Tip(el);
            tekuci++;
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el)
    {
        if(broj_elemenata == 0) {
            T = new Tip*[kapacitet];
            T[0] = new Tip(el);
            tekuci = 0;
        } else {
            if(broj_elemenata == kapacitet) {
                Tip **pom = T;
                kapacitet += kapacitet/2;
                T = new Tip*[kapacitet];
                for(int i = 0; i < broj_elemenata; i++)
                    T[i] = pom[i];
                delete[] pom;
            }
            for(int i = broj_elemenata - 1; i > tekuci; i--)
                T[i+1] = T[i];

            T[tekuci + 1] = new Tip(el);
        }
        broj_elemenata++;
    }

    Tip &operator [](int i)
    {
        return *(T[i]);
    }

    Tip operator [](int i) const
    {
        return *(T[i]);
    }

};

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    virtual ~Mapa() {}
    virtual TipVrijednosti &operator [](TipKljuca k) = 0;
    virtual TipVrijednosti operator [](TipKljuca k) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;

};

template<typename TipKljuca, typename TipVrijednosti>
class Pair
{
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca,TipVrijednosti>
{
    NizLista<Pair<TipKljuca, TipVrijednosti>> niz;
public:
    ~NizMapa()
    {
    }

    NizMapa()
    {
    }

    TipVrijednosti &operator [](TipKljuca k)
    {
        int br = niz.brojElemenata();
        for(int i = 0; i < br; i++)
            if(niz[i].kljuc == k) return niz[i].vrijednost;
        if(br > 1) niz.kraj();
        niz.dodajIza({k, TipVrijednosti()});
        niz.kraj();
        return niz.trenutni().vrijednost;
    }

    TipVrijednosti operator [](TipKljuca k) const
    {
        int br = niz.brojElemenata();
        for(int j = 0; j < br; j++)
            if(niz[j].kljuc == k) return niz[j].vrijednost;
        return TipVrijednosti();
    }

    int brojElemenata() const
    {
        return niz.brojElemenata();
    }

    void obrisi()
    {
        int br = niz.brojElemenata();
        for(int i = 0; i < br; i++)
            niz.obrisi();
    }

    void obrisi(const TipKljuca& kljuc)
    {
        bool postoji = false;
        int br = niz.brojElemenata();
        niz.pocetak();
        for(int i = 0; i < br; i++) {
            if(niz.trenutni().kljuc == kljuc) {
                postoji = true;
                niz.obrisi();
                return;
            } else {
                niz.sljedeci();
            }
        }
        if(!postoji) throw "Kljuc ne postoji";
    }
};


template <typename TipKljuca, typename TipVrijednosti>
class BSCvor
{
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    BSCvor *lijevo;
    BSCvor *desno;
    BSCvor *roditelj;
    BSCvor(const TipKljuca &k, const TipVrijednosti &v, BSCvor<TipKljuca, TipVrijednosti> *l, BSCvor<TipKljuca,TipVrijednosti> *d, BSCvor<TipKljuca,TipVrijednosti> *r)
    {
        kljuc = k;
        vrijednost = v;
        lijevo = l;
        desno = d;
        roditelj = r;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    BSCvor<TipKljuca, TipVrijednosti> *korijen = nullptr;
    int broj_elemenata = 0;

    BSCvor<TipKljuca, TipVrijednosti>* trazi(const TipKljuca &k, BSCvor<TipKljuca,TipVrijednosti> *c) const
    {
        if(c == nullptr || k == c->kljuc) return c;
        if(k > c->kljuc)
            return trazi(k, c->desno);
        return trazi(k, c->lijevo);
    }

    BSCvor<TipKljuca, TipVrijednosti>* dodaj(const TipKljuca &k, const TipVrijednosti &v, BSCvor<TipKljuca,TipVrijednosti> *&c, BSCvor<TipKljuca, TipVrijednosti> *r)
    {
        if(!c) {
            c =  new BSCvor<TipKljuca,TipVrijednosti> (k, v, nullptr, nullptr, r);
            broj_elemenata++;
            return c;
        }
        if(k == c->kljuc) return c;
        if(k > c->kljuc)
            return dodaj(k, v, c->desno, c);
        return dodaj(k, v, c->lijevo, c);
    }



    void kopiraj(BSCvor<TipKljuca, TipVrijednosti> *&c1, BSCvor<TipKljuca,TipVrijednosti> *c2, BSCvor<TipKljuca,TipVrijednosti> *r)
    {
        if(!c2) return;
        c1 = new BSCvor<TipKljuca,TipVrijednosti>(c2->kljuc, c2->vrijednost, nullptr, nullptr, r);
        kopiraj(c1->lijevo, c2->lijevo, c1);
        kopiraj(c1->lijevo, c2->desno, c1);
        broj_elemenata++;
    }

    void obrisi(BSCvor<TipKljuca,TipVrijednosti> *cvor)
    {
        if(!cvor) return;
        obrisi(cvor->lijevo);
        obrisi(cvor->desno);
        delete cvor;
        broj_elemenata--;
    }

    void obrisiPomocna(BSCvor<TipKljuca,TipVrijednosti> *cvor)
    {
        if(!cvor) return;
        if(!cvor->desno && !cvor->lijevo) {
            if(cvor->roditelj) {
                if(cvor == cvor->roditelj->desno) cvor->roditelj->desno = nullptr;
                else cvor->roditelj->lijevo = nullptr;
                delete cvor;
            } else {
                delete cvor;
                korijen = nullptr;
            }
            broj_elemenata--;
        } else if(cvor->desno && cvor->lijevo) {
            BSCvor<TipKljuca,TipVrijednosti> *t = cvor->lijevo;
            while(t->desno != nullptr) t = t->desno;
            cvor->kljuc = t->kljuc;
            cvor->vrijednost = t->vrijednost;
            obrisiPomocna(t);
        } else if((cvor->desno && !cvor->lijevo) || (!cvor->desno && cvor->lijevo)) {
            BSCvor<TipKljuca,TipVrijednosti> *child = cvor->desno;
            if(!child) child = cvor->lijevo;
            if(cvor->roditelj != nullptr) {
                if(cvor == (cvor->roditelj->desno)) cvor->roditelj->desno = child;
                else (cvor->roditelj)->lijevo = child;
            }
            child->roditelj = cvor->roditelj;
            if(child->roditelj == nullptr) korijen = child;
            delete cvor;
            broj_elemenata--;
        }
    }

public:
    ~BinStabloMapa()
    {
        obrisi(korijen);
    }

    BinStabloMapa() {}

    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &bs)
    {
        kopiraj(korijen, bs.korijen, nullptr);
    }

    BinStabloMapa<TipKljuca, TipVrijednosti> &operator =(const BinStabloMapa<TipKljuca, TipVrijednosti> &bs)
    {
        if(this == &bs) return *this;
        obrisi(korijen);
        korijen = nullptr;
        kopiraj(korijen, bs.korijen, nullptr);
        return *this;
    }

    TipVrijednosti &operator [](TipKljuca k)
    {
        BSCvor<TipKljuca,TipVrijednosti> *cvor = dodaj(k, TipVrijednosti(), korijen, nullptr);
        return cvor->vrijednost;
    }

    TipVrijednosti operator [](TipKljuca k) const
    {
        BSCvor<TipKljuca,TipVrijednosti> *cvor = trazi(k, korijen);
        if(!cvor) return TipVrijednosti();
        else return cvor->vrijednost;
    }

    int brojElemenata() const
    {
        return broj_elemenata;
    }

    void obrisi()
    {
        obrisi(korijen);
        korijen = nullptr;
    }

    void obrisi(const TipKljuca& kljuc)
    {
        BSCvor<TipKljuca,TipVrijednosti> *cvor = trazi(kljuc, korijen);
        obrisiPomocna(cvor);

    }

};

int main()
{
    NizMapa<int, int> nm; // test performansi NizMape
    cout << "NizMapa performanse: " << endl;
    int isti = rand();
    auto startNMdodavanje = high_resolution_clock::now();
    for(int i = 1; i < 1000; i++) // dodavanje 1000 elemenata u nm
        nm[rand()] = rand(); //slucajno generisani
    nm[isti] = rand();
    auto stopNMdodavanje = high_resolution_clock::now();
    auto trajanjeNMdodavanje  = duration_cast<microseconds>(stopNMdodavanje - startNMdodavanje);
    cout << "Dodavanje novog elementa u mapu: " << trajanjeNMdodavanje.count() << endl;;

    auto startNMpristup = high_resolution_clock::now();
    int postojeci = nm[isti]; // pristup postojecem elementu u nm
    auto stopNMpristup = high_resolution_clock::now();
    auto trajanjeNMpristup = duration_cast<microseconds>(stopNMpristup - startNMpristup);
    cout << "Pristup postojeceg elementa u mapi: " << trajanjeNMpristup.count() << endl;

    BinStabloMapa<int, int> bsm; // test performansi BinStabloMape
    cout << "BinStabloMapa performanse: " << endl;

    auto startBSMdodavanje = high_resolution_clock::now();
    for(int i = 1; i < 1000; i++)
        bsm[rand()] = rand(); // dodavanje 1000 elemenata u bsm
    bsm[isti] = rand();
    auto stopBSMdodavanje = high_resolution_clock::now();
    auto trajanjeBSMdodavanje = duration_cast<microseconds>(stopBSMdodavanje - startBSMdodavanje);
    cout << "Dodavanje novog elementa u mapu: " << trajanjeBSMdodavanje.count() << endl;

    auto startBSMpristup = high_resolution_clock::now();
    int postojeci2 = bsm[isti]; // pristup postojecem elementu u bsm
    auto stopBSMpristup = high_resolution_clock::now();
    auto trajanjeBSMpristup = duration_cast<microseconds>(stopBSMpristup - startBSMpristup);
    cout << "Pristup postojeceg elementa u mapi: " << trajanjeBSMpristup.count() << endl;

    //Prosjecno vrijeme izvrsavanja je izrazeno u mikrosekundama.
    //Vidimo da je, sto se tice dodavanja elementa u mapu, za NizMapu prosjecno vrijeme izvrsavanja bilo 51784ms
    //dok je za BinStabloMapu bilo 6537ms, sto znaci da su performanse dodavanja elemena u BinStabloMapu bolje nego dodavanje elementa u NizMapu
    //Takodjer, za pristup postojecem elementu u mapi, prosjecno vrijeme izvrsavanja za NizMapu bilo je 364ms,
    //dok je za BinStabloMapu bilo 224ms, sto znaci da su performanse pristupa postojecem elementu u BinStabloMapii bolji nego pristup postojecem elementu u NizMapi

    //Konacno, vidimo da su performanse BinStabloMape puno bolje nego performanse NizMape sto se vidi iz prethodnih razmatranja

    return 0;
}
