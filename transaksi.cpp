#include "transaksi.h"
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
const char *FILE_TRANSAKSI = "transaksi.txt";

void tampilkanTabelTransaksi(const vector<Transaksi> &daftar)
{
    if (daftar.empty())
    {
        cout << "Tidak ada data transaksi.\n";
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
             << setw(16) << fixed << setprecision(0) << daftar[i].jumlah_bayar
             << setw(13) << daftar[i].status_transaksi
             << daftar[i].tanggal_refund << endl;
    }

    cout << string(95, '-') << endl;
}

void tampilkanBookingBelumLunas()
{
    vector<Booking> booking = bacaBooking();

    cout << "\n=========================================================================================\n";
    cout << "                           DAFTAR BOOKING BELUM LUNAS\n";
    cout << "=========================================================================================\n";

    if (booking.empty())
    {
        cout << "Belum ada data booking.\n";
        return;
    }

    cout << left
         << setw(6) << "ID"
         << setw(18) << "Nama Member"
         << setw(16) << "Lapangan"
         << setw(13) << "Tanggal"
         << setw(8) << "Jam"
         << setw(12) << "Total"
         << setw(12) << "Dibayar"
         << setw(12) << "Sisa"
         << "Status\n";

    cout << string(108, '-') << endl;

    bool ditemukan = false;
    for (size_t i = 0; i < booking.size(); i++)
    {
        const double dibayar = hitungTotalTerbayar(booking[i].id_booking);
        const double sisa = booking[i].total_bayar - dibayar;
        if (sisa > 0.000001)
        {
            string status = (dibayar > 0) ? "DP" : "Belum Bayar";
            cout << left
                 << setw(6) << booking[i].id_booking
                 << setw(18) << booking[i].nama_member
                 << setw(16) << booking[i].nama_lapangan
                 << setw(13) << booking[i].tanggal_booking
                 << setw(8) << booking[i].jam_mulai
                 << fixed << setprecision(0)
                 << setw(12) << booking[i].total_bayar
                 << setw(12) << dibayar
                 << setw(12) << sisa
                 << status << endl;
            ditemukan = true;
        }
    }

    if (!ditemukan)
    {
        cout << "Semua booking sudah lunas.\n";
    }

    cout << string(108, '-') << endl;
}

void filterTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();
    if (daftar.empty())
    {
        cout << "\nBelum ada data transaksi.\n";
        return;
    }

    cout << "\n=== FILTER TRANSAKSI ===\n";
    cout << "1. Status Berhasil\n";
    cout << "2. Status Refund\n";
    cout << "3. Berdasarkan Tanggal Bayar (DD-MM-YYYY)\n";
    cout << "4. Kembali\n";

    int pilihan = 0;
    if (!inputAngka("Pilih: ", pilihan))
    {
        return;
    }

    vector<Transaksi> hasil;
    if (pilihan == 1 || pilihan == 2)
    {
        string target = (pilihan == 1) ? "Berhasil" : "Refund";
        for (size_t i = 0; i < daftar.size(); i++)
        {
            if (daftar[i].status_transaksi == target)
            {
                hasil.push_back(daftar[i]);
            }
        }
    }
    else if (pilihan == 3)
    {
        string tanggal;
        if (!inputTanggal("Masukkan Tanggal Bayar (DD-MM-YYYY): ", tanggal))
        {
            return;
        }
        for (size_t i = 0; i < daftar.size(); i++)
        {
            if (daftar[i].tanggal_bayar == tanggal)
            {
                hasil.push_back(daftar[i]);
            }
        }
    }
    else
    {
        return;
    }

    cout << "\n=== HASIL FILTER TRANSAKSI ===\n";
    tampilkanTabelTransaksi(hasil);
}
} // namespace

