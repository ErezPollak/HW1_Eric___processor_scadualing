#include <iostream>
#include <queue>
#include "PriorityQueue.h"
#include "List.h"
#include <chrono>
#include <thread>

using namespace std;

/// the struct that keeps the information about a process.
struct Process {

    int PID = 0;
    int remainTime = 0;
    int burstTime = 0;
    int originalArrivalTime = 0;
    int arrivalTime = 0;
    int priority = 0; // as higher priority lower.

    string toString(const int i) const {
        return "process number : " + to_string(this->PID) + "\n"
               + "priority : " + to_string(this->priority) + "\n"
               + "time arrive:  " + to_string(this->arrivalTime) + "\n"
               + "time required: " + to_string(this->remainTime) + "\n\n";
    }
};

/**
 * Holds the information that is required for the timing analysis.
 */
struct ProcessAnalysis {

    int PID;
    int burstTime;
    int timeStarted;
    int timeFinished;

    /**
     * @return the waiting time of the process
     */
    int waitingTime() const {
        return timeFinished - timeStarted - burstTime;
    }

    /**
     * @return return the turntble time of the process
     */
    int turnAroundTime() const {
        return timeFinished - timeStarted;
    }
};


///////////////////inserting functions///////////////////////////////

void manualInsert(PriorityQueue<Process> *pQueue, const int processes, const char runningMethod);

void automaticInsert(PriorityQueue<Process> *pQueue, const int processes, const char i);

void insertProcess(PriorityQueue<Process> *pQueue, const Process process, const char method);

//void printProcess(Process *pProcess, int entry, int timeFinished);

void receiveInsertMethod(char &insertMethod) {
    cout << "welcome to the Process running simulator: \n"
         << "enter the method in witch you want to insert the information about the processes to the system: "
            "for manual insert 'M' for automatic insert 'A' :" << endl;
    cin >> insertMethod;
}

void receiveRunningMethod(char &runningMethod) {
    cout << "enter the way you want the system to run: \n"
            "'F' for 'FCFS', \n"
            "'J' for 'SRTF' (preemptive SJF),\n"
            "'P' for 'Priority order' ,\n"
            "'R' for 'Round Robin' " << endl;
    cin >> runningMethod;
}

void receiveNumberOfProcesses(int &number) {
    cout << "enter the number of processes to run: " << endl;
    cin >> number;
}

/**
 * inserting the quantumTime variable and return it by reference.
 * @param quantumTime
 */
void receiveRRTimeQuantum(int &quantumTime) {
    cout << "enter the time every process have inside the processor: " << endl;
    cin >> quantumTime;

    while (quantumTime < 3 || quantumTime > 100) {
        cout << "the number must be between 10 to 100: " << endl;
        cin >> quantumTime;
    }

}

/**
 * starts the inserting process with the decision how the user wants to insert the data to the database.
 * the operation now separates into two ways in one the data in being inserted manually and the other the data is being inserted automatically.
 * @param pQueue the queue to insert the data to.
 * @param numOfProcesses the number of the iteration will be happening.
 * @param inputMethod the input method according to it the process will separate.
 * @param runningMethod to determine with what priority to insert the process to teh queue.
 */
void insertProcesses(PriorityQueue<Process> *pQueue, const int numOfProcesses, const char inputMethod,
                     const char runningMethod) {
    switch (inputMethod) {
        case 'M': {
            manualInsert(pQueue, numOfProcesses, runningMethod);
        }
            break;
        case 'A': {
            automaticInsert(pQueue, numOfProcesses, runningMethod);
        }
            break;
    }
}

/**
 * inserting the data automatically with the ranges given in the exercise.
 * @param pQueue the quqeue to insert the information to.
 * @param numOfProcesses the number of processes to insert.
 * @param runningMethod the running method needed for inserting method.
 */
void automaticInsert(PriorityQueue<Process> *pQueue, const int numOfProcesses, const char runningMethod) {
    Process p;
    int lastTime = 0;
    for (int i = 0; i < numOfProcesses; ++i) {
        p.PID = i; // looks like a realistic value
        p.priority = rand() % 100;
        p.remainTime = rand() % 400 + 10;
        p.burstTime = p.remainTime;
        p.arrivalTime = i == 0 ? 0 : rand() % 100;
        p.originalArrivalTime = p.arrivalTime;
        insertProcess(pQueue, p, runningMethod);
    }
}

/**
 * inserting the data automatically with the ranges given in the exercise.
 * @param pQueue the quqeue to insert the information to.
 * @param numOfProcesses the number of processes to insert.
 * @param runningMethod the running method needed for inserting method.
 */
