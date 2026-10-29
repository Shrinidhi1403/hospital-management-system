#include <iostream>
#include <string>
#include <algorithm>

#include "models.h"
#include "hospital.h"
#include "storage.h"

using namespace std;

// Actual shared data is defined here.
vector<Patient> patients;
vector<Doctor> doctors;
vector<Appointment> appointments;
vector<Visit> visits;

unordered_map<int, int> patientIndex;   //{index,patientID}
unordered_map<int, int> doctorIndex;    //{doctor,doctorId}

map<int, queue<int>> regularQueues;
map<int, priority_queue<EmergencyPatient>> emergencyQueues;

int nextPatientId = 1;
int nextDoctorId = 1;
int nextAppointmentId = 1;
int queueOrder = 1;

// -------------------- INDEX HELPERS --------------------

void rebuildPatientIndex()
{
    patientIndex.clear();
    for (int i = 0; i < (int)patients.size(); i++)
        patientIndex[patients[i].id] = i;
}

void rebuildDoctorIndex()
{
    doctorIndex.clear();
    for (int i = 0; i < (int)doctors.size(); i++)
        doctorIndex[doctors[i].id] = i;
}

int findPatientPosition(int id)
{
    if (patientIndex.find(id) == patientIndex.end())
        return -1;
    return patientIndex[id];
}

int findDoctorPosition(int id)
{
    if (doctorIndex.find(id) == doctorIndex.end())
        return -1;
    return doctorIndex[id];
}

// -------------------- PATIENT MANAGEMENT --------------------

void registerPatient()
{
    Patient p;
    p.id = nextPatientId++;

    cout << "\nPatient name: ";
    getline(cin, p.name);

    cout << "Age: ";
    if (!(cin >> p.age))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid age. Patient was not registered.\n";
        nextPatientId--;
        return;
    }
    cin.ignore(10000, '\n');

    cout << "Phone number: ";
    getline(cin, p.phone);
    cout << "Diagnosis / reason for visit: ";
    getline(cin, p.diagnosis);

    if (p.name.empty() || p.age < 0 || p.age > 120)
    {
        cout << "Invalid name or age. Patient was not registered.\n";
        nextPatientId--;
        return;
    }

    patients.push_back(p);
    patientIndex[p.id] = (int)patients.size() - 1;
    savePatients();
    cout << "Patient registered. Patient ID: " << p.id << '\n';
}

void displayPatients()
{
    if (patients.empty())
    {
        cout << "\nNo patients registered.\n";
        return;
    }

    cout << "\n--- PATIENTS ---\n";
    for (const Patient &p : patients)
        cout << "ID: " << p.id << " | " << p.name << " | Age: " << p.age
             << " | Phone: " << p.phone << " | Reason: " << p.diagnosis << '\n';
}

void searchPatient()
{
    int id;
    cout << "Patient ID: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    cin.ignore(10000, '\n');

    int pos = findPatientPosition(id);
    if (pos == -1)
    {
        cout << "Patient not found.\n";
        return;
    }

    const Patient &p = patients[pos];
    cout << "ID: " << p.id << "\nName: " << p.name << "\nAge: " << p.age
         << "\nPhone: " << p.phone << "\nReason: " << p.diagnosis << '\n';
}

void updatePatient()
{
    int id;
    cout << "Patient ID to update: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    cin.ignore(10000, '\n');

    int pos = findPatientPosition(id);
    if (pos == -1)
    {
        cout << "Patient not found.\n";
        return;
    }

    Patient &p = patients[pos];
    cout << "New name: ";
    getline(cin, p.name);
    cout << "New age: ";
    if (!(cin >> p.age))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid age; update cancelled.\n";
        return;
    }
    cin.ignore(10000, '\n');
    cout << "New phone: ";
    getline(cin, p.phone);
    cout << "New reason/diagnosis: ";
    getline(cin, p.diagnosis);

    if (p.name.empty() || p.age < 0 || p.age > 120)
    {
        cout << "Invalid name or age. Please correct this record.\n";
        return;
    }

    savePatients();
    cout << "Patient updated.\n";
}