vector<Transaksi> bacaTransaksi()
{
    vector<Transaksi> daftar;
    ifstream file(FILE_TRANSAKSI);
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
        if (!konversiInt(data, t.id_transaksi))
            continue;

        getline(ss, data, '|');
        if (!konversiInt(data, t.id_booking))
            continue;

        getline(ss, t.tanggal_bayar, '|');
        getline(ss, t.metode_bayar, '|');

        getline(ss, data, '|');
        if (!konversiDouble(data, t.jumlah_bayar))
            continue;

        // Kompatibilitas dengan format transaksi lama
        if (!getline(ss, t.status_transaksi, '|') || t.status_transaksi.empty())
        {
            t.status_transaksi = "Berhasil";
        }
        else if (t.status_transaksi != "Berhasil" && t.status_transaksi != "Refund")
        {
            // Menangani format lama yang menyimpan nominal uang bayar di kolom ke-6
            t.status_transaksi = "Berhasil";
        }

        if (!getline(ss, t.tanggal_refund, '|') || t.tanggal_refund.empty())
        {
            t.tanggal_refund = "-";
        }
        else if (t.status_transaksi == "Berhasil")
        {
            t.tanggal_refund = "-";
        }

        daftar.push_back(t);
    }

    return daftar;
}

bool simpanTransaksi(const vector<Transaksi> &daftar)
{
    ofstream file(FILE_TRANSAKSI);
    if (!file.is_open())
    {
        cout << "Gagal membuka file " << FILE_TRANSAKSI << "!\n";
        return false;
    }

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << daftar[i].id_transaksi << "|"
             << daftar[i].id_booking << "|"
             << daftar[i].tanggal_bayar << "|"
             << daftar[i].metode_bayar << "|"
             << fixed << setprecision(0) << daftar[i].jumlah_bayar << "|"
             << daftar[i].status_transaksi << "|"
             << daftar[i].tanggal_refund << endl;
    }

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan file " << FILE_TRANSAKSI << "!\n";
        return false;
    }
    return true;
}

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

double hitungTotalTerbayar(int idBooking)
{
    vector<Transaksi> daftar = bacaTransaksi();
    double total = 0;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_booking == idBooking &&
            daftar[i].status_transaksi == "Berhasil")
        {
            total += daftar[i].jumlah_bayar;
        }
    }
    return total;
}

