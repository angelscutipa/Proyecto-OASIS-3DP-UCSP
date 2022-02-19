#include "serialhp45.h"
#include "windows.h"
#include <string>
#include <vector>
#include <chrono>

using std::vector;

vector<string> partitionHP(string in, string limit){
    vector<string> salida;


    return salida;
}

string rstripHP(string input){            // REALIZAR LA FUNCION DE QUITAR LOS CARACTERES BLANCOS DEL FINAL
    string salida;
    return salida;
}

serialHP45::serialHP45()
{
    ser.setBaudRate(QSerialPort::Baud115200);
}


int serialHP45::Connect(string serial_port){
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
            cout<<serial_port<< "  for HP45 opened"<<endl;
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

int serialHP45::Disconnect(){
    if(this->connection_state == 1){
        //this->_stop_event.set(); // cambiar con lo que corresponde
        this->ser.close();
        cout<<"cerrando grbl connection"<<endl;
        this->connection_state = 0;
        return 0;
    }
    return connection_state;
}

void serialHP45::Update(){
    Sleep(10000);              // podria no funcionar
    string read_buffer = "";
    string temp_read = "";
    while(true){ // poner la condicion _stop_event.is_set() en base al evento
        int temp_success = 0;
        try {
            if(this->ser.isReadable() > 0){  //Activar lo que sigue descomentar
                //temp_read = this->ser.read(this->ser.isReadable()); // ojo que podria retornar char}
                temp_read = string(temp_read);  // podria requerir convertir a utf-8
                temp_success = 1;
            }
        }  catch (exception) {
            cout<<"Read error"<<endl;
            break;
        }

        if(temp_success == 1) read_buffer += temp_read;

        vector<string> temp_decode = partitionHP(read_buffer, "\n");

        if(temp_decode[1] == "\n"){                 // tal vez el salto de linea podria ser diferente
            read_buffer = temp_decode[2];
            string read_line = string(temp_decode[0]);
            read_line = rstripHP(read_line);

            if(read_line.rfind("OK",0)) this->ok_state = 1;

            if(read_line.rfind("GTP:",0)){
                vector<string> temp_read_line = partitionHP(read_line, ":");
                read_line = temp_read_line[2];
                int temp_return_string = this->b64.B64FromSingle(read_line);  // ojo con el tipo de dato que devuelve
                this->inkjet_temperature = float(temp_return_string);
                this->inkjet_temperature /= 10.0;
            }

            if(read_line.rfind("GEP:",0)){
                vector<string> temp_read_line = partitionHP(read_line, ":");
                read_line = temp_read_line[2];
                int temp_return_string = this->b64.B64FromSingle(read_line);  // ojo con el tipo de dato que devuelve
                this->inkjet_temperature = float(temp_return_string);
                this->inkjet_temperature /= 1000.0;
            }
            if(read_line.rfind("BWL:",0)){
                vector<string> temp_read_line = partitionHP(read_line, ":");
                read_line = temp_read_line[2];
                int temp_return_string = this->b64.B64FromSingle(read_line);  // ojo con el tipo de dato que devuelve
                this->inkjet_writeleft = int(temp_return_string);
            }

            if(read_line.rfind("THD:",0)){
                vector<string> temp_read_line = partitionHP(read_line, ":");
                read_line = temp_read_line[2];
                int temp_return_string = this->b64.B64FromSingle(read_line);  // ojo con el tipo de dato que devuelve
                int temp_total_nozzle = 0;
                int temp_working_nozzles = 0;
                for (int n=0; n<temp_return_string; n++){
                    temp_total_nozzle += 1;
                    if( n==1) temp_working_nozzles += 1;
                }
                this->inkjet_total_nozzles = temp_total_nozzle;
                this->inkjet_working_nozzles = temp_working_nozzles;
            }
        }
        if(this->ok_state){
            if(this->send_get_status == 1){
                this->ok_state = 0;
                this->SerialWriteRaw(this->send_status_buffer + "\r", 0);
                this->send_get_status = 0;
            }
            else if(this->BufferLeft() > 0){
                if(this->inkjet_writeleft > 50){
                    this->ok_state = 0;
                    this->BufferNext();
                    this->inkjet_writeleft -= 1;
                }
            }
        }
    }
}

void serialHP45::SerialWriteRaw(string input_string, int temp_priority){
    if(temp_priority == 1) this->window_output_buffer += input_string;

    this->ser.write(input_string.c_str()); // puede requerir que sea en utf8
}

void serialHP45::SerialWriteBufferRaw(string input_string){
    if(this->connection_state == 1){
        this->code_buffer.push_back(input_string + '\r'); // tal vez no se reconozca el \r
        this->code_buffer_left += 1;
    }
}

int serialHP45::BufferLeft(){
    return this->code_buffer_left;
}

void serialHP45::BufferNext(){
    if(this->BufferLeft() > 0){
        this->code_buffer_left -= 1;
        this->SerialWriteRaw(this->code_buffer[0], 0);
        auto elem_to_remove = code_buffer.begin() + 0;
        code_buffer.erase(elem_to_remove);              // solo podria fallar el indice de borrado
    }
}
void serialHP45::GetStatus(){
    Sleep(5000);
    while(true){        ///POner la condicion: this->_stop_event.is_set()
        Sleep(100);
        if(this->status_state == 0) this->send_status_buffer = "GTP";
        if(this->status_state == 1) this->send_status_buffer = "GEP";
        if(this->status_state == 2) this->send_status_buffer = "BWL";

        this->send_get_status = 1;
        this->status_state += 1;
        if(this->status_state > 2) this->status_state = 0;
    }
}

string serialHP45::GetWindowOutput(){
    string temp_return = this->window_output_buffer;
    this->window_output_buffer = "";
    return temp_return;
}

string serialHP45::GetWindowInput(){
    string temp_return = this->window_input_buffer;
    this->window_input_buffer = "";
    return temp_return;
}

void serialHP45::Preheat(int temp_pulses){
    if(this->connection_state == 1){
        string temp_send_pulses = "";
        temp_send_pulses = this->b64.B64ToSingle(temp_pulses);
        this->SerialWriteBufferRaw("PHT " + temp_send_pulses);
    }
}

void serialHP45::Prime(int temp_pulses){
    if(this->connection_state == 1){
        string temp_send_pulses = "";
        temp_send_pulses = this->b64.B64ToSingle(temp_pulses);
        this->SerialWriteBufferRaw("PRM " + temp_send_pulses);
    }
}

void serialHP45::SetPosition(int temp_position){
    if(this->connection_state == 1){
        string temp_send_pos = "";
        temp_send_pos = this->b64.B64ToSingle(temp_position);
        this->SerialWriteBufferRaw("SEP " + temp_send_pos);
    }

}

void serialHP45::SetDPI(int temp_dpi){
    if(this->connection_state == 1){
        string temp_send_dpi = "" ;
        temp_send_dpi = b64.B64ToSingle(temp_dpi);
        this->inkjet_dpi = int(temp_dpi);
        this->SerialWriteBufferRaw("SDP " + string(temp_send_dpi));
    }
}

void serialHP45::SetDensity(int temp_density){
    if(this->connection_state == 1){
        string temp_send_density = "";
        temp_send_density = b64.B64ToSingle(temp_density);
        this->inkjet_density = int(temp_density);
        this->SerialWriteBufferRaw("SDN " + string(temp_send_density));
    }
}

void serialHP45::ClearBuffer(){
    if(this->connection_state == 1)this->SerialWriteBufferRaw("BCL");
}

void serialHP45::TestPrinthead(){
    if(this->connection_state == 1)this->SerialWriteBufferRaw("THD");
}

void serialHP45::SendInkjetLineRaw(int temp_position, vector<int> temp_inkjet_line){
    string temp_pos_b64 = this->b64.B64ToSingle(temp_position);
    string temp_inkjet_b64 = this->b64.B64ToArray(temp_inkjet_line);

    this->SerialWriteBufferRaw("SBR " + temp_pos_b64 + " " + temp_inkjet_b64);
}
