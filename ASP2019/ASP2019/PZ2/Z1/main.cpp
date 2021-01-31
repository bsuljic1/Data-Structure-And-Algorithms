#include <iostream>
using namespace std;

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

template<typename Tip>
class Cvor
{
public:
    Tip element;
    Cvor *sljedeci;
    Cvor()
    {
        sljedeci = nullptr;
    }

    Cvor(const Cvor &c)
    {
        element = c.element;
        sljedeci = c.sljedeci;
    }
};

template<typename Tip>
class JednostrukaLista : public Lista<Tip>
{
private:
    int broj_elemenata = 0;
    Cvor<Tip> *poc = nullptr;
    Cvor<Tip> *tekuci = nullptr;
public:
    JednostrukaLista() {}

    JednostrukaLista(const JednostrukaLista &l)
    {
        broj_elemenata = l.broj_elemenata;
        if(broj_elemenata == 0) return;
        poc = new Cvor<Tip>(*l.poc);
        Cvor<Tip> *pom = poc;
        while(pom->sljedeci != nullptr) {
            pom->sljedeci = new Cvor<Tip>(*pom->sljedeci);
            pom = pom->sljedeci;
        }
    }

    JednostrukaLista(JednostrukaLista &&l)
    {
        broj_elemenata = l.broj_elemenata;
        poc = l.poc;
        tekuci = l.tekuci;
        l.broj_elemenata = 0;
        l.poc = nullptr;
        l.tekuci = nullptr;
    }

    ~JednostrukaLista()
    {
        Cvor<Tip> *pom = poc;
        Cvor<Tip> *pom2 = poc;
        while(pom != nullptr) {
            pom2 = pom->sljedeci;
            delete pom;
            pom = pom2;
        }
    }

    JednostrukaLista &operator =(const JednostrukaLista &l)
    {
        if(&l != this) {
            Cvor<Tip> *pom = poc;
            if(pom != nullptr)
                while(pom->sljedeci != nullptr) {
                    poc = poc->sljedeci;
                    delete pom;
                    pom = poc;
                }

            broj_elemenata = l.broj_elemenata;
            if(broj_elemenata != 0) {
                poc = new Cvor<Tip>(*l.poc);
                Cvor<Tip> *pom2 = poc;
                while (pom2->sljedeci != nullptr) {
                    pom2->sljedeci = new Cvor<Tip>(*pom2->sljedeci);
                    pom2 = pom2->sljedeci;
                }
            } else {
                poc = nullptr;
                tekuci = nullptr;
            }
        }

        return *this;
    }

    JednostrukaLista &operator =(JednostrukaLista &&l)
    {
        if(&l != this) {
            broj_elemenata = l.broj_elemenata;
            tekuci = l.tekuci;
            Cvor<Tip> *pom = l.poc;
            while(pom->sljedeci != nullptr) {
                l.poc = l.poc->sljedeci;
                delete pom;
                pom = poc;
            }
            poc = l.poc;
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
        Cvor<Tip> *pom = poc;
        while(pom->sljedeci != tekuci) pom = pom->sljedeci;
        tekuci = pom;
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
        while(tekuci->sljedeci != nullptr) tekuci = tekuci->sljedeci;
    }

    void obrisi()
    {
        if(broj_elemenata == 0) throw std::domain_error("Lista je prazna");
        if(poc == tekuci) {
            Cvor<Tip> *pom = poc;
            tekuci = poc->sljedeci;
            delete pom;
            poc=tekuci;
        } else {
            Cvor<Tip> *pom = poc;
            while(pom->sljedeci != tekuci) pom = pom->sljedeci;
            pom->sljedeci = tekuci->sljedeci;
            delete tekuci;
            if(pom->sljedeci != nullptr) tekuci = pom->sljedeci;
            else tekuci = pom;
        }
        broj_elemenata--;
    }

    void dodajIspred(const Tip& el)
    {
        if(poc == tekuci) {
            Cvor<Tip> *pom = new Cvor<Tip>;
            pom->element = el;
            pom->sljedeci = poc;
            poc = pom;
            if(tekuci == nullptr) {
                tekuci = poc;
            }
        } else {
            Cvor<Tip> *p = new Cvor<Tip>;
            p->element = el;
            Cvor<Tip> *pom = poc;
            while(pom->sljedeci != tekuci) pom = pom->sljedeci;
            pom->sljedeci = p;
            p->sljedeci = tekuci;
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el)
    {
        if(broj_elemenata == 0) {
            Cvor<Tip> *p = new Cvor<Tip>;
            p->element = el;
            p->sljedeci = nullptr;
            poc = p;
            tekuci = poc;
        } else {
            Cvor<Tip> *p = tekuci->sljedeci;
            tekuci->sljedeci = new Cvor<Tip>;
            tekuci->sljedeci->element = el;
            tekuci->sljedeci->sljedeci = p;
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

void testNizLista()
{
    NizLista<int> niz;
    for(int i = 1; i < 10; i++)
        niz.dodajIspred(i);
    std::cout << niz.trenutni() << std::endl;
    niz.pocetak();
    std::cout << niz.trenutni() << std::endl;
    niz.sljedeci();
    std::cout << niz.trenutni() << std::endl;
    niz.obrisi();
    std::cout << niz.brojElemenata() << std::endl;
    std::cout << niz.trenutni() << std::endl;
    niz.kraj();
    std::cout << niz.trenutni() << std::endl;
    for(int i = 0; i < niz.brojElemenata(); i++)
        niz.obrisi();
}

void testJednostrukaLista()
{
    JednostrukaLista<int> lista;
    for(int i = 1; i <= 10; i++)
        lista.dodajIspred(i);
    std::cout << lista.trenutni() << std::endl;
    lista.pocetak();
    std::cout << lista.trenutni() << std::endl;
    lista.sljedeci();
    std::cout << lista.trenutni() << std::endl;
    lista.obrisi();
    std::cout << lista.brojElemenata() << std::endl;
    std::cout << lista.trenutni() << std::endl;
    lista.kraj();
    std::cout << lista.trenutni() << std::endl;
    for(int i = 0; i < lista.brojElemenata(); i++)
        lista.obrisi();
}

int main()
{
    std::cout << "Pripremna Zadaća 2 2019/2020, Zadatak 1" << std::endl;
    testNizLista();
    std::cout << std::endl;
    testJednostrukaLista();
    return 0;
}
