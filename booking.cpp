#include "booking.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

// ==================================================
// STRUKTUR DATA MEMBER
// Format member.txt: id|nama|no_wa
// ==================================================

struct DataMember
{
    int id_member;
    string nama_member;
    string no_wa;
};

// ==================================================
// STRUKTUR DATA LAPANGAN
// Format lapangan.txt: id|nama|jenis|harga
// ==================================================

struct DataLapangan
{
    int id_lapangan;
    string nama_lapangan;
    string jenis_lapangan;
    double harga;
};

struct ItemInventaris
{
    int id;
    string jenis;
    string pilihan;
    int jumlah;
    double harga;
};

struct DetailPerlengkapanBooking
{
    int id_booking;
    int id_perlengkapan;
    string jenis;
    string pilihan;
    int jumlah;
    double harga_per_jam;
};

struct PembayaranBooking
{
    int id_transaksi;
    int id_booking;
    string jenis_pembayaran;
    string metode;
    double nominal_tagihan;
    double jumlah_dibayar;
    double kembalian;
    string waktu;
};

// ==================================================
// KONVERSI STRING KE INTEGER
// ==================================================

bool konversiInt(string data, int &hasil)
{
    stringstream ss(data);
    char sisa;

    if (!(ss >> hasil))
    {
        return false;
    }

    if (ss >> sisa)
    {
        return false;
    }

    return true;
}

// ==================================================
// KONVERSI STRING KE DOUBLE
// ==================================================

bool konversiDouble(string data, double &hasil)
{
    stringstream ss(data);
    char sisa;

    if (!(ss >> hasil))
    {
        return false;
    }

    if (ss >> sisa)
    {
        return false;
    }

    return true;
}

// ==================================================
// MEMBACA DATA MEMBER
// ==================================================

vector<DataMember> bacaDataMember()
{
    vector<DataMember> daftar;

    ifstream file("member.txt");

    if (!file.is_open())
    {
        return daftar;
    }

    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }

        stringstream ss(baris);

        string id;
        string nama;
        string no_wa;

        getline(ss, id, '|');
        getline(ss, nama, '|');
        getline(ss, no_wa, '|');

        DataMember m;

        if (!konversiInt(id, m.id_member))
        {
            continue;
        }

        m.nama_member = nama;
        m.no_wa = no_wa;

        daftar.push_back(m);
    }

    file.close();

    return daftar;
}

// ==================================================
// MEMBACA DATA LAPANGAN
// ==================================================

vector<DataLapangan> bacaDataLapangan()
{
    vector<DataLapangan> daftar;

    ifstream file("lapangan.txt");

    if (!file.is_open())
    {
        return daftar;
    }

    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }

        stringstream ss(baris);

        string id;
        string nama;
        string jenis;
        string harga;

        getline(ss, id, '|');
        getline(ss, nama, '|');
        getline(ss, jenis, '|');
        getline(ss, harga, '|');

        DataLapangan l;

        if (!konversiInt(id, l.id_lapangan))
        {
            continue;
        }

        if (!konversiDouble(harga, l.harga))
        {
            continue;
        }

        l.nama_lapangan = nama;
        l.jenis_lapangan = jenis;

        daftar.push_back(l);
    }

    file.close();

    return daftar;
}

// ==================================================
// MEMBACA DATA BOOKING
// ==================================================

vector<Booking> bacaBooking()
{
    vector<Booking> daftar;

    ifstream file("booking.txt");

    if (!file.is_open())
    {
        return daftar;
    }

    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }

        stringstream ss(baris);

        string data;
        Booking b;

        getline(ss, data, '|');
        if (!konversiInt(data, b.id_booking))
            continue;

        getline(ss, data, '|');
        if (!konversiInt(data, b.id_member))
            continue;

        getline(ss, b.nama_member, '|');

        getline(ss, data, '|');
        if (!konversiInt(data, b.id_lapangan))
            continue;

        getline(ss, b.nama_lapangan, '|');
        getline(ss, b.tanggal_booking, '|');
        getline(ss, b.jam_mulai, '|');

        getline(ss, data, '|');
        if (!konversiInt(data, b.durasi))
            continue;

        getline(ss, data, '|');
        if (!konversiDouble(data, b.total_bayar))
            continue;

        daftar.push_back(b);
    }

    file.close();

    return daftar;
}

// ==================================================
// MENYIMPAN DATA BOOKING
// ==================================================

bool simpanBooking(const vector<Booking> &daftar)
{
    ofstream file("booking.txt");

    if (!file.is_open())
    {
        cout << "Gagal membuka file booking.txt!\n";
        return false;
    }

    file << fixed << setprecision(2);
    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id_booking << "|"
             << daftar[i].id_member << "|"
             << daftar[i].nama_member << "|"
             << daftar[i].id_lapangan << "|"
             << daftar[i].nama_lapangan << "|"
             << daftar[i].tanggal_booking << "|"
             << daftar[i].jam_mulai << "|"
             << daftar[i].durasi << "|"
             << daftar[i].total_bayar << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan file booking.txt!\n";
        return false;
    }
    return true;
}

static vector<ItemInventaris> bacaInventarisPerlengkapan()
{
    vector<ItemInventaris> daftar;
    ifstream file("perlengkapan.txt");
    string baris;

    while (getline(file, baris))
    {
        stringstream ss(baris);
        string id;
        string jumlah;
        string harga;
        ItemInventaris item;

        getline(ss, id, '|');
        getline(ss, item.jenis, '|');
        getline(ss, item.pilihan, '|');
        getline(ss, jumlah, '|');
        getline(ss, harga, '|');

        if (konversiInt(id, item.id) &&
            konversiInt(jumlah, item.jumlah) &&
            konversiDouble(harga, item.harga) &&
            item.jumlah > 0 && item.harga >= 0)
        {
            daftar.push_back(item);
        }
    }

    return daftar;
}

