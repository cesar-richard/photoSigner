// This may look like C code, but it is really -*- C++ -*-
//
// Copyright Bob Friesenhahn, 2001, 2002, 2003
//
// Resize image using specified resize algorithm with Magick++ API
//
// Usage: zoom [-density resolution] [-filter algorithm] [-geometry geometry]
//             [-resample resolution] input_file output_file
//

#include <Magick++.h>
#include <tclap/CmdLine.h>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <exiv2/exiv2.hpp>

using namespace std;
using namespace Magick;
using namespace TCLAP;

string title;
string copyrightString;
string path;
string watermark;
string outputName;

void parseOptions(int argc, char** argv)
{
    try
    {
        CmdLine cmd("Command description message", ' ', "0.1");

        ValueArg<string> titleArg("t","title","Event's title",true,"","string");
        cmd.add( titleArg );

        ValueArg<string> copyrightArg("c","copyright","Copyright string",true,"","string");
        cmd.add( copyrightArg );

        ValueArg<string> pathArg("p","path","Path to the photos",false,"","string");
        cmd.add( pathArg );

        ValueArg<string> watermarkArg("w","watermark","path to watermark file",true,"wm.png","string");
        cmd.add( watermarkArg );
        
        ValueArg<string> outputFileNameArg("o","outputFileName","Output file name",true,"output","string");
        cmd.add( outputFileNameArg );
        
        cmd.parse( argc, argv );

        // Get the value parsed by each arg.
        copyrightString = copyrightArg.getValue();
        path = pathArg.getValue();
        title = titleArg.getValue();
        watermark = watermarkArg.getValue();
        outputName = outputFileNameArg.getValue();
    }
    catch (ArgException &e)    // catch any exceptions
    {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    }
}

/*static void Usage ( char **argv )
{
  cout << "Usage: " << argv[0]
       << " input_file output_file" << endl;
  exit(1);
}
*/

int addTags(string FilePath){
    string copyrightString = "C.Richard";

    try {

        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(FilePath);
        image->readMetadata();

        Exiv2::IptcData &iptcData = image->iptcData();
        if (iptcData.empty()) {
            FilePath += ": No IPTC data found in the file";
            throw Exiv2::Error(1, FilePath);
        }
        iptcData.clear();
        Exiv2::Value::AutoPtr value = Exiv2::Value::create(Exiv2::string);
        value->read("2014");
        iptcData.add(Exiv2::IptcKey("Iptc.Application2.Keywords"), value.get());
        value->read("HELLO");
        iptcData.add(Exiv2::IptcKey("Iptc.Application2.Headline"),value.get());
        image->setIptcData(iptcData);
        image->writeMetadata();

        return 0;
    }
    catch (Exiv2::AnyError& e) {
        std::cout << "Caught Exiv2 exception '" << e << "'\n";
        return -1;
    }

}

void proccessImg(string path, string fileName, string nameIndex, string outputFileName, string titleString, string watermarkPath,string copyright){
    addTags(path+fileName);
        Image image(path + fileName);
        image.fileName(fileName);
		
		Image mark(watermarkPath);//TODO mettre en param
        cout << "File Name  = \t " << image.baseFilename().c_str() << endl;
        //cout << "Original Width = " << image.baseColumns() << "px"  << endl;
        //cout << "Original heigth = " << image.baseRows() << "px" << endl;
        if(image.baseColumns()>1600||image.baseRows()>1600){
            cout << "Resizing..." << endl;
                image.resize(Geometry("1600x1600"));
        }else if(image.baseColumns()==1600||image.baseRows()==1600){
            cout << "Image deja redimensionnee." << endl;
        }else{
            cout << "Image plus petite que la cible." << endl;
        }

        image.attribute("EXIF:Copyright",copyright);//TODO Ajouter le reste des TAGs
        image.attribute("EXIF:Artist",copyright);
        image.attribute("IPTC:By-line",copyright);
        image.attribute("IPTC:Creator",copyright);
        image.attribute("IPTC:Credit",copyright);
        image.attribute("IPTC:CopyrightNotice",copyright);
        image.attribute("IPTC:ObjectName",titleString);
        image.attribute("IPTC:Title",titleString);

        int value = MaxRGB - (MaxRGB/10);
        Color couleur = Color(value,value,value);
        couleur.alpha(0.7);

        image.strokeColor(couleur); // Outline color
        image.fillColor(couleur); // Fill color
        image.strokeWidth(1);
        image.font("");
        image.fontPointsize(24);
        // Draw a String
        image.draw( DrawableText(15,image.rows()-15, copyrightString) );

        image.composite(mark,SouthEastGravity,DissolveCompositeOp);
        image.write(path + outputFileName + "_" + nameIndex + ".jpg");
}

void listFile(string path, string outputFileName, string titleString, string watermarPath, string copyright){
        DIR *pDIR;
        struct dirent *entry;
        int counter=0;
        std::stringstream ss;
        if( (pDIR=opendir(path.c_str())) ){
                while((entry = readdir(pDIR))){
                        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
                            ss.clear();
                            ss.str("");
                            ss << (++counter);
                            proccessImg(path,entry->d_name,ss.str(),outputFileName,titleString,watermarPath,copyright);
                        }
                }
                closedir(pDIR);
        }
}

int main(int argc,char **argv)
{
	parseOptions(argc,argv);
  // Initialize ImageMagick install location for Windows
  InitializeMagick(*argv);

    try {
        listFile(path,outputName,title,watermark,copyrightString); //TODO: mettre au propre le moteur d'arguments
    }
    catch( exception &error_ ){
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
    }
    return 0;
}