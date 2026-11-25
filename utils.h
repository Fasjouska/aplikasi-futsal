#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

bool konversiInt(const string &data, int &hasil);
bool konversiDouble(const string &data, double &hasil);

bool inputAngka(const string &pesan, int &hasil);
bool inputDouble(const string &pesan, double &hasil);
bool inputTeks(const string &pesan, string &hasil);
bool inputTanggal(const string &pesan, string &hasil);

bool validasiTanggal(const string &tanggal);
int jamKeMenit(const string &jam);

string keHurufKecil(const string &teks);
string escapeHtml(const string &teks);
string tanggalSekarang();
string waktuSekarang();
void bukaFileHtml(const string &namaFile);

#endif
