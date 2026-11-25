#include "utils.h"

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

bool konversiInt(const string &data, int &hasil)
{
    stringstream ss(data);
    char sisa;
    return (ss >> hasil) && !(ss >> sisa);
}

bool konversiDouble(const string &data, double &hasil)
{
    stringstream ss(data);
    char sisa;
    return (ss >> hasil) && !(ss >> sisa);
}

bool inputAngka(const string &pesan, int &hasil)
{
    while (true)
    {
        cout << pesan;
        if (cin >> hasil)
        {
            return true;
        }
        if (cin.eof())
        {
            return false;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus berupa angka!" << endl;
    }
}

bool inputDouble(const string &pesan, double &hasil)
{
    while (true)
    {
        cout << pesan;
        if (cin >> hasil)
        {
            return true;
        }
        if (cin.eof())
        {
            return false;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus berupa angka!" << endl;
    }
}

bool inputTeks(const string &pesan, string &hasil)
{
    while (true)
    {
        cout << pesan;
        if (!getline(cin, hasil))
        {
            return false;
        }
        if (!hasil.empty() && hasil.find('|') == string::npos)
        {
            return true;
        }
        cout << "Data tidak boleh kosong atau mengandung karakter '|'." << endl;
    }
}

bool validasiTanggal(const string &tanggal)
{
    if (tanggal.length() != 10 || tanggal[2] != '-' || tanggal[5] != '-')
    {
        return false;
    }

    int hari = 0, bulan = 0, tahun = 0;
    if (!konversiInt(tanggal.substr(0, 2), hari) ||
        !konversiInt(tanggal.substr(3, 2), bulan) ||
        !konversiInt(tanggal.substr(6, 4), tahun))
    {
        return false;
    }

    if (tahun < 2000 || tahun > 2100 || bulan < 1 || bulan > 12)
    {
        return false;
    }

    int hariPerBulan[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool kabisat = (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
    if (kabisat)
    {
        hariPerBulan[2] = 29;
    }

    return hari >= 1 && hari <= hariPerBulan[bulan];
}

bool inputTanggal(const string &pesan, string &hasil)
{
    while (true)
    {
        cout << pesan;
        if (!(cin >> hasil))
        {
            if (cin.eof())
            {
                return false;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (validasiTanggal(hasil))
        {
            return true;
        }
        cout << "Format tanggal tidak valid! Gunakan DD-MM-YYYY (contoh: 25-11-2026)." << endl;
    }
}

int jamKeMenit(const string &jam)
{
    int h = 0, m = 0;
    char titikDua = '\0';
    stringstream ss(jam);

    if (!(ss >> h >> titikDua >> m))
    {
        return -1;
    }
    if (titikDua != ':' || h < 0 || h > 23 || m < 0 || m > 59)
    {
        return -1;
    }

    char sisa;
    if (ss >> sisa)
    {
        return -1;
    }

    return h * 60 + m;
}

string keHurufKecil(const string &teks)
{
    string hasil = teks;
    for (size_t i = 0; i < hasil.length(); i++)
    {
        hasil[i] = static_cast<char>(tolower(static_cast<unsigned char>(hasil[i])));
    }
    return hasil;
}

string escapeHtml(const string &teks)
{
    string hasil;
    for (size_t i = 0; i < teks.length(); i++)
    {
        switch (teks[i])
        {
        case '&':
            hasil += "&amp;";
            break;
        case '<':
            hasil += "&lt;";
            break;
        case '>':
            hasil += "&gt;";
            break;
        case '"':
            hasil += "&quot;";
            break;
        case '\'':
            hasil += "&#39;";
            break;
        default:
            hasil += teks[i];
        }
    }
    return hasil;
}

string tanggalSekarang()
{
    time_t sekarang = time(0);
    tm *waktu = localtime(&sekarang);
    if (waktu == 0)
    {
        return "-";
    }
    char teks[32];
    strftime(teks, sizeof(teks), "%d-%m-%Y", waktu);
    return teks;
}

string waktuSekarang()
{
    time_t sekarang = time(0);
    tm *waktu = localtime(&sekarang);
    if (waktu == 0)
    {
        return "-";
    }
    char teks[32];
    strftime(teks, sizeof(teks), "%d-%m-%Y %H:%M:%S", waktu);
    return teks;
}

void bukaFileHtml(const string &namaFile)
{
#ifdef _WIN32
    string perintah = "start \"\" \"" + namaFile + "\"";
    system(perintah.c_str());
#else
    (void)namaFile;
#endif
}
