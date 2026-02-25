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
    cout << "P" << processes[i]->getPID() << " ";
    State s = processes[i]->getState();

    switch (s) {
    case READY:
      cout << "Ready, PC: " << processes[i]->getPC();
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
      cout << "\n";
    }
  }
  cout << "-- \n";
}

int main() {
  int quantum, num_processes;
  if (!(cin >> quantum >> num_processes)) {
    return 0;
  }
  vector<PCB *> all_processes;
  queue<PCB *> ready_queue;

  // Set New Processes
  cout << "New processes:\n";
  for (int i = 0; i < num_processes; ++i) {
    int id, work;
    cin >> id >> work;
    PCB *p = new PCB(id, work);
    all_processes.push_back(p);
    cout << "P" << p->getPID();
    if (i < num_processes - 1)
      cout << "\n";
  }
  cout << " -- \n";

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

    cout << "Kernel loading P" << current->getPID() << " -- \n";
    current->setState(RUNNING);
    displaySystemStatus(all_processes);

    int work_done = (current->getRemainingWork() < quantum)
                        ? current->getRemainingWork()
                        : quantum;
    current->execute(work_done);

    cout << "Kernel saving P" << current->getPID();

    if (current->getRemainingWork() > 0) {
      current->setState(READY);
      ready_queue.push(current);
      cout << "\n";

      if (!ready_queue.empty()) {

      } else {

        current->setState(TERMINATED);

        bool any_active = false;
        for (auto p : all_processes) {
          if (p->getState() != TERMINATED) {
            any_active = true;
          }
        }
        if (!any_active) {
          cout << " -- \n";
        } else {
          cout << "\n";
        }
      }
    }
  }
  for (auto p : all_processes) {
    delete p;
  }

  return 0;
}
