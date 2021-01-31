#include <iostream>

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

    void Obrni(int n, int m)
    {
        if(n>=m) return;
        
        Cvor<Tip> *pom_n = poc;
        Cvor<Tip> *pom_m = poc;
        
        int indeks  = 0;
        while(indeks != n){
            pom_n = pom_n->sljedeci;
            indeks++;
        }
        
        indeks = 0;
        while(indeks != m){
            pom_m = pom_m->sljedeci;
            indeks++;
        }
        
        int izbaci = (int)(m+n)/2;
        
        for(int i = n; i < izbaci; i++){
            indeks = 0;
            int pom = pom_n->element;
            pom_n->element = pom_m->element;
            pom_m->element = pom;
            pom_n = pom_n->sljedeci;
            pom_m = poc;
            while(indeks != m-1){
                pom_m = pom_m->sljedeci;
                indeks++;
            }
            m--;
        }
        
        //brisemo element sa sredine
        Cvor<Tip> *pom = poc;
        indeks = 0;
        while(indeks != izbaci){
            pom = pom->sljedeci;
            indeks++;
        }
        
        Cvor<Tip> *prijepom = poc;
         indeks = 0;
        while(indeks != izbaci-1){
            prijepom = prijepom->sljedeci;
            indeks++;
        }
        
        prijepom->sljedeci = pom->sljedeci;
        delete pom;
    }
        

};


int main()
{
    JednostrukaLista<int> l;
    l.dodajIza(450);
    l.dodajIza(500);
    l.dodajIza(466);
    l.dodajIza(20);
    l.dodajIza(15);
    l.dodajIza(5);
    l.dodajIza(2);
    l.dodajIza(1);
    l.Obrni(2, 5);
    for(int i = 0; i < l.brojElemenata() - 1; i++)
        std::cout << l[i] << " ";
    return 0;
}