static vector<DetailPerlengkapanBooking> bacaRincianPerlengkapanBooking()
{
    vector<DetailPerlengkapanBooking> daftar;
    ifstream file("booking_perlengkapan.txt");
    string baris;

    while (getline(file, baris))
    {
        stringstream ss(baris);
        string idBooking;
        string idPerlengkapan;
        string jumlah;
        string harga;
        DetailPerlengkapanBooking item;

        getline(ss, idBooking, '|');
        getline(ss, idPerlengkapan, '|');
        getline(ss, item.jenis, '|');
        getline(ss, item.pilihan, '|');
        getline(ss, jumlah, '|');
        getline(ss, harga, '|');

        if (konversiInt(idBooking, item.id_booking) &&
            konversiInt(idPerlengkapan, item.id_perlengkapan) &&
            konversiInt(jumlah, item.jumlah) &&
            konversiDouble(harga, item.harga_per_jam))
        {
            daftar.push_back(item);
        }
    }

    return daftar;
}

static bool simpanRincianPerlengkapanBooking(
    const vector<DetailPerlengkapanBooking> &daftar)
{
    ofstream file("booking_perlengkapan.txt");
    if (!file.is_open())
    {
        cout << "Gagal membuka file booking_perlengkapan.txt!\n";
        return false;
    }

    file << fixed << setprecision(2);
    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id_booking << "|"
             << daftar[i].id_perlengkapan << "|"
             << daftar[i].jenis << "|"
             << daftar[i].pilihan << "|"
             << daftar[i].jumlah << "|"
             << daftar[i].harga_per_jam << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan rincian perlengkapan booking!\n";
        return false;
    }
    return true;
}

static vector<DetailPerlengkapanBooking> rincianUntukBooking(int idBooking)
{
    vector<DetailPerlengkapanBooking> semua =
        bacaRincianPerlengkapanBooking();
    vector<DetailPerlengkapanBooking> hasil;

    for (size_t i = 0; i < semua.size(); i++)
    {
        if (semua[i].id_booking == idBooking)
        {
            hasil.push_back(semua[i]);
        }
    }
    return hasil;
}

static double totalPerlengkapanBooking(int idBooking, int durasi)
{
    vector<DetailPerlengkapanBooking> rincian =
        rincianUntukBooking(idBooking);
    double total = 0;
    for (size_t i = 0; i < rincian.size(); i++)
    {
        total += rincian[i].harga_per_jam * rincian[i].jumlah * durasi;
    }
    return total;
}

static vector<PembayaranBooking> bacaTransaksiBooking()
{
    vector<PembayaranBooking> daftar;
    ifstream file("transaksi.txt");
    string baris;

    while (getline(file, baris))
    {
        stringstream ss(baris);
        string idTransaksi;
        string idBooking;
        string nominalTagihan;
        string jumlahDibayar;
        string kembalian;
        PembayaranBooking pembayaran;

        getline(ss, idTransaksi, '|');
        getline(ss, idBooking, '|');
        getline(ss, pembayaran.jenis_pembayaran, '|');
        getline(ss, pembayaran.metode, '|');
        getline(ss, nominalTagihan, '|');
        getline(ss, jumlahDibayar, '|');
        getline(ss, kembalian, '|');
        getline(ss, pembayaran.waktu, '|');

        if (konversiInt(idTransaksi, pembayaran.id_transaksi) &&
            konversiInt(idBooking, pembayaran.id_booking) &&
            konversiDouble(nominalTagihan, pembayaran.nominal_tagihan) &&
            konversiDouble(jumlahDibayar, pembayaran.jumlah_dibayar) &&
            konversiDouble(kembalian, pembayaran.kembalian))
        {
            daftar.push_back(pembayaran);
        }
    }

    return daftar;
}

static bool simpanTransaksiBooking(const vector<PembayaranBooking> &daftar)
{
    ofstream file("transaksi.txt");
    if (!file.is_open())
    {
        cout << "Gagal membuka file transaksi.txt!\n";
        return false;
    }

    file << fixed << setprecision(2);
    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id_transaksi << "|"
             << daftar[i].id_booking << "|"
             << daftar[i].jenis_pembayaran << "|"
             << daftar[i].metode << "|"
             << daftar[i].nominal_tagihan << "|"
             << daftar[i].jumlah_dibayar << "|"
             << daftar[i].kembalian << "|"
             << daftar[i].waktu << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan transaksi!\n";
        return false;
    }
    return true;
}

static double jumlahTerbayar(int idBooking)
{
    vector<PembayaranBooking> daftar = bacaTransaksiBooking();
    double total = 0;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking == idBooking)
        {
            total += daftar[i].nominal_tagihan;
        }
    }
    return total;
}

static int idTransaksiBerikutnya(
    const vector<PembayaranBooking> &daftar)
{
    int terbesar = 0;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_transaksi > terbesar)
        {
            terbesar = daftar[i].id_transaksi;
        }
    }
    return terbesar + 1;
}

// ==================================================
// ID BOOKING BERIKUTNYA
// ==================================================

int idBookingBerikutnya()
{
    vector<Booking> daftar = bacaBooking();

    int id_terbesar = 0;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking > id_terbesar)
        {
            id_terbesar = daftar[i].id_booking;
        }
    }

    return id_terbesar + 1;
}

// ==================================================
// MENAMPILKAN DAFTAR MEMBER
// ==================================================

void tampilkanMember()
{
    vector<DataMember> daftar = bacaDataMember();

    cout << "\n============================================\n";
    cout << "                DAFTAR MEMBER\n";
    cout << "============================================\n";

    if (daftar.empty())
    {
        cout << "Belum ada member terdaftar.\n";
        return;
    }

    cout << left
         << setw(10) << "ID"
         << setw(25) << "Nama Member"
         << setw(18) << "No. WhatsApp"
         << endl;

    cout << "--------------------------------------------\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        cout << left
             << setw(10) << daftar[i].id_member
             << setw(25) << daftar[i].nama_member
             << setw(18) << daftar[i].no_wa
             << endl;
    }
}

// ==================================================
// MENAMPILKAN DAFTAR LAPANGAN
// ==================================================

