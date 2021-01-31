#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void countSort(vector<int> &a, int x)  //pomocna funkcija
{
    vector<int> b(a.size());
    vector<int> count(10);
    for(int i = 0; i < a.size(); i++)
        count[(a[i]/x) % 10]++;

    for(int i = 1; i < 10; i++)
        count[i] += count[i-1];

    for(int i = a.size() - 1; i >= 0; i--) {
        b[count[(a[i]/x) % 10] - 1] = a[i];
        count[(a[i]/x) % 10]--;
    }

    for(int i = 0; i < a.size(); i++)
        a[i] = b[i];
}

void radixSort(vector<int> &a)
{
    if(a.size() == 0) return;
    int max = *(max_element(a.begin(), a.end()));
    for(int x = 1; max/x > 0; x *= 10)
        countSort(a, x);
}


int lijevoDijete(int i)
{
    return 2*i + 1;
}

int desnoDijete(int i)
{
    return 2*i + 2;
}

int roditelj(int i){
    return (i - 1) / 2;
}

void popraviDolje(int i, vector<int> &a)
{
    int velicina = a.size();
    while(!(i >= velicina/2) && (i < velicina)) {
        int veci = lijevoDijete(i);
        int dd = desnoDijete(i);
        if((dd < velicina) && a[dd] > a[veci] ) veci = dd;
        if( a[i] > a[veci]) return;
        int temp = a[i];
        a[i] = a[veci];
        a[veci] = temp;
        i = veci;
    }
} 

void popraviGore(int i, vector<int> &a){
    while( (i != 0) && a[i] > a[roditelj(i)]){
        int temp = a[i];
        a[i] = a[roditelj(i)];
        a[roditelj(i)] = temp;
        i = roditelj(i);
    }
}

void stvoriGomilu(vector<int> &a)
{
    int velicina = a.size();
    for(int i =  velicina/2; i >= 0; i--)
        popraviDolje(i, a);
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    velicina++;
    popraviGore(velicina - 1, a);
}

int izbaciPrvi(vector<int> &a, int &velicina){
    if(a.empty()) return -1;
    velicina--;
    int temp = a[0];
    a[0] = a[velicina];
    a[velicina] = temp;
    vector<int> v(velicina);
    for(int i = 0; i < v.size(); i++) v[i] = a[i];
    stvoriGomilu(v);
    for(int i = 0; i < v.size(); i++) a[i] = v[i];
    return temp;
}

void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    int velicina = a.size();
    for(int i  = 0; i < a.size() - 1; i++)
        izbaciPrvi(a, velicina);
}

int main()
{
    return 0;
}
