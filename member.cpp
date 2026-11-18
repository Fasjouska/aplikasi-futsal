#include "member.h"

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
struct Member
{
    int id_member;
    string nama_member;
    string no_wa;
};

const char *FILE_MEMBER = "member.txt";

bool konversiId(const string &teks, int &id)
{
    stringstream ss(teks);
    char sisa;
    return (ss >> id) && !(ss >> sisa);
}

vector<Member> bacaMember()
{
    vector<Member> daftar;
    ifstream file(FILE_MEMBER);
    string baris;

    while (getline(file, baris))
    {
        stringstream ss(baris);
        string dataId;
        Member member;

        getline(ss, dataId, '|');
        getline(ss, member.nama_member, '|');
        getline(ss, member.no_wa, '|');

        if (konversiId(dataId, member.id_member) &&
            !member.nama_member.empty() && !member.no_wa.empty())
        {
            daftar.push_back(member);
        }
    }

    return daftar;
}

bool simpanMember(const vector<Member> &daftar)
{
    ofstream file(FILE_MEMBER);
    if (!file.is_open())
    {
        cout << "Gagal membuka " << FILE_MEMBER << " untuk disimpan." << endl;
        return false;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id_member << "|"
             << daftar[i].nama_member << "|"
             << daftar[i].no_wa << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan data member." << endl;
        return false;
    }
    return true;
}

int inputAngka(const string &pesan)
{
    int angka;
    while (true)
    {
        cout << pesan;
        if (cin >> angka)
        {
            return angka;
        }

        if (cin.eof())
        {
            return -1;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus berupa angka." << endl;
    }
}

bool inputTeks(const string &pesan, string &teks)
{
    cout << pesan;
    if (!getline(cin, teks))
    {
        return false;
    }

    while (teks.empty() || teks.find('|') != string::npos)
    {
        cout << "Data tidak boleh kosong atau mengandung karakter '|'."
             << endl;
        cout << pesan;
        if (!getline(cin, teks))
        {
            return false;
        }
    }
    return true;
}

int idMemberBerikutnya(const vector<Member> &daftar)
{
    int idTerbesar = 0;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_member > idTerbesar)
        {
            idTerbesar = daftar[i].id_member;
        }
    }
    return idTerbesar + 1;
}

void tampilkanMember(const vector<Member> &daftar)
{
    cout << "\n================ DATA MEMBER ================\n";
    if (daftar.empty())
    {
        cout << "Belum ada member terdaftar." << endl;
        return;
    }

    cout << left << setw(10) << "ID"
         << setw(30) << "Nama Member"
         << setw(20) << "No. WhatsApp" << endl;
    cout << "------------------------------------------------------------\n";
    for (size_t i = 0; i < daftar.size(); i++)
    {
        cout << left << setw(10) << daftar[i].id_member
             << setw(30) << daftar[i].nama_member
             << setw(20) << daftar[i].no_wa << endl;
    }
}

int cariMember(const vector<Member> &daftar, int id)
{
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_member == id)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void tambahMember()
{
    vector<Member> daftar = bacaMember();
    Member member;
    member.id_member = idMemberBerikutnya(daftar);

    cout << "\n===== TAMBAH MEMBER =====" << endl;
    cout << "ID Member: " << member.id_member << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!inputTeks("Nama Member: ", member.nama_member) ||
        !inputTeks("No. WhatsApp: ", member.no_wa))
    {
        cout << "Input dibatalkan." << endl;
        return;
    }

    daftar.push_back(member);
    if (simpanMember(daftar))
    {
        cout << "Data member berhasil disimpan." << endl;
    }
}

void ubahMember()
{
    vector<Member> daftar = bacaMember();
    tampilkanMember(daftar);
    if (daftar.empty())
    {
        return;
    }

    int id = inputAngka("Masukkan ID member yang akan diubah: ");
    int indeks = cariMember(daftar, id);
    if (indeks < 0)
    {
        cout << "ID member tidak ditemukan." << endl;
        return;
    }

    Member &member = daftar[indeks];
    cout << "Nama saat ini: " << member.nama_member << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!inputTeks("Nama baru: ", member.nama_member) ||
        !inputTeks("No. WhatsApp baru: ", member.no_wa))
    {
        cout << "Perubahan dibatalkan." << endl;
        return;
    }

    if (simpanMember(daftar))
    {
        cout << "Data member berhasil diperbarui." << endl;
    }
}

void hapusMember()
{
    vector<Member> daftar = bacaMember();
    tampilkanMember(daftar);
    if (daftar.empty())
    {
        return;
    }

    int id = inputAngka("Masukkan ID member yang akan dihapus: ");
    int indeks = cariMember(daftar, id);
    if (indeks < 0)
    {
        cout << "ID member tidak ditemukan." << endl;
        return;
    }

    char konfirmasi;
    cout << "Hapus member " << daftar[indeks].nama_member << "? (Y/T): ";
    cin >> konfirmasi;
    if (konfirmasi != 'Y' && konfirmasi != 'y')
    {
        cout << "Penghapusan dibatalkan." << endl;
        return;
    }

    daftar.erase(daftar.begin() + indeks);
    if (simpanMember(daftar))
    {
        cout << "Data member berhasil dihapus." << endl;
    }
}
}

void menuMember()
{
    int pilihan;
    do
    {
        cout << "\n================================\n";
        cout << "          MENU MEMBER\n";
        cout << "================================\n";
        cout << "1. Tambah Member\n";
        cout << "2. Lihat Member\n";
        cout << "3. Ubah Member\n";
        cout << "4. Hapus Member\n";
        cout << "5. Kembali ke Menu Utama\n";

        pilihan = inputAngka("Pilih: ");
        switch (pilihan)
        {
        case 1:
            tambahMember();
            break;
        case 2:
            tampilkanMember(bacaMember());
            break;
        case 3:
            ubahMember();
            break;
        case 4:
            hapusMember();
            break;
        case 5:
            cout << "Kembali ke menu utama." << endl;
            break;
        case -1:
            return;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 5);
}
