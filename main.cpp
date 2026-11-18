#include <iostream>
#include "lapangan.h"
#include "perlengkapan.h"
#include "booking.h"

using namespace std;
int main()
{
    int pilihan;
    do
    {
        cout << "\n============================" << endl;
        cout << " APLIKASI FUTSAL" << endl;
        cout << "============================" << endl;
        cout << "1. Data Lapangan" << endl;
        cout << "2. Data Perlengkapan" << endl;
        cout << "3. Data Booking" << endl;
        cout << "4. Keluar" << endl;
        cout << "Pilih : ";
        cin >> pilihan;
        switch (pilihan)
        {
        case 1:
            menuLapangan();
            break;
        case 2:
            menuPerlengkapan();
            break;
        case 3:
            menuBooking();
            break;
        case 4:
            cout << "Program selesai." << endl;
            break;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 4);
    return 0;
}