void manualInsert(PriorityQueue<Process> *pQueue, const int numOfProcesses, const char runningMethod) {
    Process p;
    for (int i = 0; i < numOfProcesses; ++i) {
        cout << "enter the properties of process #" << i + 1 << endl;
        p.PID = i;
        cout << "priority:" << endl;
        cin >> p.priority;
        cout << "require time:" << endl;
        cin >> p.remainTime;
        p.burstTime = p.remainTime;
        cout << "arrival time: " << endl;
        cin >> p.arrivalTime;
        p.originalArrivalTime = p.arrivalTime;
        insertProcess(pQueue, p, runningMethod);
    }
}

/**
 * inserting the process to teh queue no matter what way the data was inserted to teh system, manual or automatic.
 * @param pQueue the queue to insert the data to.
 * @param process the process that need to get in.
 * @param runningMethod the running method according to the priority will be set.
 */
void insertProcess(PriorityQueue<Process> *pQueue, const Process process, const char runningMethod) {

    switch (runningMethod) {
        //if the
        case 'I':
        case 'F':
        case 'R': {
            pQueue->enqueue(process.arrivalTime, process);
        }
            break;
        case 'J': {
            pQueue->enqueue(process.remainTime, process);
        }
            break;
        case 'P': {
            pQueue->enqueue(process.priority, process);
        }
            break;
    }
}

///////////////////short time scheduler functions///////////////////////////////

/**
 * printing  the information about the slot that the process took.
 * @param p the process
 * @param timeEntered the time that the slot started
 * @param timeFinished the time that the slot was over.
 */
void printProcess(const Process *p, const int timeEntered, const int timeFinished) {
    cout << p->PID << "#[" << timeEntered << "][" << timeFinished << "]" << endl;
}

/**
 * entering to the finished process array the process.
 * @param processAnalysis the array of analysis processes.
 * @param p process that was finished.
 */
void processAnalysisAppend(List<ProcessAnalysis> *processAnalysis, const Process *p, const int timeRunning) {

    ProcessAnalysis pa;

    pa.PID = p->PID;
    pa.burstTime = p->burstTime;
    pa.timeStarted = p->originalArrivalTime;
    pa.timeFinished = timeRunning;

    processAnalysis->List<ProcessAnalysis>::push_back(pa);

}


/**
 * the function that managing the queueing
 *
 * the algorithm is thus:
 *
 * 1)at first entering to the ready queue all the processes that needs to come in, in that second of the running.
 * 2)cutting down the time required for the running process.
 * 3)check if pulling out is required:   if so:
 *      3.1) writing down the tome it was finished.
 *      3.2) switching with the next process in the ready queue. if there is any.
 *      3.3) dequeue it from the ready queue.
 * 4) check if there is a preempt switching: if so:
 *      4.1) running the process that needs to be running.
 *
 * @param pQueue the long term queue
 * @param runningMethod the method in witch the processes need to get back inside the queue in case of a preempt.
 */
void printProcesses(PriorityQueue<Process> *longTermScheduler, const char runningMethod,
                    List<ProcessAnalysis> *processAnalysis) {

    //the process that is running at the moment.
    Process *runningProcess = nullptr;

    //keeps the time stamp for the loop.
    int timeRunning = 0;

    //keep the time the last process entered to the processor, needed for printings.
    int processEntry = 0;

    //the ready queue, keeps all the ready processes in the order they need to get in the processor.
    PriorityQueue<Process> *shortTermScheduler = new PriorityQueue<Process>();

    while (longTermScheduler->isEmpty() == false || shortTermScheduler->isEmpty() == false) {

        ///cout << "the " << timeRunning << " second." << endl;

        //entering to the processes that need to get in at the exact second.
        while (longTermScheduler->isEmpty() == false && longTermScheduler->first().arrivalTime == timeRunning) {
            ///cout << "process " << longTermScheduler->first().PID << " entered to ready queue." << endl;

            insertProcess(shortTermScheduler, longTermScheduler->first(), runningMethod);

            longTermScheduler->dequeue();
        }
        //if there is no running process at the moment it is imminently entering the processor.
        if (runningProcess == nullptr) {
            processEntry = timeRunning;
            runningProcess = &shortTermScheduler->first();
        }
        // }

        if (runningProcess != nullptr) {
            --runningProcess->remainTime;
        }

        //checks if an extraction is required.
        if (runningProcess != nullptr && (runningProcess->remainTime == 0)) {

            //inserting the process into the analysis list.
            processAnalysisAppend(processAnalysis, runningProcess, timeRunning + 1);

            //extraction from short time scheduler.
            shortTermScheduler->dequeue();

            //printing the process running properties.
            printProcess(runningProcess, processEntry, timeRunning + 1);

            //emptying the running process, the processor might not work for a while.
            runningProcess = nullptr;

            //running the first process in the queue if there is any.
            if (shortTermScheduler->isEmpty() == false) {
                //update the process entry in order to calculate the time of processing in case of a preempt.
                processEntry = timeRunning + 1;
                runningProcess = &shortTermScheduler->first();
            }
        }

        //check if a preempt is required.
        if (shortTermScheduler->isEmpty() == false && shortTermScheduler->first().PID != runningProcess->PID) {

            //printing the process running properties.
            printProcess(runningProcess, processEntry, timeRunning);

            //entering the new process to the queue.
            //update the process entry in order to calculate the time of processing in case of a preempt.
            processEntry = timeRunning;
            runningProcess = &shortTermScheduler->first();

        }

        ++timeRunning;

        //this_thread::sleep_for(chrono::milliseconds(100));
    }

    delete shortTermScheduler;

}

