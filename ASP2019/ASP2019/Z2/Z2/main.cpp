#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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

void pretraga(Stek<std::vector<int>> &s, int trazeni)
{
    if(s.brojElemenata() == 0) {
        std::cout << "Nema elementa" << std::endl;
        return;
    }

    auto vektor = s.skini();
    int broj_elemenata_vektora = vektor.size();
    bool postoji = false;
    if(broj_elemenata_vektora != 0 && trazeni >= vektor.at(0)) {
        auto pocetak = vektor.begin();
        auto kraj = vektor.end() - 1;
        int index;
        while(pocetak <= kraj) {
            auto sredina = pocetak + (kraj - pocetak)/2;
            index = sredina - vektor.begin();
            if(*sredina < trazeni)
                pocetak = sredina + 1;
            else if(*sredina >trazeni)
                kraj = sredina - 1;
            else {
                postoji = true;
                std::cout << index << " " << s.brojElemenata();
                break;
            }
        }
        if(!postoji)
            std::cout << "Nema elementa" << std::endl;
    } else
        pretraga(s, trazeni);

    s.stavi(vektor);
}

void test1()
{
    Stek<std::vector<int>> s;
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {6, 7, 8, 9, 10};
    std::vector<int> v3 = {11, 12, 13, 14, 15};
    std::vector<int> v4 = {16, 17, 18, 19, 20};
    s.stavi(v1);
    s.stavi(v2);
    s.stavi(v3);
    s.stavi(v4);
    int trazeni = 3;
    pretraga(s, trazeni);
    trazeni = 14;
    pretraga(s, trazeni);
    trazeni = 7;
    pretraga(s, trazeni);
}

void test2()
{
    Stek<std::vector<int>> s;
    std::vector<int> a = {2, 7, 9, 11};
    std::vector<int> b = { 15, 18, 22, 25};
    std::vector<int> c = {27, 56, 101, 278, 987};
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    int trazeni = 5;
    pretraga(s, trazeni); // nema elementa
    trazeni = 101;
    pretraga(s, trazeni);
}

void test3()
{
    Stek<std::vector<int>> s;
    std::vector<int> x = {11, 22, 33, 44, 55, 66, 77, 88, 99};
    std::vector<int> y = {111, 222, 333, 444, 555, 666, 777, 888, 999};
    std::vector<int> z = {1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888, 9999};
    s.stavi(x);
    s.stavi(y);
    s.stavi(z);
    int trazeni = 5555;
    pretraga(s, trazeni);
    trazeni = 556;
    pretraga(s, trazeni); // nema elementa
    trazeni = 11;
    pretraga(s, trazeni);;
}

int main()
{
    std::cout << "Zadaća 2, Zadatak 2";
    std::cout << std::endl<< "test1:" << std::endl;
    test1();
    std::cout << std::endl << "test2:" << std::endl;
    test2();
    std::cout << std::endl << "test3:" << std::endl;
    test3();
    return 0;
}
