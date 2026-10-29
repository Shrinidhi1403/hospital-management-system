#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "models.h"
#include "hospital.h"
#include "storage.h"

using namespace std;

string cleanText(string text)
{
    for (char &ch : text)
    {
        if (ch == '|')
            ch = '/';
    }
    return text;
}

void savePatients()
{
    ofstream file("data/patients.txt");
    if (!file)
    {
        cout << "Could not write data/patients.txt\n";
        return;
    }

    for (const Patient &p : patients)
        file << p.id << '|' << cleanText(p.name) << '|' << p.age << '|'
             << cleanText(p.phone) << '|' << cleanText(p.diagnosis) << '\n';
}

void saveDoctors()
{
    ofstream file("data/doctors.txt");
    if (!file)
    {
        cout << "Could not write data/doctors.txt\n";
        return;
    }

    for (const Doctor &d : doctors)
        file << d.id << '|' << cleanText(d.name) << '|'
             << cleanText(d.specialization) << '|'
             << cleanText(d.phone) << '\n';
}

void saveAppointments()
{
    ofstream file("data/appointments.txt");
    if (!file)
    {
        cout << "Could not write data/appointments.txt\n";
        return;
    }

    for (const Appointment &a : appointments)
        file << a.id << '|' << a.patientId << '|' << a.doctorId << '|'
             << a.date << '|' << a.time << '\n';
}

void saveVisits()
{
    ofstream file("data/visits.txt");
    if (!file)
    {
        cout << "Could not write data/visits.txt\n";
        return;
    }

    for (const Visit &v : visits)
        file << v.patientId << '|' << v.doctorId << '|'
             << cleanText(v.note) << '\n';
}

void saveAllData()
{
    savePatients();
    saveDoctors();
    saveAppointments();
    saveVisits();
}

void loadData()
{
    string line;

    // Load patients.
    ifstream patientFile("data/patients.txt");
    while (getline(patientFile, line))
    {
        stringstream ss(line);
        string part;
        Patient p;

        try
        {
            if (!getline(ss, part, '|')) continue;
            p.id = stoi(part);
            if (!getline(ss, p.name, '|')) continue;
            if (!getline(ss, part, '|')) continue;
            p.age = stoi(part);
            if (!getline(ss, p.phone, '|')) continue;
            if (!getline(ss, p.diagnosis, '|')) continue;
        }
        catch (...)
        {
            continue;
        }

        patients.push_back(p);
        if (p.id >= nextPatientId)
            nextPatientId = p.id + 1;
    }
    rebuildPatientIndex();

    // Load doctors.
    ifstream doctorFile("data/doctors.txt");
    while (getline(doctorFile, line))
    {
        stringstream ss(line);
        string part;
        Doctor d;

        try
        {
            if (!getline(ss, part, '|')) continue;
            d.id = stoi(part);
            if (!getline(ss, d.name, '|')) continue;
            if (!getline(ss, d.specialization, '|')) continue;
            if (!getline(ss, d.phone, '|')) continue;
        }
        catch (...)
        {
            continue;
        }

        doctors.push_back(d);
        if (d.id >= nextDoctorId)
            nextDoctorId = d.id + 1;
    }
    rebuildDoctorIndex();

    // Load appointments.
    ifstream appointmentFile("data/appointments.txt");
    while (getline(appointmentFile, line))
    {
        stringstream ss(line);
        string part;
        Appointment a;

        try
        {
            if (!getline(ss, part, '|')) continue;
            a.id = stoi(part);
            if (!getline(ss, part, '|')) continue;
            a.patientId = stoi(part);
            if (!getline(ss, part, '|')) continue;
            a.doctorId = stoi(part);
            if (!getline(ss, a.date, '|')) continue;
            if (!getline(ss, a.time, '|')) continue;
        }
        catch (...)
        {
            continue;
        }

        // Ignore orphaned appointments if their patient or doctor is missing.
        if (findPatientPosition(a.patientId) == -1 ||
            findDoctorPosition(a.doctorId) == -1)
            continue;

        appointments.push_back(a);
        if (a.id >= nextAppointmentId)
            nextAppointmentId = a.id + 1;
    }

    // Load visit history.
    ifstream visitFile("data/visits.txt");
    while (getline(visitFile, line))
    {
        stringstream ss(line);
        string part;
        Visit v;

        try
        {
            if (!getline(ss, part, '|')) continue;
            v.patientId = stoi(part);
            if (!getline(ss, part, '|')) continue;
            v.doctorId = stoi(part);
            if (!getline(ss, v.note, '|')) continue;
        }
        catch (...)
        {
            continue;
        }

        visits.push_back(v);
    }
}