/**
 * prints all the elements in the queue in the RR method.
 * @param pQueue the queue full of processes ready to run.
 * @param timeQuantum the time every process has inside the processor.
 * @param timeRunning the time the the program ran.
 */
void printProcessRR(PriorityQueue<Process> *pQueue, const int timeQuantum, List<ProcessAnalysis> *processAnalysis) {

    int timeRunning = 0;

    //working as long as the queue has elements in it.
    while (!pQueue->isEmpty()) {

        Process runningProcess = pQueue->first();
        pQueue->dequeue();

        //checks if the time that teh process has is a full slot or not.
        if (runningProcess.remainTime > timeQuantum) {

            //if this is the last process we can run it to the end.
            if (pQueue->isEmpty()) {

                printProcess(&runningProcess, timeRunning, timeRunning + runningProcess.remainTime);

                processAnalysisAppend(processAnalysis, &runningProcess, timeRunning + runningProcess.remainTime);
                return;
            }

            //printing the data of the running.

            printProcess(&runningProcess, timeRunning, timeRunning + timeQuantum);
            timeRunning += timeQuantum;

            //reduce the time of the process in the time it just had ind=side the processor.
            runningProcess.remainTime -= timeQuantum;

            //updates the time arrival to be the accurate time the process got into the queue.
            runningProcess.arrivalTime = timeRunning;
            insertProcess(pQueue, runningProcess, 'R');

        }
            //in case the process has less time than the slot.
        else {

            //printing the data of the running.
            printProcess(&runningProcess, timeRunning, timeRunning + runningProcess.remainTime);
            timeRunning += runningProcess.remainTime;

            //inserting the process into the analysis list.
            processAnalysisAppend(processAnalysis, &runningProcess, timeRunning);

        }

    }
}


/**
 * the main function runs the hole program.
 * @return
 */
int main() {

    //receiving the data via the intended functions.

    char insertMethod;
    receiveInsertMethod(insertMethod);

    char runningMethod;
    receiveRunningMethod(runningMethod);

    int numberOfProcesses;
    receiveNumberOfProcesses(numberOfProcesses);

    //the long time scheduler, determines when every process get inside the processor.
    PriorityQueue<Process> *longTermScheduler = new PriorityQueue<Process>();

    //inserting the processes to the queue.
    insertProcesses(longTermScheduler, numberOfProcesses, insertMethod, 'I');

    ///printing processes in the required order:

    //holds all the information about all the finished processes.
    List<ProcessAnalysis> *processAnalysis = new List<ProcessAnalysis>();

    // for every other method we just print the processes according
    // to the order in witch the priority defined in the queue.
    if (runningMethod != 'R') {

        cout << "\nrunning started...\n " << endl;

        //printing the processes using the appropriate function
        printProcesses(longTermScheduler, runningMethod, processAnalysis);

    } else {

        int timeQuantum;
        receiveRRTimeQuantum(timeQuantum);

        cout << "\nrunning started...\n " << endl;

        //printing the function using the function for RR running method.
        printProcessRR(longTermScheduler, timeQuantum, processAnalysis);

    }

    cout << "\nrunning over.\n\n";

    //free the taken memory taken by the queue.
    delete longTermScheduler;

    cout << "Analysis : " << endl;

    cout << "the processes according to the order they were finished:" << endl;

    int turnaroundTimeSum = 0;
    int waitingTimeSum = 0;

    for (int i = 0; i < numberOfProcesses; ++i) {

        cout << "process number: " << processAnalysis->at(i).PID
             << ", Turnaround Time : " << processAnalysis->at(i).turnAroundTime()
             << ", Waiting Time : " << processAnalysis->at(i).waitingTime() << endl;


        turnaroundTimeSum += processAnalysis->at(i).turnAroundTime();
        waitingTimeSum += processAnalysis->at(i).waitingTime();

    }

    cout << endl << "Average Times: " << endl;
    cout << "Average TurnAround: " << (double) turnaroundTimeSum / numberOfProcesses << endl;
    cout << "Average Waiting Time : " << (double) waitingTimeSum / numberOfProcesses << endl;

    delete processAnalysis;

    return 0;
}
