#include <iostream>
#include <vector>
#include <queue>

using namespace std;

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
    Cvor(int redni, UsmjereniGraf<TipOznake> &ug)
    {
        graf = &ug;
        brojCvora = redni;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake ozn)
    {
        oznaka = ozn;
    }
    int dajRedniBroj()
    {
        return brojCvora;
    }

};

template <typename TipOznake>
class Grana
{
    float tezina = 0;
    TipOznake oznaka = TipOznake();
    int polazniCvor = 0, dolazniCvor = 0;
    UsmjereniGraf<TipOznake> *graf = nullptr;
public:
    Grana() {}
    Grana(int polazni, int dolazni, UsmjereniGraf<TipOznake> &ug, float tez)
    {
        tezina = tez;
        polazniCvor = polazni;
        dolazniCvor = dolazni;
        graf = &ug;
    }
    Grana(const Grana<TipOznake> &gr)
    {
        tezina = gr.tezina;
        polazniCvor = gr.polazniCvor;
        dolazniCvor = gr.dolazniCvor;
        graf = gr.graf;
        oznaka = gr.oznaka;
    }
    float dajTezinu()
    {
        return tezina;
    }
    void postaviTezinu(float tez)
    {
        graf->dajGranu(polazniCvor, dolazniCvor).tezina = tez;
        tezina = tez;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake ozn)
    {
        graf->dajGranu(polazniCvor, dolazniCvor).oznaka = ozn;
        oznaka = ozn;
    }
    Cvor<TipOznake> dajPolazniCvor()
    {
        return graf->dajCvor(polazniCvor);
    }
    Cvor<TipOznake> dajDolazniCvor()
    {
        return graf->dajCvor(dolazniCvor);
    }
    bool operator ==(Grana<TipOznake> gr)
    {
        if(polazniCvor == gr.polazniCvor && dolazniCvor == gr.dolazniCvor) return true;
        return false;
    }
};

template <typename TipOznake>
class GranaIterator
{

    vector<vector<bool>> matrica;
    vector<Grana<TipOznake>> grane;
    int pozMat = 0;
    int polazniCvor = 0;
    int dolazniCvor = 0;
public:
    GranaIterator(vector<vector<bool>> mat, vector<Grana<TipOznake>> g, int polazni, int dolazni, int p)
    {
        polazniCvor = polazni;
        dolazniCvor = dolazni;
        pozMat = p;
        matrica = mat;
        grane = g;
    }
    GranaIterator(const GranaIterator<TipOznake> &gi)
    {
        polazniCvor = gi.polazniCvor;
        dolazniCvor = gi.dolazniCvor;
        pozMat = gi.pozMat;
        grane = gi.grane;
        matrica = gi.matrica;
    }

    GranaIterator<TipOznake> &operator = (const GranaIterator<TipOznake> &gi)
    {
        polazniCvor = gi.polazniCvor;
        dolazniCvor = gi.dolazniCvor;
        pozMat = gi.pozMat;
        grane = gi.grane;
        matrica = gi.matrica;
    }

