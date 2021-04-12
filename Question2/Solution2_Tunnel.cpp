#include<iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;
vector<int>waiting_queue_left;
vector<int>waiting_queue_right;
string signal_left="Green",signal_right="Green";
mutex m;
condition_variable cv;

void print_left(int id)
{
    cout<<endl;
    for(int i=0;i<waiting_queue_left.size();i++)
    {
        if(waiting_queue_left[i]!=id and waiting_queue_left[i]!=-1)
            cout<<waiting_queue_left[i]<<" ";
        else if(waiting_queue_left[i]==id)
            waiting_queue_left[i]=-1;
    }
}
void print_right(int id)
{
    for(int i = 0;i<waiting_queue_right.size();i++)
    {
        if(waiting_queue_right[i]!=id and waiting_queue_right[i]!=-1)
            cout<<waiting_queue_right[i]<<" ";
        else if(waiting_queue_right[i]==id)
            waiting_queue_right[i]=-1;
    }
    cout<<endl;
}
void depart()
{
    signal_right="Green";
    signal_left="Green";
    cout<<endl;
    for(int i=0;i<waiting_queue_left.size();i++)
    {
        if(waiting_queue_left[i]!=-1)
            cout<<waiting_queue_left[i]<<" ";
    }
    cout<<" || Green || Tunnel is free now  || Green || ";
    for(int i=0;i<waiting_queue_right.size();i++)
    {
        if(waiting_queue_right[i]!=-1)
            cout<<waiting_queue_right[i]<<" ";
    }
    cout<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    cv.notify_one();
}
void arrive(int id, string direction)
{
   unique_lock<mutex>ul(m);
   cv.wait(ul,[] { return (signal_left!="Red" and signal_right!="Red")?true : false;});
   print_left(id);
   signal_left="Red";
   signal_right="Red";
   if(direction == "LR")
       cout<<" || "<<signal_left<<" || Vehicle "<<id<<" is using tunnel from Left to Right || "<<signal_right<<" || ";
   else
       cout<<" || "<<signal_left<<" || Vehicle "<<id<<" is using tunnel from Right to Left || "<<signal_right<<" || ";
   print_right(id);
   this_thread::sleep_for(chrono::seconds(2));
   depart();


}
void controller()
{
    int n;

    cout<<"Enter the Number of vehicles : ";
    cin>>n;
    string s;
    vector<string>direction;
    for(int i=0;i<n;i++)
    {
        cout<<"Enter direction of vehicle "<<i<<" enter LR(left to right) or RL(right to left) : ";
        cin>>s;
        if(s=="LR")
            waiting_queue_left.push_back(i);
        else
            waiting_queue_right.push_back(i);
        direction.push_back(s);
    }
    vector<thread>threads(n);
    for(int i=0;i<n;i++)
    {
        threads[i]=thread(arrive,i,direction[i]);

    }
    for(int i=0;i<threads.size();i++)
        threads[i].join();
}
void header()
{
    cout<<"CS558 : COMPUTER SYSTEM LAB ASSIGNMENT - 04\n";
    cout<<"Instructor : Dr. Manas Khatua and Dr. Moumita Patra\n";
    cout<<"TA's : Alakesh Kalita, Arunav Saikia, Meenu Rani Dey, Priya Undriwade, Vinay Kumar Gupta\n";
    cout<<"Sumbittted By : Himanshu Patel(204101029), Ketan Karnakota(204101030)\n";
}
int main()
{
    header();
    cout<<"-----------------------------------------------------------------------------------------\n";
    controller();
    return 0;
}