#include "showqueue.h"
#include <iostream>

using namespace std;

ShowQueue::ShowQueue(int queueSize)
{
    MaxLength = queueSize;
}

int ShowQueue::Count()
{
    return data.size();
}

bool ShowQueue::IsEmpty()
{
    return data.empty();
}

bool ShowQueue::DelHead()
{
    if (IsEmpty()) {
        return false;
    } else {
        data.pop_front();
        return true;
    }
}

void ShowQueue::Clear()
{
    data.clear();
}

void ShowQueue::Append(double newdata)
{
    if (MaxLength < 1) {
        Clear();
        return;
    }
    //若队列已经满了
    while (Count() >= MaxLength) { // 当MaxLength改变之后
        // delete head then append
        DelHead();
    }
    // 把新元素挂到链尾
    data.push_back(newdata);
    return;
}

void ShowQueue::FillZero()
{
    Clear();
    for (int i = 0; i < MaxLength; i++) {
        Append(0);
    }
}

double ShowQueue::GetValue(int index)
{
    if (index >= Count()) {
        cout << "ShowQueue::GetValue : index is out of range." << endl;
        return 0;
    }
    return data.at(index);
}

void ShowQueue::CopyTo(vector<double> & eachValue)
{
    eachValue.clear();
    for (deque<double>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        eachValue.push_back(*iter);
    }
}
