#include <iostream>
#include "hospital.h"
#include "storage.h"

using namespace std;

int main()
{
    loadData();

    int role = -1;
    do
    {
        cout << "\n========== SMARTCARE HOSPITAL SYSTEM ==========\n";
        cout << "1. Admin / Receptionist\n";
        cout << "2. Doctor\n";
        cout << "0. Save and exit\n";
        cout << "Choose role: ";

        if (!(cin >> role))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        switch (role)
        {
            case 1:
                runAdminMenu();
                break;
            case 2:
                runDoctorMenu();
                break;
            case 0:
                saveAllData();
                cout << "All records saved. Goodbye!\n";
                break;
            default:
                cout << "Invalid role choice.\n";
        }
    } while (role != 0);

    return 0;
}
