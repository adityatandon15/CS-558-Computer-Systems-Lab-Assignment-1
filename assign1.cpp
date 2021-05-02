#include<bits/stdc++.h>
#define defSize 9
using namespace std;

vector<int> priority(defSize);
vector<int> arrivalTime(defSize);
vector<int> burstTime(defSize);
vector<int> completionTime(defSize,-1);
vector<int> turnAroundTime(defSize);
vector<int> waitingTime(defSize);
vector<pair<int,int>> ganttChart;

int findProcessToExecute(int at, vector<int> &tempBurstTime) {
    int maxPriority = -1;
    int maxPriorityIndex = -1;

    for(int i=0; i<defSize; i++) {
        if(arrivalTime[i] <= at && tempBurstTime[i] != 0 && (priority[i] > maxPriority || (priority[i] ==            maxPriority && arrivalTime[i] < arrivalTime[maxPriorityIndex]))) {
            maxPriority = priority[i];
            maxPriorityIndex = i;
        }
    }
    return maxPriorityIndex;
}

void preemptivePriorityScheduling() {
    vector<int> tempBurstTime;
    int j = 0;
    int maxArrivalTime = *max_element(arrivalTime.begin(), arrivalTime.end());

    tempBurstTime = burstTime;

    int totalRunningTime = accumulate(burstTime.begin(), burstTime.end(), 0);
   
    int i = 0;
    while(j != defSize) {
        if(tempBurstTime[i] == 0) j++;
        
        int proc = findProcessToExecute(i, tempBurstTime);

        if(proc == -1) {
            ganttChart.push_back(make_pair(proc,i+1));
            i++; 
        }  
        else if(i >= maxArrivalTime) {
            ganttChart.push_back(make_pair(proc, i + tempBurstTime[proc]));
            i += tempBurstTime[proc];
            tempBurstTime[proc] = 0;
        }
        else {
            ganttChart.push_back(make_pair(proc,i+1));
            tempBurstTime[proc]--;
            i++;
        }
    }
}

float calcAvgWaitingTime() {
    for(int i = ganttChart.size()-1; i>=0; i--) {
        if(completionTime[ganttChart[i].first] == -1) {
            completionTime[ganttChart[i].first] = ganttChart[i].second;
        }
    }

    for(int i=0; i<defSize; i++) {
        turnAroundTime[i] = completionTime[i] - arrivalTime[i];
    }
    for(int i=0; i<defSize; i++) {
        waitingTime[i] = turnAroundTime[i] - burstTime[i];
    }

    int totalWaitingTime = accumulate(waitingTime.begin(), waitingTime.end(), 0);
    int avgWaitingTime = totalWaitingTime / defSize;
    return avgWaitingTime;

    return 0;
}

void printGanttChart() {
    cout<<"--------------------------------------------------------------------------\n";
    cout<<"Gantt Chart\n\n";
    cout<<"Process\t\tTime Slots"<<endl;
    for(int i=0; i<ganttChart.size();) {

        int j = i+1;
        while(ganttChart[j].first == ganttChart[i].first) {
            j++;
        }
        int start = (i == 0)? 0 : ganttChart[i-1].second;
        if(ganttChart[i].first != -1)
        cout<<"P"<<ganttChart[i].first+1<<"\t\t"<<start<<" --> "<<ganttChart[j-1].second<<endl;
        i = j;
    }
}

int main() {

    string rollNumber;
    cout<<"Enter your 9 digit Roll Number"<<endl;
    cin>>rollNumber;

    if(rollNumber.length() != defSize) {
        cout<<"Invalid Roll number."<<endl;
        return 0;
    }

    srand(time(0));
   
    // //Priority, ArrivalTime and BurstTime Initialization
    for(int i=0,j=defSize-1; i<defSize,j>=0; i++,j--) {
        priority[i] = rollNumber[i] - '0';          //converting char to int.
        arrivalTime[i] = rollNumber[j] - '0';       //converting char to int.
        burstTime[i] = (rand() % 6) + 2;            //Generating a random number between 2 to 7 msec
    }

    preemptivePriorityScheduling();
    printGanttChart();
    float avgWaitingTime = calcAvgWaitingTime();
    cout<<"Average Waiting Time for processes are = "<<avgWaitingTime<<endl;
    cout<<"--------------------------------------------------------------------------\n";
    return 0;
}