void deletePatient()
{
    int id;
    cout << "Patient ID to delete: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    cin.ignore(10000, '\n');

    int pos = findPatientPosition(id);
    if (pos == -1)
    {
        cout << "Patient not found.\n";
        return;
    }

    // Remove the patient and their appointments.
    patients.erase(patients.begin() + pos);
    appointments.erase(
        remove_if(appointments.begin(), appointments.end(),
                  [id](const Appointment &a) { return a.patientId == id; }),
        appointments.end());

    rebuildPatientIndex();
    saveAllData();
    cout << "Patient and their appointments deleted.\n";
}

// -------------------- DOCTOR MANAGEMENT --------------------

void registerDoctor()
{
    Doctor d;
    d.id = nextDoctorId++;

    cout << "\nDoctor name: ";
    getline(cin, d.name);
    cout << "Specialization: ";
    getline(cin, d.specialization);
    cout << "Phone number: ";
    getline(cin, d.phone);

    if (d.name.empty() || d.specialization.empty())
    {
        cout << "Name and specialization are required.\n";
        nextDoctorId--;
        return;
    }

    doctors.push_back(d);
    doctorIndex[d.id] = (int)doctors.size() - 1;
    saveDoctors();

    cout << "Doctor registered. Doctor ID: " << d.id << '\n';
}

void displayDoctors()
{
    if (doctors.empty())
    {
        cout << "\nNo doctors registered.\n";
        return;
    }

    cout << "\n--- DOCTORS ---\n";
    for (const Doctor &d : doctors)
        cout << "Doctor ID: " << d.id << " | Dr. " << d.name
             << " | " << d.specialization << " | Phone: " << d.phone << '\n';
}

void searchDoctor()
{
    int id;
    cout << "Doctor ID: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    cin.ignore(10000, '\n');

    int pos = findDoctorPosition(id);
    if (pos == -1)
    {
        cout << "Doctor not found.\n";
        return;
    }

    const Doctor &d = doctors[pos];
    cout << "Doctor ID: " << d.id << "\nName: Dr. " << d.name
         << "\nSpecialization: " << d.specialization
         << "\nPhone: " << d.phone << '\n';
}

void searchDoctorsBySpecialization()
{
    string specialty;
    cout << "Enter specialization: ";
    getline(cin, specialty);

    bool found = false;
    for (const Doctor &d : doctors)
    {
        if (d.specialization == specialty)
        {
            cout << "Doctor ID: " << d.id << " | Dr. " << d.name
                 << " | " << d.specialization << '\n';
            found = true;
        }
    }

    if (!found)
        cout << "No doctor found for that specialization.\n";
}

// -------------------- APPOINTMENTS --------------------

void bookAppointment()
{
    Appointment a;
    a.id = nextAppointmentId++;

    cout << "\nPatient ID: ";
    if (!(cin >> a.patientId))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid patient ID.\n";
        nextAppointmentId--;
        return;
    }

    cout << "Doctor ID: ";
    if (!(cin >> a.doctorId))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid doctor ID.\n";
        nextAppointmentId--;
        return;
    }
    cin.ignore(10000, '\n');

    if (findPatientPosition(a.patientId) == -1)
    {
        cout << "Patient ID does not exist.\n";
        nextAppointmentId--;
        return;
    }
    if (findDoctorPosition(a.doctorId) == -1)
    {
        cout << "Doctor ID does not exist.\n";
        nextAppointmentId--;
        return;
    }

    cout << "Date (YYYY-MM-DD): ";
    getline(cin, a.date);
    cout << "Time (HH:MM): ";
    getline(cin, a.time);

    for (const Appointment &old : appointments)
    {
        if (old.doctorId == a.doctorId &&
            old.date == a.date && old.time == a.time)
        {
            cout << "This doctor already has an appointment at that time.\n";
            nextAppointmentId--;
            return;
        }
    }

    appointments.push_back(a);
    saveAppointments();
    cout << "Appointment booked. Appointment ID: " << a.id << '\n';
}

