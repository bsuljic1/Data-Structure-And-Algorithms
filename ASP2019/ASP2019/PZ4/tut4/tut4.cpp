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
      Cvor(const Tip &el, Cvor *sl) : element(el), sljedeci(sl) {};
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


template<typename Tip>
class Stek
{
    Cvor<Tip> *Vrh = nullptr;
    int velicina = 0;
public:
    Stek() {};

    ~Stek()
    {
        brisi();
        delete Vrh;
    }

    Stek(const Stek &s)
    {
        velicina = s.velicina;
        Cvor<Tip> *q;
        Cvor<Tip> *p = s.Vrh;
        Vrh = 0;
        while(p != 0) {
            Cvor<Tip> *n = new Cvor<Tip>(p->element, 0);
            if(Vrh == 0 ) Vrh = n;
            else
                q->sljedeci = n;
            q = n;
            p = p->sljedeci;
        }
    }

    Stek &operator =(const Stek &s)
    {
        velicina = s.velicina;
        if(&s == this) return *this;
        brisi();
        Cvor<Tip> *q;
        Cvor<Tip> *p = s.Vrh;
        Vrh = 0;
        while(p != 0) {
            Cvor<Tip> *n = new Cvor<Tip>(p->element, 0);
            if(Vrh == 0) Vrh = n;
            else
                q->sljedeci = n;
            q = n;
            p = p->sljedeci;
        }
        return *this;
    }

    void brisi()
    {
        while(Vrh != 0)
            skini();
    }
    
    
    void stavi(const Tip& el)
    {
        Cvor<Tip> *p = new Cvor<Tip>(el, Vrh);
        Vrh = p;
        //Vrh = new Cvor(el,Vrh);
        velicina++;
    }

   

    Tip skini()
    {
        if(Vrh == nullptr) throw("Stek je prazan");
        Tip el = Vrh->element;
        Cvor<Tip> *p = Vrh->sljedeci;
        delete Vrh;
        Vrh = p;
        velicina--;
        return el;
    }

    Tip& vrh() const
    {
        if(Vrh == 0) throw("Stek je prazan");
        return Vrh->element;
    }

    int brojElemenata()
    {
        return velicina;
    }
};


template<typename Tip>
void IzbaciManjeOdAverage(Red<Tip> &r){
    int brel = r.brojElemenata();
    Tip average = 0;
    for(int i = 0; i < brel; i++) {
        Tip el = r.skini();
        average += el;
        r.stavi(el);
    }
    average /= r.brojElemenata();

    for(int i = 0; i < brel; i++) {
        Tip el = r.skini();
        if(el > average) r.stavi(el);
    }
}

template<typename Tip>
void funk(Stek<Tip> &stek, Red<Tip> &red, int pocetnaDuzinaReda){
    int duzinasteka = stek.brojElemenata();
    for(int i = 0; i < duzinasteka; i++)
        red.stavi(stek.skini());
    for(int i = 0; i < pocetnaDuzinaReda+ duzinasteka; i++)
        stek.stavi(red.skini());
    for(int i = 0; i < duzinasteka; i++)
        red.stavi(stek.skini());
    for(int i = 0; i < duzinasteka; i++)
        stek.stavi(red.skini());
}

int main()
{
    Stek<int> stek;
    for(int i = 5; i <= 8; i++)
        stek.stavi(i);
    Red<int> red;
    for(int i = 1; i <= 4; i++)
        red.stavi(i);
        
    funk(stek, red, red.brojElemenata());
    int brel = stek.brojElemenata();
    for(int i = 0; i < brel; i++)
        std::cout << stek.skini();
    return 0;
}
