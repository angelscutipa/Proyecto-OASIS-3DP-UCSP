#ifndef SERIALHP45_H
#define SERIALHP45_H

#include <iostream>
#include <vector>
//#include <QtSerialPort/QSerialPort>
#include <b64.h>
#include <QSerialPort>
#include <QSerialPortInfo>

using namespace std;



class serialHP45
{
public:

    QSerialPort ser;

    B64 b64;

    int connection_state = 0; //whether connected or not
    int started_state = 0 ;
    int ok_state = 1; //whether the response of serial has been OK or not
    int error_state = 0; //error of system
    double inkjet_version = 0.0; //version of grbl
    int homed_state = 0;

    int send_get_status = 0; //whether to fetch a value
    string send_status_buffer = ""; //buffer for status
    int status_state = 0; //which status to fetch
    double inkjet_x_pos = 0.0; //the position of the printhead
    int inkjet_total_nozzles = 300; //the total amount of nozzles
    int inkjet_working_nozzles = 0; //the total amount working of nozzles
    double inkjet_temperature = 0.0; //the temperature of the printhead
    int inkjet_writeleft = 0; //the amount of read lines left
    int inkjet_dpi = 600; //the DPI of the printhead
    int inkjet_density = 100; //the density of the printhead

    vector<string> code_buffer;
    int code_buffer_left = 0;

    string window_output_buffer = ""; //holds a buffer of what was sent out
    string window_input_buffer = ""; //holds a buffer of what was received

    serialHP45();

    int Connect(string serial_port);
    int Disconnect();
    void Update();
    void SerialWriteRaw(string input_string, int temp_priority);
    void SerialWriteBufferRaw(string input_string);
    int BufferLeft();
    void BufferNext();
    void GetStatus();
    string GetWindowOutput();
    string GetWindowInput();
    void Preheat(int temp_pulses);
    void Prime(int temp_pulses);
    void SetPosition(int temp_position);
    void SetDPI(int temp_dpi);
    void SetDensity(int temp_density);
    void ClearBuffer();
    void TestPrinthead();
    void SendInkjetLineRaw(int temp_position, vector<int> temp_inkjet_line);
};



#endif // SERIALHP45_H
