#include <iostream>
#include "member.h"
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
        cout << "1. Data Member" << endl;
        cout << "2. Data Lapangan" << endl;
        cout << "3. Data Perlengkapan" << endl;
        cout << "4. Data Booking" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilih : ";
        cin >> pilihan;
        switch (pilihan)
        {
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
            cout << "Program selesai." << endl;
            break;
        default:
            cout << "Pilihan tidak tersedia!" << endl;
        }
    } while (pilihan != 5);
    return 0;
}