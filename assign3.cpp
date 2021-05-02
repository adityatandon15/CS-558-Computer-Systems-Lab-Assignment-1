#include<bits/stdc++.h>
#include<thread>
#include<condition_variable>
#include<chrono>
using namespace std;

int numChairs;
    

class Monitor {
    
  private:
    int patientsWaiting;
    int isBusy;
    condition_variable cv;
    mutex m;
    int j;

  public:
    Monitor() {
        patientsWaiting = 0;
        isBusy = 0;
        j = -1;
    }
    
    int doctor(int i) {
        this_thread::sleep_for(chrono::milliseconds((10*i)%100));
        unique_lock<mutex> ul(m);
        if(patientsWaiting >= numChairs) {
            cout<<"Patient "<<i<<" has arrived and is ask to leave (seats are full)"<<endl;
            m.unlock();
            return 0;
        }
        else if(!isBusy && patientsWaiting == 0) {
            isBusy = 1;
            cout<<"Patient "<<i<<" has arrived and is getting treated without waiting"<<endl;
            m.unlock();
            return 1;
        }
        j = i;
        patientsWaiting++;
        cout<<"Patient "<<i<<" has arrived and is waiting in Queue"<<endl;
	    cv.wait(ul, [this,i] { return ((j!=i) || (!isBusy && patientsWaiting == 1));});
            j = i;
	    isBusy = 1;
	    patientsWaiting--;
	    cout<<"Patient "<<i<<" is getting treated"<<endl;
	    m.unlock();
	    return 1;
    }
    
    void treat(int i) {
        this_thread::sleep_for(chrono::milliseconds(10));
        unique_lock<mutex> ul(m);
        cout<<"Patient "<<i<<" is treated successfully....................."<<endl;
        isBusy = 0;
        cv.notify_one();
    }
    
} M;




void func(int i) {
    int x = M.doctor(i);
        if(x) {
            M.treat(i);
        }
    }

int main() {
    
    srand(0);
    int numPatients;
    cout<<"Enter number of chairs"<<endl;
    cin>>numChairs;
    cout<<"Enter the number of patients"<<endl;
    cin>>numPatients;
    
    thread patients[numPatients];
    
    for(int i=0; i<numPatients; i++) {
        patients[i] = thread(func, i);
    }
    
    for(int i=0; i<numPatients; i++) {
        patients[i].join();
    }
    
    return 0;
}
