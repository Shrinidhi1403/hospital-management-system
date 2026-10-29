#ifndef MODELS_H
#define MODELS_H

#include <string>

using namespace std;

struct Patient
{
    int id;
    string name;
    int age;
    string phone;
    string diagnosis;
};

struct Doctor
{
    int id;
    string name;
    string specialization;
    string phone;
};

struct Appointment
{
    int id;
    int patientId;
    int doctorId;
    string date;   // YYYY-MM-DD
    string time;   // HH:MM
};

struct Visit
{
    int patientId;
    int doctorId;
    string note;
};

struct EmergencyPatient
{
    int patientId;
    int priority;  // 1 = most urgent, 3 = least urgent
    int order;     // Earlier arrival wins if priorities match

    // priority_queue normally puts the largest item first.
    // Reverse the comparison so smaller priority values come first.
    bool operator<(const EmergencyPatient& other) const
    {
        if (priority == other.priority)
            return order > other.order;

        return priority > other.priority;
    }
};

#endif
