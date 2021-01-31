#include <iostream
#include <vector>
#include <queue>

class Cvor
{
public:
    vector<Cvor *> cvorovi;
    int index;
    int oznaka = 0;
    int element;
}

int Putanja(Cvor *A, Cvor *B, Cvor *C)
{
    int najmanjiBrojCvorova = 0;

    queue<Cvor *> red;
    red.push(A);
    vector<Cvor *> obilazak;

    while(!r.empty()) {
        Cvor *cvor = red.pop();
        bool imaB = false;
        for(int i = 0; i < cvor->cvorovi.size(); i++) {
            if(cvor->cvorovi[i] == B) imaB = true;
            bool vecPosjecen = false;
            for(int j = 0; j < obilazak.size(); j++)
                if(obilazak[j] == cvor->cvorovi[i]) vecPosjecen = true;

            if(!vecPosjecen && cvor->cvorovi[i] != C) {
                red.push(cvor->cvorovi[i]);
                obilazak.push_back(cvor->cvorovi[i]);
            }
        }
        if(!imaB) najmanjiBrojCvorova++;
    }

    return najmanjiBrojCvorova;
}

int main()
{
    return 0;
}
