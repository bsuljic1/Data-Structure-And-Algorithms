#include <iostream>
#include <vector>

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
    virtual ~Mapa(){}
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

    TipVrijednosti &operator [](TipKljuca k){
        int br = niz.brojElemenata();
        for(int i = 0; i < br; i++)
            if(niz[i].kljuc == k) return niz[i].vrijednost;
        if(br > 1) niz.kraj();
        niz.dodajIza({k, TipVrijednosti()});
        niz.kraj();
        return niz.trenutni().vrijednost;
    }
    
    TipVrijednosti operator [](TipKljuca k) const{
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

void test1(){
    NizMapa<int, std::string> nizMapa;
    nizMapa[1] = "1";
    nizMapa[2] = "2";
    nizMapa[3] = "3";
    std::cout << "Broj elemenata: " << nizMapa.brojElemenata() << std::endl; //treba ispisati 3
    std::cout << "Drugi element je: " << nizMapa[2] << std::endl; //treba ispisati 2
    std::cout << "Test praznih" << nizMapa[50] << std::endl; //treba ispisati 1
    nizMapa.obrisi();
    std::cout << "Broj elemenata nakon brisanja: "<< nizMapa.brojElemenata() << std::endl; // treba ispisati 0
    
}

void test2(){
    NizMapa<std::string, int> brojStanovnikaGradova;
    brojStanovnikaGradova["Sarajevo"] = 275524;
    brojStanovnikaGradova["Gorazde"] = 22080;
    brojStanovnikaGradova["Travnik"] = 57543;
    std::cout << "Broj stanovnika Sarajeva: " << brojStanovnikaGradova["Sarajevo"] << std::endl;
    std::cout << "Broj stanovnika Travnika: " << brojStanovnikaGradova["Travnik"] << std::endl;
    std::cout << "Broj stanovnika Gorazda: " << brojStanovnikaGradova["Gorazde"] << std::endl;
}

void test3(){
    NizMapa<std::string, std::string> glavniGradovi;
    glavniGradovi["Francuska"] = "Pariz";
    glavniGradovi["Njemacka"] = "Berlin";
    glavniGradovi["Austrija"] = "Bec";
    std::cout << "Glavni grad Francuske je " << glavniGradovi["Francuska"] << ", Njemacke je " << 
                glavniGradovi["Njemacka"] << ", a Austrije je " << glavniGradovi["Austrija"] << std::endl;
}

void test4(){
    NizMapa<std::string, std::vector<std::string>> poznati;
    poznati["fudbal"] = {"Edin Dzeko", "Miralem Pjanic", "Edin Visca"};
    poznati["muzika"] = {"Dino Merlin", "Dzejla Ramovic", "Halid Beslic"};
    poznati["knjizevnici"] = {"Mesa Selimovic", "Ivo Andric", "Nura Bazdulj-Hubijar"};
    std::cout << "Poznati bosanski fudbaleri su: " << poznati["fudbal"].at(0) << ", " << poznati["fudbal"].at(1) << ", " << poznati["fudbal"].at(2) <<
            std::endl << "Muzicari " <<  poznati["muzika"].at(0) << ", " << poznati["muzika"].at(1) << ", " << poznati["muzika"].at(2) <<
            std::endl << "Knjizevnici " << poznati["knjizevnici"].at(0) << ", " << poznati["knjizevnici"].at(1) << ", " << poznati["knjizevnici"].at(2) << std::endl;
}

void test5(){
    NizMapa<double, int> zaokruzi;
    zaokruzi[5.8] = 6;
    zaokruzi[10.1] = 10;
    zaokruzi[12.6] = 13;
    std::cout << "Broj 5.8 zaokruzeno je " << zaokruzi[5.8] << std::endl <<
                "Broj 10.1 zaokruzeno je " << zaokruzi[10.1] << std::endl <<
                "Broj 12.6 zaokruzeno je " << zaokruzi[12.6] << std::endl;
}

int main()
{
    std::cout << "Pripremna Zadaca Za Vjezbu 7, Zadatak 1" << std::endl;
    std::cout <<"TEST 1" << std::endl;
    test1();
    std::cout << std::endl << "TEST 2" << std::endl;
    test2();
    std::cout << std::endl << "TEST 3" << std::endl;
    test3();
    std::cout << std::endl << "TEST 4" << std::endl;
    test4();
    std::cout << std::endl << "TEST 5" << std::endl;
    test5();
    return 0;
}
