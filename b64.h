#ifndef B64_H
#define B64_H
#include<iostream>
#include <vector>
using namespace std;

class B64
{
public:
    B64();
    string B64ToLookup(int temp_input);
    string B64ToSingle(int temp_input);
    string B64ToArray(vector<int> temp_input); // a tener cuidado con el vector tal ves sea mejor array
    int B64FromLookup(char temp_input);
    int B64FromSingle(string temp_input);
    vector<int> B64FromTestArray(string temp_input);
};

#endif // B64_H
