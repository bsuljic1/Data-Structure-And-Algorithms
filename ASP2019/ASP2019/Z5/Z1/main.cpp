#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <queue>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
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

    Pair(TipKljuca k, TipVrijednosti v)
    {
        kljuc = k;
        vrijednost = v;
    }

    Pair(const Pair<TipKljuca, TipVrijednosti> &p)
    {
        kljuc = p.kljuc;
        vrijednost = p.vrijednost;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti>
{
    int broj_elemenata = 0;
    unsigned int (*f)(TipKljuca, unsigned int) = nullptr;
    vector<list<Pair<TipKljuca, TipVrijednosti>>*> v;
public:
    ~HashMapaLan()
    {
        for(int i = 0; i < v.size(); i++) delete v[i];
    }

    HashMapaLan() : v(1000) {}

    HashMapaLan(const HashMapaLan<TipKljuca, TipVrijednosti> &hml) : v(hml.v.size())
    {
        broj_elemenata = hml.broj_elemenata;
        f = hml.f;
        for(int i = 0; i < v.size(); i++)
            if(hml.v[i]) v[i] = new list<Pair<TipKljuca, TipVrijednosti>>(*hml.v[i]);
    }

    HashMapaLan &operator =(const HashMapaLan<TipKljuca, TipVrijednosti> &hml)
    {
        if(this != &hml) {
            obrisi();
            v.resize(hml.v.size(), nullptr);
            broj_elemenata = hml.broj_elemenata;
            f = hml.f;
            for(int i = 0; i <v.size(); i++)
                if(hml.v[i]) v[i] = new list<Pair<TipKljuca, TipVrijednosti>> (*hml.v[i]);
        }
        return *this;
    }

    int brojElemenata() const
    {
        return broj_elemenata;
    }

    TipVrijednosti &operator [] (TipKljuca k)
    {
        if(f == nullptr) throw "Greska";
        unsigned int br = f(k, v.size());

        if(!v[br]) {
            v[br] = new list<Pair<TipKljuca, TipVrijednosti>> {Pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti())};
            auto ret = v[br]->begin();
            broj_elemenata++;
            return ret->vrijednost;
        } else {
            auto ret = v[br]->begin();
            for(auto it = v[br]->begin(); it != v[br]->end(); it++)
                if(it->kljuc == k) return it->vrijednost;

            while(ret->kljuc < k && ret != v[br]->end()) ret++;
            broj_elemenata++;
            return (v[br]->insert(ret, Pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti())))->vrijednost;
        }
    }

    TipVrijednosti operator[] (TipKljuca k) const
    {
        if(f == nullptr) throw "Greska";
        unsigned int br = f(k, v.size());

        if(v[br])
            for(auto it = v[br]->begin(); it != v[br]->end(); it++)
                if(it->kljuc == k) return it->vrijednost;

        return TipVrijednosti();
    }

    void obrisi()
    {
        for(int i = 0; i < v.size(); i++) {
            delete v[i];
            v[i] = nullptr;
            broj_elemenata = 0;
        }
    }

    void obrisi(const TipKljuca &k)
    {
        if(f == nullptr) throw "Greska";
        unsigned int br = f(k, v.size());

        if(v[br]) {
            for(auto it = v[br]->begin(); it != v[br]->end(); it++) {
                if(it->kljuc == k) {
                    v[br]->erase(it);
                    if(v[br]->empty()) {
                        delete v[br];
                        v[br] = nullptr;
                    }
                    broj_elemenata--;
                    break;
                }
            }
        }
    }

    void definisiHashFunkciju(unsigned int (*fun)(TipKljuca, unsigned int))
    {
        f = fun;
    }
};

