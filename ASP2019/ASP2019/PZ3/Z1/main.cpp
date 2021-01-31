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

void testFunkcija(){
    Stek<int> s;
    for(int i = 1; i <= 10; i++)
        s.stavi(i);
    std::cout << s.skini() << std::endl;
    std::cout << s.brojElemenata() << std::endl;
    std::cout << s.vrh() << std::endl;
    Stek<int> s2;
    for(int i = 1; i <= 5; i++)
        s2.stavi(i);
    s = s2;
    std::cout << s.skini() << std::endl;
    std::cout << s.vrh() << std::endl;
    s.brisi();
    s2.brisi();
    
 }


int main()
{
    std::cout << "Pripremna Zadaca Za Vjezbu 3, Zadatak 1" << std::endl;
    testFunkcija();
    return 0;
}
