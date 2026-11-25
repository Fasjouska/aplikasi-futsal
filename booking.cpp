#include "booking.h"
#include "lapangan.h"
#include "member.h"
#include "perlengkapan.h"
#include "transaksi.h"
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
const char *FILE_BOOKING = "booking.txt";
const char *FILE_BOOKING_PERLENGKAPAN = "booking_perlengkapan.txt";

bool jadwalBentrok(const string &jamA, int durasiA, const string &jamB, int durasiB)
{
    int mulaiA = jamKeMenit(jamA);
    int mulaiB = jamKeMenit(jamB);
    if (mulaiA < 0 || mulaiB < 0 || durasiA <= 0 || durasiB <= 0)
    {
        return false;
    }
    int selesaiA = mulaiA + durasiA * 60;
    int selesaiB = mulaiB + durasiB * 60;
    return mulaiA < selesaiB && selesaiA > mulaiB;
}

vector<DetailPerlengkapanBooking> bacaRincianPerlengkapanBooking()
{
    vector<DetailPerlengkapanBooking> daftar;
    ifstream file(FILE_BOOKING_PERLENGKAPAN);
    string baris;

    while (getline(file, baris))
    {
        if (baris.empty())
        {
            continue;
        }
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

bool simpanRincianPerlengkapanBooking(const vector<DetailPerlengkapanBooking> &daftar)
{
    ofstream file(FILE_BOOKING_PERLENGKAPAN);
    if (!file.is_open())
    {
        cout << "Gagal membuka file " << FILE_BOOKING_PERLENGKAPAN << "!\n";
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

vector<DetailPerlengkapanBooking> rincianUntukBooking(int idBooking)
{
    vector<DetailPerlengkapanBooking> semua = bacaRincianPerlengkapanBooking();
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

double totalPerlengkapanBooking(int idBooking, int durasi)
{
    vector<DetailPerlengkapanBooking> rincian = rincianUntukBooking(idBooking);
    double total = 0;
    for (size_t i = 0; i < rincian.size(); i++)
    {
        total += rincian[i].harga_per_jam * rincian[i].jumlah * durasi;
    }
    return total;
}

int stokPerlengkapanTerpakai(
    int idPerlengkapan,
    const string &tanggal,
    const string &jamMulai,
    int durasi,
    int idBookingDikecualikan = -1)
{
    vector<Booking> daftarBooking = bacaBooking();
    vector<DetailPerlengkapanBooking> semuaRincian = bacaRincianPerlengkapanBooking();

    int terpakai = 0;
    for (size_t i = 0; i < daftarBooking.size(); i++)
    {
        const Booking &b = daftarBooking[i];
        if (b.id_booking == idBookingDikecualikan)
        {
            continue;
        }
        if (b.tanggal_booking != tanggal)
        {
            continue;
        }
        if (!jadwalBentrok(jamMulai, durasi, b.jam_mulai, b.durasi))
        {
            continue;
        }

        for (size_t j = 0; j < semuaRincian.size(); j++)
        {
            if (semuaRincian[j].id_booking == b.id_booking &&
                semuaRincian[j].id_perlengkapan == idPerlengkapan)
            {
                terpakai += semuaRincian[j].jumlah;
            }
        }
    }

    return terpakai;
}

bool cekStokPerlengkapanUntukJadwal(
    int idBooking,
    const string &tanggal,
    const string &jamMulai,
    int durasi)
{
    vector<DetailPerlengkapanBooking> rincian = rincianUntukBooking(idBooking);
    for (size_t i = 0; i < rincian.size(); i++)
    {
        Perlengkapan master;
        if (!cariPerlengkapanById(rincian[i].id_perlengkapan, master))
        {
            continue;
        }
        int terpakai = stokPerlengkapanTerpakai(
            rincian[i].id_perlengkapan, tanggal, jamMulai, durasi, idBooking);
        int tersedia = master.jumlah - terpakai;
        if (rincian[i].jumlah > tersedia)
        {
            return false;
        }
    }
    return true;
}

bool cekJadwal(
    int id_lapangan,
    const string &tanggal,
    const string &jam,
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
        const Booking &b = daftar[i];
        if (b.id_booking == id_dikecualikan)
        {
            continue;
        }
        if (b.id_lapangan == id_lapangan && b.tanggal_booking == tanggal)
        {
            if (jadwalBentrok(jam, durasi, b.jam_mulai, b.durasi))
            {
                return true;
            }
        }
    }

    return false;
}

bool pilihPerlengkapanBooking(
    int idBooking,
    const string &tanggal,
    const string &jamMulai,
    int durasi,
    vector<DetailPerlengkapanBooking> &pilihan)
{
    vector<Perlengkapan> inventaris = bacaPerlengkapan();
    if (inventaris.empty())
    {
        cout << "\nInventaris perlengkapan kosong. Booking dilanjutkan tanpa perlengkapan.\n";
        return true;
    }

    cout << "\n========== PERLENGKAPAN TERSEDIA (PADA JADWAL INI) ==========\n";
    cout << left << setw(7) << "ID"
         << setw(12) << "Jenis"
         << setw(16) << "Pilihan"
         << setw(12) << "Tersedia"
         << "Tarif/Jam" << endl;
    cout << string(62, '-') << endl;
    for (size_t i = 0; i < inventaris.size(); i++)
    {
        int terpakai = stokPerlengkapanTerpakai(
            inventaris[i].id, tanggal, jamMulai, durasi);
        int tersedia = inventaris[i].jumlah - terpakai;
        if (tersedia < 0)
        {
            tersedia = 0;
        }

        cout << left << setw(7) << inventaris[i].id
             << setw(12) << inventaris[i].jenis
             << setw(16) << inventaris[i].pilihan
             << setw(12) << tersedia
             << "Rp" << inventaris[i].harga << endl;
    }
    cout << "Masukkan ID 0 jika selesai / tidak ingin menambah perlengkapan." << endl;

    while (true)
    {
        int idPerlengkapan = 0;
        if (!inputAngka("ID perlengkapan: ", idPerlengkapan))
        {
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

        int terpakai = stokPerlengkapanTerpakai(
            inventaris[indeks].id, tanggal, jamMulai, durasi);
        int tersedia = inventaris[indeks].jumlah - terpakai;
        if (tersedia <= 0)
        {
            cout << "Stok perlengkapan ini habis pada jadwal tersebut.\n";
            continue;
        }

        int jumlah = 0;
        ostringstream prompt;
        prompt << "Jumlah yang disewa (tersedia " << tersedia << "): ";
        if (!inputAngka(prompt.str(), jumlah))
        {
            return false;
        }
        if (jumlah <= 0 || jumlah > tersedia)
        {
            cout << "Jumlah tidak valid atau melebihi stok tersedia.\n";
            continue;
        }

        DetailPerlengkapanBooking detail;
        detail.id_booking = idBooking;
        detail.id_perlengkapan = inventaris[indeks].id;
        detail.jenis = inventaris[indeks].jenis;
        detail.pilihan = inventaris[indeks].pilihan;
        detail.jumlah = jumlah;
        detail.harga_per_jam = static_cast<double>(inventaris[indeks].harga);
        pilihan.push_back(detail);

        double subtotal = detail.harga_per_jam * detail.jumlah * durasi;
        cout << "Biaya perlengkapan: Rp "
             << fixed << setprecision(0) << subtotal << endl;
    }
}

string statusPembayaranBooking(const Booking &b, double dibayar)
{
    if (dibayar + 0.000001 >= b.total_bayar)
    {
        return "Lunas";
    }
    if (dibayar > 0)
    {
        return "DP";
    }
    return "Belum Bayar";
}

void tampilkanTabelBooking(const vector<Booking> &daftar)
{
    if (daftar.empty())
    {
        cout << "Tidak ada data booking.\n";
        return;
    }

    cout << left
         << setw(6) << "ID"
         << setw(8) << "Member"
         << setw(18) << "Nama Member"
         << setw(16) << "Lapangan"
         << setw(13) << "Tanggal"
         << setw(8) << "Jam"
         << setw(7) << "Dur"
         << setw(12) << "Total"
         << setw(12) << "Dibayar"
         << setw(12) << "Sisa"
         << "Status" << endl;

    cout << string(125, '-') << endl;

    for (size_t i = 0; i < daftar.size(); i++)
    {
        const double dibayar = hitungTotalTerbayar(daftar[i].id_booking);
        const double sisa = daftar[i].total_bayar - dibayar;
        const string status = statusPembayaranBooking(daftar[i], dibayar);

        cout << left
             << setw(6) << daftar[i].id_booking
             << setw(8) << daftar[i].id_member
             << setw(18) << daftar[i].nama_member
             << setw(16) << daftar[i].nama_lapangan
             << setw(13) << daftar[i].tanggal_booking
             << setw(8) << daftar[i].jam_mulai
             << setw(7) << daftar[i].durasi
             << fixed << setprecision(0)
             << setw(12) << daftar[i].total_bayar
             << setw(12) << dibayar
             << setw(12) << (sisa > 0 ? sisa : 0)
             << status << endl;
    }

    cout << string(125, '-') << endl;
}

void cetakUlangInvoice()
{
    vector<Booking> daftar = bacaBooking();
    if (daftar.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    tampilkanTabelBooking(daftar);

    int id = 0;
    if (!inputAngka("\nMasukkan ID Booking untuk cetak invoice: ", id))
    {
        return;
    }

    Booking b;
    if (!cariBookingById(id, b))
    {
        cout << "ID booking tidak ditemukan!\n";
        return;
    }

    cetakInvoiceBooking(b);
}
} // namespace

vector<Booking> bacaBooking()
{
    vector<Booking> daftar;
    ifstream file(FILE_BOOKING);
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

    return daftar;
}

bool simpanBooking(const vector<Booking> &daftar)
{
    ofstream file(FILE_BOOKING);
    if (!file.is_open())
    {
        cout << "Gagal membuka file " << FILE_BOOKING << "!\n";
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
        cout << "Gagal menyimpan file " << FILE_BOOKING << "!\n";
        return false;
    }
    return true;
}

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

bool cariBookingById(int id, Booking &hasil)
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

bool cekMemberDigunakanDiBooking(int idMember)
{
    vector<Booking> daftar = bacaBooking();
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_member == idMember)
        {
            return true;
        }
    }
    return false;
}

bool cekLapanganDigunakanDiBooking(int idLapangan)
{
    vector<Booking> daftar = bacaBooking();
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_lapangan == idLapangan)
        {
            return true;
        }
    }
    return false;
}

bool cekPerlengkapanDigunakanDiBooking(int idPerlengkapan)
{
    vector<DetailPerlengkapanBooking> semua = bacaRincianPerlengkapanBooking();
    for (size_t i = 0; i < semua.size(); i++)
    {
        if (semua[i].id_perlengkapan == idPerlengkapan)
        {
            return true;
        }
    }
    return false;
}

void sinkronkanNamaMemberDiBooking(int idMember, const string &namaBaru)
{
    vector<Booking> daftar = bacaBooking();
    bool berubah = false;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_member == idMember && daftar[i].nama_member != namaBaru)
        {
            daftar[i].nama_member = namaBaru;
            berubah = true;
        }
    }
    if (berubah)
    {
        simpanBooking(daftar);
    }
}

