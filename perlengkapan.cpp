#include <iostream>
#include <limits>
#include <string>
#include "perlengkapan.h"

using namespace std;

struct Perlengkapan
{
    string jenis;
    string satuan;
    int jumlah;
    int harga;
};

const int MAX_PERLENGKAPAN = 20;
Perlengkapan daftarPerlengkapan[MAX_PERLENGKAPAN];
int totalPerlengkapan = 0;

int inputAngka(const string &pesan)
{
    int nilai;
    cout << pesan;
    while (!(cin >> nilai))
    {
        if (cin.eof())
        {
            return 0;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus berupa angka. " << pesan;
    }
    return nilai;
}

int inputJumlah()
{
    int jumlah = inputAngka("Jumlah: ");
    while (jumlah <= 0)
    {
        cout << "Jumlah harus lebih dari 0." << endl;
        jumlah = inputAngka("Jumlah: ");
    }
    return jumlah;
}

int inputHarga()
{
    int harga = inputAngka("Harga sewa per jam (Rp): ");
    while (harga < 0)
    {
        cout << "Harga tidak boleh negatif." << endl;
        harga = inputAngka("Harga sewa per jam (Rp): ");
    }
    return harga;
}

string namaJenis(int pilihan)
{
    if (pilihan == 1)
    {
        return "Jersey";
    }
    if (pilihan == 2)
    {
        return "Bola";
    }
    return "Rompi";
}

string inputSatuan()
{
    cout << "Pilihan jumlah:" << endl;
    cout << "1. Paketan" << endl;
    cout << "2. Satuan" << endl;

    int pilihan = inputAngka("Pilih: ");
    while (pilihan != 1 && pilihan != 2)
    {
        cout << "Pilihan tidak tersedia." << endl;
        pilihan = inputAngka("Pilih: ");
    }
    return pilihan == 1 ? "Paketan" : "Satuan";
}

bool indeksUntukJenis(int indeks, const string &jenis)
{
    return daftarPerlengkapan[indeks].jenis == jenis;
}

void tampilkanPerlengkapan(const string &jenis)
{
    bool ditemukan = false;
    int nomor = 0;
    cout << "\n===== DATA " << jenis << " =====" << endl;
    cout << "No\tJumlah\t\tHarga/Jam";
    if (jenis != "Bola")
    {
        cout << "\tSatuan";
    }
    cout << endl;

    for (int i = 0; i < totalPerlengkapan; i++)
    {
        if (!indeksUntukJenis(i, jenis))
        {
            continue;
        }

        ditemukan = true;
        nomor++;
        cout << nomor << "\t" << daftarPerlengkapan[i].jumlah << "\t\tRp"
             << daftarPerlengkapan[i].harga;
        if (jenis != "Bola")
        {
            cout << "\t" << daftarPerlengkapan[i].satuan;
        }
        cout << endl;
    }

    if (!ditemukan)
    {
        cout << "Belum ada data " << jenis << "." << endl;
    }
}

void tambahPerlengkapan(const string &jenis)
{
    if (totalPerlengkapan >= MAX_PERLENGKAPAN)
    {
        cout << "Data perlengkapan sudah penuh!" << endl;
        return;
    }

    Perlengkapan item;
    item.jenis = jenis;

    if (jenis == "Bola")
    {
        item.jumlah = inputJumlah();
        item.harga = inputHarga();
        item.satuan = "";
    }
    else
    {
        item.harga = inputHarga();
        item.satuan = inputSatuan();
        item.jumlah = inputJumlah();
    }

    daftarPerlengkapan[totalPerlengkapan] = item;
    totalPerlengkapan++;
    cout << "Data " << jenis << " berhasil ditambahkan." << endl;
}

int cariIndeks(const string &jenis)
{
    bool ditemukan = false;
    for (int i = 0; i < totalPerlengkapan; i++)
    {
        if (indeksUntukJenis(i, jenis))
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

    tampilkanPerlengkapan(jenis);
    int nomor = inputAngka("Pilih nomor data yang akan diubah/dihapus: ");
    int urutanJenis = 0;
    for (int i = 0; i < totalPerlengkapan; i++)
    {
        if (!indeksUntukJenis(i, jenis))
        {
            continue;
        }

        urutanJenis++;
        if (urutanJenis == nomor)
        {
            return i;
        }
    }

    cout << "Nomor data tidak valid untuk kategori " << jenis << "." << endl;
    return -1;
}

void ubahPerlengkapan(const string &jenis)
{
    int indeks = cariIndeks(jenis);
    if (indeks < 0)
    {
        return;
    }

    if (jenis == "Bola")
    {
        daftarPerlengkapan[indeks].jumlah = inputJumlah();
        daftarPerlengkapan[indeks].harga = inputHarga();
    }
    else
    {
        daftarPerlengkapan[indeks].harga = inputHarga();
        daftarPerlengkapan[indeks].satuan = inputSatuan();
        daftarPerlengkapan[indeks].jumlah = inputJumlah();
    }
    cout << "Data " << jenis << " berhasil diubah." << endl;
}

void hapusPerlengkapan(const string &jenis)
{
    int indeks = cariIndeks(jenis);
    if (indeks < 0)
    {
        return;
    }

    for (int i = indeks; i < totalPerlengkapan - 1; i++)
    {
        daftarPerlengkapan[i] = daftarPerlengkapan[i + 1];
    }
    totalPerlengkapan--;
    cout << "Data " << jenis << " berhasil dihapus." << endl;
}

void menuKategori(int pilihanJenis)
{
    const string jenis = namaJenis(pilihanJenis);
    int pilihan;
    do
    {
        cout << "\n============================" << endl;
        cout << "       DATA " << jenis << endl;
        cout << "============================" << endl;
        cout << "1. Tambah " << jenis << endl;
        cout << "2. Lihat " << jenis << endl;
        cout << "3. Ubah " << jenis << endl;
        cout << "4. Hapus " << jenis << endl;
        cout << "5. Kembali ke menu perlengkapan" << endl;

        pilihan = inputAngka("Pilih: ");
        switch (pilihan)
        {
        case 1:
            tambahPerlengkapan(jenis);
            break;
        case 2:
            tampilkanPerlengkapan(jenis);
            break;
        case 3:
            ubahPerlengkapan(jenis);
            break;
        case 4:
            hapusPerlengkapan(jenis);
            break;
        case 5:
            break;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 5);
}

void menuPerlengkapan()
{
    int pilihan;
    do
    {
        cout << "\n============================" << endl;
        cout << "       DATA PERLENGKAPAN" << endl;
        cout << "============================" << endl;
        cout << "1. Jersey (harga/jam, jumlah paketan/satuan)" << endl;
        cout << "2. Bola (jumlah, harga sewa/jam)" << endl;
        cout << "3. Rompi (harga/jam, jumlah paketan/satuan)" << endl;
        cout << "4. Kembali ke Menu Utama" << endl;

        pilihan = inputAngka("Pilih: ");
        if (pilihan >= 1 && pilihan <= 3)
        {
            menuKategori(pilihan);
        }
        else if (pilihan == 4)
        {
            cout << "Kembali ke menu utama." << endl;
        }
        else
        {
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 4);
}
