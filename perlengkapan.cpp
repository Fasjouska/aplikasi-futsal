#include "perlengkapan.h"
#include "booking.h"
#include "utils.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace
{
const char *FILE_PERLENGKAPAN = "perlengkapan.txt";

bool simpanPerlengkapan(const vector<Perlengkapan> &daftar)
{
    ofstream file(FILE_PERLENGKAPAN);
    if (!file.is_open())
    {
        cout << "Gagal membuka " << FILE_PERLENGKAPAN << " untuk disimpan." << endl;
        return false;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id << "|"
             << daftar[i].jenis << "|"
             << daftar[i].pilihan << "|"
             << daftar[i].jumlah << "|"
             << daftar[i].harga << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan data perlengkapan." << endl;
        return false;
    }
    return true;
}

int idPerlengkapanBerikutnya(const vector<Perlengkapan> &daftar)
{
    int idTerbesar = 0;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id > idTerbesar)
        {
            idTerbesar = daftar[i].id;
        }
    }
    return idTerbesar + 1;
}

bool pilihJenis(string &jenis)
{
    int pilihan = 0;
    cout << "\nPilih jenis perlengkapan:" << endl;
    cout << "1. Jersey" << endl;
    cout << "2. Bola" << endl;
    cout << "3. Rompi" << endl;

    while (true)
    {
        if (!inputAngka("Pilih: ", pilihan))
        {
            return false;
        }
        if (pilihan == 1)
        {
            jenis = "Jersey";
            return true;
        }
        if (pilihan == 2)
        {
            jenis = "Bola";
            return true;
        }
        if (pilihan == 3)
        {
            jenis = "Rompi";
            return true;
        }
        cout << "Pilihan tidak tersedia." << endl;
    }
}

bool isiRincian(Perlengkapan &item)
{
    if (item.jenis == "Bola")
    {
        item.pilihan = "Satuan";
        item.harga = 5000;
        cout << "Harga bola ditetapkan Rp5000 per bola per jam." << endl;
    }
    else
    {
        int pilihan = 0;
        cout << "\nPilih jenis sewa:" << endl;
        cout << "1. Paketan (6 orang)" << endl;
        cout << "2. Satuan" << endl;

        while (true)
        {
            if (!inputAngka("Pilih: ", pilihan))
            {
                return false;
            }
            if (pilihan == 1)
            {
                item.pilihan = "Paketan";
                item.harga = item.jenis == "Jersey" ? 50000 : 25000;
                break;
            }
            if (pilihan == 2)
            {
                item.pilihan = "Satuan";
                item.harga = item.jenis == "Jersey" ? 10000 : 5000;
                break;
            }
            cout << "Pilihan tidak tersedia." << endl;
        }

        if (item.pilihan == "Paketan")
        {
            cout << "Harga " << item.jenis
                 << " paketan (6 orang): Rp" << item.harga
                 << " per jam." << endl;
        }
        else
        {
            cout << "Harga " << item.jenis << " satuan: Rp" << item.harga
                 << " per orang per jam." << endl;
        }
    }

    const string labelJumlah =
        item.jenis == "Bola" ? "Jumlah bola: " :
        item.pilihan == "Paketan" ? "Jumlah paket: " : "Jumlah satuan: ";

    while (true)
    {
        if (!inputAngka(labelJumlah, item.jumlah))
        {
            return false;
        }
        if (item.jumlah > 0)
        {
            return true;
        }
        cout << "Jumlah harus lebih dari 0." << endl;
    }
}

void tampilkanPerlengkapan(const vector<Perlengkapan> &daftar,
                           const string &jenis = "")
{
    cout << "\n================ DATA PERLENGKAPAN ================\n";
    if (daftar.empty())
    {
        cout << "Belum ada data perlengkapan." << endl;
        return;
    }

    cout << left << setw(6) << "ID"
         << setw(12) << "Jenis"
         << setw(16) << "Pilihan"
         << setw(12) << "Jumlah"
         << setw(16) << "Tarif/Jam"
         << "Total/Jam" << endl;
    cout << "---------------------------------------------------------------"
         << endl;

    bool ditemukan = false;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (!jenis.empty() && daftar[i].jenis != jenis)
        {
            continue;
        }

        ditemukan = true;
        const long long total = static_cast<long long>(daftar[i].jumlah) *
                                daftar[i].harga;
        cout << left << setw(6) << daftar[i].id
             << setw(12) << daftar[i].jenis
             << setw(16) << daftar[i].pilihan
             << setw(12) << daftar[i].jumlah
             << "Rp" << setw(14) << daftar[i].harga
             << "Rp" << total << endl;
    }

    if (!ditemukan)
    {
        cout << "Belum ada data " << jenis << "." << endl;
    }
}

