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

void simpanBooking(vector<Booking> daftar)
{
    ofstream file("booking.txt");

    if (!file.is_open())
    {
        cout << "Gagal membuka file booking.txt!\n";
        return;
    }

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
    b.total_bayar = l.harga * b.durasi;

    vector<Booking> daftar = bacaBooking();
    daftar.push_back(b);

    simpanBooking(daftar);

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
         << "Total Bayar"
         << endl;

    cout << string(105, '-') << endl;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        cout << left
             << setw(7) << daftar[i].id_booking
             << setw(8) << daftar[i].id_member
             << setw(18) << daftar[i].nama_member
             << setw(18) << daftar[i].nama_lapangan
             << setw(14) << daftar[i].tanggal_booking
             << setw(10) << daftar[i].jam_mulai
             << setw(8) << daftar[i].durasi
             << fixed << setprecision(0)
             << daftar[i].total_bayar
             << endl;
    }

    cout << string(105, '-') << endl;
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
        b.total_bayar = l.harga * b.durasi;

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
        b.total_bayar = l.harga * durasi;

        break;
    }

    case 5:
        return;

    default:
        cout << "Pilihan tidak tersedia!\n";
        return;
    }

    simpanBooking(daftar);

    cout << "\nData booking berhasil diperbarui!\n";
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

    char konfirmasi;

    cout << "Yakin ingin menghapus booking ID "
         << id << "? (Y/T): ";

    cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y')
    {
        daftar.erase(daftar.begin() + posisi);

        simpanBooking(daftar);

        cout << "Data booking berhasil dihapus!\n";
    }
    else
    {
        cout << "Penghapusan dibatalkan.\n";
    }
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
        cout << "6. Kembali\n";
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
            cout << "Kembali ke menu utama...\n";
            break;

        default:
            cout << "Pilihan tidak tersedia!\n";
        }

    } while (pilihan != 6);
}