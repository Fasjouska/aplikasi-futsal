#ifndef BOOKING_H
#define BOOKING_H

#include <iostream>
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

vector<Booking> bacaBooking();
bool simpanBooking(const vector<Booking> &daftar);
int idBookingBerikutnya();

void tambahBooking();
void lihatBooking();
void editBooking();
void hapusBooking();
void cetakBooking();
void menuTransaksi();
void menuBooking();

#endif