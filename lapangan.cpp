#include "lapangan.h"
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
const char *FILE_LAPANGAN = "lapangan.txt";

bool simpanLapangan(const vector<Lapangan> &daftar)
{
    ofstream file(FILE_LAPANGAN);
    if (!file.is_open())
    {
        cout << "Gagal membuka file " << FILE_LAPANGAN << "!" << endl;
        return false;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id << "|"
             << daftar[i].nama << "|"
             << daftar[i].type << "|"
             << daftar[i].harga << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan data lapangan!" << endl;
        return false;
    }
    return true;
}

int idLapanganBerikutnya(const vector<Lapangan> &daftar)
{
    int maxId = 0;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id > maxId)
        {
            maxId = daftar[i].id;
        }
    }
    return maxId + 1;
}

bool pilihTypeLapangan(string &type, int &harga)
{
    int pilihan = 0;
    cout << "\nPilih Type Lapangan" << endl;
    cout << "1. Indoor  - Rp150000 / jam" << endl;
    cout << "2. Outdoor - Rp100000 / jam" << endl;

    if (!inputAngka("Pilih Type : ", pilihan))
    {
        return false;
    }

    if (pilihan == 1)
    {
        type = "Indoor";
        harga = 150000;
        return true;
    }
    if (pilihan == 2)
    {
        type = "Outdoor";
        harga = 100000;
        return true;
    }

    cout << "Pilihan tidak tersedia!" << endl;
    return false;
}

void tampilkanLapangan(const vector<Lapangan> &daftar)
{
    cout << "\n================ DATA LAPANGAN ================\n";
    if (daftar.empty())
    {
        cout << "Belum ada data lapangan." << endl;
        return;
    }

    cout << left << setw(8) << "ID"
         << setw(24) << "Nama Lapangan"
         << setw(14) << "Type"
         << "Harga/Jam" << endl;
    cout << "------------------------------------------------------------\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        cout << left << setw(8) << daftar[i].id
             << setw(24) << daftar[i].nama
             << setw(14) << daftar[i].type
             << "Rp" << daftar[i].harga << endl;
    }
}

void tambahLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();
    Lapangan l;
    l.id = idLapanganBerikutnya(daftar);

    cout << "\n===== TAMBAH DATA LAPANGAN =====" << endl;
    cout << "ID Lapangan : " << l.id << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!inputTeks("Nama Lapangan : ", l.nama))
    {
        cout << "Input dibatalkan." << endl;
        return;
    }

    if (!pilihTypeLapangan(l.type, l.harga))
    {
        return;
    }

    daftar.push_back(l);
    if (simpanLapangan(daftar))
    {
        cout << "\nData lapangan berhasil disimpan!" << endl;
        cout << "ID Lapangan : " << l.id << endl;
        cout << "Nama        : " << l.nama << endl;
        cout << "Type        : " << l.type << endl;
        cout << "Harga       : Rp" << l.harga << endl;
    }
}

void editLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();
    cout << "\n===== EDIT DATA LAPANGAN =====" << endl;

    if (daftar.empty())
    {
        cout << "Belum ada data lapangan." << endl;
        return;
    }

    tampilkanLapangan(daftar);

    int id = 0;
    if (!inputAngka("\nMasukkan ID Lapangan yang akan diedit : ", id))
    {
        return;
    }

    int indeks = -1;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id == id)
        {
            indeks = static_cast<int>(i);
            break;
        }
    }

    if (indeks < 0)
    {
        cout << "ID Lapangan tidak ditemukan!" << endl;
        return;
    }

    Lapangan &l = daftar[indeks];
    cout << "\nData Lapangan Ditemukan" << endl;
    cout << "Nama Lama : " << l.nama << endl;
    cout << "Type Lama : " << l.type << endl;
    cout << "Harga Lama: Rp" << l.harga << endl;

    string namaBaru;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!inputTeks("\nNama Lapangan Baru : ", namaBaru))
    {
        cout << "Edit dibatalkan." << endl;
        return;
    }

    string typeBaru;
    int hargaBaru = 0;
    if (!pilihTypeLapangan(typeBaru, hargaBaru))
    {
        cout << "Edit dibatalkan." << endl;
        return;
    }

    l.nama = namaBaru;
    l.type = typeBaru;
    l.harga = hargaBaru;

    if (simpanLapangan(daftar))
    {
        sinkronkanNamaLapanganDiBooking(l.id, l.nama);
        cout << "\nData lapangan berhasil diperbarui!" << endl;
        cout << "ID Lapangan : " << l.id << endl;
        cout << "Nama        : " << l.nama << endl;
        cout << "Type        : " << l.type << endl;
        cout << "Harga       : Rp" << l.harga << endl;
    }
}

void hapusLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();
    cout << "\n===== HAPUS DATA LAPANGAN =====" << endl;

    if (daftar.empty())
    {
        cout << "Belum ada data lapangan." << endl;
        return;
    }

    tampilkanLapangan(daftar);

    int id = 0;
    if (!inputAngka("\nMasukkan ID Lapangan yang akan dihapus : ", id))
    {
        return;
    }

    int indeks = -1;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id == id)
        {
            indeks = static_cast<int>(i);
            break;
        }
    }

    if (indeks < 0)
    {
        cout << "ID Lapangan tidak ditemukan!" << endl;
        return;
    }

    if (cekLapanganDigunakanDiBooking(id))
    {
        cout << "Lapangan tidak dapat dihapus karena masih tercatat pada data booking!" << endl;
        return;
    }

    cout << "\nData yang akan dihapus:" << endl;
    cout << "Nama  : " << daftar[indeks].nama << endl;
    cout << "Type  : " << daftar[indeks].type << endl;
    cout << "Harga : Rp" << daftar[indeks].harga << endl;

    char konfirmasi;
    cout << "\nYakin ingin menghapus data? (Y/T) : ";
    cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y')
    {
        daftar.erase(daftar.begin() + indeks);
        if (simpanLapangan(daftar))
        {
            cout << "\nData lapangan berhasil dihapus!" << endl;
        }
    }
    else
    {
        cout << "\nPenghapusan dibatalkan." << endl;
    }
}
} // namespace

