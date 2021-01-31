#include <iostream>

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

template<typename Tip>
class Cvor
{
public:
    Tip element;
    Cvor *prethodni;
    Cvor *sljedeci;
    Cvor()
    {
        prethodni = nullptr;
        sljedeci = nullptr;
    }

    Cvor(const Cvor &c)
    {
        element = c.element;
        prethodni = c.prethodni;
        sljedeci = c.sljedeci;
    }
};

template<typename Tip>
class DvostrukaLista : public Lista<Tip>
{
private:
    int broj_elemenata = 0;
    Cvor<Tip> *poc = nullptr;
    Cvor<Tip> *Kraj = nullptr;
    Cvor<Tip> *tekuci = nullptr;
public:
    DvostrukaLista() {}

    DvostrukaLista(const DvostrukaLista<Tip> &l)
    {
        broj_elemenata = l.broj_elemenata;
        if(broj_elemenata == 0) return;
        poc = new Cvor<Tip>(*l.poc);
        Cvor<Tip> *pom = poc;
        Cvor<Tip> *pr = nullptr;
        while(pom->sljedeci != nullptr) {
            pom->sljedeci = new Cvor<Tip>(*pom->sljedeci);
            pom->prethodni = pr;
            pr = pom;
            pom = pom->sljedeci;
        }
        Kraj = pom;
        //tekuci = poc;
    }

    DvostrukaLista(DvostrukaLista<Tip> &&l)
    {
        broj_elemenata = l.broj_elemenata;
        poc = l.poc;
        Kraj = l.Kraj;
        tekuci = l.tekuci;
        l.broj_elemenata = 0;
        l.poc = nullptr;
        l.tekuci = nullptr;
        l.Kraj = nullptr;
    }

    ~DvostrukaLista()
    {
        Cvor<Tip> *pom = poc;
        Cvor<Tip> *pom2 = poc;
        while(pom != nullptr) {
            pom2 = pom->sljedeci;
            delete pom;
            pom = pom2;
        }
    }

    DvostrukaLista &operator =(const DvostrukaLista<Tip> &l)
    {
        if(&l != this) {
            Cvor<Tip> *pom = poc;
            while(pom != nullptr) {
                poc = pom->sljedeci;
                delete pom;
                pom = poc;
            }

            broj_elemenata = l.broj_elemenata;
            if(broj_elemenata != 0) {
                poc = new Cvor<Tip>(*l.poc);
                Cvor<Tip> *pom2 = poc;
                Cvor<Tip> *pr = nullptr;
                while (pom2->sljedeci != nullptr) {
                    pom2->sljedeci = new Cvor<Tip>(*pom2->sljedeci);
                    pom2->prethodni = pr;
                    pr = pom2;
                    pom2 = pom2->sljedeci;
                }
                Kraj = pom2;
            } else {
                poc = nullptr;
                tekuci = nullptr;
                Kraj = nullptr;
            }
        }

        return *this;
    }

    DvostrukaLista &operator =(DvostrukaLista<Tip> &&l)
    {
        if(&l != this) {
            broj_elemenata = l.broj_elemenata;
            tekuci = l.tekuci;
            Cvor<Tip> *pom = poc;
            while(pom != nullptr) {
                poc = poc->sljedeci;
                delete pom;
                pom = poc;
            }
            poc = l.poc;
            Kraj = l.Kraj;
        }
        return *this;
    }

    int brojElemenata() const
    {
        return broj_elemenata;
    }

