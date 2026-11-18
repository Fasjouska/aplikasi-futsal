#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "lapangan.h"

using namespace std;

// =====================================
// STRUKTUR DATA LAPANGAN
// =====================================

struct Lapangan
{
    int id;
    string nama;
    string type;
    int harga;
};

// =====================================
// MEMBACA DATA LAPANGAN
// =====================================

vector<Lapangan> bacaLapangan()
{
    vector<Lapangan> daftar;

    ifstream file("lapangan.txt");
    string baris;

    while (getline(file, baris))
    {
        stringstream ss(baris);

        Lapangan l;
        string dataID;
        string dataHarga;

        getline(ss, dataID, '|');
        getline(ss, l.nama, '|');
        getline(ss, l.type, '|');
        getline(ss, dataHarga, '|');

        if (!dataID.empty() && !dataHarga.empty())
        {
            l.id = atoi(dataID.c_str());
            l.harga = atoi(dataHarga.c_str());

            daftar.push_back(l);
        }
    }

    file.close();

    return daftar;
}

// =====================================
// MENYIMPAN ULANG DATA LAPANGAN
// =====================================

void simpanLapangan(vector<Lapangan> daftar)
{
    ofstream file("lapangan.txt");

    if (!file.is_open())
    {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    for (int i = 0; i < (int)daftar.size(); i++)
    {
        file << daftar[i].id << "|"
             << daftar[i].nama << "|"
             << daftar[i].type << "|"
             << daftar[i].harga << endl;
    }

    file.close();
}

// =====================================
// ID LAPANGAN OTOMATIS
// =====================================

int idBerikutnya()
{
    vector<Lapangan> daftar = bacaLapangan();

    int maxId = 0;

    for (int i = 0; i < (int)daftar.size(); i++)
    {
        if (daftar[i].id > maxId)
        {
            maxId = daftar[i].id;
        }
    }

    return maxId + 1;
}

// =====================================
// TAMBAH LAPANGAN
// =====================================

void tambahLapangan()
{
    Lapangan l;

    cout << "\n===== TAMBAH DATA LAPANGAN =====" << endl;

    l.id = idBerikutnya();

    cout << "ID Lapangan : " << l.id << endl;

    cout << "Nama Lapangan : ";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, l.nama);

    if (l.nama.empty() || l.nama.find('|') != string::npos)
    {
        cout << "Nama lapangan tidak valid!" << endl;
        return;
    }

    cout << "\nPilih Type Lapangan" << endl;
    cout << "1. Indoor  - Rp150000 / jam" << endl;
    cout << "2. Outdoor - Rp100000 / jam" << endl;

    cout << "Pilih Type : ";
    cin >> l.id; // Sementara digunakan untuk input pilihan

    if (l.id == 1)
    {
        l.type = "Indoor";
        l.harga = 150000;
    }
    else if (l.id == 2)
    {
        l.type = "Outdoor";
        l.harga = 100000;
    }
    else
    {
        cout << "Pilihan tidak tersedia!" << endl;
        return;
    }

    // Mengembalikan ID sebenarnya
    l.id = idBerikutnya();

    vector<Lapangan> daftar = bacaLapangan();

    daftar.push_back(l);

    simpanLapangan(daftar);

    cout << "\nData lapangan berhasil disimpan!" << endl;
    cout << "ID Lapangan : " << l.id << endl;
    cout << "Nama        : " << l.nama << endl;
    cout << "Type        : " << l.type << endl;
    cout << "Harga       : Rp" << l.harga << endl;
}

// =====================================
// LIHAT LAPANGAN
// =====================================

void lihatLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();

    cout << "\n========== DATA LAPANGAN ==========" << endl;

    cout << "ID\tNama\t\tType\t\tHarga" << endl;
    cout << "---------------------------------------------" << endl;

    if (daftar.empty())
    {
        cout << "Belum ada data lapangan." << endl;
        return;
    }

    for (int i = 0; i < (int)daftar.size(); i++)
    {
        cout << daftar[i].id << "\t"
             << daftar[i].nama << "\t\t"
             << daftar[i].type << "\t\t"
             << "Rp" << daftar[i].harga << endl;
    }
}