int cariIndeks(const vector<Perlengkapan> &daftar, const string &jenis)
{
    bool ditemukan = false;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].jenis == jenis)
        {
            ditemukan = true;
            break;
        }
    }
    if (!ditemukan)
    {
        cout << "Belum ada data " << jenis << "." << endl;
        return -1;
    }

    tampilkanPerlengkapan(daftar, jenis);
    int id = 0;
    if (!inputAngka("Masukkan ID perlengkapan: ", id))
    {
        return -1;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].jenis == jenis && daftar[i].id == id)
        {
            return static_cast<int>(i);
        }
    }

    cout << "ID tidak ditemukan pada kategori " << jenis << "." << endl;
    return -1;
}

void tambahPerlengkapan()
{
    vector<Perlengkapan> daftar = bacaPerlengkapan();
    Perlengkapan item;

    cout << "\n===== TAMBAH PERLENGKAPAN =====" << endl;
    if (!pilihJenis(item.jenis) || !isiRincian(item))
    {
        cout << "Penambahan dibatalkan." << endl;
        return;
    }

    item.id = idPerlengkapanBerikutnya(daftar);
    daftar.push_back(item);
    if (simpanPerlengkapan(daftar))
    {
        cout << "Data berhasil ditambahkan dengan ID " << item.id << "."
             << endl;
    }
}

void editPerlengkapan()
{
    vector<Perlengkapan> daftar = bacaPerlengkapan();
    string jenis;
    if (!pilihJenis(jenis))
    {
        return;
    }

    int indeks = cariIndeks(daftar, jenis);
    if (indeks < 0)
    {
        return;
    }

    Perlengkapan perubahan = daftar[indeks];
    if (!isiRincian(perubahan))
    {
        cout << "Perubahan dibatalkan." << endl;
        return;
    }

    daftar[indeks] = perubahan;
    if (simpanPerlengkapan(daftar))
    {
        cout << "Data perlengkapan berhasil diubah." << endl;
    }
}

void hapusPerlengkapan()
{
    vector<Perlengkapan> daftar = bacaPerlengkapan();
    string jenis;
    if (!pilihJenis(jenis))
    {
        return;
    }

    int indeks = cariIndeks(daftar, jenis);
    if (indeks < 0)
    {
        return;
    }

    if (cekPerlengkapanDigunakanDiBooking(daftar[indeks].id))
    {
        cout << "Perlengkapan tidak dapat dihapus karena masih tercatat pada data booking!" << endl;
        return;
    }

    char konfirmasi;
    cout << "Hapus data ID " << daftar[indeks].id << "? (Y/T): ";
    if (!(cin >> konfirmasi))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input konfirmasi tidak valid." << endl;
        return;
    }

    if (konfirmasi != 'Y' && konfirmasi != 'y')
    {
        cout << "Penghapusan dibatalkan." << endl;
        return;
    }

    daftar.erase(daftar.begin() + indeks);
    if (simpanPerlengkapan(daftar))
    {
        cout << "Data perlengkapan berhasil dihapus." << endl;
    }
}

