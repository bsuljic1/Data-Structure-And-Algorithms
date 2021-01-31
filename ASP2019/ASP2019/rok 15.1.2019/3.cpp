#include <iostream>
#include <vector>

using namespace std;

class Cvor
{
public:
    vector<Cvor *>cvorovi;
    int index = 0;
    int oznaka = 0;
    int element;
    int *brojManjihIliJednakih;
}


vector<int> countingSortGraf(Cvor *pocetni, int brojCvorova)
{
    vector<Cvor*> obilazak;
    vector<int> ret

    queue<Cvor> r;
    r.push(pocetni);
    obilazak.push_back(pocetni->brojManjihIliJednakih);
    while(!r.empty() && obilazak.size() != brojCvorova) {
        Cvor c= r.front();
        r.pop();
        for(int i = 0; i < c->cvorovi.size(); i++) {
            bool vecPosjecen = false;
            for(int j = 0; j < obilazak.size(); j++)
                if(obilazak[j] == c->cvorovi[i]) vecPosjecen = true;

            if(vecPosjecen == false) {
                obilazak.push_back(c->cvorovi[i]);
                for(int k = 0; k < c->cvorovi[i]->cvorovi.size(); k++)
                    r.push(c->cvorovi[i]->cvorovi[k]);
                ret.push_back(c->cvorovi[i]->brojManjihIliJednakih);
            }
        }
    }
    return ret;
}

int main()
{
    return 0;
}