// =====================================
// EDIT LAPANGAN
// =====================================

void editLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();

    int id;
    int pilihan;

    cout << "\n===== EDIT DATA LAPANGAN =====" << endl;

    if (daftar.empty())
    {
        cout << "Belum ada data lapangan." << endl;
        return;
    }

    lihatLapangan();

    cout << "\nMasukkan ID Lapangan yang akan diedit : ";
    cin >> id;

    bool ditemukan = false;

    for (int i = 0; i < (int)daftar.size(); i++)
    {
        if (daftar[i].id == id)
        {
            ditemukan = true;

            cout << "\nData Lapangan Ditemukan" << endl;
            cout << "Nama Lama : " << daftar[i].nama << endl;
            cout << "Type Lama : " << daftar[i].type << endl;
            cout << "Harga Lama: Rp" << daftar[i].harga << endl;

            cout << "\nNama Lapangan Baru : ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, daftar[i].nama);

            if (daftar[i].nama.empty() ||
                daftar[i].nama.find('|') != string::npos)
            {
                cout << "Nama tidak valid! Edit dibatalkan." << endl;
                return;
            }

            cout << "\nPilih Type Lapangan Baru" << endl;
            cout << "1. Indoor  - Rp150000 / jam" << endl;
            cout << "2. Outdoor - Rp100000 / jam" << endl;

            cout << "Pilih Type : ";
            cin >> pilihan;

            if (pilihan == 1)
            {
                daftar[i].type = "Indoor";
                daftar[i].harga = 150000;
            }
            else if (pilihan == 2)
            {
                daftar[i].type = "Outdoor";
                daftar[i].harga = 100000;
            }
            else
            {
                cout << "Pilihan tidak tersedia! Edit dibatalkan." << endl;
                return;
            }

            simpanLapangan(daftar);

            cout << "\nData lapangan berhasil diperbarui!" << endl;

            cout << "ID Lapangan : " << daftar[i].id << endl;
            cout << "Nama        : " << daftar[i].nama << endl;
            cout << "Type        : " << daftar[i].type << endl;
            cout << "Harga       : Rp" << daftar[i].harga << endl;

            break;
        }
    }

    if (!ditemukan)
    {
        cout << "ID Lapangan tidak ditemukan!" << endl;
    }
}

// =====================================
// HAPUS LAPANGAN
// =====================================

void hapusLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();

    int id;
    char konfirmasi;

    cout << "\n===== HAPUS DATA LAPANGAN =====" << endl;

    if (daftar.empty())
    {
        cout << "Belum ada data lapangan." << endl;
        return;
    }

    lihatLapangan();

    cout << "\nMasukkan ID Lapangan yang akan dihapus : ";
    cin >> id;

    bool ditemukan = false;

    for (int i = 0; i < (int)daftar.size(); i++)
    {
        if (daftar[i].id == id)
        {
            ditemukan = true;

            cout << "\nData yang akan dihapus:" << endl;
            cout << "Nama  : " << daftar[i].nama << endl;
            cout << "Type  : " << daftar[i].type << endl;
            cout << "Harga : Rp" << daftar[i].harga << endl;

            cout << "\nYakin ingin menghapus data? (Y/T) : ";
            cin >> konfirmasi;

            if (konfirmasi == 'Y' || konfirmasi == 'y')
            {
                daftar.erase(daftar.begin() + i);

                simpanLapangan(daftar);

                cout << "\nData lapangan berhasil dihapus!" << endl;
            }
            else
            {
                cout << "\nPenghapusan dibatalkan." << endl;
            }

            break;
        }
    }

    if (!ditemukan)
    {
        cout << "ID Lapangan tidak ditemukan!" << endl;
    }
}

// =====================================
// MENU DATA LAPANGAN
// =====================================