    Tip &trenutni()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        return tekuci->element;
    }

    Tip trenutni() const
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        return tekuci->element;
    }

    bool prethodni()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        if(tekuci == poc) return false;
        tekuci = tekuci->prethodni;
        return true;
    }

    bool sljedeci()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        if(tekuci->sljedeci == nullptr) return false;
        tekuci = tekuci->sljedeci;
        return true;
    }

    void pocetak()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        tekuci = poc;
    }

    void kraj()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        tekuci = Kraj;
    }

    void obrisi()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        if(poc == tekuci) {
            Cvor<Tip> *pom = poc;
            poc = poc->sljedeci;
            delete pom;
            tekuci = poc;
        } else {
            Cvor<Tip> *pom = tekuci->prethodni;
            pom->sljedeci = tekuci->sljedeci;
            delete tekuci;
            if(pom->sljedeci != nullptr) tekuci = pom->sljedeci;
            else {
                tekuci = pom;
                Kraj = pom;
            }
        }
        broj_elemenata--;
    }

    void dodajIspred(const Tip& el)
    {
        if(poc == tekuci) {
            Cvor<Tip> *pom = new Cvor<Tip>;
            pom->element = el;
            pom->sljedeci = poc;
            if(broj_elemenata != 0) poc->prethodni = pom;
            poc = pom;
            if(tekuci == nullptr) {
                tekuci = poc;
                Kraj = poc;
            }
        } else {
            Cvor<Tip> *p = new Cvor<Tip>;
            p->element = el;
            tekuci->prethodni->sljedeci = p;
            p->prethodni = tekuci->prethodni;
            tekuci->prethodni = p;
            p->sljedeci = tekuci;
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el)
    {
        if(broj_elemenata == 0) {
            Cvor<Tip> *p = new Cvor<Tip>;
            p->element = el;
            poc = p;
            tekuci = poc;
            Kraj = poc;
        } else {
            Cvor<Tip> *p = tekuci->sljedeci;
            tekuci->sljedeci = new Cvor<Tip>;
            tekuci->sljedeci->element = el;
            tekuci->sljedeci->sljedeci = p;
            tekuci->sljedeci->prethodni = tekuci;
            if(tekuci == Kraj) Kraj = tekuci->sljedeci;
            if(p != nullptr)
                p->prethodni = tekuci->sljedeci;
        }
        broj_elemenata++;

    }

    Tip &operator [](int i)
    {
        Cvor<Tip> *pom = poc;
        for(int j = 0; j < i; j++)
            pom = pom->sljedeci;
        return pom->element;
    }

    Tip operator [] (int i) const
    {
        Cvor<Tip> *pom = poc;
        for(int j = 0; j < i; j++)
            pom = pom->sljedeci;
        return pom->element;
    }

};
template<typename Tip>
class DvostraniRed
{
    DvostrukaLista<Tip> red;
public:
    DvostraniRed()
    {
    }

    ~DvostraniRed()
    {
    }

    DvostraniRed(const DvostraniRed<Tip> &dr) : red(dr.red)
    {
    }

    DvostraniRed &operator=(const DvostraniRed &dr)
    {
        red = dr.red;
        return *this;
    }

    DvostraniRed &operator=(DvostraniRed &&dr)
    {
        red = dr.red;
        return *this;
    }

    void brisi()
    {
        while(red.brojElemenata() != 0) red.obrisi();
    }

    int brojElemenata()
    {
        return red.brojElemenata();
    }

    void staviNaCelo(const Tip &el)
    {
        if(red.brojElemenata() != 0)red.kraj();
        red.dodajIza(el);
    }

    Tip skiniSaCela()
    {
        if(red.brojElemenata() != 0)red.kraj();
        Tip el = red.trenutni();
        red.obrisi();
        return el;
    }

    void staviNaVrh(const Tip &el)
    {
        if(red.brojElemenata() != 0) red.pocetak();
        red.dodajIspred(el);
    }

    Tip skiniSaVrha()
    {
        if(red.brojElemenata() != 0) red.pocetak();
        Tip el = red.trenutni();
        red.obrisi();
        return el;
    }

    Tip& celo()
    {
        if(red.brojElemenata() != 0) red.kraj();
        return red.trenutni();
    }

    Tip vrh()
    {
        if(red.brojElemenata() != 0) red.pocetak();
        return red.trenutni();
    }
};

