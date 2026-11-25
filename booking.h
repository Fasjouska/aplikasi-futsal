#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>

using namespace std;

struct Booking
{
    int id_booking;
    int id_member;
    string nama_member;
    int id_lapangan;
    string nama_lapangan;
    string tanggal_booking;
    string jam_mulai;
    int durasi;
    double total_bayar;
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

vector<Booking> bacaBooking();
bool simpanBooking(const vector<Booking> &daftar);
int idBookingBerikutnya();
bool cariBookingById(int id, Booking &hasil);

bool cekMemberDigunakanDiBooking(int idMember);
bool cekLapanganDigunakanDiBooking(int idLapangan);
bool cekPerlengkapanDigunakanDiBooking(int idPerlengkapan);
void sinkronkanNamaMemberDiBooking(int idMember, const string &namaBaru);
void sinkronkanNamaLapanganDiBooking(int idLapangan, const string &namaBaru);

bool cetakInvoiceBooking(const Booking &booking);
void tambahBooking();
void lihatBooking();
void cariBooking();
void editBooking();
void hapusBooking();
void cetakBooking();
void menuBooking();

#endif
