#include "imageconverter.h"
#include <fstream>
#include <algorithm>

vector<vector<int>> Czeros(int h, int w){
    vector<vector<int>> vec;
    for(int i=0; i<h; i++){
        vector<int> a;
        for(int j=0; j<w; j++){
            a.push_back(0);
        }
        vec.push_back(a);
    }

    return vec;
}

vector<string> partition(string in, string limit){
    vector<string> salida;


    return salida;
}

template <std::ctype_base::mask mask>
class IsNot
{
    std::locale myLocale;       // To ensure lifetime of facet...
    std::ctype<char> const* myCType;
public:
    IsNot( std::locale const& l = std::locale() )
        : myLocale( l )
        , myCType( &std::use_facet<std::ctype<char> >( l ) )
    {
    }
    bool operator()( char ch ) const
    {
        return ! myCType->is( mask, ch );
    }
};

typedef IsNot<std::ctype_base::space> IsNotSpace;

std::string lstrip( std::string const& original )
{
    std::string::const_iterator right = std::find_if( original.rbegin(), original.rend(), IsNotSpace() ).base();
    std::string::const_iterator left = std::find_if(original.begin(), right, IsNotSpace() );
    return std::string( left, right );
}


ImageConverter::ImageConverter()
{

}


int ImageConverter::Openfile(string temp_file_path){

    if (temp_file_path.size()==0){
        this->file_type = 0;
        return 0;
    }

    else{   //MUCHO MUCHO CUIDADO AQUI QUE SI LA RUTA TIENE CONTRASLASH PODRIA HABER UN GRAN ERROR
        this->file_path = temp_file_path;
        string filename = this->file_path.substr(this->file_path.find_last_of("/\\") + 1);
        string file_extension = this->file_path.substr(this->file_path.find_last_of("."));

        if (file_extension == ".svg") this->file_type = 2;

        else this->file_type = 1;

        int temp_success = 0;
        if(file_type == 1){
            try {
                QString qstr = QString::fromStdString(this->file_path);  // OJO
                conv_image = QImage(qstr);
                this->input_image = QPixmap::fromImage(this->conv_image);
                this->output_image = QPixmap::fromImage(this->conv_image);
                temp_success = 1;
            }  catch (exception) {
                cout<<"0"<<endl;
            }
            if(temp_success == 1){
                this->image_array_width = conv_image.width();
                this->image_array_height = conv_image.height();
                this->image_array = Czeros(this->image_array_height, this->image_array_width);  //Con libreria numpy c++
                return 1;
            }
        }

        if(file_type == 2){
            try {
                file_object=new ifstream();
                file_object->open(this->file_path);// COn cuidado aqui en python se lee
                temp_success = 1;
            }  catch (exception) {
                cout<<0<<endl;
            }
            if(temp_success == 1){
                this->SVGGetData();
                this->image_array = Czeros(this->image_array_height, this->image_array_width);  //Con libreria numpy c++
                this->SVGLayerToArray(0);
                return 2;
            }
        }

    }
    return 3;
}

void ImageConverter::Threshold(float temp_threshold){
    for(int w=0; w<this->image_array_width; w++){
        for(int h=0; h<this->image_array_height; h++){
            QColor pixel_color = this->conv_image.pixelColor(w,h);
            int qred = pixel_color.red();
            int qgreen = pixel_color.green();
            int qblue = pixel_color.blue();
            int alpha = pixel_color.alpha();
            float falpha = float(alpha/255);
            float finalpha = 1.0 - falpha;

            float background = 255 * finalpha;
            float gray = (qred + qgreen + qblue)/3;
            gray = gray * falpha;
            gray = gray + background;

            if(gray < temp_threshold) this->image_array[h][w]=1;
            else this->image_array[h][w]=0;
        }
    }
}

void ImageConverter::SetDPI(int temp_dpi){
    this->dpi=temp_dpi;
}

int ImageConverter::GetDPI(){
    return this->dpi;
}

void ImageConverter::ArrayToImage(){
    this->output_image = QPixmap(this->image_array_width, this->image_array_height);
    QImage temp_image = output_image.toImage();//OJO OJIN OJITO AQUI DEBERIA ENTRAR UN QPIXMAP SEGUN PYTHON PERO NO HAY
    QColor temp_color;
    for(int w=0; w<this->image_array_width; w++){
        for(int h=0; h<this->image_array_height; h++){
            if(image_array[h][w] == 0) temp_color.setRgb(255,255,255);
            else if(image_array[h][w] == 1) temp_color.setRgb(0,0,0);
            temp_image.setPixelColor(w,h,temp_color);
        }
    }
    this->output_image = QPixmap::fromImage(temp_image);
}

