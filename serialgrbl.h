#ifndef SERIALGRBL_H
#define SERIALGRBL_H

#include <iostream>
//#include <QtSerialPort/QSerialPort>
#include <vector>
#include <serialhp45.h>
#include <QSerialPort>
#include <QSerialPortInfo>

using namespace std;

class SerialGRBL
{
public:
    // Definir el serial;
    // definir baudarte del serial
    // definir timeout del serial.
    QSerialPort ser;

    int connection_state = 0;
    int started_state = 0;
    int ok_state = 0;
    int error_state = 0;
    string motion_state = "";
    int motion_state_index = 0;
    int motion_state_index_set = 0;
    int motion_state_changed = 1;
    int motion_state_index_limit = 100;
    int homed_state = 0;

    int send_get_status = 0;
    double motion_x_pos = 0.0;
    double motion_y_pos = 0.0;
    double motion_z_pos = 0.0;
    double motion_a_pos = 0.0;

    double printer_size_x = 480.0;
    double printer_size_y = 480.0;
    double printer_size_f = 480.0;
    double printer_size_b = 480.0;

    int spreader_state = 0;

    double nl_back_pos_x = 100.0;           //where x starts feeding
    double nl_back_pos_y = 240.0;           //where y starts and ends while feeding
    double nl_front_pos_x = 475.0;          //where x ends
    double nl_travel_speed = 12000.0;       //how fast new layer travels
    double nl_feed_speed = 6000.0;          //how fast new layer feeds (default 3000)
    double nl_piston_speed = 150.0;         //how fast the pistons move
    double nl_piston_clearance = 0.25;      //how much the pistons lower after a new layer to clear the spreader
    double nl_piston_hysteresis = 1.00;     //how much the piston needs to move down before it can move up
    double nl_piston_feed_overfill = 1.1;   //the fraction which feed supplies more than built takes
    double nl_end_tolerance = 10.0;         //how close to the end pos the gantry needs to be to consider new layer done
    int nl_state = 1;                    //state of new layer, 1 is done, 0 is in progress

    int printer_homing_dir_x = -1;         //homing direction
    int printer_homing_dir_y = 1;          //homing direction
    int printer_homing_pulloff = 5;        //how much the printer steps back after a home

    string window_output_buffer = "";
    string window_input_buffer = "";

    float grbl_version = 0.0;

    vector<string> gcode_buffer;
    int gcode_buffer_left = 0;

    SerialGRBL();

    int Connect(string port);
    int Disconnect();
    void Update();
    void SerialWriteRaw(string input_string, int temp_priority);
    void SerialWriteBufferRaw(string input_string);
    void SerialGotoXY(float temp_x, float temp_y, float temp_f);
    void SerialGotoHome(float temp_f);
    void Jog(string temp_axis, string temp_distance, string temp_feed);
    int SpreaderToggle();
    void SpreaderSet(int temp_spreader_state);
    void NewLayer(double temp_thickness, int temp_override_build = 0);
    int BufferLeft();
    void BufferNext();
    void home();
    void GetStatus();
    string GetWindowOutput();
    string GetWindowInput();
    void StatusIndexSet();
    int StatusIndexChanged();
};



#endif // SERIALGRBL_H