template <typename TipOznake> class Cvor;
template<typename TipOznake> class Grana;
template<typename TipOznake> class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf
{
public:
    UsmjereniGraf() = default;
    virtual ~UsmjereniGraf() {};
    virtual int dajBrojCvorova () = 0;
    virtual void postaviBrojCvorova (int broj) = 0;
    virtual void dodajGranu (int polazniCvor, int dolazniCvor, float tezinaGrane) = 0;
    virtual void obrisiGranu (int polazniCvor, int dolazniCvor) = 0;
    virtual void postaviTezinuGrane (int polazniCvor, int dolazniCvor, float tezinaGrane) = 0;
    virtual float dajTezinuGrane (int polazniCvor, int dolazniCvor) = 0;
    virtual bool postojiGrana (int polazniCvor, int dolazniCvor) = 0;
    virtual void postaviOznakuCvora (int brojCvora, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora (int brojCvora) = 0;
    virtual void postaviOznakuGrane (int polazniCvor, int dolazniCvor, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane (int polazniCvor, int dolazniCvor) = 0;
    virtual Cvor<TipOznake> &dajCvor(int brojCvora) = 0;
    virtual Grana<TipOznake> &dajGranu(int polazniCvor, int dolazniCvor) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake>
class Cvor
{
    int brojCvora = 0;
    TipOznake oznaka = TipOznake();
    UsmjereniGraf<TipOznake> *graf = nullptr;
public:
    Cvor() {}
    Cvor(TipOznake ozn, int redni, UsmjereniGraf<TipOznake> *ug)
    {
        graf = ug;
        brojCvora = redni;
        oznaka = ozn;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake ozn)
    {
        graf->postaviOznakuCvora(brojCvora, ozn);
    }
    void postaviOzn(TipOznake ozn){
        oznaka = ozn;
    }
    int dajRedniBroj()
    {
        return brojCvora;
    }

    friend class Grana<TipOznake>;

};

template <typename TipOznake>
class Grana
{
    float tezina = 0;
    TipOznake oznaka = TipOznake();
    Cvor<TipOznake> *polazniCvor = nullptr;
    Cvor<TipOznake> *dolazniCvor = nullptr;
public:
    Grana() {}
    Grana(float tez, TipOznake ozn, Cvor<TipOznake> *pC, Cvor<TipOznake> *dC)
        : tezina(tez), oznaka(ozn),  polazniCvor(pC), dolazniCvor(dC) {}

    float dajTezinu()
    {
        return tezina;
    }
    void postaviTezinu(float tez)
    {
        polazniCvor->graf->postaviTezinuGrane(polazniCvor->dajRedniBroj(), dolazniCvor->dajRedniBroj(), tez);
    }
    void postaviTez(float tez){
        tezina = tez;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake ozn)
    {
        polazniCvor->graf->postaviOznakuGrane(polazniCvor->dajRedniBroj(), dolazniCvor->dajRedniBroj(), ozn);
    }
    void postaviOzn(TipOznake ozn){
        oznaka = ozn;
    }
    Cvor<TipOznake> dajPolazniCvor()
    {
        return *polazniCvor;
    }
    Cvor<TipOznake> dajDolazniCvor()
    {
        return *dolazniCvor;
    }
    bool jednaki(int p, int d)
    {
        return (p == polazniCvor->dajRedniBroj() && d == dolazniCvor->dajRedniBroj());
    }
};


template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake>
{
    int br = 0;
    vector<Cvor<TipOznake>*> cvorovi;
    vector<vector<Grana<TipOznake>*>> lg;
public:
    ~ListaGraf()
    {
        for(int i = 0; i < cvorovi.size(); i++) delete cvorovi[i];
        for(int i = 0; i < lg.size(); i++)
            for(int j = 0; j < lg[i].size(); j++) delete lg[i][j];
    }

    ListaGraf(int broj)
    {
        cvorovi.resize(broj, nullptr);
        for(int i = 0; i < broj; i++)
            cvorovi[i] = new Cvor<TipOznake> (TipOznake(), i, this);
        lg.resize(broj);
    }

    int dajBrojCvorova()
    {
        return cvorovi.size();
    }

    void postaviBrojCvorova (int broj)
    {
        if(broj < cvorovi.size()) throw "Greska";

        int vel = cvorovi.size();
        for(int i = vel; i < broj; i++)
            cvorovi[i] = new Cvor<TipOznake> (TipOznake(), i, this);
        lg.resize(broj);
    }

    void dodajGranu (int polazniCvor, int dolazniCvor, float tezinaGrane)
    {
        if(polazniCvor > cvorovi.size()) throw "Greska";
        lg[polazniCvor].push_back(new Grana<TipOznake>(tezinaGrane, TipOznake{}, cvorovi[polazniCvor], cvorovi[dolazniCvor]));
    }

    void obrisiGranu (int polazniCvor, int dolazniCvor)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) throw "Greska";

        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor)) {
                delete lg[polazniCvor][i];
                lg[polazniCvor].erase(lg[polazniCvor].begin() + i);
                return;
            }

        throw "Greska";
    }

    void postaviTezinuGrane (int polazniCvor, int dolazniCvor, float tezinaGrane)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) throw "Greska";
        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor)) {
                lg[polazniCvor][i]->postaviTez(tezinaGrane);
                return;
            }

        throw "Greska";
    }

    float dajTezinuGrane (int polazniCvor, int dolazniCvor)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) throw "Greska";
        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor))
                return lg[polazniCvor][i]->dajTezinu();

        throw "Greska";
    }
    bool postojiGrana (int polazniCvor, int dolazniCvor)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) return false;
        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor))
                return true;

        return false;
    }

    void postaviOznakuCvora (int brojCvora, TipOznake oznaka)
    {
        if(!cvorovi[brojCvora]) throw "Greska";
        cvorovi[brojCvora]->postaviOzn(oznaka);
    }

    TipOznake dajOznakuCvora (int brojCvora)
    {
        if(!cvorovi[brojCvora]) throw "Greska";
        return cvorovi[brojCvora]->dajOznaku();
    }

    void postaviOznakuGrane (int polazniCvor, int dolazniCvor, TipOznake oznaka)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) throw "Greska";
        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor)) {
                lg[polazniCvor][i]->postaviOzn(oznaka);
                return;
            }

        throw "Greska";
    }

    virtual TipOznake dajOznakuGrane (int polazniCvor, int dolazniCvor)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) throw "Greska";
        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor))
                return  lg[polazniCvor][i]->dajOznaku();;

        throw "Greska";
    }

    Cvor<TipOznake> &dajCvor(int brojCvora)
    {
        if(!cvorovi[brojCvora]) throw "Greska";
        return *cvorovi[brojCvora];
    }

    Grana<TipOznake> &dajGranu(int polazniCvor, int dolazniCvor)
    {
        if(dolazniCvor > cvorovi.size() || polazniCvor > cvorovi.size()) throw "Greska";
        for(int i = 0; i < lg[polazniCvor].size(); i++)
            if(lg[polazniCvor][i]->jednaki(polazniCvor, dolazniCvor))
                return *lg[polazniCvor][i];

        throw "Greska";
    }

    GranaIterator<TipOznake> dajGranePocetak()
    {
        for(int i = 0; i < lg.size(); i++)
            for(int j = 0; j < lg[i].size(); j++)
                if(lg[i][j]) return GranaIterator<TipOznake> (this, i, j);

        return GranaIterator<TipOznake> (this, 0, 0);
    }

    GranaIterator<TipOznake> dajGraneKraj()
    {
        for(int i = lg.size() - 1; i > 0; i--)
            for(int j = lg[i].size() - 1; j > 0; j--)
                if(lg[i][j]) {
                    if(j < lg[i].size() - 1) return GranaIterator<TipOznake>(this, i, j + 1);
                    else return GranaIterator<TipOznake> (this, i + 1, 0);
                }
        return GranaIterator<TipOznake> (this, lg.size() - 1, lg[lg.size() - 1].size() - 1);
    }

    friend class GranaIterator<TipOznake>;
    template<typename TipOznak>
    friend void bfs(UsmjereniGraf<TipOznak> *ug, vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni);

    template<typename TipOznak>
    friend void dfs(UsmjereniGraf<TipOznak> *ug, vector<Cvor<TipOznak>> &obilazak, Cvor<TipOznak> c);
};

