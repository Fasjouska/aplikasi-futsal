#ifndef PERLENGKAPAN_H
#define PERLENGKAPAN_H

#include <string>
#include <vector>

using namespace std;

struct Perlengkapan
{
    int id;
    string jenis;
    string pilihan;
    int jumlah;
    int harga;
};

vector<Perlengkapan> bacaPerlengkapan();
bool cariPerlengkapanById(int id, Perlengkapan &hasil);
void menuPerlengkapan();

#endif
