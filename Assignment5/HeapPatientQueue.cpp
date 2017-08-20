/**
 * @brief The following code involves methods neccessary to make a Heap behave as a priority
 * queue. It completes the method definitions of the HeapPatientQueue class.
 * SectionLeader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

//THIS IS AN EXTENSION

#include "HeapPatientQueue.h"
#include "strlib.h"

/**
 * @brief HeapPatientQueue::HeapPatientQueue The default constructor for the queue of patients.
 * Initializes the binary heap's size and locates heap memory for it.
 */
HeapPatientQueue::HeapPatientQueue() {
    capacity = INITIAL_CAPACITY;
    patients = new HeapPatient[capacity];
    count = 0;
}

/**
 * @brief HeapPatientQueue::~HeapPatientQueue The destructor of the queue of patients. Deletes
 * the binary heap from the memory.
 */
HeapPatientQueue::~HeapPatientQueue() {
    delete [] patients;
}

/**
 * @brief HeapPatientQueue::clear Clears the binary heap and deletes the memory it covers.
 * After the call, patients pointer points to an empty binary heap.
 */
void HeapPatientQueue::clear() {
    delete [] patients;
    capacity = INITIAL_CAPACITY;
    patients = new HeapPatient[capacity];
    count = 0;
}

/**
 * @brief HeapPatientQueue::frontName Returns the name of the most urgent patient in the queue of
 * patients, without removing him/her from the queue. In this case, returns the name of the root of
 * the binary heap. Throws a string exception if there are no patients in the queue.
 * @return The name of the most urgent patient in the queue of patients.
 */
string HeapPatientQueue::frontName() {
    if (count == 0) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        return patients[1].name; //root is index 1 in order to make the arithmetic simpler
    }
}

/**
 * @brief HeapPatientQueue::frontName Returns the priority of the most urgent patient in the queue
 * of patients, without removing him/her from the queue. In this case, returns the priority of the
 * root of the binary heap. Throws a string exception if there are no patients in the queue.
 * @return The priority level of the most urgent patient in the queue of patients.
 */
int HeapPatientQueue::frontPriority() {
    if (count == 0) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        return patients[1].priority; //root is index 1 in order to make the arithmetic simpler
    }
}

/**
 * @brief VectorPatientQueue::isEmpty Returns false if there are at least one person in the queue
 * of patients and returns true if the queue is empty.
 * @return True if the queue is empty, false otherwise.
 */
bool HeapPatientQueue::isEmpty() {
    return count == 0;
}

/**
 * @brief HeapPatientQueue::newPatient Enqueues a new patient with the given name and priority to
 * the queue of patients. Performs "bubble up" algorithm to place the enqued element to the binary
 * heap.
 * @param name The name of the incoming patient.
 * @param priority The priority level of the incoming patient.
 */
void HeapPatientQueue::newPatient(string name, int priority) {
    if (count + 1 == capacity) {
        expand();
    }
    HeapPatient incoming = {name, priority};
    patients[count + 1] = incoming;
    count++;
    bubbleUp(count);
}

/**
 * @brief HeapPatientQueue::processPatient Dequeus the most urgent patient from the queue of
 * patients. Performs "bubble down" algorithm to reform the binary heap after the removal.
 * Throws a string exception if the queue is empty.
 * @return The name of the dequeued patient.
 */
string HeapPatientQueue::processPatient() {
    if (count == 0) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    } else {
        string outgoing = patients[1].name;
        patients[1] = patients[count];
        count--;
        if (count < capacity / 2) {
            shrink();
        }
        bubbleDown(1);
        return outgoing;
    }
}

/**
 * @brief HeapPatientQueue::upgradePatient Upgrades the priority level of the patient with the
 * given name to be more urgent. Throws a string exception if the queue is empty or there is no
 * such patient or newPriority is not more urgent then the old one.
 * @param name The name of the patient whose priority level will be updated.
 * @param newPriority The new priority level of the patient.
 */
void HeapPatientQueue::upgradePatient(string name, int newPriority) {
    if (count == 0) { //error case
        throw("Invalid Operation: The queue of patients is empty.");
    }
    int index = 0;
    for (int i = 1; i <= count; i++) {
        if (patients[i].name == name) {
            if (patients[i].priority >= newPriority) {
                patients[i].priority = newPriority;
            } else { //error case
                throw("Invalid Operation: The priority of the patient is already greater than the"
                " new priority.");
            }
            index = i;
            break;
        }
    }
    if (index == 0) { //error case
        throw("Invalid Operation: There is no patient with the given name.");
    } else {
        bubbleUp(index);
    }
}