void tambahTransaksi()
{
    vector<Booking> booking = bacaBooking();
    if (booking.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    tampilkanBookingBelumLunas();

    Booking b;
    Transaksi t;

    cout << "\n============================================\n";
    cout << "              TAMBAH TRANSAKSI\n";
    cout << "============================================\n";

    if (!inputAngka("Masukkan ID Booking: ", t.id_booking))
    {
        return;
    }

    if (!cariBookingById(t.id_booking, b))
    {
        cout << "ID booking tidak ditemukan!\n";
        return;
    }

    const double sudahDibayar = hitungTotalTerbayar(t.id_booking);
    const double sisaTagihan = b.total_bayar - sudahDibayar;

    if (sisaTagihan <= 0.000001)
    {
        cout << "Booking tersebut sudah lunas!\n";
        return;
    }

    cout << "\nData Booking Ditemukan!\n";
    cout << "ID Booking   : " << b.id_booking << endl;
    cout << "Nama Member  : " << b.nama_member << endl;
    cout << "Lapangan     : " << b.nama_lapangan << endl;
    cout << "Tanggal      : " << b.tanggal_booking << endl;
    cout << "Jam Mulai    : " << b.jam_mulai << endl;
    cout << "Durasi       : " << b.durasi << " jam\n";
    cout << "Total Biaya  : Rp " << fixed << setprecision(0) << b.total_bayar << endl;
    cout << "Sudah Dibayar: Rp " << sudahDibayar << endl;
    cout << "Sisa Tagihan : Rp " << sisaTagihan << endl;

    cout << "\nJenis Pembayaran:\n";
    cout << "1. Bayar Lunas (Rp " << sisaTagihan << ")\n";
    cout << "2. Bayar Sebagian / DP\n";

    int jenisBayar = 0;
    if (!inputAngka("Pilih Jenis Pembayaran: ", jenisBayar))
    {
        return;
    }

    if (jenisBayar == 1)
    {
        t.jumlah_bayar = sisaTagihan;
    }
    else if (jenisBayar == 2)
    {
        double nominal = 0;
        if (!inputDouble("Masukkan nominal pembayaran: Rp ", nominal))
        {
            return;
        }
        if (nominal <= 0 || nominal > sisaTagihan + 0.000001)
        {
            cout << "Nominal harus lebih dari 0 dan tidak melebihi sisa tagihan (Rp "
                 << sisaTagihan << ")!\n";
            return;
        }
        t.jumlah_bayar = nominal;
    }
    else
    {
        cout << "Pilihan jenis pembayaran tidak tersedia!\n";
        return;
    }

    if (!inputTanggal("\nTanggal Pembayaran (DD-MM-YYYY): ", t.tanggal_bayar))
    {
        return;
    }

    cout << "\nMetode Pembayaran:\n";
    cout << "1. Tunai\n";
    cout << "2. Transfer\n";
    cout << "3. QRIS\n";

    int pilihan = 0;
    if (!inputAngka("Pilih Metode: ", pilihan))
    {
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
    t.status_transaksi = "Berhasil";
    t.tanggal_refund = "-";

    vector<Transaksi> daftar = bacaTransaksi();
    daftar.push_back(t);

    if (!simpanTransaksi(daftar))
    {
        return;
    }

    const double totalTerbayarBaru = sudahDibayar + t.jumlah_bayar;
    const double sisaBaru = b.total_bayar - totalTerbayarBaru;

    cout << "\n============================================\n";
    cout << "       TRANSAKSI BERHASIL DISIMPAN\n";
    cout << "============================================\n";
    cout << "ID Transaksi : " << t.id_transaksi << endl;
    cout << "ID Booking   : " << t.id_booking << endl;
    cout << "Tanggal Bayar: " << t.tanggal_bayar << endl;
    cout << "Metode Bayar : " << t.metode_bayar << endl;
    cout << "Jumlah Bayar : Rp " << fixed << setprecision(0) << t.jumlah_bayar << endl;
    cout << "Status       : " << t.status_transaksi << endl;

    if (sisaBaru <= 0.000001)
    {
        cout << "Status Bayar : LUNAS\n";
        cetakInvoiceBooking(b);
    }
    else
    {
        cout << "Status Bayar : DP / Belum Lunas (Sisa: Rp " << sisaBaru << ")\n";
        cout << "Invoice final akan dibuat otomatis setelah pembayaran lunas.\n";
    }
}

void lihatTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();

    cout << "\n==========================================================================\n";
    cout << "                         DATA TRANSAKSI\n";
    cout << "==========================================================================\n";

    tampilkanTabelTransaksi(daftar);
}

void refundTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();
    if (daftar.empty())
    {
        cout << "\nBelum ada transaksi untuk di-refund.\n";
        return;
    }

    lihatTransaksi();

    int id = 0;
    if (!inputAngka("\nMasukkan ID Transaksi yang akan di-refund: ", id))
    {
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
    cout << "Jumlah Refund: Rp " << fixed << setprecision(0) << t.jumlah_bayar << endl;

    char konfirmasi;
    cout << "\nKonfirmasi refund? (Y/T): ";
    cin >> konfirmasi;

    if (konfirmasi != 'Y' && konfirmasi != 'y')
    {
        cout << "Refund dibatalkan.\n";
        return;
    }

    if (!inputTanggal("Tanggal Refund (DD-MM-YYYY): ", t.tanggal_refund))
    {
        return;
    }

    t.status_transaksi = "Refund";

    if (simpanTransaksi(daftar))
    {
        cout << "\n============================================\n";
        cout << "          REFUND BERHASIL DIPROSES\n";
        cout << "============================================\n";
        cout << "ID Transaksi : " << t.id_transaksi << endl;
        cout << "Jumlah Refund: Rp " << fixed << setprecision(0) << t.jumlah_bayar << endl;
        cout << "Status       : " << t.status_transaksi << endl;
        cout << "Tanggal      : " << t.tanggal_refund << endl;
    }
}

void cetakLaporanTransaksi()
{
    vector<Transaksi> daftar = bacaTransaksi();
    if (daftar.empty())
    {
        cout << "\nBelum ada data transaksi untuk dicetak.\n";
        return;
    }

    const string namaFile = "laporan_transaksi.html";
    ofstream file(namaFile.c_str());
    if (!file.is_open())
    {
        cout << "Gagal membuat laporan transaksi!\n";
        return;
    }

    double total_berhasil = 0;
    double total_refund = 0;
    int jumlah_berhasil = 0;
    int jumlah_refund = 0;

    file << "<!DOCTYPE html><html lang='id'><head><meta charset='UTF-8'>"
         << "<title>Laporan Transaksi</title><style>"
         << "body{font-family:Arial,sans-serif;margin:30px;color:#333;}"
         << "h2{text-align:center;}"
         << "table{width:100%;border-collapse:collapse;margin-top:15px;}"
         << "th,td{border:1px solid #333;padding:8px;text-align:center;}"
         << "th{background:#ddd;}"
         << ".refund{color:#dc3545;font-weight:bold;}"
         << ".berhasil{color:#198754;font-weight:bold;}"
         << ".ringkasan{margin-top:20px;width:450px;}"
         << ".ringkasan th{text-align:left;width:60%;}"
         << ".ringkasan td{text-align:right;}"
         << "button{padding:10px 20px;background:#198754;color:white;border:0;border-radius:5px;cursor:pointer;margin-bottom:15px;}"
         << "@media print{button{display:none;}}"
         << "</style></head><body>"
         << "<button onclick='window.print()'>Cetak Laporan</button>"
         << "<h2>LAPORAN DATA TRANSAKSI PEMBAYARAN</h2>"
         << "<p>Tanggal Cetak: " << escapeHtml(tanggalSekarang()) << "</p>"
         << "<table><tr>"
         << "<th>ID Transaksi</th><th>ID Booking</th><th>Tanggal Bayar</th>"
         << "<th>Metode</th><th>Jumlah Bayar</th><th>Status</th><th>Tanggal Refund</th>"
         << "</tr>";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        file << "<tr>"
             << "<td>" << daftar[i].id_transaksi << "</td>"
             << "<td>" << daftar[i].id_booking << "</td>"
             << "<td>" << escapeHtml(daftar[i].tanggal_bayar) << "</td>"
             << "<td>" << escapeHtml(daftar[i].metode_bayar) << "</td>"
             << "<td>Rp " << fixed << setprecision(0) << daftar[i].jumlah_bayar << "</td>";

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
            total_berhasil += daftar[i].jumlah_bayar;
        }

        file << "<td>" << escapeHtml(daftar[i].tanggal_refund) << "</td>"
             << "</tr>";
    }

    const double total_kotor = total_berhasil + total_refund;
    const double pendapatan_bersih = total_berhasil;

    file << "</table>"
         << "<table class='ringkasan'>"
         << "<tr><th>Jumlah Transaksi Berhasil</th><td>" << jumlah_berhasil << "</td></tr>"
         << "<tr><th>Jumlah Transaksi Refund</th><td>" << jumlah_refund << "</td></tr>"
         << "<tr><th>Total Pembayaran Masuk (Kotor)</th><td>Rp " << fixed << setprecision(0) << total_kotor << "</td></tr>"
         << "<tr><th>Total Dana Refund</th><td>Rp " << total_refund << "</td></tr>"
         << "<tr><th>Pendapatan Bersih</th><td><b>Rp " << pendapatan_bersih << "</b></td></tr>"
         << "</table></body></html>";

    file.close();
    if (!file)
    {
        cout << "Gagal menyimpan laporan transaksi!\n";
        return;
    }

    cout << "\nLaporan transaksi berhasil dibuat: " << namaFile << endl;
    bukaFileHtml(namaFile);
}

void menuTransaksi()
{
    int pilihan = 0;
    do
    {
        cout << "\n============================================\n";
        cout << "               MENU TRANSAKSI\n";
        cout << "============================================\n";
        cout << "1. Tambah Transaksi (DP / Pelunasan)\n";
        cout << "2. Lihat Semua Transaksi\n";
        cout << "3. Filter Transaksi\n";
        cout << "4. Refund Transaksi\n";
        cout << "5. Cetak Laporan Transaksi\n";
        cout << "6. Kembali\n";
        cout << "============================================\n";

        if (!inputAngka("Pilih Menu: ", pilihan))
        {
            return;
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
            filterTransaksi();
            break;
        case 4:
            refundTransaksi();
            break;
        case 5:
            cetakLaporanTransaksi();
            break;
        case 6:
            cout << "Kembali ke menu utama...\n";
            break;
        default:
            cout << "Pilihan tidak tersedia!\n";
        }
    } while (pilihan != 6);
}
