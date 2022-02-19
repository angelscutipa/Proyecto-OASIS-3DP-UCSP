#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QSignalMapper"
#include "windows.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList items;
    items.append("100");
    items.append("200");
    items.append("300");
    this->ui->comboBox->addItems(items);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->grbl.home();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->grbl.Jog("Y", "10", "6000");
}

void MainWindow::on_pushButton_24_clicked()
{
    this->grblConnect();
}

void MainWindow::on_pushButton_25_clicked()
{
    this->GrblSendCommand();
}

void MainWindow::on_pushButton_26_clicked()
{
    this->InkjetConnect();
}

void MainWindow::on_pushButton_27_clicked()
{
    this->InkjetSendCommand();
}

void MainWindow::on_pushButton_5_clicked()
{
    this->grbl.SerialGotoXY(5, 245, 12000);
}

void MainWindow::on_pushButton_6_clicked()
{
    this->grbl.Jog("X", "10", "6000");
}

void MainWindow::on_pushButton_4_clicked()
{
    this->grbl.Jog("X", "-10", "6000");
}

void MainWindow::on_pushButton_7_clicked()
{
    this->grbl.Jog("Y", "-10", "6000");
}

void MainWindow::on_pushButton_8_clicked()
{
    this->grbl.Jog("A", "-1", "150");
}

void MainWindow::on_pushButton_10_clicked()
{
    this->grbl.Jog("A", "1", "150");
}

void MainWindow::on_pushButton_9_clicked()
{
    this->grbl.Jog("Z", "-1", "150");
}

void MainWindow::on_pushButton_11_clicked()
{
    this->grbl.Jog("Z", "1", "150");
}

void MainWindow::on_pushButton_3_clicked()
{
    this->GRBLSpreader();
}

void MainWindow::on_pushButton_12_clicked()
{
    this->GRBLNewLayer();
}

void MainWindow::on_pushButton_13_clicked()
{
    this->GRBLPrimeLayer();
}

void MainWindow::on_pushButton_19_clicked()
{
    this->InkjetSetPosition();
}

void MainWindow::on_pushButton_20_clicked()
{
    this->inkjet.Preheat(5000);
}

void MainWindow::on_pushButton_21_clicked()
{
    this->inkjet.Prime(100);
}

void MainWindow::on_pushButton_22_clicked()
{
    this->InkjetSetDensity();
}

void MainWindow::on_pushButton_23_clicked()
{
    this->inkjet.TestPrinthead();
}

void MainWindow::on_pushButton_14_clicked()
{
    this->OpenFile("");
}

void MainWindow::on_pushButton_15_clicked()
{
    this->RenderOutput();
}

void MainWindow::on_pushButton_16_clicked()
{
    this->RunPrintArray();
}

void MainWindow::on_pushButton_17_clicked()
{
    this->PausePrint();
}

void MainWindow::on_pushButton_18_clicked()
{
    this->AbortPrint();
}

void MainWindow::grblConnect(){
    if(this->grbl_connection_state == 0){
        string temp_port = this->ui->lineEdit_12->text().toStdString();
        int temp_succes = this->grbl.Connect(temp_port);
        if(temp_succes == 1){
            this->ui->pushButton_24->setText("Disconnect");
            this->grbl_connection_state = 1;
            this->ui->lineEdit_12->clear();
            /*  self._grbl_stop_event = threading.Event()
                self.grbl_update_thread = threading.Thread(target=self.GrblUpdate)
                self.grbl_update_thread.start()
            */
        }
        else{
            cout<<"connection with GRBL failed"<<endl;
        }
    }
    else{
        this->grbl.Disconnect();
        this->grbl_connection_state = 0;
        this->ui->pushButton_24->setText("Connect");
        /*  self._grbl_stop_event.set()              */
    }
}

//void GrblUpdate(){}

