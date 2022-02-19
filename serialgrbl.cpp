#include "serialgrbl.h"
#include "windows.h"
#include <string.h>
#include <string>
#include <vector>
#include <chrono>
#include "qextserialport.h"

vector<string> partitionGRBL(string in, string limit){
    vector<string> salida;


    return salida;
}

string rstripGRBL(string input){            // REALIZAR LA FUNCION DE QUITAR LOS CARACTERES BLANCOS DEL FINAL
    string salida;
    return salida;
}

string lower(string input){            // REALIZAR LA FUNCION DE QUITAR LOS CARACTERES BLANCOS DEL FINAL
    string salida;
    return salida;
}


string lstripGRBL(string input, string caracter){            // REALIZAR LA FUNCION DE QUITAR LOS CARACTERES BLANCOS DEL FINAL
    string salida;
    return salida;
}

SerialGRBL::SerialGRBL()
{
    ser.setBaudRate(QSerialPort::Baud115200);
}


int SerialGRBL::Connect(string serial_port){
    int temp_com_success = 0;
    int nothing;
    ser.setPortName(QString::fromStdString(serial_port));
    if (this->connection_state == 0){
        temp_com_success = 0;
        try {
            ser.open(QSerialPort::ReadWrite);
            temp_com_success = 1;
        }  catch (exception) {
            nothing = 0;
        }
        if(temp_com_success == 1){
            cout<<serial_port<< "  for grbl opened"<<endl;
            this->connection_state = 1;
            this->started_state = 0;
            this->ok_state = 1;
            this->error_state = 0;
            this->homed_state = 0;
            //PONER LAS DEFINICIONES CON THREADS AQUI
            /*self._stop_event = threading.Event()
            self.update_thread = threading.Thread(target=self.Update)
            self.update_thread.start()
            self.status_thread = threading.Thread(target=self.GetStatus)
            self.status_thread.start()*/

            return 1;
        }
        else return 0;
    }
    return 0;
}

int SerialGRBL::Disconnect(){
    if(this->connection_state == 1){
        //this->_stop_event.set(); // cambiar con lo que corresponde
        this->ser.close();
        cout<<"cerrando grbl connection"<<endl;
        this->connection_state = 0;
        return 0;
    }
    return connection_state;
}


void SerialGRBL::Update(){
    Sleep(10000);              // podria no funcionar
    string read_buffer = "";
    string temp_read = "";
    char *leido={};
    while(true){ // poner la condicion _stop_event.is_set() en base al evento
        int temp_success = 0;
        try {
            if(this->ser.isReadable() > 0){
                this->ser.read(leido, 100);
                //temp_read = this->ser.read(this->ser.in_waiting); // ojo que podria retornar char}
                temp_read = string(leido);  // podria requerir convertir a utf-8
                temp_success = 1;
            }
        }  catch (exception) {
            cout<<"Read error"<<endl;
            break;
        }

        if(temp_success == 1) read_buffer += temp_read;

        vector<string> temp_decode = partitionGRBL(read_buffer, "\n");

        if(temp_decode[1] == "\n"){                 // tal vez el salto de linea podria ser diferente
            read_buffer = temp_decode[2];
            string read_line = string(temp_decode[0]);
            read_line = lower(read_line);
            read_line = rstripGRBL(read_line);

            if(read_line.rfind("ok",0)){
                this->ok_state = 1;
                if(this->homed_state == 2) this->homed_state = 1;
            }
            if(read_line.rfind("error",0)){
                this->window_input_buffer += string(read_line);
                this->error_state = 1;
                cout<<"error found, getting error type"<<endl;
            }
            if(read_line.rfind("[",0)){
                this->window_input_buffer += string(read_line);
                cout<<"message found, getting message"<<endl;
            }
            if(read_line.rfind("<",0)){
                vector<string> temp_read_line = partitionGRBL(read_line,",");
                string temp_line = lstripGRBL(temp_read_line[0], "<");
                read_line = temp_read_line[2];
                this->motion_state = string(temp_line);
                this->motion_state_index += 1;
                if(this->motion_state_index > this->motion_state_index_limit){
                    this->motion_state_index = 0;
                }
                temp_read_line = partitionGRBL(read_line, ":");
                temp_line = temp_read_line[0];
                read_line = temp_read_line[2];

                if(temp_line == "wpos"){
                    temp_read_line = partitionGRBL(read_line, ",");
                    temp_line = temp_read_line[0];
                    this->motion_x_pos = stof(temp_line);
                    read_line = temp_read_line[2];

                    temp_read_line = partitionGRBL(read_line, ",");
                    temp_line = temp_read_line[0];
                    this->motion_y_pos = stof(temp_line);
                    read_line = temp_read_line[2];

                    temp_read_line = partitionGRBL(read_line, ",");
                    temp_line = temp_read_line[0];
                    this->motion_z_pos = stof(temp_line);
                    read_line = temp_read_line[2];

                    temp_read_line = partitionGRBL(read_line, ",");
                    temp_line = temp_read_line[0];
                    this->motion_a_pos = stof(temp_line);
                    read_line = temp_read_line[2];
                }
            }
            if(read_line.rfind("grbl",0)){
                this->started_state = 1;
                cout<<"grbl found, getting version"<<endl;
                temp_decode = partitionGRBL(read_line, " ");
                temp_decode = partitionGRBL(temp_decode[2], "f ");
                if(temp_decode[1] == "k "){
                    this->window_input_buffer += string(/*temp_decode*/ temp_decode[1]); //Ojo ojin ojito aqui
                    this->grbl_version = stof(temp_decode[0]);
                    cout<<"GRBL Version: " + temp_decode[0] <<endl;
                }

            }
        }
        if(this->ok_state == 1 && this->started_state == 1){
            if(this->send_get_status == 1){
                this->ok_state = 0;
                this->SerialWriteRaw("?\r", 0);     // siempre tener cuidado con el \r
                this->send_get_status = 0;
            }
            else if(this->BufferLeft() > 0){
                this->ok_state = 0;
                this->BufferNext();
            }
        }
        if(this->nl_state == 0){
            if(this->motion_state == "idle"){
                if((this->motion_x_pos > this->nl_front_pos_x - this->nl_end_tolerance) && (this->motion_x_pos > this->nl_front_pos_x + this->nl_end_tolerance)){
                    this->nl_state = 1;
                    cout<<"new layer done"<<endl;
                }
            }
        }

    }
}