void ImageConverter::RGBToImage(){
    QPixmap output_image = QPixmap(this->image_array_width, this->image_array_height);
    QImage temp_image = output_image.toImage();//OJO OJIN OJITO AQUI DEBERIA ENTRAR UN QPIXMAP SEGUN PYTHON PERO NO HAY
    QColor temp_color;
    QColor pixel_color;
    for(int w=0; w<this->image_array_width; w++){
        for(int h=0; h<this->image_array_height; h++){
            pixel_color = conv_image.pixelColor(w,h);
            int qred = pixel_color.red();
            int qgreen = pixel_color.green();
            int qblue = pixel_color.blue();

            temp_color.setRgb(qred, qgreen, qblue);
            temp_image.setPixelColor(w,h,temp_color);
        }
    }
    output_image = QPixmap::fromImage(temp_image);
}


void ImageConverter::AlphaMaskToImage(){
    QPixmap output_image = QPixmap(this->image_array_width, this->image_array_height);
    QImage temp_image = output_image.toImage();//OJO OJIN OJITO AQUI DEBERIA ENTRAR UN QPIXMAP SEGUN PYTHON PERO NO HAY
    QColor temp_color;
    QColor pixel_color;
    for(int w=0; w<this->image_array_width; w++){
        for(int h=0; h<this->image_array_height; h++){
            pixel_color = conv_image.pixelColor(w,h);
            int alpha = pixel_color.alpha();
            temp_color.setRgb(alpha, alpha, alpha);
            temp_image.setPixelColor(w,h,temp_color);
        }
    }
    output_image = QPixmap::fromImage(temp_image);
}

void ImageConverter::SVGGetData(){
    int temp_layer_counter = 0;
    this->svg_layers_names.clear();
    this->svg_layer_height.clear();

    //file_object->open(this->file_path);// COn cuidado aqui en python se lee
    string L;
    while(getline(*file_object, L)){
        cout<<L<<endl;
        if(L.rfind("<svg ",0) != string::npos){
            vector<string> temp_decode = partition(L, "<svg ");//No olvidarse de programar esta funcion
            string temp_decode2 = temp_decode[2];
            while(true){
                char temp='"';
                string s(1,temp);
                temp_decode = partition(temp_decode2, s);
                if(temp_decode[1] != s) break;

                if(lstrip(temp_decode[0]) == "width="){
                    temp_decode = partition(temp_decode[2], s);
                    this->svg_width = stof(temp_decode[0]);
                }
                if(lstrip(temp_decode[0]) == "height="){
                    temp_decode = partition(temp_decode[2], s);
                    this->svg_height = stof(temp_decode[0]);
                }
                temp_decode2 = temp_decode[2];
            }
        }

        if(L.rfind("  <g ",0) != string::npos){
            vector<string> temp_decode = partition(L, "<svg ");//No olvidarse de programar esta funcion
            string temp_decode2 = temp_decode[2];
            while(true){
                char temp='"';
                string s(1,temp);
                temp_decode = partition(temp_decode2, s);
                if(temp_decode[1] != s) break;

                if(lstrip(temp_decode[0]) == "id="){
                    temp_decode = partition(temp_decode[2], s);
                    temp_layer_counter += 1;
                    string temp_layer_name = temp_decode[0];
                    this->svg_layers_names.push_back(temp_layer_name);
                }
                if(lstrip(temp_decode[0]) == "slic3r:z="){
                    temp_decode = partition(temp_decode[2], s);
                    float temp_layer_height = stof(temp_decode[0]);
                    temp_layer_height *= 1000000;
                    this->svg_layer_height.push_back(temp_layer_height);
                }
                temp_decode2 = temp_decode[2];
            }
        }
    }

    this->image_array_height = int(this->svg_width / 25.4*this->dpi) + 1;
    this->image_array_width = int(this->svg_height / 25.4*this->dpi) + 1;

    cout<<"Image size: " << image_array_width << "," << this->image_array_height<<endl;
    cout<<"Layers = " << temp_layer_counter<<endl;
}