void cetakPerlengkapan()
{
    vector<Perlengkapan> daftar = bacaPerlengkapan();
    const string namaFile = "laporan_perlengkapan.html";
    ofstream file(namaFile.c_str());
    if (!file.is_open())
    {
        cout << "Gagal membuat file laporan perlengkapan!" << endl;
        return;
    }

    file << "<!DOCTYPE html>\n"
         << "<html lang='id'><head><meta charset='UTF-8'>\n"
         << "<title>Laporan Data Perlengkapan</title>\n"
         << "<style>\n"
         << "body{font-family:Arial,sans-serif;margin:40px;color:#333;}\n"
         << ".header{text-align:center;border-bottom:3px solid #333;padding-bottom:15px;margin-bottom:25px;}\n"
         << "table{width:100%;border-collapse:collapse;}\n"
         << "th,td{border:1px solid #555;padding:10px;text-align:left;}\n"
         << "th{background:#e5e5e5;}\n"
         << "button{padding:10px 20px;background:#198754;color:white;border:0;border-radius:5px;margin-bottom:20px;}\n"
         << "@media print{button{display:none;}body{margin:20px;}}\n"
         << "</style></head><body>\n"
         << "<div class='header'><h2>LAPORAN DATA PERLENGKAPAN</h2>"
         << "<p>SISTEM INFORMASI MANAJEMEN FUTSAL</p></div>\n"
         << "<button onclick='window.print()'>Cetak Laporan</button>\n"
         << "<p>Tanggal Cetak: " << escapeHtml(tanggalSekarang()) << "</p>\n"
         << "<table><tr><th>ID</th><th>Jenis</th><th>Pilihan</th>"
         << "<th>Jumlah</th><th>Tarif/Jam</th><th>Total/Jam</th></tr>\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        const long long total = static_cast<long long>(daftar[i].jumlah) *
                                daftar[i].harga;
        string pilihan = daftar[i].pilihan;
        if ((daftar[i].jenis == "Jersey" || daftar[i].jenis == "Rompi") &&
            pilihan == "Paketan")
        {
            pilihan += " (6 orang)";
        }

        file << "<tr><td>" << daftar[i].id
             << "</td><td>" << escapeHtml(daftar[i].jenis)
             << "</td><td>" << escapeHtml(pilihan)
             << "</td><td>" << daftar[i].jumlah
             << "</td><td>Rp " << daftar[i].harga
             << "</td><td>Rp " << total << "</td></tr>\n";
    }
    if (daftar.empty())
    {
        file << "<tr><td colspan='6'>Belum ada data perlengkapan.</td></tr>\n";
    }
    file << "</table></body></html>\n";
    file.close();

    if (!file)
    {
        cout << "Gagal menyimpan laporan perlengkapan." << endl;
        return;
    }

    cout << "Laporan berhasil dibuat: " << namaFile << endl;
    bukaFileHtml(namaFile);
}
} // namespace

vector<Perlengkapan> bacaPerlengkapan()
{
    vector<Perlengkapan> daftar;
    ifstream file(FILE_PERLENGKAPAN);
    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }
        stringstream ss(baris);
        string dataId;
        string dataJumlah;
        string dataHarga;
        Perlengkapan item;

        getline(ss, dataId, '|');
        getline(ss, item.jenis, '|');
        getline(ss, item.pilihan, '|');
        getline(ss, dataJumlah, '|');
        getline(ss, dataHarga, '|');

        if (konversiInt(dataId, item.id) &&
            konversiInt(dataJumlah, item.jumlah) &&
            konversiInt(dataHarga, item.harga))
        {
            daftar.push_back(item);
        }
    }

    return daftar;
}

bool cariPerlengkapanById(int id, Perlengkapan &hasil)
{
    vector<Perlengkapan> daftar = bacaPerlengkapan();
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id == id)
        {
            hasil = daftar[i];
            return true;
        }
    }
    return false;
}

void menuPerlengkapan()
{
    int pilihan = 0;
    do
    {
        cout << "\n================================\n";
        cout << "       MENU PERLENGKAPAN\n";
        cout << "================================\n";
        cout << "1. Tambah Perlengkapan\n";
        cout << "2. Lihat Perlengkapan\n";
        cout << "3. Edit Perlengkapan\n";
        cout << "4. Hapus Perlengkapan\n";
        cout << "5. Cetak Laporan Perlengkapan\n";
        cout << "6. Kembali ke Menu Utama\n";

        if (!inputAngka("Pilih: ", pilihan))
        {
            return;
        }

        switch (pilihan)
        {
        case 1:
            tambahPerlengkapan();
            break;
        case 2:
            tampilkanPerlengkapan(bacaPerlengkapan());
            break;
        case 3:
            editPerlengkapan();
            break;
        case 4:
            hapusPerlengkapan();
            break;
        case 5:
            cetakPerlengkapan();
            break;
        case 6:
            cout << "Kembali ke menu utama." << endl;
            break;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 6);
}