/**
 * @brief VectorPatientQueue::toString Returns a string representation of the patient queue.
 * The returned string involves information such as the names and priority values of the
 * patients.
 * @return The string representation of the patient queue.
 */
string HeapPatientQueue::toString() {
    string patientQueue = "{";
    for (int i = 1; i <= count; i++) {
        if (i == count) {
            patientQueue += integerToString(patients[i].priority) + ":" + patients[i].name;
        } else {
            patientQueue += integerToString(patients[i].priority) + ":" + patients[i].name + ", ";
        }
    }
    return patientQueue + "}";
}

/**
 * @brief HeapPatientQueue::expand Private function helping the other functions when count is
 * about to exceed capacity. Expands the binary heap exponentially by a factor of 2. Clears
 * the old memory and copies the original values to the expanded binary heap.
 */
void HeapPatientQueue::expand() {
    HeapPatient* newPatients = new HeapPatient[capacity * 2]; //locating new heap memory
    for (int i = 1; i <= count; i++) { //copying values to upgraded array
        newPatients[i] = patients[i];
    }
    delete [] patients; //deleting the memory occupied by the old array
    patients = newPatients; //making patients point to the upgraded array
    capacity *= 2; //exponentially updating the capacity (by a factor of 2)
}

/**
 * @brief HeapPatientQueue::expand Private function helping the other functions when count is
 * half of the capacity. Shrinks the binary heap exponentially by a factor of 0.5. Clears
 * the old memory and copies the original values to the shrinked binary heap.
 */
void HeapPatientQueue::shrink() {
    if (capacity != INITIAL_CAPACITY) {
        HeapPatient* newPatients = new HeapPatient[capacity / 2]; //locating new heap memory
        for (int i = 1; i <= count; i++) { //copying values to upgraded array
            newPatients[i] = patients[i];
        }
        delete [] patients; //deleting the memory occupied by the old array
        patients = newPatients; //making patients point to the upgraded array
        capacity /= 2; //exponentially updating the capacity (by a factor of 0.5)
    }
}

/**
 * @brief HeapPatientQueue::bubbleUp Private helper function implementing the bubble-up algorithm.
 */
void HeapPatientQueue::bubbleUp(int index) {
    while (index > 1 && patients[index].priority <= patients[index / 2].priority) {
        if (patients[index].priority == patients[index / 2].priority) {
            if (patients[index].name <= patients[index / 2].name) {
                swap(index, index / 2); //breaking ties by string comparison
            }
        } else {
            swap(index, index / 2);
        }
        index /= 2;
    }
}

/**
 * @brief HeapPatientQueue::bubbleDown Private helper function implementing the bubble-down
 * algorithm.
 */
void HeapPatientQueue::bubbleDown(int index) {
    int left = index * 2;
    while (left <= count) {
        int moreUrgent = left;
        int right = left + 1;
        if (right <= count) {
            if (patients[right].priority <= patients[left].priority) {
                if (patients[right].priority == patients[left].priority) {
                    if (patients[right].name <= patients[left].name) {
                        moreUrgent++; //breaking ties by string comparison
                    }
                } else {
                    moreUrgent++;
                }
            }
        }
        if (patients[moreUrgent].priority <= patients[index].priority) {
            if (patients[moreUrgent].priority == patients[index].priority) {
                if (patients[moreUrgent].name <= patients[index].name) {
                    swap(index, moreUrgent); //breaking ties by string comparison
                }
            } else {
                swap(index, moreUrgent);
            }
            index = moreUrgent;
            left = index * 2;
        } else {
            break;
        }
    }
}

/**
 * @brief HeapPatientQueue::swap Private function helping other functions when two patients need
 * to be swapped.
 * @param index1 The index of the first element to swap.
 * @param index2 The index of the second element to swap.
 */
void HeapPatientQueue::swap(int index1, int index2) {
    HeapPatient temp = patients[index1];
    patients[index1] = patients[index2];
    patients[index2] = temp;
}
