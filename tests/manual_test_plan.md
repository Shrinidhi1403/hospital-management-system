# SmartCare Extended Manual Test Plan

Record actual results before the final demonstration.

## Patient and doctor records
1. Register two patients and display them.
2. Search an existing patient ID and a missing ID.
3. Register two doctors with different specializations.
4. Search doctor by ID and by specialization.
5. Restart the app and confirm patient and doctor records persist.

## Appointments
6. Book an appointment for an existing patient and doctor.
7. Try booking the same doctor at the same date and time; it should be rejected.
8. View all appointments and a doctor's personal schedule.

## Doctor-wise queues
9. Add a regular patient to Doctor A's queue.
10. Add an emergency patient to Doctor A's queue.
11. Add a patient to Doctor B's queue and verify it does not appear in Doctor A's queue.
12. View queue order and estimated wait. The program assumes 10 minutes per consultation.
13. Enter Doctor mode, use Doctor A's ID, view the schedule and queue, then call the next patient.
14. Try an unknown Doctor ID.

## Reports and persistence
15. Sort patients by name and check patient search still works.
16. Complete a visit and view visit history.
17. Exit and restart; patient, doctor, appointment and visit records should reload.

Limitations to explain: doctor queues are in-memory only and reset after restart; login is simulated by selecting a role and entering a doctor ID, not secure authentication; estimated waits are illustrative, not guaranteed.