    Grana<TipOznake> operator * () const
    {
        int red = -1;
        int kolona  = -1;
        int br = 0;
        bool boolean = false;

        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica[0].size(); j++) {
                if(matrica[i][j] && br <= pozMat) {
                    red = i;
                    kolona = j;
                    br++;
                } else if(br > pozMat) {
                    boolean = true;
                    break;
                }
            }
            if(boolean == true) break;
        }

        for(int i = 0; i < grane.size(); i++)
            if(grane[i].dajDolazniCvor().dajRedniBroj() == kolona && grane[i].dajPolazniCvor().dajRedniBroj() == red)
                return grane[i];
        throw std::domain_error("Greska");
    }

    Grana<TipOznake> operator *()
    {
        int red = -1;
        int kolona  = -1;
        int br = 0;
        bool boolean = false;

        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica[0].size(); j++) {
                if(matrica[i][j] && br <= pozMat) {
                    red = i;
                    kolona = j;
                    br++;
                } else if(br > pozMat) {
                    boolean = true;
                    break;
                }
            }
            if(boolean == true) break;
        }

        for(int i = 0; i < grane.size(); i++)
            if(grane[i].dajDolazniCvor().dajRedniBroj() == kolona && grane[i].dajPolazniCvor().dajRedniBroj() == red)
                return grane[i];
        throw std::domain_error("Greska");
    }

    bool operator==(const GranaIterator &iter) const
    {
        int red = -1;
        int kolona  = -1;
        int br = 0;
        bool boolean = false;

        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica[0].size(); j++) {
                if(matrica[i][j] && br <= pozMat) {
                    red = i;
                    kolona = j;
                    br++;
                } else if(br > pozMat) {
                    boolean = true;
                    break;
                }
            }
            if(boolean == true) break;
        }


        if(matrica[red][kolona] == iter.matrica[red][kolona]) return true;
        return false;
    }
    bool operator !=(const GranaIterator &iter) const
    {
        int red = -1;
        int kolona  = -1;
        int br = 0;
        bool boolean = false;

        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica[0].size(); j++) {
                if(matrica[i][j] && br <= pozMat) {
                    red = i;
                    kolona = j;
                    br++;
                } else if(br > pozMat) {
                    boolean = true;
                    break;
                }
            }
            if(boolean == true) break;
        }

        if(matrica[red][kolona] != iter.matrica[red][kolona]) return false;
        if(pozMat == grane.size()) return false;
        return true;
    }
    GranaIterator& operator++()
    {
        int red = -1;
        int kolona  = -1;
        int br = 0;
        bool boolean = false;

        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica[0].size(); j++) {
                if(matrica[i][j] && br <= pozMat) {
                    red = i;
                    kolona = j;
                    br++;
                } else if(br > pozMat) {
                    boolean = true;
                    break;
                }
            }
            if(boolean == true) break;
        }

        if(red == matrica.size() - 1 && kolona == matrica[0].size()) throw std::domain_error("Greska");
        pozMat++;
        return *this;
    }

    GranaIterator operator++(int x)
    {
        int red = -1;
        int kolona  = -1;
        int br = 0;
        bool boolean = false;

        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica[0].size(); j++) {
                if(matrica[i][j] && br <= pozMat) {
                    red = i;
                    kolona = j;
                    br++;
                } else if(br > pozMat) {
                    boolean = true;
                    break;
                }
            }
            if(boolean == true) break;
        }

        if(((red + x) >= matrica.size() - 1) || ((kolona + x) >= matrica[0].size())) throw std::domain_error("Greska");
        pozMat += x;
        return *this;

    }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake>
{

    vector<Cvor<TipOznake>> cvorovi;
    vector<vector<bool>> matrica;
    vector<Grana<TipOznake>> grane;
public:
    ~MatricaGraf() {}

    MatricaGraf(int brojCvorova)
    {
        if(brojCvorova <= 0) throw std::domain_error("Greska");
        matrica.resize(brojCvorova);
        for(int i = 0; i < brojCvorova; i++) matrica[i].resize(brojCvorova);
        for(int i = 0; i < brojCvorova; i++) {
            for(int j = 0; j < brojCvorova; j++) matrica[i][j] = false;
            cvorovi.push_back(Cvor<TipOznake> (i, *this));
        }
        matrica.resize(brojCvorova);
        for(int i = 0; i < brojCvorova; i++) matrica[i].resize(brojCvorova);
    }

    MatricaGraf(const MatricaGraf<TipOznake> &mat)
    {
        cvorovi = mat.cvorovi;
        matrica = mat.matrica;
        grane = mat.grane;
    }

    MatricaGraf<TipOznake> &operator = (const MatricaGraf<TipOznake> &mat)
    {
        if(matrica != this->matrica) {
            cvorovi = mat.cvorovi;
            matrica = mat.matrica;
            grane = mat.grane;
        }
        return *this;
    }

    int dajBrojCvorova ()
    {
        return cvorovi.size();
    }

    void postaviBrojCvorova (int br)
    {
        if(br < matrica.size()) throw std::domain_error("Greska");
        int a = matrica.size();

        for(int i = a; i < br; i++)
            cvorovi.push_back(Cvor<TipOznake>(i, *this));
        if(a < br) {
            matrica.resize(br);
            for(int i = 0; i < br; i++) matrica[i].resize(br);
            for(int i = 0; i < br; i++)
                for(int j = 0; j < br; j++)
                    if(i >= a || j >= a) matrica[i][j] = false;
        }
    }

    void dodajGranu (int polazniCvor, int dolazniCvor, float tezinaGrane = 0)
    {
        if(polazniCvor < 0 || dolazniCvor < 0 ||dolazniCvor >= cvorovi.size() || polazniCvor >= cvorovi.size()) throw std::domain_error("Greska");
        if(matrica[polazniCvor][dolazniCvor] == false) {
            grane.push_back(Grana<TipOznake>(polazniCvor, dolazniCvor, *this, tezinaGrane));
            matrica[polazniCvor][dolazniCvor] = true;
        } else throw std::domain_error("Greska");
    }

    void obrisiGranu (int polazniCvor, int dolazniCvor)
    {
        matrica[polazniCvor][dolazniCvor] = false;

        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazniCvor, dolazniCvor, *this, 0)) {
                grane.erase(grane.begin() + i);
                break;
            }
        }
    }

    void postaviTezinuGrane (int polazniCvor, int dolazniCvor, float tezinaGrane)
    {
        if(matrica[polazniCvor][dolazniCvor] == true) {
            for(int i = 0; i < grane.size(); i++) {
                if(grane[i] == Grana<TipOznake>(polazniCvor, dolazniCvor, *this, tezinaGrane)) {
                    grane[i].postaviTezinu(tezinaGrane);
                    break;
                }
            }
        }
    }

    float dajTezinuGrane (int polazniCvor, int dolazniCvor)
    {
        if(matrica[polazniCvor][dolazniCvor] == true) {
            for(int i = 0; i < grane.size(); i++)
                if(grane[i] == Grana<TipOznake>(polazniCvor, dolazniCvor, *this, 0))
                    return grane[i].dajTezinu();
        }
    }

    bool postojiGrana (int polazniCvor, int dolazniCvor)
    {
        if(matrica[polazniCvor][dolazniCvor]) return true;
        return false;
    }

    void postaviOznakuCvora (int brojCvora, TipOznake oznaka)
    {
        if(brojCvora < 0 || brojCvora >= cvorovi.size()) throw "Greska";
        cvorovi[brojCvora].postaviOznaku(oznaka);
    }

    TipOznake dajOznakuCvora (int brojCvora)
    {
        if(brojCvora < 0 || brojCvora >= cvorovi.size()) throw "Greska";
        return cvorovi[brojCvora].dajOznaku();
    }

    void postaviOznakuGrane (int polazniCvor, int dolazniCvor, TipOznake oznaka)
    {
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i] == Grana<TipOznake>(polazniCvor, dolazniCvor, *this, 0)) {
                grane[i].postaviOznaku(oznaka);
                break;
            }
        }
    }

    TipOznake dajOznakuGrane (int polazniCvor, int dolazniCvor)
    {
        for(int i = 0; i < grane.size(); i++)
            if(grane[i] == Grana<TipOznake>(polazniCvor, dolazniCvor, *this, 0))
                return grane[i].dajOznaku();

        throw std::domain_error("Greska");

    }

    Cvor<TipOznake> &dajCvor(int brojCvora)
    {
        return cvorovi[brojCvora];
    }

    Grana<TipOznake> &dajGranu(int polazniCvor, int dolazniCvor)
    {
        for(int i = 0; i < grane.size(); i++)
            if(grane[i] == Grana<TipOznake>(polazniCvor, dolazniCvor, *this, 0)) return grane[i];

        throw std::domain_error("Greska");
    }

    GranaIterator<TipOznake> dajGraneKraj()
    {

        return GranaIterator<TipOznake>(matrica, grane, grane[grane.size() - 1].dajPolazniCvor().dajRedniBroj(), grane[grane.size() - 1].dajDolazniCvor().dajRedniBroj(), grane.size() - 1);
    }

    GranaIterator<TipOznake> dajGranePocetak()
    {
        return GranaIterator<TipOznake>(matrica, grane, grane[0].dajPolazniCvor().dajRedniBroj(), grane[0].dajDolazniCvor().dajRedniBroj(), 0);
    }


};

template<typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *ug, vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni)
{
    queue<Cvor<TipOznake>> r;
    r.push(pocetni);
    obilazak.push_back(pocetni);
    while(!r.empty() && obilazak.size() != ug->dajBrojCvorova()) {
        Cvor<TipOznake> c= r.front();
        r.pop();
        bool boolean = false;
        vector<Cvor<TipOznake>> v;
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

template<typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *ug, vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> c)
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
    std::cout << "Pripremna Zadaca Za Vjezbu 10, Zadatak 1";
    return 0;
}
