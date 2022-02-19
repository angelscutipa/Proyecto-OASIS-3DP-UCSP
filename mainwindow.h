#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialgrbl.h"
#include "serialhp45.h"
#include "imageconverter.h"
#include "b64.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SerialGRBL grbl;
    serialHP45 inkjet;
    B64 b64;

    int file_loaded = 0;

    QStringList input_file_name;

    int printing_state = 0;
    int printing_abort_flag = 0;
    int printing_pause_flag = 0;

    int grbl_connection_state = 0;
    int inkjet_connection_state = 0;

    float build_center_x = 0;
    float build_center_y = 0;
    float print_speed = 0;
    float travel_speed = 0;
    float acceleration_distance = 0;
    int printing_dpi = 0;
    int printing_sweep_size = 0;
    float pixel_to_pos_multiplier = 0;
    int image_size_x = 0;
    int image_size_y = 0;
    int layers = 0;
    int current_layer = 0;
    int current_layer_height = 0;

    float svg_offset_y = 0;
    float svg_offset_x = 0;

    int sweep_x_min = 0;
    int sweep_x_max = 0;
    int sweep_x_size = 0;
    int sweeps = 0;
    int sweep_x_pix = 0;
    int sweep_x_pos = 0;

    int sweep_y_min = 0;
    int sweep_y_max = 0;
    int temp_threshold = 128;

    int sweep_y_start_pix = 0;
    int sweep_y_end_pix = 0;
    int sweep_y_start_pos = 0;
    int sweep_y_end_pos = 0;

    int sweep_x_min_pos = 0;
    int sweep_x_max_pos = 0;
    float y_start_pos = 0;
    float x_start_pos = 0;
    float y_acceleration_distance = 0;

    vector<string> inkjet_line_buffer;
    int inkjet_lines_left = 0;
    string inkjet_line_history;
    float sweep_size;
    int printing_direction = 0;

    float y_printing_start_pos = 0;
    float y_printing_end_pos = 0;
    float x_printing_pos = 0;

    ImageConverter imageconverter;
    QPixmap input_image_display;
    QPixmap output_image_display;

    MainWindow(QWidget *parent = nullptr);

    void grblConnect();
    void GrblSendCommand();
    void InkjetConnect();
    void InkjetUpdate();
    void InkjetSendCommand();
    void GRBLSpreader();
    void GRBLNewLayer();
    void GRBLPrimeLayer();
    void OpenFile(string in);
    void UpdateLayer();
    void InkjetSetPosition();
    void inkjet_preheat();
    void InkjetSetDensity();

    void RenderOutput();
    void RenderInput();
    void RenderAlpha();
    void RenderRGB();

    void RunPrintArray();
    void PausePrint();
    void AbortPrint();

    void PrintSVG();
    void PrintArray();
    void SavePng();

    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_horizontalSlider_2_actionTriggered(int action);

    void on_horizontalSlider_2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