template <typename TipOznake>
class GranaIterator
{

    ListaGraf<TipOznake> *lg = nullptr;
    int polazniCvor = 0, dolazniCvor = 0;
public:
    GranaIterator<TipOznake> (ListaGraf<TipOznake> *lista, int p, int d)
    {
        lg = lista;
        polazniCvor = p;
        dolazniCvor = d;
    }


    Grana<TipOznake> operator *()
    {
        return *lg->lg[polazniCvor][dolazniCvor];
        throw "Greska";
    }

    bool operator==(const GranaIterator &iter) const
    {
        return (lg == iter.lg && polazniCvor == iter.polazniCvor && dolazniCvor == iter.dolazniCvor);
    }
    bool operator !=(const GranaIterator &iter) const
    {
        return !(lg == iter.lg && polazniCvor == iter.polazniCvor && dolazniCvor == iter.dolazniCvor);
    }
    GranaIterator& operator++()
    {
        if(polazniCvor == lg->lg.size() - 1 && dolazniCvor == lg->lg[lg->lg.size() - 1].size() - 1) return  *this;
        if(dolazniCvor != lg->lg[polazniCvor].size() - 1) dolazniCvor++;
        else {
            polazniCvor++;
            dolazniCvor = 0;
        }
        return *this;
    }

