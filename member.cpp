#include "member.h"
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
const char *FILE_MEMBER = "member.txt";

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

int cariIndeksMember(const vector<Member> &daftar, int id)
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

void cariMemberMenu()
{
    vector<Member> daftar = bacaMember();
    if (daftar.empty())
    {
        cout << "\nBelum ada member terdaftar." << endl;
        return;
    }

    string kataKunci;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!inputTeks("Masukkan kata kunci (Nama / No. WA): ", kataKunci))
    {
        return;
    }

    string kunciLower = keHurufKecil(kataKunci);
    vector<Member> hasil;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (keHurufKecil(daftar[i].nama_member).find(kunciLower) != string::npos ||
            keHurufKecil(daftar[i].no_wa).find(kunciLower) != string::npos)
        {
            hasil.push_back(daftar[i]);
        }
    }

    if (hasil.empty())
    {
        cout << "Member dengan kata kunci \"" << kataKunci << "\" tidak ditemukan." << endl;
        return;
    }

    tampilkanMember(hasil);
}

void ubahMember()
{
    vector<Member> daftar = bacaMember();
    tampilkanMember(daftar);
    if (daftar.empty())
    {
        return;
    }

    int id = 0;
    if (!inputAngka("Masukkan ID member yang akan diubah: ", id))
    {
        return;
    }

    int indeks = cariIndeksMember(daftar, id);
    if (indeks < 0)
    {
        cout << "ID member tidak ditemukan." << endl;
        return;
    }

    Member &member = daftar[indeks];
    cout << "Nama saat ini : " << member.nama_member << endl;
    cout << "No. WA saat ini: " << member.no_wa << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!inputTeks("Nama baru: ", member.nama_member) ||
        !inputTeks("No. WhatsApp baru: ", member.no_wa))
    {
        cout << "Perubahan dibatalkan." << endl;
        return;
    }

    if (simpanMember(daftar))
    {
        sinkronkanNamaMemberDiBooking(member.id_member, member.nama_member);
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

    int id = 0;
    if (!inputAngka("Masukkan ID member yang akan dihapus: ", id))
    {
        return;
    }

    int indeks = cariIndeksMember(daftar, id);
    if (indeks < 0)
    {
        cout << "ID member tidak ditemukan." << endl;
        return;
    }

    if (cekMemberDigunakanDiBooking(id))
    {
        cout << "Member tidak dapat dihapus karena masih tercatat pada data booking!" << endl;
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
} // namespace

vector<Member> bacaMember()
{
    vector<Member> daftar;
    ifstream file(FILE_MEMBER);
    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }
        stringstream ss(baris);
        string dataId;
        Member member;

        getline(ss, dataId, '|');
        getline(ss, member.nama_member, '|');
        getline(ss, member.no_wa, '|');

        if (konversiInt(dataId, member.id_member) &&
            !member.nama_member.empty() && !member.no_wa.empty())
        {
            daftar.push_back(member);
        }
    }

    return daftar;
}

bool cariMemberById(int id, Member &hasil)
{
    vector<Member> daftar = bacaMember();
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_member == id)
        {
            hasil = daftar[i];
            return true;
        }
    }
    return false;
}

void tampilkanDaftarMember()
{
    tampilkanMember(bacaMember());
}

void cetakLaporanMember()
{
    vector<Member> daftar = bacaMember();
    const string namaFile = "laporan_member.html";
    ofstream file(namaFile.c_str());

    if (!file.is_open())
    {
        cout << "Gagal membuat file laporan member!" << endl;
        return;
    }

    file << "<!DOCTYPE html>\n"
         << "<html lang='id'>\n<head>\n<meta charset='UTF-8'>\n"
         << "<title>Laporan Data Member</title>\n"
         << "<style>\n"
         << "body { font-family: Arial, sans-serif; margin: 40px; color: #333; }\n"
         << ".header { text-align: center; border-bottom: 3px solid #333; padding-bottom: 15px; margin-bottom: 25px; }\n"
         << "table { width: 100%; border-collapse: collapse; }\n"
         << "th, td { border: 1px solid #555; padding: 10px; text-align: left; }\n"
         << "th { background-color: #e5e5e5; }\n"
         << "button { padding: 10px 20px; background: #198754; color: white; border: none; cursor: pointer; border-radius: 5px; margin-bottom: 20px; }\n"
         << ".footer { margin-top: 50px; text-align: right; }\n"
         << "@media print { button { display: none; } body { margin: 20px; } }\n"
         << "</style>\n</head>\n<body>\n"
         << "<div class='header'><h2>LAPORAN DATA MEMBER</h2>"
         << "<p>SISTEM INFORMASI MANAJEMEN FUTSAL</p></div>\n"
         << "<button onclick='window.print()'>Cetak Laporan</button>\n"
         << "<p>Tanggal Cetak : " << escapeHtml(tanggalSekarang()) << "</p>\n"
         << "<p>Total Member  : " << daftar.size() << "</p>\n"
         << "<table>\n<tr><th width='10%'>ID</th><th>Nama Member</th><th>No. WhatsApp</th></tr>\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << "<tr><td>" << daftar[i].id_member
             << "</td><td>" << escapeHtml(daftar[i].nama_member)
             << "</td><td>" << escapeHtml(daftar[i].no_wa)
             << "</td></tr>\n";
    }
    if (daftar.empty())
    {
        file << "<tr><td colspan='3'>Belum ada data member.</td></tr>\n";
    }

    file << "</table>\n"
         << "<div class='footer'><p>Mengetahui,</p><br><br><br>"
         << "<p>_____________________</p><p>Administrator</p></div>\n"
         << "</body>\n</html>\n";
    file.close();

    if (!file)
    {
        cout << "Gagal menyimpan laporan member." << endl;
        return;
    }

    cout << "Laporan berhasil dibuat: " << namaFile << endl;
    bukaFileHtml(namaFile);
}

void menuMember()
{
    int pilihan = 0;
    do
    {
        cout << "\n================================\n";
        cout << "          MENU MEMBER\n";
        cout << "================================\n";
        cout << "1. Tambah Member\n";
        cout << "2. Lihat Member\n";
        cout << "3. Cari Member\n";
        cout << "4. Ubah Member\n";
        cout << "5. Hapus Member\n";
        cout << "6. Cetak Laporan Member\n";
        cout << "7. Kembali ke Menu Utama\n";

        if (!inputAngka("Pilih: ", pilihan))
        {
            return;
        }

        switch (pilihan)
        {
        case 1:
            tambahMember();
            break;
        case 2:
            tampilkanDaftarMember();
            break;
        case 3:
            cariMemberMenu();
            break;
        case 4:
            ubahMember();
            break;
        case 5:
            hapusMember();
            break;
        case 6:
            cetakLaporanMember();
            break;
        case 7:
            cout << "Kembali ke menu utama." << endl;
            break;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 7);
}