void sinkronkanNamaLapanganDiBooking(int idLapangan, const string &namaBaru)
{
    vector<Booking> daftar = bacaBooking();
    bool berubah = false;
    for (size_t i = 0; i < daftar.size(); i++)
    {
        if (daftar[i].id_lapangan == idLapangan && daftar[i].nama_lapangan != namaBaru)
        {
            daftar[i].nama_lapangan = namaBaru;
            berubah = true;
        }
    }
    if (berubah)
    {
        simpanBooking(daftar);
    }
}

void tambahBooking()
{
    vector<Member> member = bacaMember();
    vector<Lapangan> lapangan = bacaLapangan();

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
    Member m;
    Lapangan l;

    cout << "\n============================================\n";
    cout << "                 TAMBAH BOOKING\n";
    cout << "============================================\n";

    tampilkanDaftarMember();

    if (!inputAngka("\nMasukkan ID Member: ", b.id_member))
    {
        return;
    }

    if (!cariMemberById(b.id_member, m))
    {
        cout << "\nBooking ditolak!\n";
        cout << "ID member tidak ditemukan. Pelanggan harus terdaftar sebagai member.\n";
        return;
    }

    b.nama_member = m.nama_member;
    cout << "\nMember ditemukan!\n";
    cout << "Nama Member : " << m.nama_member << endl;
    cout << "No. WhatsApp: " << m.no_wa << endl;

    tampilkanDaftarLapangan();

    if (!inputAngka("\nMasukkan ID Lapangan: ", b.id_lapangan))
    {
        return;
    }

    if (!cariLapanganById(b.id_lapangan, l))
    {
        cout << "ID lapangan tidak ditemukan!\n";
        return;
    }

    b.nama_lapangan = l.nama;

    if (!inputTanggal("Tanggal Booking (DD-MM-YYYY): ", b.tanggal_booking))
    {
        return;
    }

    cout << "Jam Mulai (HH:MM): ";
    cin >> b.jam_mulai;

    if (jamKeMenit(b.jam_mulai) < 0)
    {
        cout << "Format jam tidak valid! Gunakan HH:MM (contoh: 14:00).\n";
        return;
    }

    if (!inputAngka("Durasi Booking (jam): ", b.durasi) || b.durasi <= 0)
    {
        cout << "Durasi harus lebih dari 0!\n";
        return;
    }

    if (jamKeMenit(b.jam_mulai) + b.durasi * 60 > 1440)
    {
        cout << "Jam selesai tidak boleh melewati pukul 24:00!\n";
        return;
    }

    if (cekJadwal(b.id_lapangan, b.tanggal_booking, b.jam_mulai, b.durasi))
    {
        cout << "\nBooking gagal!\n";
        cout << "Jadwal lapangan sudah digunakan atau tidak valid.\n";
        return;
    }

    b.id_booking = idBookingBerikutnya();

    vector<DetailPerlengkapanBooking> perlengkapanDipilih;
    if (!pilihPerlengkapanBooking(
            b.id_booking, b.tanggal_booking, b.jam_mulai, b.durasi, perlengkapanDipilih))
    {
        cout << "Booking dibatalkan." << endl;
        return;
    }

    b.total_bayar = static_cast<double>(l.harga) * b.durasi;
    for (size_t i = 0; i < perlengkapanDipilih.size(); i++)
    {
        b.total_bayar += perlengkapanDipilih[i].harga_per_jam *
                         perlengkapanDipilih[i].jumlah * b.durasi;
    }

    vector<Booking> daftar = bacaBooking();
    vector<DetailPerlengkapanBooking> semuaRincian = bacaRincianPerlengkapanBooking();
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
            cout << "Peringatan: rincian perlengkapan booking perlu diperiksa." << endl;
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
    cout << "Total Bayar : Rp " << fixed << setprecision(0) << b.total_bayar << endl;
    cout << "Silakan buka menu Transaksi untuk melakukan pembayaran (DP / Lunas).\n";
}