void tampilkanLapangan()
{
    vector<DataLapangan> daftar = bacaDataLapangan();

    cout << "\n============================================\n";
    cout << "               DAFTAR LAPANGAN\n";
    cout << "============================================\n";

    if (daftar.empty())
    {
        cout << "Data lapangan belum tersedia.\n";
        return;
    }

    cout << left
         << setw(10) << "ID"
         << setw(22) << "Nama Lapangan"
         << setw(15) << "Jenis"
         << setw(15) << "Harga/Jam"
         << endl;

    cout << "--------------------------------------------\n";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        cout << left
             << setw(10) << daftar[i].id_lapangan
             << setw(22) << daftar[i].nama_lapangan
             << setw(15) << daftar[i].jenis_lapangan
             << fixed << setprecision(0)
             << setw(15) << daftar[i].harga
             << endl;
    }
}

// ==================================================
// MENCARI MEMBER BERDASARKAN ID
// ==================================================

bool cariMember(int id, DataMember &hasil)
{
    vector<DataMember> daftar = bacaDataMember();

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

// ==================================================
// MENCARI LAPANGAN BERDASARKAN ID
// ==================================================

bool cariLapangan(int id, DataLapangan &hasil)
{
    vector<DataLapangan> daftar = bacaDataLapangan();

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_lapangan == id)
        {
            hasil = daftar[i];
            return true;
        }
    }

    return false;
}

// ==================================================
// KONVERSI JAM KE MENIT
// Format HH:MM
// ==================================================

