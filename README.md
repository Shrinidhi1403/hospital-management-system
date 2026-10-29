# SmartCare — Hospital Management System

A beginner-friendly C++17 DSAL mini-project for patient and doctor management, appointment scheduling, doctor-wise waiting queues, hashing, sorting, and file handling.

## Features

### Patient management
- Register, display, search, update, and delete patients.
- Search by patient ID using an `unordered_map` index.

### Doctor management
- Register doctors with a generated Doctor ID, name, specialization, and phone.
- Display and search doctors by ID.
- Filter doctors by specialization.

### Appointments
- Book appointments by Patient ID and Doctor ID.
- Reject appointments for missing patients/doctors.
- Reject duplicate booking of the same doctor at the same date and time.
- View all appointments or a specific doctor's schedule.

### Doctor interface and queues
- Select the Doctor role and enter a registered Doctor ID.
- View that doctor's appointments and queue.
- Each doctor has a separate regular FIFO queue and emergency priority queue.
- Emergency patients are called before regular patients; emergency cases are ordered by priority, with earlier arrival first when priorities match.
- Display estimated wait times using an assumed 10-minute consultation duration.
- Call the next patient and record a visit.

### Reports and persistence
- Sort patients by name.
- Display a hospital summary and visit history.
- Save patients, doctors, appointments, and visits in text files.

## Project structure

```text
SmartCare_Doctor_Extended/
├── main.cpp
├── hospital.cpp
├── hospital.h
├── storage.cpp
├── storage.h
├── models.h
├── Makefile
├── README.md
├── .gitignore
├── data/
│   └── .gitkeep
└── tests/
    └── manual_test_plan.md
```

- `main.cpp`: role selection and application entry point.
- `models.h`: data structures for patients, doctors, appointments, visits, and emergency queue entries.
- `hospital.h`: declarations of shared data and functions.
- `hospital.cpp`: patient, doctor, appointment, queue, report, and menu logic.
- `storage.h` and `storage.cpp`: declarations and implementations for loading and saving records.

## Requirements

- C++17 compiler such as `g++`.
- Linux, macOS, or Windows with a compatible C++ environment.
- `make` is optional.

## Compile and run

From the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp hospital.cpp storage.cpp -o smartcare
./smartcare
```

Or:

```bash
make
./smartcare
```

Run the program from the project directory so the relative `data/` folder is found.

## Data structures

| Structure | Use |
|---|---|
| `vector<Patient>` and `vector<Doctor>` | Store records |
| `unordered_map<int, int>` | Map patient/doctor IDs to vector positions |
| `queue<int>` per doctor | Regular patients in FIFO order |
| `priority_queue<EmergencyPatient>` per doctor | Emergency patients ordered by priority |
| `vector<Appointment>` | Appointment storage and sorting |
| `vector<Visit>` | Visit history |
| File streams | Persistent records |

## Complexity overview

Let `n` be the number of patients, `d` the number of doctors, and `a` the number of appointments.

| Operation | Typical time |
|---|---|
| Patient/doctor ID lookup | Average O(1), worst-case O(n) |
| Display all patients or doctors | O(n) or O(d) |
| Sort patients by name | O(n log n) |
| Push/pop regular queue | O(1) |
| Push/pop emergency priority queue | O(log q), where q is that doctor's emergency queue size |
| Appointment conflict check | O(a) |
| Display a doctor's appointments | O(a), plus sorting matching appointments |
| Load/save records | Linear in the number of stored records |

## Queue rules and waiting-time estimate

Each doctor has independent queues. Emergency cases are served before regular cases. Among emergency cases, priority `1` is most urgent; equal priorities are served in arrival order. Regular patients are served FIFO.

The displayed waiting time is an illustrative estimate using 10 minutes per patient ahead. It is not a guarantee and is not clinical triage advice.

## Persistence and limitations

Patient, doctor, appointment, and visit records are stored in:
- `data/patients.txt`
- `data/doctors.txt`
- `data/appointments.txt`
- `data/visits.txt`

Doctor waiting queues are kept in memory and reset when the program restarts. The role selection and Doctor ID prompt are a demonstration only, not secure authentication. Dates and times are text fields and are not fully calendar-validated. This project is an academic prototype, not production hospital software. Do not enter real patient information.

## Suggested demonstration

1. Register a patient and a doctor.
2. Book an appointment and demonstrate duplicate-slot rejection.
3. Register a second doctor and show that each doctor has a separate schedule.
4. Add regular and emergency patients to Doctor A's queue.
5. Enter Doctor mode using Doctor A's ID; display the queue and wait estimates.
6. Call the next patient and record a visit.
7. Show reports and visit history.
8. Exit and restart to demonstrate file persistence.

## Team

- Shri
- Siddhant
- Ekaksh
- Arshad
- Chinmay

## Academic use

Prepared as a Data Structures and Applications Laboratory mini-project.
