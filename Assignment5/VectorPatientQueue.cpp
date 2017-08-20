/**
 * @brief The following code involves methods neccessary to make a Vector behave as a priority
 * queue. It completes the method definitions of the VectorPatientQueue class.
 * SectionLeader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#include "VectorPatientQueue.h"
#include "vector.h"
#include "strlib.h"

/**
 * @brief VectorPatientQueue::VectorPatientQueue The default constructor of the VectorPatientQueue
 * class. Initializes an empty Vector which will behave as a priority queue of patients.
 */
VectorPatientQueue::VectorPatientQueue() {}

/**
 * @brief VectorPatientQueue::~VectorPatientQueue The destructor of the VectorPatientQueue class.
 * The Vector of Stanford C++ Library handles memory leaks by itself. Thus, the destructor is
 * only defined.
 */
VectorPatientQueue::~VectorPatientQueue() {}

/**
 * @brief VectorPatientQueue::clear Removes all elements in the collection of patients.
 */
void VectorPatientQueue::clear() {
    patients.clear(); //removing all elements from the vector
}

/**
 * @brief VectorPatientQueue::frontName Returns the name of the most urgent patient in the queue of
 * patients without removing the patient from the queue. Throws a string exception if there are no
 * patients in the queue of patients.
 * @return The name of the most urgent patient.
 */
string VectorPatientQueue::frontName() {
    if (patients.isEmpty()) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        int outgoing = 0;
        for (int i = 1; i < patients.size(); i++) {
            if (patients[i].priority < patients[outgoing].priority) {
                outgoing = i;
            } else if (patients[i].priority == patients[outgoing].priority) {
                if (patients[i].timestamp < patients[outgoing].timestamp) {
                    outgoing = i;
                }
            }
        }
        return patients[outgoing].name;
    }
}

/**
 * @brief VectorPatientQueue::frontPriority Returns the priority of the most urgent patient in the
 * queue of patients without removing the patient from the queue. Throws a string exception if
 * there are no patients in the queue of patients.
 * @return The priority of the most urgent patient.
 */
int VectorPatientQueue::frontPriority() {
    if (patients.isEmpty()) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        int outgoing = 0;
        for (int i = 1; i < patients.size(); i++) {
            if (patients[i].priority < patients[outgoing].priority) {
                outgoing = i;
            }
        }
        return patients[outgoing].priority;
    }
}

/**
 * @brief VectorPatientQueue::isEmpty Returns false if there are at least one person in the queue
 * of patients and returns true if the queue is empty.
 * @return True if the queue is empty, false otherwise.
 */
bool VectorPatientQueue::isEmpty() {
    return patients.isEmpty();
}

/**
 * @brief VectorPatientQueue::newPatient Enques a new patient to the queue of patients. Incoming
 * Patients have a priority level and a name.
 * @param name The name of the incoming patient.
 * @param priority The priority level of the incoming patient.
 */
void VectorPatientQueue::newPatient(string name, int priority) {
    Patient incoming = {name, priority, patients.size() + 1};
    patients.add(incoming);
}

/**
 * @brief VectorPatientQueue::processPatient Dequeues the paitent with the highest priority from
 * the queue of patients. Returns the name of the dequeued patient. Throws a string exception if
 * there are no patients in the queue of patients.
 * @return The name of the dequeued patient.
 */
string VectorPatientQueue::processPatient() {
    if (patients.isEmpty()) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        int outgoing = 0;
        for (int i = 1; i < patients.size(); i++) {
            if (patients[i].priority < patients[outgoing].priority) {
                outgoing = i;
            } else if (patients[i].priority == patients[outgoing].priority) {
                if (patients[i].timestamp < patients[outgoing].timestamp) {
                    outgoing = i;
                }
            }
        }
        string patientName = patients[outgoing].name;
        patients.remove(outgoing);
        return patientName;
    }
}

/**
 * @brief VectorPatientQueue::upgradePatient Modifies an existing patient's priority. Searches the
 * patient by name, then changes the priority to be more urgent. Throws a string exception if the
 * existing person has a higher priority or the person does not exist in the queue.
 * @param name The patient's name, which will be searched in the queue.
 * @param newPriority The given priority that will be changed with the searched patient's priority.
 */
void VectorPatientQueue::upgradePatient(string name, int newPriority) {
    if (patients.isEmpty()) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        int toModify = 0;
        for (int i = 1; i < patients.size(); i++) {
            if (patients[i].name == name) {
                if (patients[toModify].name == name) {
                    if (patients[toModify].priority > patients[i].priority) {
                        toModify = i;
                    } else if (patients[toModify].priority == patients[i].priority &&
                               patients[toModify].timestamp > patients[i].timestamp) {
                        toModify = i;
                    }
                } else {
                    toModify = i;
                }
            }
        }
        if (patients[toModify].name != name) { //error case
            throw("Invalid Operation: There is no patient with the given name.");
        } else if (patients[toModify].priority < newPriority) { //error case
            throw("Invalid Operation: The priority of the patient is already greater than the"
            " new priority.");
        } else {
            patients[toModify].priority = newPriority;
        }
    }
}

/**
 * @brief VectorPatientQueue::toString Returns a string representation of the patient queue.
 * The returned string involves information such as the names and priority values of the
 * patients.
 * @return The string representation of the patient queue.
 */
string VectorPatientQueue::toString() {
    string patientQueue = "";
    for (int i = 0; i < patients.size(); i++) {
        if (i == patients.size() - 1) {
            patientQueue += integerToString(patients[i].priority) + ":" + patients[i].name;
        } else {
            patientQueue += integerToString(patients[i].priority) + ":" + patients[i].name + ", ";
        }
    }
    return "{" + patientQueue + "}";
}