void displayAppointments()
{
    if (appointments.empty())
    {
        cout << "\nNo appointments booked.\n";
        return;
    }

    sort(appointments.begin(), appointments.end(),
         [](const Appointment &a, const Appointment &b)
         {
             if (a.date == b.date)
                 return a.time < b.time;
             return a.date < b.date;
         });

    cout << "\n--- ALL APPOINTMENTS ---\n";
    for (const Appointment &a : appointments)
    {
        int p = findPatientPosition(a.patientId);
        int d = findDoctorPosition(a.doctorId);
        string patientName = (p == -1) ? "Unknown" : patients[p].name;
        string doctorName = (d == -1) ? "Unknown" : doctors[d].name;

        cout << "Appointment " << a.id << " | Patient: " << patientName
             << " (ID " << a.patientId << ") | Dr. " << doctorName
             << " (ID " << a.doctorId << ") | " << a.date << " " << a.time << '\n';
    }
}

void viewDoctorAppointments(int doctorId)
{
    if (findDoctorPosition(doctorId) == -1)
    {
        cout << "Doctor not found.\n";
        return;
    }

    vector<Appointment> ownAppointments;
    for (const Appointment &a : appointments)
    {
        if (a.doctorId == doctorId)
            ownAppointments.push_back(a);
    }

    sort(ownAppointments.begin(), ownAppointments.end(),
         [](const Appointment &a, const Appointment &b)
         {
             if (a.date == b.date)
                 return a.time < b.time;
             return a.date < b.date;
         });

    int d = findDoctorPosition(doctorId);
    cout << "\n--- APPOINTMENTS FOR DR. " << doctors[d].name << " ---\n";

    if (ownAppointments.empty())
    {
        cout << "No appointments scheduled.\n";
        return;
    }

    for (const Appointment &a : ownAppointments)
    {
        int p = findPatientPosition(a.patientId);
        string patientName = (p == -1) ? "Unknown" : patients[p].name;
        cout << a.date << " " << a.time << " | Patient: " << patientName
             << " | Patient ID: " << a.patientId << '\n';
    }
}

// -------------------- DOCTOR-WISE QUEUES --------------------

void addPatientToDoctorQueue()
{
    int patientId, doctorId, type;

    cout << "Patient ID: ";
    if (!(cin >> patientId))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid patient ID.\n";
        return;
    }
    cout << "Doctor ID: ";
    if (!(cin >> doctorId))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid doctor ID.\n";
        return;
    }

    if (findPatientPosition(patientId) == -1 ||
        findDoctorPosition(doctorId) == -1)
    {
        cin.ignore(10000, '\n');
        cout << "Patient or doctor not found.\n";
        return;
    }

    cout << "1. Regular patient\n2. Emergency patient\nChoose: ";
    if (!(cin >> type))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid choice.\n";
        return;
    }
    cin.ignore(10000, '\n');

    if (type == 1)
    {
        regularQueues[doctorId].push(patientId);
        cout << "Patient added to this doctor's regular queue.\n";
    }
    else if (type == 2)
    {
        int priority;
        cout << "Priority (1 = most urgent, 2 = urgent, 3 = less urgent): ";
        if (!(cin >> priority))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid priority.\n";
            return;
        }
        cin.ignore(10000, '\n');

        if (priority < 1 || priority > 3)
        {
            cout << "Priority must be 1, 2, or 3.\n";
            return;
        }

        EmergencyPatient e;
        e.patientId = patientId;
        e.priority = priority;
        e.order = queueOrder++;
        emergencyQueues[doctorId].push(e);
        cout << "Emergency patient added to this doctor's queue.\n";
    }
    else
    {
        cout << "Invalid queue type.\n";
    }
}

