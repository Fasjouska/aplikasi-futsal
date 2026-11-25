#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include <string>
#include <vector>

using namespace std;

struct Transaksi
{
    int id_transaksi;
    int id_booking;
    string tanggal_bayar;
    string metode_bayar;
    double jumlah_bayar;
    string status_transaksi;
    string tanggal_refund;
};

vector<Transaksi> bacaTransaksi();
bool simpanTransaksi(const vector<Transaksi> &daftar);
int idTransaksiBerikutnya();
double hitungTotalTerbayar(int idBooking);

void tambahTransaksi();
void lihatTransaksi();
void refundTransaksi();
void cetakLaporanTransaksi();
void menuTransaksi();

#endif
