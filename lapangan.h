#ifndef LAPANGAN_H
#define LAPANGAN_H

#include <string>
#include <vector>

using namespace std;

struct Lapangan
{
    int id;
    string nama;
    string type;
    int harga;
};

vector<Lapangan> bacaLapangan();
bool cariLapanganById(int id, Lapangan &hasil);
void tampilkanDaftarLapangan();
void cetakLapangan();
void menuLapangan();

#endif
