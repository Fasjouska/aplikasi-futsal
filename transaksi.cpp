#include "transaksi.h"
#include "booking.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;

// ==================================================
// KONVERSI STRING KE INTEGER
// ==================================================

bool konversiIntTransaksi(string data, int &hasil)
{
    stringstream ss(data);
    char sisa;

    if (!(ss >> hasil))
        return false;
    if (ss >> sisa)
        return false;

    return true;
}

// ==================================================
// KONVERSI STRING KE DOUBLE
// ==================================================

bool konversiDoubleTransaksi(string data, double &hasil)
{
    stringstream ss(data);
    char sisa;

    if (!(ss >> hasil))
        return false;
    if (ss >> sisa)
        return false;

    return true;
}

// ==================================================
// MEMBACA DATA TRANSAKSI
// Format baru:
// id|booking|tanggal|metode|jumlah|status|tanggal_refund
// ==================================================

vector<Transaksi> bacaTransaksi()
{
    vector<Transaksi> daftar;

    ifstream file("transaksi.txt");

    if (!file.is_open())
    {
        return daftar;
    }

    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
            continue;

        stringstream ss(baris);
        string data;
        Transaksi t;

        getline(ss, data, '|');
        if (!konversiIntTransaksi(data, t.id_transaksi))
            continue;

        getline(ss, data, '|');
        if (!konversiIntTransaksi(data, t.id_booking))
            continue;

        getline(ss, t.tanggal_bayar, '|');
        getline(ss, t.metode_bayar, '|');

        getline(ss, data, '|');
        if (!konversiDoubleTransaksi(data, t.jumlah_bayar))
            continue;

        // Kompatibilitas dengan format transaksi lama
        if (!getline(ss, t.status_transaksi, '|'))
        {
            t.status_transaksi = "Berhasil";
        }

        if (!getline(ss, t.tanggal_refund, '|'))
        {
            t.tanggal_refund = "-";
        }

        if (t.status_transaksi.empty())
        {
            t.status_transaksi = "Berhasil";
        }

        if (t.tanggal_refund.empty())
        {
            t.tanggal_refund = "-";
        }

        daftar.push_back(t);
    }

    file.close();

    return daftar;
}

// ==================================================
// MENYIMPAN DATA TRANSAKSI
// ==================================================

void simpanTransaksi(vector<Transaksi> daftar)
{
    ofstream file("transaksi.txt");

    if (!file.is_open())
    {
        cout << "Gagal membuka file transaksi.txt!\n";
        return;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id_transaksi << "|"
             << daftar[i].id_booking << "|"
             << daftar[i].tanggal_bayar << "|"
             << daftar[i].metode_bayar << "|"
             << fixed << setprecision(0)
             << daftar[i].jumlah_bayar << "|"
             << daftar[i].status_transaksi << "|"
             << daftar[i].tanggal_refund
             << endl;
    }

    file.close();
}

// ==================================================
// ID TRANSAKSI BERIKUTNYA
// ==================================================

int idTransaksiBerikutnya()
{
    vector<Transaksi> daftar = bacaTransaksi();

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
// MENCARI BOOKING BERDASARKAN ID
// ==================================================

bool cariBookingTransaksi(int id, Booking &hasil)
{
    vector<Booking> daftar = bacaBooking();

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking == id)
        {
            hasil = daftar[i];
            return true;
        }
    }

    return false;
}

// ==================================================
// CEK BOOKING SUDAH DIBAYAR
// Refund tidak dihitung sebagai pembayaran aktif
// ==================================================

bool bookingSudahDibayar(int id_booking)
{
    vector<Transaksi> daftar = bacaTransaksi();

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking == id_booking &&
            daftar[i].status_transaksi == "Berhasil")
        {
            return true;
        }
    }

    return false;
}

// ==================================================
// MENAMPILKAN BOOKING BELUM DIBAYAR
// ==================================================