int ImageConverter::SVGLayerToArray(int temp_layer ){
    if(temp_layer >= this->svg_layers) return 0;
    int temp_line_found = 0;
    ifstream file_object;
    file_object.open (this->file_path);// COn cuidado aqui en python se lee
    string L;
    while(getline(file_object, L)){
        if(L.rfind("  <g ",0)){
            vector<string> temp_decode = partition(L, "  <g ");
            string temp_decode2 = temp_decode[2];
            while(true){
                char temp='"';
                string s(1,temp);
                temp_decode = partition(temp_decode2, s);
                if(temp_decode[1] != s) break;

                if(lstrip(temp_decode[0]) == "id="){
                    temp_decode = partition(temp_decode[2],s);
                    string temp_layer_name = temp_decode[0];
                    if(temp_layer_name == this->svg_layers_names[temp_layer]){
                        temp_line_found = 1;
                        vector<vector<int>> temp_toggle_array = Czeros(this->image_array_height, this->image_array_width);
                        this->image_array = Czeros(this->image_array_height, this->image_array_width);
                    }
                }
                temp_decode2=temp_decode[2];
            }
        }

        if(temp_line_found == 1){
            if(L.rfind("    <polygon ",0)){
                vector<string> temp_decode = partition(L, "    <polygon ");
                string temp_decode2 = temp_decode[2];
                while(true){
                    char temp='"';
                    string s(1,temp);
                    temp_decode = partition(temp_decode2, s);
                    if(temp_decode[1] != s) break;

                    if(lstrip(temp_decode[0]) == "points="){
                        temp_decode = partition(temp_decode[2], s);
                        string temp_points = temp_decode[0];

                        this->ArrayAddPolygon(temp_points);
                    }

                    temp_decode2 = temp_decode[2];
                }
            }
            if(L.rfind("  </g>",0)){
                temp_line_found = 0;
                this->ArrayConvert();
            }
        }
    }
    return 0;
}

void ImageConverter::ArrayAddPolygon(string temp_input){
    temp_input = temp_input + " ";
    vector<float> temp_x;
    vector<float> temp_y;

    float x_start;
    float x_end;
    float y_start;
    float y_end;

    float temp_dpi_multiplier = 25.4/this->dpi;

    while(true){
        vector<string> temp_input2 = partition(temp_input, " ");
        if(temp_input2[1] != " "){
            break;
        }
        vector<string> temp_xy= partition(temp_input2[0], ",");

        temp_x.push_back(stof(temp_xy[0]));
        temp_y.push_back(stof(temp_xy[2]));

        temp_input = temp_input2[2];
    }
    int temp_array_size=temp_x.size();
    for(int i=0; i<temp_array_size; i++){
        if(temp_x[i-1] < temp_x[i]){
            x_start = temp_x[i - 1];
            x_end = temp_x[i];
            y_start = temp_y[i-1];
            y_end = temp_y[i-1];
        }
        else{
            x_start = temp_x[i - 1];
            x_end = temp_x[i];
            y_start = temp_y[i-1];
            y_end = temp_y[i-1];
        }

        int x_pixel_start = int(x_start/temp_dpi_multiplier);
        int x_pixel_end = int(x_end/temp_dpi_multiplier) + 1;

        if(x_start != x_end){
            for(int x=x_pixel_start; x<x_pixel_end; x++){
                float temp_x_pos = float(x * temp_dpi_multiplier);
                if(temp_x_pos >= x_start && temp_x_pos <= x_end){
                    float delta_x = x_end - x_start;
                    float delta_y = y_end - y_start;
                    float calc_x = temp_x_pos - x_start;
                    float calc_p = calc_x/delta_x;

                    float calc_y = float(delta_y * calc_p) + y_start;
                    int temp_y_pixel = int(calc_y / temp_dpi_multiplier);

                    if((x >= 0) && (x < this->image_array_height) && (temp_y_pixel >= 0) && (temp_y_pixel < this->image_array_width)){
                        if(this->image_array[x][temp_y_pixel] == 0){
                            this->image_array[x][temp_y_pixel] = 1;
                        }
                        else{
                            this->image_array[x][temp_y_pixel] = 0;
                        }
                    }
                }
            }
        }
    }
}

void ImageConverter::ArrayConvert(){
    for(int x=0; x<this->image_array_height; x++){
        int temp_toggle_state=0;
        for(int y=0; y<this->image_array_width; y++){
            if(this->image_array[x][y] != 0){
                if(temp_toggle_state == 1) temp_toggle_state = 0;
                else temp_toggle_state = 1;
            }
            this->image_array[x][y] = temp_toggle_state;
        }
    }
}


