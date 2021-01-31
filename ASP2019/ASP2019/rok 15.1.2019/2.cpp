#include <iostream>
#include <vector>

int hash(int el)
{
    return
}

void prebaciHash(vector<vector<int>> h, int* t, int velT)
{
    t = new int[velT];
    for(int i  = 0; i < velT; i++) t[i] = 0;

    for(int i = 0; i < h.size(); i++)
        for(int j = 0; j < h[i].size()) {
            bool zauzeto = true;
            for(int k = i; k < velT; k++)
                if(t[k] == 0) {
                    t[k] = t[i][j];
                    zauzeto = false;
                    break;
                }
            if(zauzeto) {
                for(int l = 0; l < i; l++) {
                    if(t[l] == 0) {
                        t[l] = t[i][j];
                        zauzeto = false;
                        break;
                    }
                }
            }
            if(zauzeto) {
                int novaVel = velT*2;
                int *novi = new int[novaVel];
                for(int m = 0; m < velT; m++) {
                    novi[m] = t[m];
                }
                for(int m = velT; m < novaVel; m++) {
                    novi[m] = 0;
                }
                t = novi;
                t[velT] = h[i][j];
                velT = novaVel;
            }
        }


}


}



int main()
{
    return 0;
}