void tampilkanBookingBelumDibayar()
{
    vector<Booking> booking = bacaBooking();

    cout << "\n============================================================\n";
    cout << "              DAFTAR BOOKING BELUM DIBAYAR\n";
    cout << "============================================================\n";

    if (booking.empty())
    {
        cout << "Belum ada data booking.\n";
        return;
    }

    cout << left
         << setw(8) << "ID"
         << setw(18) << "Nama Member"
         << setw(18) << "Lapangan"
         << setw(15) << "Tanggal"
         << setw(12) << "Jam"
         << "Total Bayar\n";

    cout << string(85, '-') << endl;

    bool ditemukan = false;

    for (size_t i = 0; i < booking.size(); i++)
    {
        if (!bookingSudahDibayar(booking[i].id_booking))
        {

            cout << left
                 << setw(8) << booking[i].id_booking
                 << setw(18) << booking[i].nama_member
                 << setw(18) << booking[i].nama_lapangan
                 << setw(15) << booking[i].tanggal_booking
                 << setw(12) << booking[i].jam_mulai
                 << fixed << setprecision(0)
                 << booking[i].total_bayar
                 << endl;

            ditemukan = true;
        }
    }

    if (!ditemukan)
    {
        cout << "Semua booking sudah dibayar.\n";
    }

    cout << string(85, '-') << endl;
}

// ==================================================
// TAMBAH TRANSAKSI
// ==================================================

