#include <iostream>
#include <queue>
#include <vector>

using namespace std;

enum State { NEW, READY, RUNNING, TERMINATED };

class PCB {
private:
  int pid;
  int pc;
  State state;
  int remaining_work;

public:
  PCB(int id, int work) : pid(id), pc(1), state(NEW), remaining_work(work) {}

  // Getters
  int getPID() const { return pid; }
  int getPC() const { return pc; }
  State getState() const { return state; }
  int getRemainingWork() const { return remaining_work; }

  // Setters
  void setState(State s) { state = s; }

  void execute(int units) {
    remaining_work -= units;
    pc += units;
  }
};
void displaySystemStatus(const vector<PCB *> &processes) {
  for (size_t i = 0; i < processes.size(); i++) {
    cout << "P" << processes[i]->getPID();
    State s = processes[i]->getState();

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
    case NEW:
      break;
    default:
      break;
    }

    if (i < processes.size() - 1) {
      cout << endl;
    }
  }
  cout << endl << "--" << endl;
}

int main() {
  int quantum, num_processes;
  if (!(cin >> quantum >> num_processes)) {
    cout << "No valid inputs";
    return 1;
  }
  if (quantum < 1) {
    cout << "Invalid Quantum";
    return 1;
  }
  if (num_processes < 0) {
    cout << "Cannot have negative processes";
    return 1;
  }
  vector<PCB *> all_processes;
  queue<PCB *> ready_queue;

  // Set New Processes
  cout << "New processes:" << endl;
  for (int i = 0; i < num_processes; ++i) {
    int id, work;
    cin >> id >> work;
    if (id < 0) {
      cout << "Cannot have a negative id";
      return 1;
    }
    if (work < 0) {
      cout << "cannot have negative work";
      return 1;
    }
    if (cin.fail()) {
      cout << "ID and Work must be integers";
      return 1;
    }

    PCB *p = new PCB(id, work);
    all_processes.push_back(p);
    cout << "P" << p->getPID();
    if (i < num_processes - 1)
      cout << endl;
  }
  cout << endl << "--" << endl;

  // Set all to ready
  for (auto p : all_processes) {
    p->setState(READY);
    ready_queue.push(p);
  }
  displaySystemStatus(all_processes);

  // Scheduler
  while (!ready_queue.empty()) {
    PCB *current = ready_queue.front();
    ready_queue.pop();

    cout << "Kernel loading P" << current->getPID() << endl;
    cout << "--" << endl;

    current->setState(RUNNING);
    displaySystemStatus(all_processes);

    int work_done = (current->getRemainingWork() < quantum)
                        ? current->getRemainingWork()
                        : quantum;
    current->execute(work_done);

    cout << "Kernel saving P" << current->getPID() << endl;

    if (current->getRemainingWork() > 0) {
      current->setState(READY);
      ready_queue.push(current);
    } else {
      current->setState(TERMINATED);
    }

    if (!ready_queue.empty() || current->getState() == TERMINATED) {
    } else {

      bool any_active = false;
      for (auto p : all_processes) {
        if (p->getState() != TERMINATED) {
          any_active = true;
        }
      }
      if (!any_active) {
        cout << endl << "--" << endl;
      } else {
        cout << endl;
      }
    }
  }

  for (auto p : all_processes) {
    delete p;
  }

  return 0;
}