void testBrisi()
{
    DvostraniRed<int> dr;
    for(int i = 1; i <= 5; i++)
        dr.staviNaCelo(i);
    std::cout << "Broj elemenata prije brisanja: " << dr.brojElemenata() << std::endl; //treba ispisati 5
    dr.brisi();
    std::cout << "Broj elemenata poslije brisanja: " << dr.brojElemenata() << std::endl; //treba ispisati 0
}

void testStaviNaCelo()
{
    DvostraniRed<int> dr;
    for(int i = 1; i<=5; i++)
        dr.staviNaCelo(i);
    std::cout << "Broj elemenata poslije 5 stavljanja na celo: " << dr.brojElemenata() << std::endl; //treba ispisati 5
}

void testStaviNaVrh()
{
    DvostraniRed<int> dr;
    for(int i = 1; i <= 5; i++)
        dr.staviNaVrh(i);
    std::cout << "Broj elemenata poslije 5 stavljanja na vrh: " << dr.brojElemenata() << std::endl; //treba ispisati 5
}

void testSkiniSaCela()
{
    DvostraniRed<int> dr;
    for(int i  = 1; i <= 5; i++)
        dr.staviNaCelo(i);
    std::cout << "Broj elemenata prije skidanja sa cela: " << dr.brojElemenata() << std::endl; // treba ispisati 5
    std::cout << "Skini sa cela: " << dr.skiniSaCela() << std::endl; // treba ispisati 5(element sa cela)
    std::cout << "Broj elemenata poslije skidanja sa cela: " << dr.brojElemenata() <<std::endl; // treba ispisati 4
}

void testSkiniSaVrha(){
    DvostraniRed<int> dr;
    for(int i = 1; i <= 5; i++)
        dr.staviNaCelo(i);
    std::cout << "Broj elemenata prije skidanja sa vrha: " << dr.brojElemenata() << std::endl; // treba ispisati 5
    std::cout << "Skini sa vrha: " << dr.skiniSaVrha() << std::endl; // treba ispisati 1(element sa vrha)
    std::cout << "Broj elemenata poslije skidanja sa vrha: " << dr.brojElemenata() <<std::endl; // treba ispisati 4
}

void testBrojElemenata()
{
    DvostraniRed<int> dr;
    for(int i = 1; i <= 5; i++)
        dr.staviNaCelo(i);
    std::cout << "Broj elemenata u dvostranom redu: " << dr.brojElemenata() << std::endl; // treba ispisati 5
}

void testCelo()
{
    DvostraniRed<int> dr;
    for(int i  = 1; i <= 5; i++)
        dr.staviNaCelo(i);
    std::cout << "Element na celu dvostranog reda je: " << dr.celo() << std::endl; // treba ispisati 5
}

void testVrh()
{
    DvostraniRed<int> dr;
     for(int i  = 1; i <= 5; i++)
        dr.staviNaCelo(i);
    std::cout << "Element na vrhu dvostranog reda je: " << dr.vrh() << std::endl; // treba ispisati 1

}

int main()
{
    std::cout << "Zadaća 2, Zadatak 1" << std::endl;
    std::cout << "Test brisi: " <<std::endl;
    testBrisi();
    std::cout << "Test staviNaCelo: " <<std::endl;
    testStaviNaCelo();
    std::cout << "Test staviNaVrh: " <<std::endl;
    testStaviNaVrh();
    std::cout << "Test skiniSaCela: " <<std::endl;
    testSkiniSaCela();
    std::cout << "Test skiniSaVrha: " <<std::endl;
    testSkiniSaVrha();
    std::cout << "Test brojElemenata: " <<std::endl;
    testBrojElemenata();
    std::cout << "Test celo: " <<std::endl;
    testCelo();
    std::cout << "Test vrh: " <<std::endl;
    testVrh();
    return 0;
}
