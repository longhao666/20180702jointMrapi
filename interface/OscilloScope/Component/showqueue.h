/*
 * 自定义循环队列
 * 测试代码如下

    ShowQueue sq(5);

    for (int i = 0; i < 15; ++i) {
        sq.Append(i);
    }
    for (int i = 0; i < sq.Count(); ++i) {
        cout << sq.GetValue(i) << " ";
    }
    cout << endl;
    sq.MaxLength = 3;
    sq.FillZero();
    for (int i = 0; i < sq.Count(); ++i) {
        cout << sq.GetValue(i) << " ";
    }
    cout << endl;
    sq.MaxLength = 6;
    sq.FillZero();
    for (int i = 0; i < 15; ++i) {
        sq.Append(i);
    }
    for (int i = 0; i < sq.Count(); ++i) {
        cout << sq.GetValue(i) << " ";
    }
    cout << endl;
    vector<double> newdata;
    sq.CopyTo(newdata);
    for (vector<double>::iterator i = newdata.begin(); i != newdata.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;

 */

#ifndef SHOWQUEUE_H
#define SHOWQUEUE_H

#include <deque>
#include <vector>

class ShowQueue
{
public:
    int MaxLength;

    explicit ShowQueue(int queueSize);
    int Count();
    bool IsEmpty();
    bool DelHead();
    void Clear();
    void Append(double data); // 追加
    void FillZero();
    double GetValue(int index);
    void CopyTo(std::vector<double> & eachValue); // 将deque里的数据赋给vector

private:
    std::deque<double> data;
};

#endif // SHOWQUEUE_H
