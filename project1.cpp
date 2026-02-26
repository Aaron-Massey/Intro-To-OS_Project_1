#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
// Process States
enum State { NEW, READY, RUNNING, TERMINATED };

class PCB {
  // Store the process ID, program counter, state, and remaining units
private:
  int pid;
  int pc;
  State state;
  int remaining_work;

public:
  // Constructor for Process Control Blocks
  PCB(int id, int work) : pid(id), pc(1), state(NEW), remaining_work(work) {}

  // Getters
  int getPID() const { return pid; }
  int getPC() const { return pc; }
  State getState() const { return state; }
  int getRemainingWork() const { return remaining_work; }

  // Setters
  void setState(State s) { state = s; }

  // Simulate work
  void execute(int units) {
    remaining_work -= units;
    pc += units;
  }
};

// Display the process and scheduler information
void displaySystemStatus(vector<PCB *> &processes) {
  for (size_t i = 0; i < processes.size(); i++) {
    State s = processes[i]->getState();
    if (s == NEW) {
      continue;
    }
    // Print the current process ID
    cout << "P" << processes[i]->getPID() << " ";

    // Print the process state
    switch (s) {
    case READY:
      cout << "Ready, pc " << processes[i]->getPC();
      break;
    case RUNNING:
      cout << "Running";
      break;
    case TERMINATED:
      cout << "Terminated";
      break;
    default:
      break;
    }
    cout << endl;
  }
  // Remove the displayed process if it is in the TERMINATED state
  auto it = remove_if(processes.begin(), processes.end(), [](PCB *p) {
    if (p->getState() == TERMINATED) {
      delete p; // Clean up memory
      return true;
    }
    return false;
  });
  processes.erase(it, processes.end());
}

// Returns if the id already exists in the process vector
bool doesIdExist(const vector<PCB *> &processes, int id) {
  for (auto p : processes) {
    if (p->getPID() == id) {
      return true;
    }
  }
  return false;
}

int main() {
  // Initialize the work per run and number of processes
  int quantum, num_processes;
  // Validate Inputs
  if (!(cin >> quantum >> num_processes)) {
    cout << "No valid inputs";
    return 1;
  }
  if (quantum < 1) {
    cout << "Invalid Quantum";
    return 1;
  }
  if (num_processes < 1) {
    cout << "Cannot have negative processes";
    return 1;
  }

  // Create datastructures
  vector<PCB *> all_processes;
  queue<PCB *> ready_queue;

  // Input New Processes
  cout << "New processes:" << endl;
  for (int i = 0; i < num_processes; ++i) {
    int id, work;
    cin >> id >> work;

    // Validate Inputs
    if (id < 0) {
      cout << "Cannot have a negative id";
      return 1;
    }
    if (work < 1) {
      cout << "cannot have negative work";
      return 1;
    }
    if (cin.fail()) {
      cout << "ID and Work must be integers";
      return 1;
    }
    if (doesIdExist(all_processes, id)) {
      cout << "Cannot have repeated ID values";
      return 1;
    }

    // Create PCBs and adds them to the queue
    PCB *p = new PCB(id, work);
    all_processes.push_back(p);
    cout << "P" << p->getPID() << endl;
  }
  cout << "--" << endl;

  // Set the processes to READY
  for (auto p : all_processes) {
    p->setState(READY);
    ready_queue.push(p);
  }
  displaySystemStatus(all_processes);
  cout << "--" << endl;

  // Scheduler
  while (!ready_queue.empty()) {
    // Point to the current process
    PCB *current = ready_queue.front();
    ready_queue.pop();

    // Context Switch load
    cout << "Kernel loading P" << current->getPID() << endl;
    cout << "--" << endl;

    // Set process to running and display
    current->setState(RUNNING);
    displaySystemStatus(all_processes);
    cout << "--" << endl;

    // Execute Work (Set the work to be done to the quantum or the rest of the
    // work remaining, depending on what is smaller).
    int work_done = (current->getRemainingWork() < quantum)
                        ? current->getRemainingWork()
                        : quantum;
    current->execute(work_done);

    // Context Switch Save
    cout << "Kernel saving P" << current->getPID() << endl;

    // Update State depending on how much work is left
    if (current->getRemainingWork() > 0) {
      current->setState(READY);
      ready_queue.push(current);
    } else {
      current->setState(TERMINATED);
    }
  }
  // If there are processes left, print them
  if (!all_processes.empty()) {
    cout << "--" << endl;
    displaySystemStatus(all_processes);
  }
  // If there are somehow any remaining, delete them
  for (auto p : all_processes) {
    delete p;
  }

  return 0;
}