void displayDoctorQueue(int doctorId)
{
    int doctorPos = findDoctorPosition(doctorId);
    if (doctorPos == -1)
    {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "\n--- QUEUE FOR DR. " << doctors[doctorPos].name << " ---\n";

    priority_queue<EmergencyPatient> emergencyCopy = emergencyQueues[doctorId];
    queue<int> regularCopy = regularQueues[doctorId];

    int emergencyCount = (int)emergencyCopy.size();
    int regularCount = (int)regularCopy.size();

    cout << "Emergency patients (served before regular patients):\n";
    if (emergencyCopy.empty())
        cout << "  None\n";

    int emergencyPosition = 0;
    while (!emergencyCopy.empty())
    {
        EmergencyPatient e = emergencyCopy.top();
        emergencyCopy.pop();
        emergencyPosition++;

        int p = findPatientPosition(e.patientId);
        string name = (p == -1) ? "Unknown" : patients[p].name;
        cout << "  " << emergencyPosition << ". " << name
             << " (ID " << e.patientId << "), priority " << e.priority
             << ", estimated wait about " << (emergencyPosition - 1) * 10
             << " min\n";
    }

    cout << "Regular patients (FIFO):\n";
    if (regularCopy.empty())
        cout << "  None\n";

    int regularPosition = 0;
    while (!regularCopy.empty())
    {
        int id = regularCopy.front();
        regularCopy.pop();
        regularPosition++;

        int p = findPatientPosition(id);
        string name = (p == -1) ? "Unknown" : patients[p].name;
        int waitMinutes = (emergencyCount + regularPosition - 1) * 10;

        cout << "  " << regularPosition << ". " << name
             << " (ID " << id << "), estimated wait about "
             << waitMinutes << " min\n";
    }

    cout << "Assumption: each consultation takes 10 minutes. Times are estimates only.\n";
    (void)regularCount;
}

void showEstimatedWait(int doctorId)
{
    displayDoctorQueue(doctorId);
}

void callNextPatient(int doctorId)
{
    if (findDoctorPosition(doctorId) == -1)
    {
        cout << "Doctor not found.\n";
        return;
    }

    int patientId;
    if (!emergencyQueues[doctorId].empty())
    {
        EmergencyPatient e = emergencyQueues[doctorId].top();
        emergencyQueues[doctorId].pop();
        patientId = e.patientId;
        cout << "\nCalling emergency patient (priority " << e.priority << ").\n";
    }
    else if (!regularQueues[doctorId].empty())
    {
        patientId = regularQueues[doctorId].front();
        regularQueues[doctorId].pop();
        cout << "\nCalling next regular patient.\n";
    }
    else
    {
        cout << "\nThis doctor's queue is empty.\n";
        return;
    }

    int p = findPatientPosition(patientId);
    if (p == -1)
    {
        cout << "Patient record no longer exists.\n";
        return;
    }

    cout << "Patient: " << patients[p].name << " (ID " << patientId << ")\n";
    string note;
    cout << "Visit note: ";
    getline(cin, note);

    Visit v;
    v.patientId = patientId;
    v.doctorId = doctorId;
    v.note = note;
    visits.push_back(v);
    saveVisits();
    cout << "Visit recorded.\n";
}

// -------------------- SORTING, REPORTS AND VISITS --------------------

void sortPatientsByName()
{
    sort(patients.begin(), patients.end(),
         [](const Patient &a, const Patient &b) { return a.name < b.name; });

    rebuildPatientIndex();
    cout << "Patients sorted by name.\n";
    displayPatients();
}

void showReport()
{
    int waitingRegular = 0;
    int waitingEmergency = 0;

    for (const auto &entry : regularQueues)
        waitingRegular += (int)entry.second.size();

    for (const auto &entry : emergencyQueues)
        waitingEmergency += (int)entry.second.size();

    cout << "\n--- HOSPITAL SUMMARY ---\n";
    cout << "Total patients: " << patients.size() << '\n';
    cout << "Total doctors: " << doctors.size() << '\n';
    cout << "Total appointments: " << appointments.size() << '\n';
    cout << "Regular patients waiting: " << waitingRegular << '\n';
    cout << "Emergency patients waiting: " << waitingEmergency << '\n';
    cout << "Visits recorded: " << visits.size() << '\n';
}

void showVisitHistory()
{
    if (visits.empty())
    {
        cout << "\nNo visits recorded.\n";
        return;
    }

    cout << "\n--- VISIT HISTORY ---\n";
    for (const Visit &v : visits)
    {
        int p = findPatientPosition(v.patientId);
        int d = findDoctorPosition(v.doctorId);
        string patientName = (p == -1) ? "Unknown" : patients[p].name;
        string doctorName = (d == -1) ? "Unknown" : doctors[d].name;

        cout << "Patient: " << patientName << " (ID " << v.patientId << ")"
             << " | Doctor: Dr. " << doctorName << " (ID " << v.doctorId << ")"
             << " | Note: " << v.note << '\n';
    }
}

// -------------------- MENUS --------------------

void showAdminMenu()
{
    cout << "\n========== ADMIN / RECEPTION MENU ==========\n";
    cout << "PATIENTS\n";
    cout << "1. Register patient\n2. Display patients\n3. Search patient\n";
    cout << "4. Update patient\n5. Delete patient\n";
    cout << "DOCTORS\n";
    cout << "6. Register doctor\n7. Display doctors\n8. Search doctor\n";
    cout << "9. Search doctors by specialization\n";
    cout << "APPOINTMENTS AND QUEUES\n";
    cout << "10. Book appointment\n11. Display all appointments\n";
    cout << "12. Add patient to a doctor's queue\n";
    cout << "13. Display a doctor's queue and estimated waits\n";
    cout << "REPORTS\n";
    cout << "14. Sort patients by name\n15. Hospital summary\n16. Visit history\n";
    cout << "0. Save and return to role selection\n";
    cout << "Choice: ";
}

void runAdminMenu()
{
    int choice = -1;

    do
    {
        showAdminMenu();
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        switch (choice)
        {
            case 1: registerPatient(); break;
            case 2: displayPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: registerDoctor(); break;
            case 7: displayDoctors(); break;
            case 8: searchDoctor(); break;
            case 9: searchDoctorsBySpecialization(); break;
            case 10: bookAppointment(); break;
            case 11: displayAppointments(); break;
            case 12: addPatientToDoctorQueue(); break;
            case 13:
            {
                int id;
                cout << "Doctor ID: ";
                if (cin >> id)
                {
                    cin.ignore(10000, '\n');
                    showEstimatedWait(id);
                }
                else
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid doctor ID.\n";
                }
                break;
            }
            case 14: sortPatientsByName(); break;
            case 15: showReport(); break;
            case 16: showVisitHistory(); break;
            case 0: saveAllData(); break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void runDoctorMenu()
{
    int doctorId;
    cout << "\nEnter your Doctor ID: ";
    if (!(cin >> doctorId))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid Doctor ID.\n";
        return;
    }
    cin.ignore(10000, '\n');

    int pos = findDoctorPosition(doctorId);
    if (pos == -1)
    {
        cout << "Doctor not found. Ask the admin to register you first.\n";
        return;
    }

    int choice = -1;
    do
    {
        pos = findDoctorPosition(doctorId);
        cout << "\n========== DOCTOR DASHBOARD ==========\n";
        cout << "Welcome, Dr. " << doctors[pos].name << " (ID " << doctorId << ")\n";
        cout << "1. View my appointments\n";
        cout << "2. View my patient queue and estimated wait times\n";
        cout << "3. Call next patient and record visit\n";
        cout << "0. Log out\n";
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        switch (choice)
        {
            case 1: viewDoctorAppointments(doctorId); break;
            case 2: displayDoctorQueue(doctorId); break;
            case 3: callNextPatient(doctorId); break;
            case 0: cout << "Logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