void menuLapangan()
{
    int pilihan;

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

        cout << "Pilih : ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahLapangan();
            break;

        case 2:
            lihatLapangan();
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

void cetakLapangan()
{
    vector<Lapangan> daftar = bacaLapangan();
    ofstream file("laporan_lapangan.html");

    if (!file.is_open())
    {
        cout << "Gagal membuat file laporan!" << endl;
        return;
    }

    time_t sekarang = time(0);
    tm *waktu = localtime(&sekarang);

    char tanggal[50];
    if (waktu != 0)
    {
        strftime(tanggal, sizeof(tanggal), "%d-%m-%Y", waktu);
    }
    else
    {
        tanggal[0] = '\0';
    }

    file << "<!DOCTYPE html>\n";
    file << "<html lang='id'>\n";
    file << "<head>\n";
    file << "<meta charset='UTF-8'>\n";
    file << "<title>Laporan Data Lapangan</title>\n";

    file << "<style>\n";
    file << "body { font-family: Arial, sans-serif; margin: 40px; color: #333; }\n";
    file << ".header { text-align: center; border-bottom: 3px solid #333; "
            "padding-bottom: 15px; margin-bottom: 25px; }\n";
    file << "h2 { margin-bottom: 5px; }\n";
    file << ".info { margin-bottom: 20px; }\n";
    file << "table { width: 100%; border-collapse: collapse; }\n";
    file << "th, td { border: 1px solid #555; padding: 12px; }\n";
    file << "th { background-color: #e5e5e5; text-align: center; }\n";
    file << "td:first-child { text-align: center; }\n";
    file << ".footer { margin-top: 50px; text-align: right; }\n";
    file << "button { padding: 10px 20px; background: #198754; color: white; "
            "border: none; cursor: pointer; border-radius: 5px; margin-bottom: 20px; }\n";
    file << "@media print { button { display: none; } body { margin: 20px; } }\n";
    file << "</style>\n";
    file << "</head>\n";
    file << "<body>\n";
    file << "<div class='header'>\n";
    file << "<h2>LAPORAN DATA LAPANGAN</h2>\n";
    file << "<p>SISTEM INFORMASI MANAJEMEN FUTSAL</p>\n";
    file << "</div>\n";
    file << "<button onclick='window.print()'>";
    file << "Cetak Laporan";
    file << "</button>\n";
    file << "<div class='info'>\n";
    if (tanggal[0] != '\0')
    {
        file << "<p>Tanggal Cetak : " << tanggal << "</p>\n";
    }
    file << "</div>\n";
    file << "<table>\n";
    file << "<tr>\n";
    file << "<th width='10%'>No</th>\n";
    file << "<th>Nama Lapangan</th>\n";
    file << "<th>Jenis Lapangan</th>\n";
    file << "<th>Harga Sewa</th>\n";
    file << "</tr>\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << "<tr>\n";
        file << "<td>" << i + 1 << "</td>\n";
        file << "<td>" << escapeHtml(daftar[i].nama) << "</td>\n";
        file << "<td>" << escapeHtml(daftar[i].type) << "</td>\n";
        file << "<td>Rp " << daftar[i].harga << "</td>\n";
        file << "</tr>\n";
    }
    if (daftar.empty())
    {
        file << "<tr><td colspan='4'>Belum ada data lapangan.</td></tr>\n";
    }
    file << "</table>\n";

    file << "<div class='footer'>\n";
    file << "<p>Mengetahui,</p>\n";
    file << "<br><br><br>\n";
    file << "<p>_____________________</p>\n";
    file << "<p>Administrator</p>\n";
    file << "</div>\n";

    file << "</body>\n";
    file << "</html>\n";
    file.close();

    if (!file)
    {
        cout << "Gagal menyimpan file laporan!" << endl;
        return;
    }

    cout << "Laporan berhasil dibuat!" << endl;
    system("start \"\" \"laporan_lapangan.html\"");
}