void lihatBooking()
{
    vector<Booking> daftar = bacaBooking();

    cout << "\n============================================\n";
    cout << "                 DATA BOOKING\n";
    cout << "============================================\n";

    tampilkanTabelBooking(daftar);
}

void cariBooking()
{
    vector<Booking> daftar = bacaBooking();
    if (daftar.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    cout << "\n=== CARI / FILTER BOOKING ===\n";
    cout << "1. Berdasarkan Tanggal (DD-MM-YYYY)\n";
    cout << "2. Berdasarkan Nama Member\n";
    cout << "3. Kembali\n";

    int pilihan = 0;
    if (!inputAngka("Pilih: ", pilihan))
    {
        return;
    }

    vector<Booking> hasil;
    if (pilihan == 1)
    {
        string tanggal;
        if (!inputTanggal("Masukkan Tanggal (DD-MM-YYYY): ", tanggal))
        {
            return;
        }
        for (size_t i = 0; i < daftar.size(); i++)
        {
            if (daftar[i].tanggal_booking == tanggal)
            {
                hasil.push_back(daftar[i]);
            }
        }
    }
    else if (pilihan == 2)
    {
        string kataKunci;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!inputTeks("Masukkan Nama Member: ", kataKunci))
        {
            return;
        }
        string kunciLower = keHurufKecil(kataKunci);
        for (size_t i = 0; i < daftar.size(); i++)
        {
            if (keHurufKecil(daftar[i].nama_member).find(kunciLower) != string::npos)
            {
                hasil.push_back(daftar[i]);
            }
        }
    }
    else
    {
        return;
    }

    cout << "\n=== HASIL PENCARIAN BOOKING ===\n";
    tampilkanTabelBooking(hasil);
}