vector<Lapangan> bacaLapangan()
{
    vector<Lapangan> daftar;
    ifstream file(FILE_LAPANGAN);
    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }
        stringstream ss(baris);
        Lapangan l;
        string dataID;
        string dataHarga;

        getline(ss, dataID, '|');
        getline(ss, l.nama, '|');
        getline(ss, l.type, '|');
        getline(ss, dataHarga, '|');

        if (konversiInt(dataID, l.id) &&
            konversiInt(dataHarga, l.harga) &&
            !l.nama.empty())
        {
            daftar.push_back(l);
        }
    }

    return daftar;
}

bool cariLapanganById(int id, Lapangan &hasil)
{
    vector<Lapangan> daftar = bacaLapangan();
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

void tampilkanDaftarLapangan()
{
    tampilkanLapangan(bacaLapangan());
}

void cetakLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();
    const string namaFile = "laporan_lapangan.html";
    ofstream file(namaFile.c_str());

    if (!file.is_open())
    {
        cout << "Gagal membuat file laporan!" << endl;
        return;
    }

    file << "<!DOCTYPE html>\n"
         << "<html lang='id'>\n<head>\n<meta charset='UTF-8'>\n"
         << "<title>Laporan Data Lapangan</title>\n"
         << "<style>\n"
         << "body { font-family: Arial, sans-serif; margin: 40px; color: #333; }\n"
         << ".header { text-align: center; border-bottom: 3px solid #333; padding-bottom: 15px; margin-bottom: 25px; }\n"
         << "h2 { margin-bottom: 5px; }\n"
         << ".info { margin-bottom: 20px; }\n"
         << "table { width: 100%; border-collapse: collapse; }\n"
         << "th, td { border: 1px solid #555; padding: 12px; }\n"
         << "th { background-color: #e5e5e5; text-align: center; }\n"
         << "td:first-child { text-align: center; }\n"
         << ".footer { margin-top: 50px; text-align: right; }\n"
         << "button { padding: 10px 20px; background: #198754; color: white; border: none; cursor: pointer; border-radius: 5px; margin-bottom: 20px; }\n"
         << "@media print { button { display: none; } body { margin: 20px; } }\n"
         << "</style>\n</head>\n<body>\n"
         << "<div class='header'>\n"
         << "<h2>LAPORAN DATA LAPANGAN</h2>\n"
         << "<p>SISTEM INFORMASI MANAJEMEN FUTSAL</p>\n"
         << "</div>\n"
         << "<button onclick='window.print()'>Cetak Laporan</button>\n"
         << "<div class='info'>\n"
         << "<p>Tanggal Cetak : " << escapeHtml(tanggalSekarang()) << "</p>\n"
         << "</div>\n"
         << "<table>\n<tr>\n"
         << "<th width='10%'>No</th>\n"
         << "<th>Nama Lapangan</th>\n"
         << "<th>Jenis Lapangan</th>\n"
         << "<th>Harga Sewa</th>\n"
         << "</tr>\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << "<tr>\n"
             << "<td>" << i + 1 << "</td>\n"
             << "<td>" << escapeHtml(daftar[i].nama) << "</td>\n"
             << "<td>" << escapeHtml(daftar[i].type) << "</td>\n"
             << "<td>Rp " << daftar[i].harga << "</td>\n"
             << "</tr>\n";
    }
    if (daftar.empty())
    {
        file << "<tr><td colspan='4'>Belum ada data lapangan.</td></tr>\n";
    }

    file << "</table>\n"
         << "<div class='footer'>\n"
         << "<p>Mengetahui,</p>\n<br><br><br>\n"
         << "<p>_____________________</p>\n"
         << "<p>Administrator</p>\n"
         << "</div>\n</body>\n</html>\n";
    file.close();

    if (!file)
    {
        cout << "Gagal menyimpan file laporan!" << endl;
        return;
    }

    cout << "Laporan berhasil dibuat: " << namaFile << endl;
    bukaFileHtml(namaFile);
}

void menuLapangan()
{
    int pilihan = 0;
    do
    {
        cout << "\n================================" << endl;
        cout << "        MENU DATA LAPANGAN" << endl;
        cout << "================================" << endl;
        cout << "1. Tambah Lapangan" << endl;
        cout << "2. Lihat Lapangan" << endl;
        cout << "3. Edit Lapangan" << endl;
        cout << "4. Hapus Lapangan" << endl;
        cout << "5. Cetak Lapangan" << endl;
        cout << "6. Kembali" << endl;

        if (!inputAngka("Pilih : ", pilihan))
        {
            return;
        }

        switch (pilihan)
        {
        case 1:
            tambahLapangan();
            break;
        case 2:
            tampilkanDaftarLapangan();
            break;
        case 3:
            editLapangan();
            break;
        case 4:
            hapusLapangan();
            break;
        case 5:
            cetakLapangan();
            break;
        case 6:
            cout << "Kembali ke menu utama." << endl;
            break;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 6);
}
