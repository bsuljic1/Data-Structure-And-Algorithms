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

    DvostrukaLista(const DvostrukaLista &l)
    {
        broj_elemenata = l.broj_elemenata;
        if(broj_elemenata == 0) return;
        poc = new Cvor<Tip>(*l.poc);
        Cvor<Tip> *pom = poc;
        while(pom->sljedeci != nullptr) {
            pom->sljedeci = new Cvor<Tip>(*pom->sljedeci);
            pom = pom->sljedeci;
        }
        Kraj = l.Kraj;
    }

    DvostrukaLista(DvostrukaLista &&l)
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

    DvostrukaLista &operator =(const DvostrukaLista &l)
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
                Kraj = nullptr;
            }
        }

        return *this;
    }

    DvostrukaLista &operator =(DvostrukaLista &&l)
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
            tekuci = poc->sljedeci;
            delete pom;
            poc=tekuci;
        } else {
            Kraj = Kraj->prethodni;
            Cvor<Tip> *pom = tekuci->prethodni;
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
            if(tekuci->sljedeci == nullptr) Kraj = tekuci;
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el)
    {
        if(broj_elemenata == 0) {
            Cvor<Tip> *p = new Cvor<Tip>;
            p->element = el;
            p->sljedeci = nullptr;
            p->prethodni = nullptr;
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
            if(p != nullptr) {
                p->prethodni = tekuci->sljedeci;
                if(p->sljedeci == nullptr) Kraj = p;
            }
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
Tip dajMaksimum(const Lista<Tip>& n)
{
    DvostrukaLista<Tip> l = DvostrukaLista<Tip>(*((DvostrukaLista<Tip>*)(&n)));
    l.pocetak();
    Tip max = l.trenutni();
    while(l.sljedeci()) {
        if(l.trenutni() > max) max = l.trenutni();
    }
    return max;
}

void testbrojElemenata()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++)
        lista.dodajIza(i);
    std::cout << "Broj elemenata: " << lista.brojElemenata(); // treba ispisati 5
}

void testtrenutni()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++)
        lista.dodajIza(i);
    std::cout << "Trenutni: " << lista.trenutni(); //treba ispisati 1
}

void testprethodni()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++) {
        lista.dodajIza(i);
        lista.sljedeci();
    }
    lista.kraj();
    lista.prethodni();
    std::cout << "Trenutni: " << lista.trenutni(); // treba ispisati 4
}

void testsljedeci()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++) {
        lista.dodajIza(i);
        lista.sljedeci();
    }
    lista.pocetak();
    lista.sljedeci();
    std::cout << "Sljedeci: " << lista.trenutni(); // treba ispisati 2
}

void testpocetak()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++)
        lista.dodajIza(i);
    lista.pocetak();
    std::cout <<"Pocetak: "<< lista.trenutni(); // treba ispisati 1
}

void testkraj()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++) {
        lista.dodajIza(i);
        lista.sljedeci();
    }
    lista.kraj();
    std::cout << "Kraj: " << lista.trenutni(); // treba ispisati 5
}

void testobrisi()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++)
        lista.dodajIza(i);
    std::cout << "Broj elemenata prije brisanja trenutnog: " << lista.brojElemenata() << std::endl;
    lista.obrisi();
    std::cout << "Broj elemenata poslije brisanja trenutnog: " << lista.brojElemenata() << std::endl;
}

void testdodajIspred()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++) {
        lista.dodajIspred(i);
        lista.pocetak();
    }
    lista.pocetak();
    lista.dodajIspred(10); // dodaje prvi element treba biti 10, elementi su unazad
    for(int i = 0; i < lista.brojElemenata(); i++)
        std::cout << lista[i] << std::endl;

}

void testdodajIza()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++) {
        lista.dodajIza(i);
        lista.sljedeci();
    }
    lista.pocetak();
    lista.dodajIza(10); // dodaje drugi element treba biti 10
    for(int i = 0; i < lista.brojElemenata(); i++)
        std::cout << lista[i] << std::endl;
}

void testoperatori()
{
    DvostrukaLista<int> l1;
    for(int i  = 1; i <= 5; i++) {
        l1.dodajIza(i);
        l1.sljedeci();
    }
    DvostrukaLista<int> l2 = l1;
    for(int i = 0; i < l2.brojElemenata(); i++)
        std::cout << l2[i] << std::endl;
}

void testdajMaximum()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 5; i++)
        lista.dodajIza(i);
    std::cout << "Maximum: " << dajMaksimum(lista) << std::endl;
}


int main()
{
    std::cout << "Test brojElemenata: ";
    testbrojElemenata();
    std::cout << std::endl;
    std::cout << "Test trenutni: ";
    testtrenutni();
    std::cout << std::endl;
    std::cout << "Test prethodni: ";
    testprethodni();
    std::cout << std::endl;
    std::cout << "Test sljedeci: ";
    testsljedeci();
    std::cout << std::endl;
    std::cout << "Test pocetak: ";
    testpocetak();
    std::cout << std::endl;
    std::cout << "Test kraj: ";
    testkraj();
    std::cout << std::endl;
    std::cout << "Test obrisi: ";
    testobrisi();
    std::cout << std::endl;
    std::cout << "Test dodajIspred: ";
    testdodajIspred();
    std::cout << "Test dodajIza: ";
    testdodajIza();
    std::cout << "Test operatori: ";
    testoperatori();
    std::cout << "Test dajMaksimum: ";
    testdajMaximum();
    return 0;
}