void MainWindow::GrblSendCommand(){
    if(this->grbl_connection_state==1){
        QString temp_command = this->ui->lineEdit_13->text();
        temp_command += "\r";
        this->grbl.SerialWriteBufferRaw(temp_command.toStdString());
        this->ui->lineEdit_13->clear();
    }
}
void MainWindow::InkjetConnect(){
    if(this->inkjet_connection_state == 0){
        QString temp_port = this->ui->lineEdit_14->text();
        int temp_succes = this->inkjet.Connect(temp_port.toStdString());
        if(temp_succes == 1){
            this->ui->pushButton_26->setText("Disconnect");
            this->inkjet_connection_state = 1;
            this->ui->lineEdit_14->clear();
            /*
                self._inkjet_stop_event = threading.Event()
                self.inkjet_update_thread = threading.Thread(target=self.InkjetUpdate)
                self.inkjet_update_thread.start()
             */
        }
        else{
            cout<<"Connection with HP failed"<<endl;
        }
    }
    else{
        this->inkjet.Disconnect();
        this->inkjet_connection_state = 0;
        this->ui->pushButton_26->setText("Connect");
        //self._inkjet_stop_event.set() #close the HP45 serial thread
    }
}
void MainWindow::InkjetUpdate(){
    Sleep(1000);
    while(false){   //while not self._inkjet_stop_event.is_set():
        string inkjet_serial_in = this->inkjet.GetWindowInput();
        string inkjet_serial_out = this->inkjet.GetWindowOutput();


        this->ui->plainTextEdit_3->moveCursor(QTextCursor().End);
        this->ui->plainTextEdit_3->insertPlainText(QString::fromStdString(inkjet_serial_out));
        this->ui->plainTextEdit_3->moveCursor(QTextCursor().End);

        this->ui->plainTextEdit_4->moveCursor(QTextCursor().End);
        this->ui->plainTextEdit_4->insertPlainText(QString::fromStdString(inkjet_serial_in));
        this->ui->plainTextEdit_4->moveCursor(QTextCursor().End);

        this->ui->lineEdit_10->setText(QString::fromStdString(to_string(this->inkjet.inkjet_temperature)));
        this->ui->lineEdit_9->setText(QString::fromStdString(to_string(this->inkjet.inkjet_x_pos)));
        this->ui->lineEdit_11->setText(QString::fromStdString(to_string(this->inkjet.inkjet_writeleft)));

        this->ui->lineEdit_8->setText(QString::fromStdString(to_string(this->inkjet.inkjet_working_nozzles)) + "/" + QString::fromStdString(to_string(this->inkjet.inkjet_total_nozzles)));
        Sleep(200);
    }
}
void MainWindow::InkjetSendCommand(){
    if(this->inkjet_connection_state == 1){
        QString temp_command = this->ui->lineEdit_15->text();
        temp_command += "\r";
        this->inkjet.SerialWriteBufferRaw(temp_command.toStdString());
        this->ui->lineEdit_15->clear();
    }
}
void MainWindow::InkjetSetPosition(){
    if(this->inkjet_connection_state == 1){
        Sleep(300);
        float temp_pos = this->grbl.motion_y_pos;
        temp_pos *= 1000.0;
        this->inkjet.SetPosition((int)temp_pos);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QString temp_dpi = this->ui->comboBox->currentText();
    int temp_dpi_val = 0;
    int temp_success = 0;
    try {
        temp_dpi_val = temp_dpi.toInt();
        temp_success = 1;
    }  catch (exception) {
        NULL;
    }

    if(temp_success == 1){
        if(this->printing_state == 0){
            cout<<"DPI to set: "<<temp_dpi_val<<endl;
            if(this->inkjet_connection_state == 1){
                this->inkjet.SetDPI(temp_dpi_val);
            }
            this->imageconverter.SetDPI(temp_dpi_val);
            if(this->file_loaded != 0){
                cout<<"resising image"<<endl;
                this->OpenFile(this->input_file_name[0].toStdString());
            }
        }
    }
}

void MainWindow::InkjetSetDensity(){
    if(this->inkjet_connection_state == 1){
        QString temp_density = this->ui->lineEdit_7->text();
        int temp_density_val = 0;
        int temp_success = 0;
        try {
            temp_density_val = temp_density.toInt();
            temp_success = 1;
        }  catch (exception) {
            NULL;
        }

        if(temp_success == 1){
            this->inkjet.SetDensity(temp_density_val);
        }
    }
}

void MainWindow::GRBLSpreader(){
    int temp_return = this->grbl.SpreaderToggle();
    if(temp_return == 1){
        this->ui->pushButton_3->setText("Spreader off");
    }
    else{
        this->ui->pushButton_3->setText("Spreader on");
    }
}

void MainWindow::GRBLNewLayer(){
    if(this->grbl_connection_state == 1){
        QString temp_layer_thickness = this->ui->lineEdit->text();
        float temp_layer_thickness_val = 0;
        int temp_success = 0;
        try {
            temp_layer_thickness_val = temp_layer_thickness.toFloat();
            temp_success = 1;
        }  catch (exception) {
            NULL;
        }

        if(temp_success == 1){
            this->grbl.NewLayer(temp_layer_thickness_val);
        }
    }
}

void MainWindow::GRBLPrimeLayer(){
    if(this->grbl_connection_state == 1){
        QString temp_layer_thickness = this->ui->lineEdit->text();
        float temp_layer_thickness_val = 0;
        int temp_success = 0;
        try {
            temp_layer_thickness_val = temp_layer_thickness.toFloat();
            temp_success = 1;
        }  catch (exception) {
            NULL;
        }

        if(temp_success == 1){
            this->grbl.NewLayer(temp_layer_thickness_val, 1);
        }
    }
}

void MainWindow::OpenFile(string temp_input_file){
    QFileDialog Qfile;
    int temp_response;
    if(temp_input_file != ""){
        temp_response = this->imageconverter.Openfile(temp_input_file);
    }
    else{
        this->input_file_name = Qfile.getOpenFileNames(this, tr("Open File"),"",tr("Images (*.png *.svg *.jpg)"));
        cout<<"file name: "<< input_file_name.at(0).toStdString()<<endl;
        temp_response = this->imageconverter.Openfile(this->input_file_name.at(0).toStdString());
    }
    cout<<"Esto sale al abrir: "<<temp_response<<endl;
    if(temp_response == 1){
        this->RenderInput();
        this->file_loaded = 1;
    }
    if(temp_response == 2){
        this->file_loaded = 2;
        this->ui->horizontalSlider->setMaximum(this->imageconverter.svg_layers - 1);
        this->RenderOutput();
    }
}


void MainWindow::UpdateLayer(){
    if(this->imageconverter.file_type == 2 && this->printing_state == 0){
        int temp_layer = this->ui->horizontalSlider->value();
        this->ui->label_8->setText(QString::fromStdString("Layer: " + to_string(temp_layer)));
        this->imageconverter.SVGLayerToArray(temp_layer);
        this->RenderOutput();
    }
}

void MainWindow::PausePrint(){
    if(this->file_loaded == 2){
        if(this->printing_pause_flag == 0){
            this->printing_pause_flag = 1;
            this->ui->pushButton_17->setText("Resume");
        }
        else{
            this->printing_pause_flag = 0;
            this->ui->pushButton_17->setText("Pause");
        }
    }
}

void MainWindow::AbortPrint(){
    if(this->file_loaded == 2){
        int temp_response = QMessageBox::question(this, "Abort print", "Do you really want to abort the print?", QMessageBox::Yes|QMessageBox::No);
        if(temp_response == QMessageBox::Yes){
            this->printing_abort_flag = 1;
        }
    }
}
void MainWindow::RenderInput(){
    this->input_image_display = this->imageconverter.input_image;
    if(this->input_image_display.width() > 300 && this->input_image_display.height() > 300){
        this->input_image_display = this->input_image_display.scaled(300,300, Qt::KeepAspectRatio);
    }
    this->ui->label_17->setPixmap(this->input_image_display);
}
void MainWindow::RenderOutput(){
    cout<<this->file_loaded<<endl;
    if(this->file_loaded == 1){
        //int temp_threshold = this->ui->horizontalSlider_2->value();
        cout<<temp_threshold<<endl;
        //temp_threshold = 190;
        this->imageconverter.Threshold(temp_threshold);
        this->imageconverter.ArrayToImage();
        this->output_image_display = this->imageconverter.output_image;
        if(this->output_image_display.width() > 300 && this->output_image_display.height() > 300){
            this->output_image_display = this->output_image_display.scaled(300, 300, Qt::KeepAspectRatio);
        }
        this->ui->label_18->setPixmap(this->output_image_display);
    }

    if(this->file_loaded == 2){
        this->imageconverter.ArrayToImage();
        this->output_image_display = this->imageconverter.output_image;
        if(this->output_image_display.width() > 300 && this->output_image_display.height() > 300){
            this->output_image_display = this->output_image_display.scaled(300, 300, Qt::KeepAspectRatio);
        }
        this->ui->label_18->setPixmap(this->output_image_display);
    }
}

void MainWindow::RenderAlpha(){
    this->imageconverter.AlphaMaskToImage();
    this->output_image_display = this->imageconverter.output_image;
    if(this->output_image_display.width() > 300 && this->output_image_display.height() > 300){
        this->output_image_display = this->output_image_display.scaled(300, 300, Qt::KeepAspectRatio);
    }
    this->ui->label_18->setPixmap(this->output_image_display);
}

void MainWindow::RenderRGB(){
    this->imageconverter.RGBToImage();
    this->output_image_display = this->imageconverter.output_image;
    if(this->output_image_display.width() > 300 && this->output_image_display.height() > 300){
        this->output_image_display = this->output_image_display.scaled(300, 300, Qt::KeepAspectRatio);
    }
    this->ui->label_18->setPixmap(this->output_image_display);
}

void MainWindow::RunPrintArray(){
    if(this->file_loaded == 1){
        /*
         * self._printing_stop_event = threading.Event()
            self.printing_thread = threading.Thread(target=self.PrintArray)
            self.printing_thread.start()
         */
    }
    if(this->file_loaded == 2){
        /*
         * self._printing_stop_event = threading.Event()
            self.printing_thread = threading.Thread(target=self.PrintSVG)
            self.printing_thread.start()
         */
    }
}

void MainWindow::PrintSVG(){
    cout<<"Starting print from SVG"<<endl;

    if(this->file_loaded == 2 && this->inkjet_connection_state == 1 && this->grbl_connection_state == 1){
        this->printing_state = 2;
        this->inkjet.ClearBuffer();
        this->grbl.home();

        this->build_center_x = 163.0;
        this->build_center_y = 111.0;
        this->print_speed = 3000.0;
        this->travel_speed = 15000.0;
        this->acceleration_distance = 20.0;
        this->printing_dpi = this->imageconverter.dpi;
        this->printing_sweep_size = (int)(this->printing_dpi / 2);
        this->pixel_to_pos_multiplier = 25.4 / this->printing_dpi;
        this->image_size_x = this->imageconverter.image_array_height;
        this->image_size_y = this->imageconverter.image_array_width;
        this->layers = this->imageconverter.svg_layers;
        this->current_layer = 0;
        this->current_layer_height = this->imageconverter.svg_layer_height[0];

        cout<<"Starting print at height: " << this->current_layer_height <<endl;

        this->printing_abort_flag = 0;
        this->printing_pause_flag = 0;

        this->inkjet.SetDPI(this->printing_dpi);

        this->svg_offset_y = this->imageconverter.svg_height/2;
        this->svg_offset_x = this->imageconverter.svg_width/2;

        if(this->grbl_connection_state == 1){
            while(this->grbl.motion_state != "idle"){
                Sleep(100);
                continue;
            }
        }

        Sleep(250);
        this->InkjetSetPosition();
        Sleep(250);

        while(true){
            this->imageconverter.SVGLayerToArray(this->current_layer);
            this->ui->horizontalSlider->setValue(this->current_layer);
            this->ui->label_8->setText(QString::fromStdString("Layer: " + to_string(this->current_layer)));
            this->RenderOutput();

            cout<<"Printing layer: " << this->current_layer<<endl;

            while(this->grbl.nl_state == 0){
                Sleep(100);
                continue;
            }

            if(this->printing_abort_flag == 1) break;

            this->sweep_x_min = 0;
            this->sweep_x_max = 0;

            int temp_break_loop = 0;

            for(int h = 0; h<this->image_size_x; h++){
                for(int w = 0; w<this->image_size_y; w++){
                    if(this->imageconverter.image_array[h][w] != 0){
                        this->sweep_x_min = h;
                        temp_break_loop = 1;
                        cout<<"X-min on row: " <<h<<endl;
                        break;
                    }
                }
                if(temp_break_loop == 1) break;
            }
            temp_break_loop = 0;

            for(int h = this->image_size_x; h > 0; h--){
                for(int w = 0; w<this->image_size_y; w++){
                    if(this->imageconverter.image_array[h][w] != 0){
                        this->sweep_x_max = h;
                        temp_break_loop = 1;
                        cout<<"X-max on row: " <<h<<endl;
                        break;
                    }
                }
                if(temp_break_loop == 1) break;
            }

            int temp_round;

            this->sweep_x_size = this->sweep_x_max - this->sweep_x_min;
            cout<<"Sweep size in pixels: " <<this->sweep_x_size<<endl;
            if(this->sweep_x_size % this->printing_sweep_size == 0) temp_round = 1;
            else temp_round = 0;

            this->sweeps = (int)(this->sweep_x_size / this->printing_sweep_size);

            if(temp_round == 0) this->sweeps += 1;

            cout<<"Sweeps in layer: "<< this->sweeps<<endl;

            this->sweep_x_pix = this->sweep_x_max - this->printing_sweep_size;

            for(int L=0; L<this->sweeps; L++){
                cout<<"printing sweep" << L <<endl;
                this->sweep_x_pos = (this->sweep_x_pix * this->pixel_to_pos_multiplier) + this->build_center_x - this->svg_offset_x;
                int temp_break_loop = 0;
                for(int w=0; w<this->image_size_y; w++){
                    for(int h=this->sweep_x_pix; h<(this->sweep_x_pix + this->printing_sweep_size); h++){
                        if(h > 0){
                            if(this->imageconverter.image_array[h][w] != 0){
                                this->sweep_y_min = w;
                                temp_break_loop = 1;
                                break;
                            }
                        }
                    }
                    if(temp_break_loop == 1) break;
                }
                temp_break_loop = 0;
                for( int w=this->image_size_y; w>0 ; w--){
                    for(int h=this->sweep_x_pix; h<(this->sweep_x_pix + this->printing_sweep_size); h++){
                        if(h > 0){
                            if(this->imageconverter.image_array[h][w] != 0){
                                this->sweep_y_max = w;
                                temp_break_loop = 1;
                                break;
                            }
                        }
                    }
                    if(temp_break_loop == 1){
                        break;
                    }
                }

                // Calcular posicion
                this->sweep_y_start_pix = this->sweep_y_min;
                this->sweep_y_end_pix = this->sweep_y_max;
                this->sweep_y_start_pos = (this->sweep_y_start_pix * this->pixel_to_pos_multiplier) + this->build_center_y - this->svg_offset_y - this->acceleration_distance;
                this->sweep_y_end_pos = (this->sweep_y_end_pix * this->pixel_to_pos_multiplier) + this->build_center_y - this->svg_offset_y + this->acceleration_distance;
                cout<<"Sweep from: " << this->sweep_y_start_pos<<", to: " <<this->sweep_y_end_pos<<endl;

                cout<<"Filling local buffer with inkjet"<<endl;
                string temp_line_history = "";
                string temp_line_string = "";
                vector<int> temp_line_array(this->printing_sweep_size, 0);
                temp_line_history = this->b64.B64ToArray(temp_line_array);
                temp_line_string = temp_line_history;


                int temp_pos = ((this->sweep_y_start_pix - 1) * this->pixel_to_pos_multiplier) + this->build_center_y - this->svg_offset_y;
                temp_pos *= 1000;
                string temp_b64_pos = b64.B64ToSingle(temp_pos);
                this->inkjet.SerialWriteBufferRaw("SBR "+ temp_b64_pos + " " + temp_line_string);
                cout<<"SBR "+ temp_b64_pos + " " + temp_line_string << ", real pos: " <<temp_pos<<endl;

                for(int w = this->sweep_y_start_pix; w < this->sweep_y_end_pix; w++){
                    int temp_line_changed = 0;
                    int temp_counter = 0;
                    for(int h=this->sweep_x_pix; h<(this->sweep_x_pix + this->printing_sweep_size); h++){
                        if( h >= 0 ) temp_line_array[temp_counter] = this->imageconverter.image_array[h][w];
                        else temp_line_array[temp_counter] = 0;
                        temp_counter += 1;
                    }

                    temp_line_string = b64.B64ToArray(temp_line_array);
                    if(temp_line_string != temp_line_history){
                        temp_line_history = temp_line_string;
                        temp_pos = (w * this->pixel_to_pos_multiplier) + this->build_center_y - this->svg_offset_y;
                        temp_pos *= 1000;
                        temp_b64_pos = b64.B64ToSingle(temp_pos);
                        this->inkjet.SerialWriteBufferRaw("SBR "+ temp_b64_pos + " " + temp_line_string);
                        cout<<"SBR "+ temp_b64_pos + " " + temp_line_string << ", real pos: " <<temp_pos<<endl;
                    }
                }

                temp_line_array.clear();
                vector<int> temp_line_array_T(this->printing_sweep_size, 0);// OJO AQUI
                temp_line_array = temp_line_array_T;
                temp_line_string = b64.B64ToArray(temp_line_array);
                temp_pos = ((this->sweep_y_end_pix + 1) * this->pixel_to_pos_multiplier) + this->build_center_y - this->svg_offset_y;
                temp_pos *= 1000;
                temp_b64_pos = b64.B64ToSingle(temp_pos);
                this->inkjet.SerialWriteBufferRaw("SBR "+ temp_b64_pos + " " + temp_line_string);
                cout<<"SBR "+ temp_b64_pos + " " + temp_line_string << ", real pos: " <<temp_pos<<endl;

                cout<<" Making printing buffer done: "<<endl;

                this->grbl.SerialGotoXY(this->sweep_x_pos, this->sweep_y_start_pos, this->travel_speed); //OJO
                this->grbl.StatusIndexSet();
                while(true){
                    Sleep(100);
                    if(this->grbl.StatusIndexChanged() == 1 && this->grbl.motion_state == "idle"){
                        break;
                    }
                }

                while(this->inkjet.BufferLeft() > 0){
                    Sleep(100);
                }

                Sleep(200);
                this->InkjetSetPosition();
                Sleep(200);

                this->grbl.SerialGotoXY(this->sweep_x_pos, this->sweep_y_end_pos, this->print_speed);
                this->grbl.StatusIndexSet();
                while(true){
                    Sleep(100);
                    if(this->grbl.StatusIndexChanged() == 1 && this->grbl.motion_state == "idle") break;
                }

                if(this->printing_pause_flag == 1){
                    this->grbl.SerialGotoHome(this->travel_speed);
                    this->grbl.StatusIndexSet();
                    while(true){
                        if(this->grbl.StatusIndexChanged() == 1 && this->grbl.motion_state == "idle" && this->printing_pause_flag == 0) break;
                    }
                }
                if(this->printing_abort_flag == 1) break;
                this->sweep_x_pix = this->sweep_x_pix - this->printing_sweep_size;
            }

            this->current_layer += 1;
            if(this->current_layer >= this->layers){
                cout<<"Last layer printed"<<endl;
                break;
            }
            if(this->printing_abort_flag == 1){
                cout<<"Print aborted"<<endl;
                break;
            }

            int temp_layer_thickness = this->imageconverter.svg_layer_height[this->current_layer] - this->current_layer_height;
            cout<<"Adding new layer, thickness: " <<temp_layer_thickness<<endl;
            this->current_layer_height = this->imageconverter.svg_layer_height[this->current_layer];
            this->grbl.NewLayer(temp_layer_thickness);

        }
        if(this->grbl_connection_state == 1){
            this->grbl.SerialGotoHome(this->travel_speed);
            this->grbl.StatusIndexSet();
            while(true){
                if(this->grbl.StatusIndexChanged() == 1 && this->grbl.motion_state == "idle") break;
            }
        }

        this->printing_state = 0;
    }
}

void MainWindow::PrintArray(){
    if(this->grbl_connection_state == 0) return;

    //declaromos el inkjet_line_buffer

    this->inkjet_lines_left = 0;
    this->inkjet_line_history = "";
    this->travel_speed = 12000.0;
    this->print_speed = 3000.0;

    this->inkjet.ClearBuffer();

    this->grbl.home();

    this->sweep_x_min = 0;
    this->sweep_x_max = 0;
    int temp_break_loop = 0;

    for(int h=0; h<this->imageconverter.image_array_height; h++){
        for(int w=0; w<this->imageconverter.image_array_width; w++){
            if(this->imageconverter.image_array[h][w] != 0){
                this->sweep_x_min = h;
                temp_break_loop = 1;
                cout<<"X-min on row "<<h<<endl;
                break;
            }
        }
        if(temp_break_loop == 1){
            break;
        }
    }
    temp_break_loop = 0;
    for(int h = this->imageconverter.image_array_height; h>0; h++){
        for(int w=0; w<this->imageconverter.image_array_width; w++){
            if(this->imageconverter.image_array[h][w] != 0){
                this->sweep_x_max = h;
                temp_break_loop = 1;
                cout<<"X-max on row "<<h<<endl;
                break;
            }
        }
        if(temp_break_loop == 1){
            break;
        }
    }

    this->sweep_size = this->imageconverter.GetDPI()/2;
    cout<<"Sweep size: "<<this->sweep_size<<endl;
    this->pixel_to_pos_multiplier = 25.4/this->imageconverter.GetDPI();

    this->y_start_pos = 100.0;
    this->x_start_pos = 150.0;
    this->y_acceleration_distance = 25.0;

    this->sweep_x_min_pos = this->sweep_x_min;

    int temp_sweep_stop = 0;
    while(temp_sweep_stop == 0){
        if(this->sweep_x_min_pos + this->sweep_size <= this->sweep_x_max){
            this->sweep_x_max_pos = this->sweep_x_min_pos + this->sweep_size;
        }
        else{
            this->sweep_x_max_pos = this->sweep_x_max;
            temp_sweep_stop = 1;
        }
        cout<<"Sweep from: " <<this->sweep_x_min_pos << ", to: " <<this->sweep_x_max_pos<<endl;
        this->sweep_y_min = 0;
        this->sweep_y_max = 0;

        temp_break_loop = 0;

        for(int w = 0; w < this->imageconverter.image_array_width; w++){
            for(int h = this->sweep_x_min_pos; h < this->sweep_x_max_pos; h++){
                if(this->imageconverter.image_array[h][w]!=0){
                    this->sweep_y_min = w;
                    temp_break_loop = 1;
                    break;
                }
            }
            if(temp_break_loop == 1){
                break;
            }
        }
        temp_break_loop = 0;
        for(int w = this->imageconverter.image_array_width; w>0; w--){
            for(int h=this->sweep_x_min_pos; h<this->sweep_x_max_pos; h++){
                if(this->imageconverter.image_array[h][w] != 0){
                    this->sweep_y_max = w;
                    temp_break_loop = 1;
                    break;
                }
            }
            if(temp_break_loop == 1) break;
        }
        cout<<"sweep Y min: "<<this->sweep_y_min <<", Y max: "<<this->sweep_y_max<<endl;

        this->printing_direction = 1;
        if(this->printing_direction == 1){
            this->y_printing_start_pos = this->sweep_y_min * this->pixel_to_pos_multiplier;
            this->y_printing_start_pos += this->y_start_pos - this->y_acceleration_distance;
            this->y_printing_end_pos = this->sweep_y_max * this->pixel_to_pos_multiplier;
            this->y_printing_end_pos += this->y_start_pos + this->y_acceleration_distance;
            cout<<"Sweep ranges from: " <<this->y_printing_start_pos<< "mm, to: " <<this->y_printing_end_pos<< "mm"<<endl;
        }
        this->x_printing_pos = this->sweep_x_min_pos * this->pixel_to_pos_multiplier;
        this->x_printing_pos += this->x_start_pos;

        cout<<"Filling local buffer with inkjet"<<endl;

        string temp_line_history = "";
        string temp_line_string = "";
        vector<int> temp_line_array(this->sweep_size, 0);
        temp_line_history = this->b64.B64ToArray(temp_line_array);
        temp_line_string = temp_line_history;

        if(this->printing_direction == 1){
            int temp_pos = ((this->sweep_y_min - 1)*this->pixel_to_pos_multiplier) + this->y_start_pos;
            temp_pos *= 1000;
            string temp_b64_pos = b64.B64ToSingle(temp_pos);
            this->inkjet_line_buffer.push_back("SBR "+ temp_b64_pos + " " + temp_line_string);
            this->inkjet_lines_left += 1;
        }

        for(int w = this->sweep_y_min; w<this->sweep_y_max; w++){
            int temp_line_changed = 0;
            int temp_counter;
            for(int h = this->sweep_x_min_pos; h<this->sweep_x_max_pos; h++){
                temp_line_array[temp_counter] = this->imageconverter.image_array[h][w];
                temp_counter++;
            }
            temp_line_string = b64.B64ToArray(temp_line_array);
            if(temp_line_string != temp_line_history){
                temp_line_history = temp_line_string;
                int temp_pos = (w * this->pixel_to_pos_multiplier) + this->y_start_pos;
                temp_pos *= 1000;
                string temp_b64_pos = b64.B64ToSingle(temp_pos);
                this->inkjet_line_buffer.push_back("SBR "+ temp_b64_pos + " " + temp_line_string);
                this->inkjet_lines_left += 1;
            }
        }

        temp_line_array.clear();
        vector<int> temp_line_array_T(this->printing_sweep_size, 0);// OJO AQUI
        temp_line_array = temp_line_array_T;
        temp_line_string = b64.B64ToArray(temp_line_array);
        if(this->printing_direction == 1){
            int temp_pos = ((this->sweep_y_max + 1) * this->pixel_to_pos_multiplier) + this->y_start_pos;
            temp_pos *= 1000;
            string temp_b64_pos = b64.B64ToSingle(temp_pos);
            this->inkjet_line_buffer.push_back("SBR "+ temp_b64_pos + " " + temp_line_string);
            this->inkjet_lines_left += 1;
        }

        cout<<"Making printing buffer done: "<<endl;

        while(this->grbl.motion_state != "idle"){
            continue;
        }
        cout<<"break from idle, moving to filling buffers"<<endl;

        this->InkjetSetPosition();

        cout<<"Filling inkjet buffer"<<endl;
        int temp_lines_sent = 0;

        while(true){
            if(this->inkjet_lines_left > 0){
                this->inkjet.SerialWriteBufferRaw(this->inkjet_line_buffer[0]);
                cout<<this->inkjet_line_buffer[0]<<endl;
                //Eliminar ese elemento
                this->inkjet_lines_left -= 1;
                temp_lines_sent += 1;
            }
            else break;
        }
        cout<<"filling motion buffer"<<endl;

        this->grbl.SerialGotoXY(this->x_printing_pos, this->y_printing_start_pos, this->travel_speed);
        this->grbl.SerialGotoXY(this->x_printing_pos, this->y_printing_end_pos, this->print_speed);

        while(true){
            if(this->grbl.StatusIndexChanged() == 1 && this->grbl.motion_state == "idle"){
                cout<<"break conditions for print while loop"<<endl;
                break;
            }
        }
        this->sweep_x_min_pos += this->sweep_size;
    }
    cout<<"Printing done"<<endl;
}

void MainWindow::SavePng(){
    if(this->file_loaded == 2){
        //if not os.path.exists('demo'):
        //os.makedirs('demo')#make demo folder
        for(int L=0; L<this->imageconverter.svg_layers; L++){
            cout<<"Layer "<<L<<endl;
            this->imageconverter.SVGLayerToArray(L);
            this->RenderOutput();
            //this->imageconverter.output_image.save("demo\Layer" + to_string(L)+ ".png", "PNG");
        }
    }
}
void MainWindow::inkjet_preheat(){}








void MainWindow::on_horizontalSlider_2_actionTriggered(int action)
{
    //this->temp_threshold = action;
    //this->ui->label_9->setText(QString::fromStdString("Threshold: " + to_string(action)));
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    this->temp_threshold = value;
    this->ui->label_9->setText(QString::fromStdString("Threshold: " + to_string(value)));
}
