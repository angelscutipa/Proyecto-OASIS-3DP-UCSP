#include "b64.h"
#include <iostream>

using namespace std;
B64::B64()
{

}

string B64::B64ToLookup(int temp_input){

    if(temp_input<0) temp_input=0;
    if(temp_input>63) temp_input=63;

    string temp_return="";
    string temp_b64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    temp_return+=temp_b64[temp_input];
    return temp_return;
}
string B64::B64ToSingle(int temp_input){
    string temp_return_value = "";
    int temp_negative=1;
    int temp_convert=0;
    temp_input=int(temp_input);

    if (temp_input < 0){
        temp_negative = -1;
        temp_input *= (-1);
    }

    while(true){
        temp_convert= temp_input & 63; //A tener cuidado con esto
        temp_return_value += this->B64ToLookup(temp_convert);
        temp_input = temp_input >> 6;

        if (temp_input == 0) break;
    }

    if (temp_negative == -1) temp_return_value += "-";

    temp_return_value = string(temp_return_value.rbegin(), temp_return_value.rend());;///// REVISAR ESTO
    return temp_return_value;
}

string B64::B64ToArray(vector<int> temp_input){
    string temp_return_value = "";
    int temp_value = 0;
    int temp_output = 0;
    int temp_counter = 0;
    int temp_b6 = 0;
    int temp_inf = 0;

    while(true){
        temp_inf=5-temp_b6;
        temp_value = int(temp_input[temp_counter]);
        if(temp_value > 1) temp_value = 1;
        temp_value = temp_value << temp_inf;
        temp_output = temp_output | temp_value;
        temp_value = 0;
        temp_b6 = temp_b6+1;
        temp_counter = temp_counter+1;
        if(temp_b6 >= 6 || temp_counter >= int(temp_input.size())){
            temp_b6 = 0;
            temp_return_value += B64ToLookup(temp_output);
            temp_output = 0;

            if(temp_counter >= int(temp_input.size())) return temp_return_value;
        }
    }
}

int B64::B64FromLookup(char tmp_input){
    int temp_input=tmp_input;
    if (temp_input >= 'A' && temp_input <= 'Z') return temp_input - 'A'; //0-25
    if (temp_input >= 'a' && temp_input <= 'z') return temp_input - 71; //26-51
    if (temp_input >= '0' && temp_input <= '9') return temp_input + 4; //52-61
    if (temp_input == '+') return 62; //62
    if (temp_input == '/') return 63; //63
    return -1;
}

int B64::B64FromSingle(string temp_input){       // aqui podrias ser necesario usar puntero y funcion que retorne void
    if(temp_input.size() > 0){
        int temp_return_value = 0;
        int temp_negative = 1;
        int temp_order = 1;
        if (temp_input[0] == '-'){
            temp_negative = -1;
        }

        if(temp_input.size() == 0) return 0;
        temp_input= string(temp_input.rbegin(), temp_input.rend());

        while(true){
            int temp_val = this->B64FromLookup(temp_input[0]);
            if (temp_val != -1){
                temp_return_value += (temp_val * temp_order);
                temp_order *= 64;
                temp_input = temp_input.substr(1);  // remove first character
            }
            else {
                temp_return_value *= temp_negative;
                return temp_return_value;
            }

            if(temp_input.size() <= 0){
                temp_return_value *= temp_negative;
                return temp_return_value;
            }
        }
    }
    return 0;
}

vector<int> B64::B64FromTestArray(string temp_input){
    temp_input = string(temp_input.rbegin(), temp_input.rend());

    vector<int> temp_return_value;

    while(true){
        int temp_val = this->B64FromLookup(temp_input[0]);
        if(temp_val != -1){
            temp_input = temp_input.substr(1);
            for(int b=0; b<6; b++){
                int temp_decode = temp_val;
                temp_decode = (temp_decode >> b) & 1;
                temp_return_value.push_back(int(temp_decode));
            }
        }
        else return temp_return_value;

        if (temp_input.size() <= 0) return temp_return_value;
    }
}