void SerialGRBL::SerialWriteRaw(string input_string, int temp_priority){
    if(temp_priority == 1){
        this->window_output_buffer += input_string;
    }
    const char *cstr = input_string.c_str();
    this->ser.write(cstr);
    //this->ser.write(input_string); // es posible que sea necesario convertir a utf-8
}

void SerialGRBL::SerialWriteBufferRaw(string input_string){
    if(this->connection_state == 1){
        this->gcode_buffer.push_back(input_string + "\r");// puede fallar "\r"
        this->gcode_buffer_left += 1;
    }
}

void SerialGRBL::SerialGotoXY(float temp_x, float temp_y, float temp_f){
    string temp_string = "G1 X";
    temp_string += to_string(temp_x);
    temp_string += " Y";
    temp_string += to_string(temp_y);
    if(temp_f){
        temp_string += " F";
        temp_string += to_string(temp_f);
    }
    this->SerialWriteBufferRaw(temp_string);
}

void SerialGRBL::SerialGotoHome(float temp_f){
    double temp_x, temp_y;
    if(this->printer_homing_dir_x == -1){
        temp_x = 0 + this->printer_homing_pulloff;
    }
    else{
        temp_x = this->printer_size_x - this->printer_homing_pulloff;
    }
    if(this->printer_homing_dir_y == -1){
        temp_y = 0 + this->printer_homing_pulloff;
    }
    else{
        temp_y = this->printer_size_y - this->printer_homing_pulloff;
    }

    this->SerialGotoXY(temp_x, temp_y, temp_f);
}

void SerialGRBL::Jog(string temp_axis, string temp_distance, string temp_feed){
    if(this->homed_state == 1){
        this->SerialWriteBufferRaw("G91");
        string temp_string = "G1 ";
        temp_string += temp_axis;
        temp_string += temp_distance;
        temp_string += " F";
        temp_string += temp_feed;
        this->SerialWriteBufferRaw(temp_string);
        this->SerialWriteBufferRaw("G90");
    }
    else {
        cout<<"Home printer before jogging"<<endl;
    }
}

int SerialGRBL::SpreaderToggle(){
    if(this->spreader_state == 0){
        this->spreader_state = 1;
        this->SerialWriteBufferRaw("M4");
    }
    else{
        this->spreader_state = 0;
        this->SerialWriteBufferRaw("M5");
    }
    return this->spreader_state;
}

void SerialGRBL::SpreaderSet(int temp_spreader_state){
    if(temp_spreader_state == 1){
        this->spreader_state = 1;
        this->SerialWriteBufferRaw("M4");
    }
    else{
        this->spreader_state = 0;
        this->SerialWriteBufferRaw("M5");
    }
}