void tambahTransaksi()
{
    vector<Booking> booking = bacaBooking();

    if (booking.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    tampilkanBookingBelumDibayar();

    Booking b;
    Transaksi t;

    cout << "\n============================================\n";
    cout << "              TAMBAH TRANSAKSI\n";
    cout << "============================================\n";

    cout << "Masukkan ID Booking: ";

    if (!(cin >> t.id_booking))
    {
        cout << "ID booking harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (!cariBookingTransaksi(t.id_booking, b))
    {
        cout << "ID booking tidak ditemukan!\n";
        return;
    }

    if (bookingSudahDibayar(t.id_booking))
    {
        cout << "Booking tersebut sudah dibayar!\n";
        return;
    }

    cout << "\nData Booking Ditemukan!\n";
    cout << "ID Booking  : " << b.id_booking << endl;
    cout << "Nama Member : " << b.nama_member << endl;
    cout << "Lapangan    : " << b.nama_lapangan << endl;
    cout << "Tanggal     : " << b.tanggal_booking << endl;
    cout << "Jam Mulai   : " << b.jam_mulai << endl;
    cout << "Durasi      : " << b.durasi << " jam\n";

    cout << "Total Bayar : Rp "
         << fixed << setprecision(0)
         << b.total_bayar << endl;

    cout << "\nTanggal Pembayaran (DD-MM-YYYY): ";
    cin >> t.tanggal_bayar;

    cout << "\nMetode Pembayaran:\n";
    cout << "1. Tunai\n";
    cout << "2. Transfer\n";
    cout << "3. QRIS\n";
    cout << "Pilih Metode: ";

    int pilihan;

    if (!(cin >> pilihan))
    {
        cout << "Pilihan harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    switch (pilihan)
    {
    case 1:
        t.metode_bayar = "Tunai";
        break;

    case 2:
        t.metode_bayar = "Transfer";
        break;

    case 3:
        t.metode_bayar = "QRIS";
        break;

    default:
        cout << "Metode pembayaran tidak tersedia!\n";
        return;
    }

    t.id_transaksi = idTransaksiBerikutnya();
    t.jumlah_bayar = b.total_bayar;
    t.status_transaksi = "Berhasil";
    t.tanggal_refund = "-";

    vector<Transaksi> daftar = bacaTransaksi();

    daftar.push_back(t);

    simpanTransaksi(daftar);

    cout << "\n============================================\n";
    cout << "       TRANSAKSI BERHASIL DISIMPAN\n";
    cout << "============================================\n";

    cout << "ID Transaksi : " << t.id_transaksi << endl;
    cout << "ID Booking   : " << t.id_booking << endl;
    cout << "Tanggal Bayar: " << t.tanggal_bayar << endl;
    cout << "Metode Bayar : " << t.metode_bayar << endl;

    cout << "Jumlah Bayar : Rp "
         << fixed << setprecision(0)
         << t.jumlah_bayar << endl;

    cout << "Status       : " << t.status_transaksi << endl;

    cetakInvoiceBooking(b);
}

// ==================================================
// LIHAT DATA TRANSAKSI
// ==================================================

void lihatTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();

    cout << "\n==========================================================================\n";
    cout << "                         DATA TRANSAKSI\n";
    cout << "==========================================================================\n";

    if (daftar.empty())
    {
        cout << "Belum ada data transaksi.\n";
        return;
    }

    cout << left
         << setw(8) << "ID"
         << setw(12) << "ID Booking"
         << setw(16) << "Tgl Bayar"
         << setw(13) << "Metode"
         << setw(16) << "Jumlah Bayar"
         << setw(13) << "Status"
         << "Tgl Refund\n";

    cout << string(95, '-') << endl;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        cout << left
             << setw(8) << daftar[i].id_transaksi
             << setw(12) << daftar[i].id_booking
             << setw(16) << daftar[i].tanggal_bayar
             << setw(13) << daftar[i].metode_bayar
             << setw(16) << fixed << setprecision(0)
             << daftar[i].jumlah_bayar
             << setw(13) << daftar[i].status_transaksi
             << daftar[i].tanggal_refund
             << endl;
    }

    cout << string(95, '-') << endl;
}

// ==================================================
// REFUND TRANSAKSI
// ==================================================

void refundTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();

    if (daftar.empty())
    {
        cout << "\nBelum ada transaksi untuk di-refund.\n";
        return;
    }

    lihatTransaksi();

    int id;

    cout << "\nMasukkan ID Transaksi yang akan di-refund: ";

    if (!(cin >> id))
    {
        cout << "ID transaksi harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int posisi = -1;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_transaksi == id)
        {
            posisi = static_cast<int>(i);
            break;
        }
    }

    if (posisi == -1)
    {
        cout << "Data transaksi tidak ditemukan!\n";
        return;
    }

    Transaksi &t = daftar[posisi];

    if (t.status_transaksi == "Refund")
    {
        cout << "Transaksi ini sudah pernah di-refund!\n";
        return;
    }

    cout << "\n============================================\n";
    cout << "             DETAIL REFUND\n";
    cout << "============================================\n";

    cout << "ID Transaksi : " << t.id_transaksi << endl;
    cout << "ID Booking   : " << t.id_booking << endl;
    cout << "Metode Bayar : " << t.metode_bayar << endl;

    cout << "Jumlah Refund: Rp "
         << fixed << setprecision(0)
         << t.jumlah_bayar << endl;

    char konfirmasi;

    cout << "\nKonfirmasi refund? (Y/T): ";
    cin >> konfirmasi;

    if (konfirmasi != 'Y' && konfirmasi != 'y')
    {
        cout << "Refund dibatalkan.\n";
        return;
    }

    cout << "Tanggal Refund (DD-MM-YYYY): ";
    cin >> t.tanggal_refund;

    t.status_transaksi = "Refund";

    simpanTransaksi(daftar);

    cout << "\n============================================\n";
    cout << "          REFUND BERHASIL DIPROSES\n";
    cout << "============================================\n";

    cout << "ID Transaksi : " << t.id_transaksi << endl;
    cout << "Jumlah Refund: Rp "
         << fixed << setprecision(0)
         << t.jumlah_bayar << endl;
    cout << "Status       : " << t.status_transaksi << endl;
    cout << "Tanggal      : " << t.tanggal_refund << endl;
}

// ==================================================
// CETAK LAPORAN TRANSAKSI
// ==================================================

void cetakLaporanTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();

    if (daftar.empty())
    {
        cout << "\nBelum ada data transaksi untuk dicetak.\n";
        return;
    }

    ofstream file("laporan_transaksi.html");

    if (!file.is_open())
    {
        cout << "Gagal membuat laporan transaksi!\n";
        return;
    }

    double total_transaksi = 0;
    double total_refund = 0;
    int jumlah_berhasil = 0;
    int jumlah_refund = 0;

    file << "<!DOCTYPE html>";
    file << "<html><head>";
    file << "<meta charset='UTF-8'>";
    file << "<title>Laporan Transaksi</title>";

    file << "<style>";
    file << "body{font-family:Arial;margin:30px;}";
    file << "h2{text-align:center;}";
    file << "table{width:100%;border-collapse:collapse;}";
    file << "th,td{border:1px solid #333;padding:8px;text-align:center;}";
    file << "th{background:#ddd;}";
    file << ".refund{color:red;font-weight:bold;}";
    file << ".berhasil{color:green;font-weight:bold;}";
    file << "@media print{button{display:none;}}";
    file << "</style>";

    file << "</head><body>";

    file << "<h2>LAPORAN DATA TRANSAKSI PEMBAYARAN</h2>";

    file << "<table>";
    file << "<tr>";
    file << "<th>ID Transaksi</th>";
    file << "<th>ID Booking</th>";
    file << "<th>Tanggal Bayar</th>";
    file << "<th>Metode</th>";
    file << "<th>Jumlah Bayar</th>";
    file << "<th>Status</th>";
    file << "<th>Tanggal Refund</th>";
    file << "</tr>";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << "<tr>";

        file << "<td>" << daftar[i].id_transaksi << "</td>";
        file << "<td>" << daftar[i].id_booking << "</td>";
        file << "<td>" << daftar[i].tanggal_bayar << "</td>";
        file << "<td>" << daftar[i].metode_bayar << "</td>";

        file << "<td>Rp "
             << fixed << setprecision(0)
             << daftar[i].jumlah_bayar
             << "</td>";

        if (daftar[i].status_transaksi == "Refund")
        {
            file << "<td class='refund'>Refund</td>";
            jumlah_refund++;
            total_refund += daftar[i].jumlah_bayar;
        }
        else
        {
            file << "<td class='berhasil'>Berhasil</td>";
            jumlah_berhasil++;
            total_transaksi += daftar[i].jumlah_bayar;
        }

        file << "<td>" << daftar[i].tanggal_refund << "</td>";

        file << "</tr>";
    }

    file << "</table>";

    file << "<h3>Jumlah Transaksi Berhasil: "
         << jumlah_berhasil << "</h3>";

    file << "<h3>Total Pembayaran Berhasil: Rp "
         << fixed << setprecision(0)
         << total_transaksi << "</h3>";

    file << "<h3>Jumlah Refund: "
         << jumlah_refund << "</h3>";

    file << "<h3>Total Dana Refund: Rp "
         << fixed << setprecision(0)
         << total_refund << "</h3>";

    file << "<h3>Pendapatan Bersih: Rp "
         << fixed << setprecision(0)
         << total_transaksi - total_refund << "</h3>";

    file << "<br><br>";
    file << "<button onclick='window.print()'>Cetak Laporan</button>";

    file << "</body></html>";

    file.close();

    cout << "\nLaporan transaksi berhasil dibuat!\n";
    cout << "Nama file: laporan_transaksi.html\n";

    system("start laporan_transaksi.html");
}

// ==================================================
// MENU TRANSAKSI
// ==================================================

void menuTransaksi()
{
    int pilihan;

    do
    {
        cout << "\n============================================\n";
        cout << "               MENU TRANSAKSI\n";
        cout << "============================================\n";
        cout << "1. Tambah Transaksi\n";
        cout << "2. Lihat Transaksi\n";
        cout << "3. Refund Transaksi\n";
        cout << "4. Cetak Laporan Transaksi\n";
        cout << "5. Kembali\n";
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
            tambahTransaksi();
            break;

        case 2:
            lihatTransaksi();
            break;

        case 3:
            refundTransaksi();
            break;

        case 4:
            cetakLaporanTransaksi();
            break;

        case 5:
            cout << "Kembali ke menu utama...\n";
            break;

        default:
            cout << "Pilihan tidak tersedia!\n";
        }

    } while (pilihan != 5);
}