int jamKeMenit(string jam)
{
    int h, m;
    char titikDua;

    stringstream ss(jam);

    if (!(ss >> h >> titikDua >> m))
    {
        return -1;
    }

    if (titikDua != ':' || h < 0 || h > 23 ||
        m < 0 || m > 59)
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

// ==================================================
// CEK BENTROK JADWAL
// ==================================================

bool cekJadwal(
    int id_lapangan,
    string tanggal,
    string jam,
    int durasi,
    int id_dikecualikan = -1)
{
    vector<Booking> daftar = bacaBooking();

    int mulai_baru = jamKeMenit(jam);

    if (mulai_baru < 0 || durasi <= 0)
    {
        return true;
    }

    int selesai_baru = mulai_baru + durasi * 60;

    if (selesai_baru > 1440)
    {
        return true;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        Booking b = daftar[i];

        if (b.id_booking == id_dikecualikan)
        {
            continue;
        }

        if (b.id_lapangan == id_lapangan &&
            b.tanggal_booking == tanggal)
        {

            int mulai_lama = jamKeMenit(b.jam_mulai);

            if (mulai_lama < 0)
            {
                continue;
            }

            int selesai_lama = mulai_lama + b.durasi * 60;

            if (mulai_baru < selesai_lama &&
                selesai_baru > mulai_lama)
            {
                return true;
            }
        }
    }

    return false;
}

static bool pilihPerlengkapanBooking(
    int idBooking,
    int durasi,
    vector<DetailPerlengkapanBooking> &pilihan)
{
    vector<ItemInventaris> inventaris = bacaInventarisPerlengkapan();
    if (inventaris.empty())
    {
        cout << "\nInventaris perlengkapan kosong. Booking dilanjutkan tanpa "
                "perlengkapan.\n";
        return true;
    }

    cout << "\n========== PERLENGKAPAN TERSEDIA ==========\n";
    cout << left << setw(7) << "ID"
         << setw(12) << "Jenis"
         << setw(16) << "Pilihan"
         << setw(12) << "Tersedia"
         << "Tarif/Jam" << endl;
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < inventaris.size(); i++)
    {
        cout << left << setw(7) << inventaris[i].id
             << setw(12) << inventaris[i].jenis
             << setw(16) << inventaris[i].pilihan
             << setw(12) << inventaris[i].jumlah
             << "Rp" << fixed << setprecision(0)
             << inventaris[i].harga << endl;
    }
    cout << "Masukkan ID 0 jika tidak ingin menambah perlengkapan." << endl;

    while (true)
    {
        int idPerlengkapan;
        cout << "ID perlengkapan: ";
        if (!(cin >> idPerlengkapan))
        {
            cout << "ID perlengkapan harus berupa angka.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return false;
        }
        if (idPerlengkapan == 0)
        {
            return true;
        }

        int indeks = -1;
        for (size_t i = 0; i < inventaris.size(); i++)
        {
            if (inventaris[i].id == idPerlengkapan)
            {
                indeks = static_cast<int>(i);
                break;
            }
        }
        if (indeks < 0)
        {
            cout << "ID perlengkapan tidak ditemukan.\n";
            continue;
        }

        bool sudahDipilih = false;
        for (size_t i = 0; i < pilihan.size(); i++)
        {
            if (pilihan[i].id_perlengkapan == idPerlengkapan)
            {
                sudahDipilih = true;
                break;
            }
        }
        if (sudahDipilih)
        {
            cout << "Perlengkapan ini sudah dipilih untuk booking ini.\n";
            continue;
        }

        int jumlah;
        cout << "Jumlah yang disewa (tersedia "
             << inventaris[indeks].jumlah << "): ";
        if (!(cin >> jumlah) || jumlah <= 0 ||
            jumlah > inventaris[indeks].jumlah)
        {
            cout << "Jumlah tidak valid atau melebihi stok tersedia.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        DetailPerlengkapanBooking detail;
        detail.id_booking = idBooking;
        detail.id_perlengkapan = inventaris[indeks].id;
        detail.jenis = inventaris[indeks].jenis;
        detail.pilihan = inventaris[indeks].pilihan;
        detail.jumlah = jumlah;
        detail.harga_per_jam = inventaris[indeks].harga;
        pilihan.push_back(detail);

        double subtotal =
            detail.harga_per_jam * detail.jumlah * durasi;
        cout << "Biaya perlengkapan: Rp "
             << fixed << setprecision(0) << subtotal << endl;
    }
}

// ==================================================
// TAMBAH BOOKING
// KHUSUS MEMBER TERDAFTAR
// ==================================================

void tambahBooking()
{
    vector<DataMember> member = bacaDataMember();
    vector<DataLapangan> lapangan = bacaDataLapangan();

    if (member.empty())
    {
        cout << "\nBelum ada member terdaftar.\n";
        cout << "Silakan daftarkan pelanggan terlebih dahulu.\n";
        return;
    }

    if (lapangan.empty())
    {
        cout << "\nData lapangan belum tersedia.\n";
        return;
    }

    Booking b;
    DataMember m;
    DataLapangan l;

    cout << "\n============================================\n";
    cout << "                 TAMBAH BOOKING\n";
    cout << "============================================\n";

    tampilkanMember();

    cout << "\nMasukkan ID Member: ";

    if (!(cin >> b.id_member))
    {
        cout << "ID member harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (!cariMember(b.id_member, m))
    {
        cout << "\nBooking ditolak!\n";
        cout << "ID member tidak ditemukan.\n";
        cout << "Pelanggan harus terdaftar sebagai member.\n";
        return;
    }

    b.nama_member = m.nama_member;

    cout << "\nMember ditemukan!\n";
    cout << "Nama Member : " << m.nama_member << endl;
    cout << "No. WhatsApp: " << m.no_wa << endl;

    tampilkanLapangan();

    cout << "\nMasukkan ID Lapangan: ";

    if (!(cin >> b.id_lapangan))
    {
        cout << "ID lapangan harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (!cariLapangan(b.id_lapangan, l))
    {
        cout << "ID lapangan tidak ditemukan!\n";
        return;
    }

    b.nama_lapangan = l.nama_lapangan;

    cout << "Tanggal Booking (DD-MM-YYYY): ";
    cin >> b.tanggal_booking;

    cout << "Jam Mulai (HH:MM): ";
    cin >> b.jam_mulai;

    if (jamKeMenit(b.jam_mulai) < 0)
    {
        cout << "Format jam tidak valid!\n";
        return;
    }

    cout << "Durasi Booking (jam): ";

    if (!(cin >> b.durasi) || b.durasi <= 0)
    {
        cout << "Durasi harus berupa angka lebih dari 0!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (jamKeMenit(b.jam_mulai) + b.durasi * 60 > 1440)
    {
        cout << "Jam selesai tidak boleh melewati pukul 24:00!\n";
        return;
    }

    if (cekJadwal(
            b.id_lapangan,
            b.tanggal_booking,
            b.jam_mulai,
            b.durasi))
    {
        cout << "\nBooking gagal!\n";
        cout << "Jadwal lapangan sudah digunakan atau tidak valid.\n";
        return;
    }

    b.id_booking = idBookingBerikutnya();

    vector<DetailPerlengkapanBooking> perlengkapanDipilih;
    if (!pilihPerlengkapanBooking(
            b.id_booking, b.durasi, perlengkapanDipilih))
    {
        cout << "Booking dibatalkan." << endl;
        return;
    }

    b.total_bayar = l.harga * b.durasi;
    for (size_t i = 0; i < perlengkapanDipilih.size(); i++)
    {
        b.total_bayar += perlengkapanDipilih[i].harga_per_jam *
                         perlengkapanDipilih[i].jumlah * b.durasi;
    }

    vector<Booking> daftar = bacaBooking();
    vector<DetailPerlengkapanBooking> semuaRincian =
        bacaRincianPerlengkapanBooking();
    vector<DetailPerlengkapanBooking> rincianLama = semuaRincian;
    semuaRincian.insert(
        semuaRincian.end(),
        perlengkapanDipilih.begin(),
        perlengkapanDipilih.end());

    if (!simpanRincianPerlengkapanBooking(semuaRincian))
    {
        return;
    }

    daftar.push_back(b);
    if (!simpanBooking(daftar))
    {
        if (!simpanRincianPerlengkapanBooking(rincianLama))
        {
            cout << "Peringatan: rincian perlengkapan booking perlu diperiksa."
                 << endl;
        }
        return;
    }

    cout << "\n============================================\n";
    cout << "          BOOKING BERHASIL DITAMBAHKAN\n";
    cout << "============================================\n";

    cout << "ID Booking  : " << b.id_booking << endl;
    cout << "ID Member   : " << b.id_member << endl;
    cout << "Nama Member : " << b.nama_member << endl;
    cout << "Lapangan    : " << b.nama_lapangan << endl;
    cout << "Tanggal     : " << b.tanggal_booking << endl;
    cout << "Jam Mulai   : " << b.jam_mulai << endl;
    cout << "Durasi      : " << b.durasi << " jam\n";

    cout << "Total Bayar : Rp "
         << fixed << setprecision(0)
         << b.total_bayar << endl;
    cout << "DP 40%      : Rp "
         << ceil(b.total_bayar * 0.40) << endl;
    cout << "Silakan buka menu Transaksi untuk membayar DP atau melunasi.\n";
}

// ==================================================
// LIHAT DATA BOOKING
// ==================================================

void lihatBooking()
{
    vector<Booking> daftar = bacaBooking();

    cout << "\n============================================\n";
    cout << "                 DATA BOOKING\n";
    cout << "============================================\n";

    if (daftar.empty())
    {
        cout << "Belum ada data booking.\n";
        return;
    }

    cout << left
         << setw(7) << "ID"
         << setw(8) << "Member"
         << setw(18) << "Nama Member"
         << setw(18) << "Lapangan"
         << setw(14) << "Tanggal"
         << setw(10) << "Jam"
         << setw(8) << "Durasi"
         << setw(14) << "Total"
         << setw(14) << "DP 40%"
         << setw(14) << "Dibayar"
         << "Sisa"
         << endl;

    cout << string(145, '-') << endl;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        const double dibayar = jumlahTerbayar(daftar[i].id_booking);
        const double sisa = daftar[i].total_bayar - dibayar;
        cout << left
             << setw(7) << daftar[i].id_booking
             << setw(8) << daftar[i].id_member
             << setw(18) << daftar[i].nama_member
             << setw(18) << daftar[i].nama_lapangan
             << setw(14) << daftar[i].tanggal_booking
             << setw(10) << daftar[i].jam_mulai
             << setw(8) << daftar[i].durasi
             << setw(14)
             << fixed << setprecision(0)
             << daftar[i].total_bayar
             << setw(14) << ceil(daftar[i].total_bayar * 0.40)
             << setw(14) << dibayar
             << (sisa > 0 ? sisa : 0)
             << endl;
    }

    cout << string(145, '-') << endl;
}

// ==================================================
// EDIT DATA BOOKING
// ==================================================

void editBooking()
{
    vector<Booking> daftar = bacaBooking();

    if (daftar.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    int id;

    cout << "\nMasukkan ID Booking yang akan diedit: ";

    if (!(cin >> id))
    {
        cout << "ID booking harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int posisi = -1;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking == id)
        {
            posisi = static_cast<int>(i);
            break;
        }
    }

    if (posisi == -1)
    {
        cout << "Data booking tidak ditemukan!\n";
        return;
    }

    Booking &b = daftar[posisi];

    cout << "\nData booking ditemukan.\n";
    cout << "Nama Member : " << b.nama_member << endl;
    cout << "Lapangan    : " << b.nama_lapangan << endl;
    if (jumlahTerbayar(b.id_booking) > 0)
    {
        cout << "Booking sudah menerima pembayaran dan tidak dapat diubah.\n";
        return;
    }

    cout << "\nPilih data yang akan diubah:\n";
    cout << "1. Ganti Member\n";
    cout << "2. Ganti Lapangan\n";
    cout << "3. Ganti Tanggal\n";
    cout << "4. Ganti Jam dan Durasi\n";
    cout << "5. Kembali\n";
    cout << "Pilihan: ";

    int pilihan;

    if (!(cin >> pilihan))
    {
        cout << "Pilihan harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    DataMember m;
    DataLapangan l;

    switch (pilihan)
    {
    case 1:
    {
        tampilkanMember();

        int id_member;

        cout << "\nMasukkan ID Member baru: ";

        if (!(cin >> id_member))
        {
            cout << "ID member harus berupa angka!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (!cariMember(id_member, m))
        {
            cout << "Member tidak ditemukan. Perubahan dibatalkan.\n";
            return;
        }

        b.id_member = m.id_member;
        b.nama_member = m.nama_member;

        break;
    }

    case 2:
    {
        tampilkanLapangan();

        int id_lapangan;

        cout << "\nMasukkan ID Lapangan baru: ";

        if (!(cin >> id_lapangan))
        {
            cout << "ID lapangan harus berupa angka!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (!cariLapangan(id_lapangan, l))
        {
            cout << "Lapangan tidak ditemukan.\n";
            return;
        }

        if (cekJadwal(
                id_lapangan,
                b.tanggal_booking,
                b.jam_mulai,
                b.durasi,
                b.id_booking))
        {
            cout << "Jadwal lapangan tersebut sudah digunakan.\n";
            return;
        }

        b.id_lapangan = l.id_lapangan;
        b.nama_lapangan = l.nama_lapangan;
        b.total_bayar = l.harga * b.durasi +
                        totalPerlengkapanBooking(b.id_booking, b.durasi);

        break;
    }

    case 3:
    {
        string tanggal;

        cout << "Tanggal baru (DD-MM-YYYY): ";
        cin >> tanggal;

        if (cekJadwal(
                b.id_lapangan,
                tanggal,
                b.jam_mulai,
                b.durasi,
                b.id_booking))
        {
            cout << "Jadwal tanggal tersebut sudah digunakan.\n";
            return;
        }

        b.tanggal_booking = tanggal;

        break;
    }

    case 4:
    {
        string jam;
        int durasi;

        cout << "Jam mulai baru (HH:MM): ";
        cin >> jam;

        if (jamKeMenit(jam) < 0)
        {
            cout << "Format jam tidak valid!\n";
            return;
        }

        cout << "Durasi baru (jam): ";

        if (!(cin >> durasi) || durasi <= 0)
        {
            cout << "Durasi tidak valid!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (jamKeMenit(jam) + durasi * 60 > 1440)
        {
            cout << "Jam selesai melewati pukul 24:00!\n";
            return;
        }

        if (cekJadwal(
                b.id_lapangan,
                b.tanggal_booking,
                jam,
                durasi,
                b.id_booking))
        {
            cout << "Jadwal tersebut sudah digunakan.\n";
            return;
        }

        if (!cariLapangan(b.id_lapangan, l))
        {
            cout << "Data lapangan tidak ditemukan.\n";
            return;
        }

        b.jam_mulai = jam;
        b.durasi = durasi;
        b.total_bayar = l.harga * durasi +
                        totalPerlengkapanBooking(b.id_booking, durasi);

        break;
    }

    case 5:
        return;

    default:
        cout << "Pilihan tidak tersedia!\n";
        return;
    }

    if (simpanBooking(daftar))
    {
        cout << "\nData booking berhasil diperbarui!\n";
    }
}

// ==================================================
// HAPUS DATA BOOKING
// ==================================================

void hapusBooking()
{
    vector<Booking> daftar = bacaBooking();

    if (daftar.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    int id;

    cout << "\nMasukkan ID Booking yang akan dihapus: ";

    if (!(cin >> id))
    {
        cout << "ID booking harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int posisi = -1;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking == id)
        {
            posisi = static_cast<int>(i);
            break;
        }
    }

    if (posisi == -1)
    {
        cout << "Data booking tidak ditemukan!\n";
        return;
    }

    if (jumlahTerbayar(id) > 0)
    {
        cout << "Booking sudah menerima pembayaran dan tidak dapat dihapus.\n";
        return;
    }

    char konfirmasi;

    cout << "Yakin ingin menghapus booking ID "
         << id << "? (Y/T): ";

    cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y')
    {
        vector<DetailPerlengkapanBooking> semuaRincian =
            bacaRincianPerlengkapanBooking();
        vector<DetailPerlengkapanBooking> rincianLama = semuaRincian;
        vector<DetailPerlengkapanBooking> rincianBaru;
        for (size_t i = 0; i < semuaRincian.size(); i++)
        {
            if (semuaRincian[i].id_booking != id)
            {
                rincianBaru.push_back(semuaRincian[i]);
            }
        }

        if (!simpanRincianPerlengkapanBooking(rincianBaru))
        {
            return;
        }

        daftar.erase(daftar.begin() + posisi);
        if (!simpanBooking(daftar))
        {
            if (!simpanRincianPerlengkapanBooking(rincianLama))
            {
                cout << "Peringatan: rincian perlengkapan perlu diperiksa.\n";
            }
            return;
        }
        cout << "Data booking berhasil dihapus!\n";
    }
    else
    {
        cout << "Penghapusan dibatalkan.\n";
    }
}

static string escapeHtmlBooking(const string &teks)
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

static string waktuSekarang()
{
    time_t sekarang = time(0);
    tm *waktu = localtime(&sekarang);
    char teks[32];
    if (waktu == 0)
    {
        return "Tidak tersedia";
    }
    strftime(teks, sizeof(teks), "%d-%m-%Y %H:%M:%S", waktu);
    return teks;
}

static bool cetakInvoiceBooking(const Booking &booking)
{
    const double totalTerbayar = jumlahTerbayar(booking.id_booking);
    if (totalTerbayar + 0.000001 < booking.total_bayar)
    {
        cout << "Invoice final belum dapat dibuat karena pembayaran belum lunas.\n";
        return false;
    }

    vector<DetailPerlengkapanBooking> perlengkapan =
        rincianUntukBooking(booking.id_booking);
    vector<PembayaranBooking> pembayaran = bacaTransaksiBooking();
    DataMember member;
    const bool memberDitemukan = cariMember(booking.id_member, member);
    const double dpWajib = ceil(booking.total_bayar * 0.40);
    double totalPerlengkapan = 0;
    for (size_t i = 0; i < perlengkapan.size(); i++)
    {
        totalPerlengkapan += perlengkapan[i].harga_per_jam *
                             perlengkapan[i].jumlah * booking.durasi;
    }
    double totalLapangan = booking.total_bayar - totalPerlengkapan;
    if (totalLapangan < 0)
    {
        totalLapangan = 0;
    }

    ostringstream namaFile;
    namaFile << "invoice_booking_" << booking.id_booking << ".html";
    ofstream file(namaFile.str().c_str());
    if (!file.is_open())
    {
        cout << "Gagal membuat file invoice!\n";
        return false;
    }

    file << "<!DOCTYPE html><html lang='id'><head><meta charset='UTF-8'>"
         << "<title>Invoice Booking "
         << booking.id_booking
         << "</title><style>"
         << "body{font-family:Arial,sans-serif;margin:35px;color:#333;}"
         << "h1,h2{text-align:center;}"
         << "table{width:100%;border-collapse:collapse;margin:15px 0;}"
         << "th,td{border:1px solid #555;padding:8px;text-align:left;}"
         << "th{background:#e5e5e5;}"
         << ".right{text-align:right;}"
         << ".paid{color:#198754;font-weight:bold;}"
         << "button{padding:10px 20px;background:#198754;color:white;"
            "border:0;border-radius:5px;margin-bottom:15px;}"
         << "@media print{button{display:none;}body{margin:15px;}}"
         << "</style></head><body>"
         << "<button onclick='window.print()'>Cetak Invoice</button>"
         << "<h1>INVOICE PEMBAYARAN FUTSAL</h1>"
         << "<p><b>No. Invoice:</b> INV-" << booking.id_booking << "</p>"
         << "<p><b>Tanggal cetak:</b> " << escapeHtmlBooking(waktuSekarang())
         << "</p>"
         << "<h2>Detail Member</h2><table>"
         << "<tr><th>ID Member</th><td>" << booking.id_member << "</td></tr>"
         << "<tr><th>Nama Member</th><td>"
         << escapeHtmlBooking(booking.nama_member) << "</td></tr>";
    if (memberDitemukan)
    {
        file << "<tr><th>No. WhatsApp</th><td>"
             << escapeHtmlBooking(member.no_wa) << "</td></tr>";
    }
    file << "</table>"
         << "<h2>Detail Booking</h2><table>"
         << "<tr><th>ID Booking</th><td>" << booking.id_booking << "</td></tr>"
         << "<tr><th>Lapangan</th><td>"
         << escapeHtmlBooking(booking.nama_lapangan)
         << " (ID " << booking.id_lapangan << ")</td></tr>"
         << "<tr><th>Tanggal</th><td>"
         << escapeHtmlBooking(booking.tanggal_booking) << "</td></tr>"
         << "<tr><th>Jam mulai</th><td>"
         << escapeHtmlBooking(booking.jam_mulai) << "</td></tr>"
         << "<tr><th>Durasi</th><td>" << booking.durasi << " jam</td></tr>"
         << "</table>"
         << "<h2>Rincian Biaya</h2><table>"
         << "<tr><th>Item</th><th>Jumlah/Durasi</th>"
            "<th>Tarif per jam</th><th>Total</th></tr>"
         << "<tr><td>Sewa lapangan</td><td>" << booking.durasi
         << " jam</td><td>Rp "
         << fixed << setprecision(0)
         << (booking.durasi > 0 ? totalLapangan / booking.durasi : 0)
         << "</td><td>Rp " << totalLapangan << "</td></tr>";

    for (size_t i = 0; i < perlengkapan.size(); i++)
    {
        const double subtotal = perlengkapan[i].harga_per_jam *
                                perlengkapan[i].jumlah * booking.durasi;
        string namaPilihan = perlengkapan[i].jenis + " - " +
                             perlengkapan[i].pilihan;
        if (perlengkapan[i].pilihan == "Paketan")
        {
            namaPilihan += " (6 orang/paket)";
        }
        file << "<tr><td>" << escapeHtmlBooking(namaPilihan)
             << "</td><td>" << perlengkapan[i].jumlah << " x "
             << booking.durasi << " jam</td><td>Rp "
             << perlengkapan[i].harga_per_jam
             << "</td><td>Rp " << subtotal << "</td></tr>";
    }

    file << "</table>"
         << "<h2>Ringkasan Pembayaran</h2><table>"
         << "<tr><th>Total biaya</th><td>Rp " << booking.total_bayar
         << "</td></tr>"
         << "<tr><th>DP wajib (40%)</th><td>Rp " << dpWajib
         << "</td></tr>"
         << "<tr><th>Total dibayar</th><td>Rp " << totalTerbayar
         << "</td></tr>"
         << "<tr><th>Status</th><td class='paid'>LUNAS</td></tr>"
         << "</table><h2>Riwayat Transaksi</h2><table>"
         << "<tr><th>Jenis</th><th>Metode</th><th>Tagihan</th>"
            "<th>Uang diterima</th><th>Kembalian</th><th>Waktu</th></tr>";
    for (size_t i = 0; i < pembayaran.size(); i++)
    {
        if (pembayaran[i].id_booking != booking.id_booking)
        {
            continue;
        }
        file << "<tr><td>"
             << escapeHtmlBooking(pembayaran[i].jenis_pembayaran)
             << "</td><td>" << escapeHtmlBooking(pembayaran[i].metode)
             << "</td><td>Rp " << pembayaran[i].nominal_tagihan
             << "</td><td>Rp " << pembayaran[i].jumlah_dibayar
             << "</td><td>Rp " << pembayaran[i].kembalian
             << "</td><td>" << escapeHtmlBooking(pembayaran[i].waktu)
             << "</td></tr>";
    }
    file << "</table><p>Terima kasih telah melakukan pemesanan.</p>"
         << "</body></html>";
    file.close();

    if (!file)
    {
        cout << "Gagal menyimpan invoice!\n";
        return false;
    }

    cout << "Invoice berhasil dibuat: " << namaFile.str() << endl;
    string perintah = "start \"\" \"" + namaFile.str() + "\"";
    system(perintah.c_str());
    return true;
}

static bool inputNominalPembayaran(double &nominal)
{
    while (true)
    {
        cout << "Jumlah uang yang dibayarkan (Rp): ";
        if (cin >> nominal)
        {
            if (isfinite(nominal) && nominal > 0)
            {
                return true;
            }
            cout << "Nominal harus lebih dari 0 dan berupa angka valid.\n";
            continue;
        }
        if (cin.eof())
        {
            return false;
        }
        cout << "Nominal harus berupa angka.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static void prosesTransaksi()
{
    vector<Booking> daftarBooking = bacaBooking();
    if (daftarBooking.empty())
    {
        cout << "Belum ada booking untuk dibayar.\n";
        return;
    }

    lihatBooking();
    int idBooking;
    cout << "Masukkan ID booking: ";
    if (!(cin >> idBooking))
    {
        cout << "ID booking harus berupa angka.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int indeks = -1;
    for (size_t i = 0; i < daftarBooking.size(); i++)
    {
        if (daftarBooking[i].id_booking == idBooking)
        {
            indeks = static_cast<int>(i);
            break;
        }
    }
    if (indeks < 0)
    {
        cout << "ID booking tidak ditemukan.\n";
        return;
    }

    const Booking &booking = daftarBooking[indeks];
    const double sudahDibayar = jumlahTerbayar(idBooking);
    const double dpWajib = ceil(booking.total_bayar * 0.40);
    const double sisaTagihan = booking.total_bayar - sudahDibayar;
    if (sisaTagihan <= 0.000001)
    {
        cout << "Booking sudah lunas.\n";
        cetakInvoiceBooking(booking);
        return;
    }

    cout << fixed << setprecision(0);
    cout << "Total biaya       : Rp " << booking.total_bayar << endl;
    cout << "DP minimal (40%)  : Rp " << dpWajib << endl;
    cout << "Sudah dibayar     : Rp " << sudahDibayar << endl;
    cout << "Sisa tagihan      : Rp " << sisaTagihan << endl;

    int pilihanPembayaran;
    double nominalTagihan;
    string jenisPembayaran;
    if (sudahDibayar + 0.000001 < dpWajib)
    {
        cout << "\n1. Bayar DP 40% (Rp " << dpWajib - sudahDibayar << ")\n";
        cout << "2. Pelunasan (Rp " << sisaTagihan << ")\n";
        cout << "Pilih: ";
        if (!(cin >> pilihanPembayaran) ||
            (pilihanPembayaran != 1 && pilihanPembayaran != 2))
        {
            cout << "Pilihan pembayaran tidak valid.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if (pilihanPembayaran == 1)
        {
            nominalTagihan = dpWajib - sudahDibayar;
            jenisPembayaran = "DP 40%";
        }
        else
        {
            nominalTagihan = sisaTagihan;
            jenisPembayaran = "Pelunasan";
        }
    }
    else
    {
        nominalTagihan = sisaTagihan;
        jenisPembayaran = "Pelunasan";
        cout << "Pembayaran berikutnya adalah pelunasan.\n";
    }

    cout << "\nMetode pembayaran:\n";
    cout << "1. Cash\n";
    cout << "2. QRIS\n";
    cout << "3. Transfer\n";
    int pilihanMetode;
    cout << "Pilih metode: ";
    if (!(cin >> pilihanMetode) ||
        pilihanMetode < 1 || pilihanMetode > 3)
    {
        cout << "Metode pembayaran tidak valid.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    string metode = pilihanMetode == 1 ? "Cash" :
                    pilihanMetode == 2 ? "QRIS" : "Transfer";
    double jumlahDibayar;
    if (!inputNominalPembayaran(jumlahDibayar))
    {
        return;
    }
    if (jumlahDibayar < nominalTagihan)
    {
        cout << "Pembayaran kurang. Tidak menerima bon; transaksi dibatalkan.\n";
        return;
    }

    vector<PembayaranBooking> transaksi = bacaTransaksiBooking();
    PembayaranBooking pembayaran;
    pembayaran.id_transaksi = idTransaksiBerikutnya(transaksi);
    pembayaran.id_booking = idBooking;
    pembayaran.jenis_pembayaran = jenisPembayaran;
    pembayaran.metode = metode;
    pembayaran.nominal_tagihan = nominalTagihan;
    pembayaran.jumlah_dibayar = jumlahDibayar;
    pembayaran.kembalian = jumlahDibayar - nominalTagihan;
    pembayaran.waktu = waktuSekarang();
    transaksi.push_back(pembayaran);

    if (!simpanTransaksiBooking(transaksi))
    {
        return;
    }

    const double totalTerbayarBaru = sudahDibayar + nominalTagihan;
    cout << "\nTransaksi berhasil.\n";
    cout << "Metode pembayaran : " << metode << endl;
    cout << "Tagihan dibayar   : Rp " << nominalTagihan << endl;
    cout << "Uang diterima     : Rp " << jumlahDibayar << endl;
    cout << "Kembalian         : Rp " << pembayaran.kembalian << endl;

    if (totalTerbayarBaru + 0.000001 >= booking.total_bayar)
    {
        cout << "Pembayaran lunas. Membuat invoice...\n";
        cetakInvoiceBooking(booking);
    }
    else
    {
        cout << "DP berhasil. Sisa tagihan: Rp "
             << booking.total_bayar - totalTerbayarBaru << endl;
        cout << "Invoice final dibuat setelah seluruh tagihan lunas.\n";
    }
}

static void lihatStatusTransaksi()
{
    vector<Booking> daftar = bacaBooking();
    if (daftar.empty())
    {
        cout << "Belum ada data booking.\n";
        return;
    }

    cout << left << setw(8) << "ID"
         << setw(18) << "Nama Member"
         << setw(16) << "Total"
         << setw(16) << "Dibayar"
         << "Sisa" << endl;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        const double dibayar = jumlahTerbayar(daftar[i].id_booking);
        const double sisa = daftar[i].total_bayar - dibayar;
        cout << left << setw(8) << daftar[i].id_booking
             << setw(18) << daftar[i].nama_member
             << setw(16) << fixed << setprecision(0)
             << daftar[i].total_bayar
             << setw(16) << dibayar
             << (sisa > 0 ? sisa : 0) << endl;
    }
}

void menuTransaksi()
{
    int pilihan;
    do
    {
        cout << "\n============================================\n";
        cout << "                MENU TRANSAKSI\n";
        cout << "============================================\n";
        cout << "1. Bayar DP atau Pelunasan\n";
        cout << "2. Lihat Status Pembayaran\n";
        cout << "3. Kembali ke Menu Booking\n";
        cout << "Pilih: ";
        if (!(cin >> pilihan))
        {
            if (cin.eof())
            {
                return;
            }
            cout << "Pilihan harus berupa angka.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (pilihan)
        {
        case 1:
            prosesTransaksi();
            break;
        case 2:
            lihatStatusTransaksi();
            break;
        case 3:
            break;
        default:
            cout << "Pilihan tidak tersedia.\n";
        }
    } while (pilihan != 3);
}

// ==================================================
// CETAK LAPORAN BOOKING
// ==================================================

void cetakBooking()
{
    vector<Booking> daftar = bacaBooking();

    if (daftar.empty())
    {
        cout << "\nBelum ada data booking untuk dicetak.\n";
        return;
    }

    ofstream file("laporan_booking.html");

    if (!file.is_open())
    {
        cout << "Gagal membuat laporan booking!\n";
        return;
    }

    file << "<!DOCTYPE html>";
    file << "<html><head>";
    file << "<meta charset='UTF-8'>";
    file << "<title>Laporan Booking</title>";

    file << "<style>";
    file << "body{font-family:Arial;margin:30px;}";
    file << "h2{text-align:center;}";
    file << "table{width:100%;border-collapse:collapse;}";
    file << "th,td{border:1px solid #333;padding:8px;text-align:center;}";
    file << "th{background:#ddd;}";
    file << "@media print{button{display:none;}}";
    file << "</style>";

    file << "</head><body>";

    file << "<h2>LAPORAN DATA BOOKING LAPANGAN</h2>";

    file << "<table>";
    file << "<tr>";
    file << "<th>ID Booking</th>";
    file << "<th>ID Member</th>";
    file << "<th>Nama Member</th>";
    file << "<th>Lapangan</th>";
    file << "<th>Tanggal</th>";
    file << "<th>Jam</th>";
    file << "<th>Durasi</th>";
    file << "<th>Total Bayar</th>";
    file << "</tr>";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << "<tr>";

        file << "<td>" << daftar[i].id_booking << "</td>";
        file << "<td>" << daftar[i].id_member << "</td>";
        file << "<td>" << daftar[i].nama_member << "</td>";
        file << "<td>" << daftar[i].nama_lapangan << "</td>";
        file << "<td>" << daftar[i].tanggal_booking << "</td>";
        file << "<td>" << daftar[i].jam_mulai << "</td>";
        file << "<td>" << daftar[i].durasi << " jam</td>";

        file << "<td>Rp "
             << fixed << setprecision(0)
             << daftar[i].total_bayar
             << "</td>";

        file << "</tr>";
    }

    file << "</table>";

    file << "<br><br>";
    file << "<button onclick='window.print()'>Cetak Laporan</button>";

    file << "</body></html>";

    file.close();

    cout << "\nLaporan berhasil dibuat!\n";
    cout << "Nama file: laporan_booking.html\n";

    // Membuka laporan di browser Windows
    system("start laporan_booking.html");
}

// ==================================================
// MENU BOOKING
// ==================================================

void menuBooking()
{
    int pilihan;

    do
    {
        cout << "\n============================================\n";
        cout << "                  MENU BOOKING\n";
        cout << "============================================\n";
        cout << "1. Tambah Booking\n";
        cout << "2. Lihat Data Booking\n";
        cout << "3. Edit Data Booking\n";
        cout << "4. Hapus Data Booking\n";
        cout << "5. Cetak Laporan Booking\n";
        cout << "6. Menu Transaksi\n";
        cout << "7. Kembali\n";
        cout << "============================================\n";
        cout << "Pilih Menu: ";

        if (!(cin >> pilihan))
        {
            cout << "Input harus berupa angka!\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        switch (pilihan)
        {
        case 1:
            tambahBooking();
            break;

        case 2:
            lihatBooking();
            break;

        case 3:
            editBooking();
            break;

        case 4:
            hapusBooking();
            break;

        case 5:
            cetakBooking();
            break;

        case 6:
            menuTransaksi();
            break;

        case 7:
            cout << "Kembali ke menu utama...\n";
            break;

        default:
            cout << "Pilihan tidak tersedia!\n";
        }

    } while (pilihan != 7);
}