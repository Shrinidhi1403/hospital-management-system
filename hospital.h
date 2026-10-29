#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include "models.h"

using namespace std;

extern vector<Patient> patients;
extern vector<Doctor> doctors;
extern vector<Appointment> appointments;
extern vector<Visit> visits;

// Patient ID -> position in patients vector; Doctor ID -> position in doctors vector.
extern unordered_map<int, int> patientIndex;
extern unordered_map<int, int> doctorIndex;

// Each doctor has their own regular and emergency queues.
extern map<int, queue<int>> regularQueues;
extern map<int, priority_queue<EmergencyPatient>> emergencyQueues;

extern int nextPatientId;
extern int nextDoctorId;
extern int nextAppointmentId;
extern int queueOrder;

// Index and lookup helpers
void rebuildPatientIndex();
void rebuildDoctorIndex();
int findPatientPosition(int id);
int findDoctorPosition(int id);

// Patient management
void registerPatient();
void displayPatients();
void searchPatient();
void updatePatient();
void deletePatient();

// Doctor management
void registerDoctor();
void displayDoctors();
void searchDoctor();
void searchDoctorsBySpecialization();

// Appointments
void bookAppointment();
void displayAppointments();
void viewDoctorAppointments(int doctorId);

// Queue management
void addPatientToDoctorQueue();
void displayDoctorQueue(int doctorId);
void callNextPatient(int doctorId);
void showEstimatedWait(int doctorId);

// Reports and visits
void sortPatientsByName();
void showReport();
void showVisitHistory();

// Role-based menus
void showAdminMenu();
void runAdminMenu();
void runDoctorMenu();

#endif