void editBooking()
{
    vector<Booking> daftar = bacaBooking();
    if (daftar.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    tampilkanTabelBooking(daftar);

    int id = 0;
    if (!inputAngka("\nMasukkan ID Booking yang akan diedit: ", id))
    {
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

    if (hitungTotalTerbayar(b.id_booking) > 0)
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

    int pilihan = 0;
    if (!inputAngka("Pilihan: ", pilihan))
    {
        return;
    }

    Member m;
    Lapangan l;

    switch (pilihan)
    {
    case 1:
    {
        tampilkanDaftarMember();
        int id_member = 0;
        if (!inputAngka("\nMasukkan ID Member baru: ", id_member))
        {
            return;
        }
        if (!cariMemberById(id_member, m))
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
        tampilkanDaftarLapangan();
        int id_lapangan = 0;
        if (!inputAngka("\nMasukkan ID Lapangan baru: ", id_lapangan))
        {
            return;
        }
        if (!cariLapanganById(id_lapangan, l))
        {
            cout << "Lapangan tidak ditemukan.\n";
            return;
        }
        if (cekJadwal(id_lapangan, b.tanggal_booking, b.jam_mulai, b.durasi, b.id_booking))
        {
            cout << "Jadwal lapangan tersebut sudah digunakan.\n";
            return;
        }
        b.id_lapangan = l.id;
        b.nama_lapangan = l.nama;
        b.total_bayar = static_cast<double>(l.harga) * b.durasi +
                        totalPerlengkapanBooking(b.id_booking, b.durasi);
        break;
    }

    case 3:
    {
        string tanggal;
        if (!inputTanggal("Tanggal baru (DD-MM-YYYY): ", tanggal))
        {
            return;
        }
        if (cekJadwal(b.id_lapangan, tanggal, b.jam_mulai, b.durasi, b.id_booking))
        {
            cout << "Jadwal tanggal tersebut sudah digunakan.\n";
            return;
        }
        if (!cekStokPerlengkapanUntukJadwal(b.id_booking, tanggal, b.jam_mulai, b.durasi))
        {
            cout << "Stok perlengkapan yang disewa tidak mencukupi pada tanggal baru.\n";
            return;
        }
        b.tanggal_booking = tanggal;
        break;
    }

    case 4:
    {
        string jam;
        int durasi = 0;

        cout << "Jam mulai baru (HH:MM): ";
        cin >> jam;
        if (jamKeMenit(jam) < 0)
        {
            cout << "Format jam tidak valid!\n";
            return;
        }

        if (!inputAngka("Durasi baru (jam): ", durasi) || durasi <= 0)
        {
            cout << "Durasi tidak valid!\n";
            return;
        }

        if (jamKeMenit(jam) + durasi * 60 > 1440)
        {
            cout << "Jam selesai melewati pukul 24:00!\n";
            return;
        }

        if (cekJadwal(b.id_lapangan, b.tanggal_booking, jam, durasi, b.id_booking))
        {
            cout << "Jadwal tersebut sudah digunakan.\n";
            return;
        }
        if (!cekStokPerlengkapanUntukJadwal(b.id_booking, b.tanggal_booking, jam, durasi))
        {
            cout << "Stok perlengkapan yang disewa tidak mencukupi pada jadwal baru.\n";
            return;
        }
        if (!cariLapanganById(b.id_lapangan, l))
        {
            cout << "Data lapangan tidak ditemukan.\n";
            return;
        }

        b.jam_mulai = jam;
        b.durasi = durasi;
        b.total_bayar = static_cast<double>(l.harga) * durasi +
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

void hapusBooking()
{
    vector<Booking> daftar = bacaBooking();
    if (daftar.empty())
    {
        cout << "\nBelum ada data booking.\n";
        return;
    }

    tampilkanTabelBooking(daftar);

    int id = 0;
    if (!inputAngka("\nMasukkan ID Booking yang akan dihapus: ", id))
    {
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

    if (hitungTotalTerbayar(id) > 0)
    {
        cout << "Booking sudah menerima pembayaran dan tidak dapat dihapus.\n";
        return;
    }

    char konfirmasi;
    cout << "Yakin ingin menghapus booking ID " << id << "? (Y/T): ";
    cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y')
    {
        vector<DetailPerlengkapanBooking> semuaRincian = bacaRincianPerlengkapanBooking();
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

bool cetakInvoiceBooking(const Booking &booking)
{
    const double totalTerbayar = hitungTotalTerbayar(booking.id_booking);
    if (totalTerbayar + 0.000001 < booking.total_bayar)
    {
        cout << "Invoice final belum dapat dibuat karena pembayaran belum lunas.\n";
        return false;
    }

    vector<DetailPerlengkapanBooking> perlengkapan = rincianUntukBooking(booking.id_booking);
    vector<Transaksi> pembayaran = bacaTransaksi();
    Member member;
    const bool memberDitemukan = cariMemberById(booking.id_member, member);

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
         << "<title>Invoice Booking " << booking.id_booking << "</title><style>"
         << "body{font-family:Arial,sans-serif;margin:35px;color:#333;}"
         << "h1,h2{text-align:center;}"
         << "table{width:100%;border-collapse:collapse;margin:15px 0;}"
         << "th,td{border:1px solid #555;padding:8px;text-align:left;}"
         << "th{background:#e5e5e5;}"
         << ".paid{color:#198754;font-weight:bold;}"
         << "button{padding:10px 20px;background:#198754;color:white;border:0;border-radius:5px;margin-bottom:15px;cursor:pointer;}"
         << "@media print{button{display:none;}body{margin:15px;}}"
         << "</style></head><body>"
         << "<button onclick='window.print()'>Cetak Invoice</button>"
         << "<h1>INVOICE PEMBAYARAN FUTSAL</h1>"
         << "<p><b>No. Invoice:</b> INV-" << booking.id_booking << "</p>"
         << "<p><b>Tanggal cetak:</b> " << escapeHtml(waktuSekarang()) << "</p>"
         << "<h2>Detail Member</h2><table>"
         << "<tr><th>ID Member</th><td>" << booking.id_member << "</td></tr>"
         << "<tr><th>Nama Member</th><td>" << escapeHtml(booking.nama_member) << "</td></tr>";
    if (memberDitemukan)
    {
        file << "<tr><th>No. WhatsApp</th><td>" << escapeHtml(member.no_wa) << "</td></tr>";
    }
    file << "</table>"
         << "<h2>Detail Booking</h2><table>"
         << "<tr><th>ID Booking</th><td>" << booking.id_booking << "</td></tr>"
         << "<tr><th>Lapangan</th><td>" << escapeHtml(booking.nama_lapangan)
         << " (ID " << booking.id_lapangan << ")</td></tr>"
         << "<tr><th>Tanggal</th><td>" << escapeHtml(booking.tanggal_booking) << "</td></tr>"
         << "<tr><th>Jam mulai</th><td>" << escapeHtml(booking.jam_mulai) << "</td></tr>"
         << "<tr><th>Durasi</th><td>" << booking.durasi << " jam</td></tr>"
         << "</table>"
         << "<h2>Rincian Biaya</h2><table>"
         << "<tr><th>Item</th><th>Jumlah/Durasi</th><th>Tarif per jam</th><th>Total</th></tr>"
         << "<tr><td>Sewa lapangan</td><td>" << booking.durasi << " jam</td><td>Rp "
         << fixed << setprecision(0)
         << (booking.durasi > 0 ? totalLapangan / booking.durasi : 0)
         << "</td><td>Rp " << totalLapangan << "</td></tr>";

    for (size_t i = 0; i < perlengkapan.size(); i++)
    {
        const double subtotal = perlengkapan[i].harga_per_jam *
                                perlengkapan[i].jumlah * booking.durasi;
        string namaPilihan = perlengkapan[i].jenis + " - " + perlengkapan[i].pilihan;
        if (perlengkapan[i].pilihan == "Paketan")
        {
            namaPilihan += " (6 orang/paket)";
        }
        file << "<tr><td>" << escapeHtml(namaPilihan)
             << "</td><td>" << perlengkapan[i].jumlah << " x " << booking.durasi << " jam</td><td>Rp "
             << perlengkapan[i].harga_per_jam
             << "</td><td>Rp " << subtotal << "</td></tr>";
    }

    file << "</table>"
         << "<h2>Ringkasan Pembayaran</h2><table>"
         << "<tr><th>Total biaya</th><td>Rp " << booking.total_bayar << "</td></tr>"
         << "<tr><th>Total dibayar</th><td>Rp " << totalTerbayar << "</td></tr>"
         << "<tr><th>Status</th><td class='paid'>LUNAS</td></tr>"
         << "</table><h2>Riwayat Transaksi</h2><table>"
         << "<tr><th>Tanggal</th><th>Metode</th><th>Jumlah</th><th>Status</th></tr>";
    for (size_t i = 0; i < pembayaran.size(); i++)
    {
        if (pembayaran[i].id_booking != booking.id_booking)
        {
            continue;
        }
        file << "<tr><td>" << escapeHtml(pembayaran[i].tanggal_bayar)
             << "</td><td>" << escapeHtml(pembayaran[i].metode_bayar)
             << "</td><td>Rp " << pembayaran[i].jumlah_bayar
             << "</td><td>" << escapeHtml(pembayaran[i].status_transaksi)
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
    bukaFileHtml(namaFile.str());
    return true;
}

void cetakBooking()
{
    vector<Booking> daftar = bacaBooking();
    if (daftar.empty())
    {
        cout << "\nBelum ada data booking untuk dicetak.\n";
        return;
    }

    const string namaFile = "laporan_booking.html";
    ofstream file(namaFile.c_str());
    if (!file.is_open())
    {
        cout << "Gagal membuat laporan booking!\n";
        return;
    }

    file << "<!DOCTYPE html><html lang='id'><head><meta charset='UTF-8'>"
         << "<title>Laporan Booking</title><style>"
         << "body{font-family:Arial,sans-serif;margin:30px;color:#333;}"
         << "h2{text-align:center;}"
         << "table{width:100%;border-collapse:collapse;margin-top:15px;}"
         << "th,td{border:1px solid #333;padding:8px;text-align:center;}"
         << "th{background:#ddd;}"
         << "button{padding:10px 20px;background:#198754;color:white;border:0;border-radius:5px;cursor:pointer;margin-bottom:15px;}"
         << "@media print{button{display:none;}}"
         << "</style></head><body>"
         << "<button onclick='window.print()'>Cetak Laporan</button>"
         << "<h2>LAPORAN DATA BOOKING LAPANGAN</h2>"
         << "<p>Tanggal Cetak: " << escapeHtml(tanggalSekarang()) << "</p>"
         << "<table><tr>"
         << "<th>ID Booking</th><th>ID Member</th><th>Nama Member</th>"
         << "<th>Lapangan</th><th>Tanggal</th><th>Jam</th><th>Durasi</th>"
         << "<th>Total Bayar</th><th>Dibayar</th><th>Status</th>"
         << "</tr>";

    for (size_t i = 0; i < daftar.size(); i++)
    {
        const double dibayar = hitungTotalTerbayar(daftar[i].id_booking);
        const string status = statusPembayaranBooking(daftar[i], dibayar);
        file << "<tr>"
             << "<td>" << daftar[i].id_booking << "</td>"
             << "<td>" << daftar[i].id_member << "</td>"
             << "<td>" << escapeHtml(daftar[i].nama_member) << "</td>"
             << "<td>" << escapeHtml(daftar[i].nama_lapangan) << "</td>"
             << "<td>" << escapeHtml(daftar[i].tanggal_booking) << "</td>"
             << "<td>" << escapeHtml(daftar[i].jam_mulai) << "</td>"
             << "<td>" << daftar[i].durasi << " jam</td>"
             << "<td>Rp " << fixed << setprecision(0) << daftar[i].total_bayar << "</td>"
             << "<td>Rp " << dibayar << "</td>"
             << "<td>" << escapeHtml(status) << "</td>"
             << "</tr>";
    }

    file << "</table></body></html>";
    file.close();

    if (!file)
    {
        cout << "Gagal menyimpan laporan booking!\n";
        return;
    }

    cout << "\nLaporan berhasil dibuat: " << namaFile << endl;
    bukaFileHtml(namaFile);
}

void menuBooking()
{
    int pilihan = 0;
    do
    {
        cout << "\n============================================\n";
        cout << "                  MENU BOOKING\n";
        cout << "============================================\n";
        cout << "1. Tambah Booking\n";
        cout << "2. Lihat Data Booking\n";
        cout << "3. Cari / Filter Booking\n";
        cout << "4. Edit Data Booking\n";
        cout << "5. Hapus Data Booking\n";
        cout << "6. Cetak Laporan Booking\n";
        cout << "7. Cetak Ulang Invoice Booking\n";
        cout << "8. Kembali\n";
        cout << "============================================\n";

        if (!inputAngka("Pilih Menu: ", pilihan))
        {
            return;
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
            cariBooking();
            break;
        case 4:
            editBooking();
            break;
        case 5:
            hapusBooking();
            break;
        case 6:
            cetakBooking();
            break;
        case 7:
            cetakUlangInvoice();
            break;
        case 8:
            cout << "Kembali ke menu utama...\n";
            break;
        default:
            cout << "Pilihan tidak tersedia!\n";
        }
    } while (pilihan != 8);
}
