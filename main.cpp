#include "booking.h"
#include "lapangan.h"
#include "member.h"
#include "perlengkapan.h"
#include "transaksi.h"
#include "utils.h"

#include <iostream>

using namespace std;

int main() {
  int pilihan = 0;
  do {
    cout << "\n============================" << endl;
    cout << "      APLIKASI FUTSAL" << endl;
    cout << "============================" << endl;
    cout << "1. Data Member" << endl;
    cout << "2. Data Lapangan" << endl;
    cout << "3. Data Perlengkapan" << endl;
    cout << "4. Data Booking" << endl;
    cout << "5. Menu Transaksi" << endl;
    cout << "6. Keluar" << endl;

    if (!inputAngka("Pilih : ", pilihan)) {
      cout << "\nProgram selesai." << endl;
      break;
    }

    switch (pilihan) {
    case 1:
      menuMember();
      break;
    case 2:
      menuLapangan();
      break;
    case 3:
      menuPerlengkapan();
      break;
    case 4:
      menuBooking();
      break;
    case 5:
      menuTransaksi();
      break;
    case 6:
      cout << "Program selesai." << endl;
      break;
    default:
      cout << "Pilihan tidak tersedia!" << endl;
    }
  } while (pilihan != 6);

  return 0;
}