void SerialGRBL::NewLayer(double temp_thickness, int temp_override_build){
    double temp_b_feed_distance;
    if(this->homed_state == 1){
        if(temp_override_build == 0){
            cout<<"Normal new layer"<<endl;
            temp_b_feed_distance = temp_thickness - this->nl_piston_clearance - this->nl_piston_hysteresis;
        }
        else{
            cout<<"Only feed"<<endl;
            temp_b_feed_distance =  (this->nl_piston_clearance * (-1)) - this->nl_piston_hysteresis;
        }

        double temp_f_feed_distance = (temp_thickness * this->nl_piston_feed_overfill * -1) - this->nl_piston_clearance - this->nl_piston_hysteresis;
        double temp_hysteresis_clearance =  this->nl_piston_clearance - this->nl_piston_hysteresis;


        this->SerialGotoXY(this->nl_back_pos_x, this->nl_back_pos_y, this->nl_travel_speed);   // OJO con esto el string


        this->SerialWriteBufferRaw("G91");
        this->SerialWriteBufferRaw("G1 Z" + to_string(this->nl_piston_hysteresis) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G1 Z" + to_string(temp_b_feed_distance) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G1 A" + to_string(this->nl_piston_hysteresis) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G1 A" + to_string(temp_f_feed_distance) + " F" + to_string(this->nl_piston_speed));


        this->SerialWriteBufferRaw("G90");
        this->SpreaderSet(1);
        this->SerialGotoXY(this->nl_front_pos_x, this->nl_back_pos_y, this->nl_feed_speed);
        this->SpreaderSet(0);
        this->SerialWriteBufferRaw("G91");
        this->SerialWriteBufferRaw("G1 Z" + to_string(this->nl_piston_hysteresis) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G1 Z" + to_string(temp_hysteresis_clearance) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G1 A" + to_string(this->nl_piston_hysteresis) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G1 A" + to_string(temp_hysteresis_clearance) + " F" + to_string(this->nl_piston_speed));
        this->SerialWriteBufferRaw("G90");

        this->StatusIndexSet();
        while(this->StatusIndexChanged() == 0){
            Sleep(5);  // tal ves el tiempo
        }
        this->nl_state = 0;
    }
}


int SerialGRBL::BufferLeft(){
    return this->gcode_buffer_left;
}

void SerialGRBL::BufferNext(){
    if(this->BufferLeft() > 0){
        this->gcode_buffer_left -= 1;
        this->SerialWriteRaw(this->gcode_buffer[0], 0);
        auto elem_to_remove = gcode_buffer.begin() + 0;
        gcode_buffer.erase(elem_to_remove);              // solo podria fallar el indice de borrado
    }
}

void SerialGRBL::home(){
    cout<<"hola como estas"<<endl;
    this->SerialWriteBufferRaw("$h");

    double temp_x, temp_y;
    if(this->printer_homing_dir_x == -1){
        temp_x = this->printer_homing_pulloff;
    }
    else{
        temp_x = this->printer_size_x - this->printer_homing_pulloff;
    }
    if(this->printer_homing_dir_y == -1){
        temp_y = this->printer_homing_pulloff;
    }
    else{
        temp_y = this->printer_size_y - this->printer_homing_pulloff;
    }

    this->SerialWriteBufferRaw("G92 X" + to_string(temp_x) + " Y" + to_string(temp_y));
    this->homed_state = 2;
    this->motion_state = "home";
}

void SerialGRBL::GetStatus(){
    Sleep(5000);
    while(true){// not event     Soluciona esto aqui entra un evento de un thread
        Sleep(100);
        this->send_get_status = 1;
    }
}

string SerialGRBL::GetWindowOutput(){
    string temp_return = this->window_output_buffer;
    this->window_output_buffer = "";
    return temp_return;
}

string SerialGRBL::GetWindowInput(){
    string temp_return = this->window_input_buffer;
    this->window_input_buffer = "";
    return temp_return;
}

void SerialGRBL::StatusIndexSet(){
    this->motion_state_changed = 0;
    this->motion_state_index = 0;
    this->motion_state_index_set = this->motion_state_index;
}

int SerialGRBL::StatusIndexChanged(){
    if(this->motion_state_changed == 0){
        if(this->motion_state_index_set + 3 > this->motion_state_index) return 0;
        else {
            this->motion_state_changed = 1;
            return 1;
        }
    }
    else return 1;
}
