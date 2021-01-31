#include <iostream>

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
class Red
{
    int broj_elemenata = 0;
    Cvor<Tip> *pocetak = nullptr;
    Cvor<Tip> *kraj = nullptr;
public:
    Red() {}

    ~Red()
    {
        Cvor<Tip> *pom = pocetak;
        if(pom != nullptr) {
            while(pom->sljedeci != nullptr) {
                pocetak = pocetak->sljedeci;
                delete pom;
                pom = pocetak;
            }
            delete kraj;
        }
    }


    Red(const Red &r)
    {
        broj_elemenata = r.broj_elemenata;
        if(broj_elemenata == 0) return;
        pocetak = new Cvor<Tip>(*r.pocetak);
        Cvor<Tip> *pom = pocetak;
        while(pom->sljedeci != nullptr) {
            pom->sljedeci = new Cvor<Tip>(*pom->sljedeci);
            pom = pom->sljedeci;
        }
        kraj = r.kraj;
    }

    Red &operator =(const Red &r)
    {
        if(&r != this) {
            Cvor<Tip> *pom = pocetak;
            if(pom != nullptr) {
                while(pom->sljedeci != nullptr) {
                    pocetak = pocetak->sljedeci;
                    delete pom;
                    pom = pocetak;
                }
                delete kraj;
            }
            broj_elemenata = r.broj_elemenata;
            if(broj_elemenata != 0) {
                pocetak = new Cvor<Tip>(*r.pocetak);
                Cvor<Tip> *pom2 = pocetak;
                while(pom2->sljedeci != nullptr) {
                    pom2->sljedeci = new Cvor<Tip>(*pom2->sljedeci);
                    pom2 = pom2->sljedeci;
                }
                kraj = r.kraj;
            } else {
                pocetak = nullptr;
                kraj = nullptr;
            }
        }
        return *this;
    }

    Red &operator =(Red &&r)
    {
        if(&r != this) {
            broj_elemenata = r.broj_elemenata;
            kraj = r.kraj;
            Cvor<Tip> *pom = r.pocetak;
            while(pom->sljedeci != nullptr) {
                r.pocetak = r.pocetak->sljedeci;
                delete pom;
                pom = r.pocetak;
            }
            pocetak = r.pocetak;
        }
        return *this;
    }

    void brisi()
    {
        if(broj_elemenata == 0) throw std::domain_error("Red je prazan");
        broj_elemenata = 0;
        Cvor<Tip> *pom = pocetak;
        while(pocetak->sljedeci != nullptr) {
            pocetak = pocetak->sljedeci;
            delete pom;
            pom = pocetak;
        }
        delete kraj;
    }

    void stavi(const Tip &el)
    {
        if(broj_elemenata == 0) {
            Cvor<Tip> *pom = new Cvor<Tip>;
            pom->element = el;
            pom->sljedeci = nullptr;
            pocetak = pom;
            kraj = pom;
        } else {
            Cvor<Tip> *pom = new Cvor<Tip>;
            pom->element = el;
            pom->sljedeci = nullptr;
            kraj->sljedeci = pom;
            kraj = kraj->sljedeci;
        }
        broj_elemenata++;
    }

    Tip skini()
    {
        if(broj_elemenata == 0) throw std::domain_error("Red je prazan");
        Tip el = pocetak->element;
        Cvor<Tip> *pom = pocetak;
        pocetak = pocetak->sljedeci;
        delete pom;
        broj_elemenata--;
        return el;
    }

    Tip& celo()
    {
        if(broj_elemenata == 0) throw std::domain_error("Red je prazan");
        return pocetak->element;
    }

    int brojElemenata()
    {
        return broj_elemenata;
    }

};

void testbrojElemenata()
{
    Red<int> red;
    for(int i = 1; i <=5; i++)
        red.stavi(i);
    std::cout << red.brojElemenata() << std::endl; // treba ispisati 5
}

void teststavi()
{
    Red<int> red;
    for(int i = 1; i <= 10; i++) // treba staviti 5 elemenata u red
        red.stavi(i);
    int br = red.brojElemenata();
    for(int i = 0; i < br; i++) // ispisuje elemente i skida
        std::cout << red.skini() << std::endl;
}

void testskini()
{
    Red<int> red;
    for(int i = 1; i <= 5; i++)
        red.stavi(i);
    std::cout << "Broj elemenata prije izbacivanja: " << red.brojElemenata() << std::endl; // treba ispisati 5
    int br = red.brojElemenata();
    for(int i = 0; i < br ; i++) // treba redom ispisivati elemente
        std::cout << red.skini() << " ";
    std::cout << "Broj elemenata poslije skini: " << red.brojElemenata() << std::endl; // treba ispisati 0
}

void testbrisi()
{
    Red<int> red;
    for(int i = 1; i <= 5; i++)
        red.stavi(i);
    std::cout << "Broj elemenata prije brisanja: " << red.brojElemenata() << std::endl; // treba ispisati 5
    red.brisi();
    std::cout << "Broj elemenata poslije brisanja: " << red.brojElemenata() << std::endl; //treba ispisati 0
}

void testcelo()
{
    Red<int> red;
    for(int i = 1; i <= 5; i++)
        red.stavi(i);
    std::cout << "Celo: " << red.celo() << std:: endl; //treba ispisati 1
}

void testoperatori()
{
    Red<int> red1;
    for(int i =1; i <= 5; i++)
        red1.stavi(i);
    Red<int> red2 = red1;
    int br = red2.brojElemenata();
    for(int i  = 0; i < br; i++)
        std::cout << red2.skini() << std::endl;
}

int main()
{
    std::cout << "Pripremna Zadaca Za Vjezbu 4, Zadatak 1" << std::endl;
    std::cout << "Test brisi: ";
    testbrisi();
    std::cout << "Test brojElemenata: ";
    testbrojElemenata();
    std::cout << "Test celo: ";
    testcelo();
    std::cout << "Test operator: ";
    testoperatori();
    std::cout << "Test skini: ";
    testskini();
    std::cout << "Test stavi: ";
    teststavi();
    return 0;
}
