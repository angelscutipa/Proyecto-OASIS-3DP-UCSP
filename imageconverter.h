#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H
#include <iostream>
#include <vector>
#include <QImage>
#include <QPixmap>

using namespace std;

class ImageConverter
{
public:

    int dpi = 600;
    int burst_size = this->dpi/2;
    int file_type = 0;
    string file_path = "";
    int image_array_width = 0;
    int image_array_height = 0;

    int svg_layers = 0;
    vector<string> svg_layers_names;
    vector<float> svg_layer_height;
    float svg_width = 0;
    float svg_height = 0;

    ifstream *file_object;

    vector<vector<int>> image_array;

    QImage conv_image;
    QPixmap input_image;
    QPixmap output_image;

    ImageConverter();

    int Openfile(string temp_file_path);
    void Threshold(float temp_threshold);     // Esto depende de la clase de la libreria que se utilice para imagen
    void SetDPI(int temp_dpi);
    int GetDPI();
    void ArrayToImage();
    void RGBToImage();
    void AlphaMaskToImage();
    void SVGGetData();
    int SVGLayerToArray(int temp_layer );    //Revisar el tipo de dato de temp_layer
    void ArrayAddPolygon(string temp_input);
    void ArrayConvert();

};

#endif // IMAGECONVERTER_H