    GranaIterator operator++(int)
    {
        GranaIterator<TipOznake> g (lg, polazniCvor, dolazniCvor);
        if(polazniCvor == lg->lg.size() - 1 && dolazniCvor == lg->lg[lg->lg.size() - 1].size() - 1) return  *this;
        if(dolazniCvor != lg->lg[polazniCvor].size() - 1) dolazniCvor++;
        else {
            polazniCvor++;
            dolazniCvor = 0;
        }
        return g;
    }
};


template<typename TipOznak>
void bfs(UsmjereniGraf<TipOznak> *ug, vector<Cvor<TipOznak>> &obilazak, Cvor<TipOznak> pocetni)
{
    queue<Cvor<TipOznak>> r;
    r.push(pocetni);
    obilazak.push_back(pocetni);
    while(!r.empty() && obilazak.size() != ug->dajBrojCvorova()) {
        Cvor<TipOznak> c= r.front();
        r.pop();
        bool boolean = false;
        vector<Cvor<TipOznak>> v;
        for(int i = 0; i < ug->dajBrojCvorova(); i++)
            if(ug->postojiGrana(c.dajRedniBroj(), i)) {
                v.push_back(ug->dajCvor(i));
                r.push(ug->dajCvor(i));
            }
        for(int  i = 0; i < v.size(); i++) {
            boolean = false;
            for(int j = 0; j < obilazak.size(); j++)
                if(v[i].dajRedniBroj() == obilazak[j].dajRedniBroj()) boolean = true;
            if(boolean == false) obilazak.push_back(v[i]);
        }
    }
}

template<typename TipOznak>
void dfs(UsmjereniGraf<TipOznak> *ug, vector<Cvor<TipOznak>> &obilazak, Cvor<TipOznak> c)
{
    bool boolean = false;
    for(int i = 0; i < obilazak.size(); i++)
        if(obilazak[i].dajRedniBroj() == c.dajRedniBroj()) {
            boolean = true;
            break;
        }
    if(boolean == false) obilazak.push_back(c);

    for(int i = 0; i < ug->dajBrojCvorova(); i++)
        if(boolean == false && ug->postojiGrana(c.dajRedniBroj(), i))
            dfs(ug, obilazak, ug->dajCvor(i));
}






int main()
{
    return 0;
}
