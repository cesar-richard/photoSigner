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
#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <dirent.h>

using namespace std;
using namespace Magick;


/*static void Usage ( char **argv )
{
  cout << "Usage: " << argv[0]
       << " input_file output_file" << endl;
  exit(1);
}
*/

void proccessImg(string path, string fileName){
    string copyrightString = "C.Richard";
        Image image(path + fileName);
        image.fileName(fileName);

        Image mark("wm.png");
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
        image.attribute("exif:Copyright","Cesar Richard");
        image.attribute("exif:Artist","Cesar Richard");
        //image.annotate("COUCOU",SouthWestGravity);

        int value = MaxRGB - (MaxRGB/9);
        Color couleur = Color(value,value,value);
        couleur.alpha(0.85);

        image.strokeColor(couleur); // Outline color
        image.fillColor(couleur); // Fill color
        image.strokeWidth(1);
        image.font("");
        image.fontPointsize(16);
        // Draw a String
        image.draw( DrawableText(15,image.rows()-15, copyrightString) );

        image.composite(mark,SouthEastGravity);

        image.write(path + "OKI_" + image.fileName());
        //image.display( );
}

void listFile(string path){

        DIR *pDIR;
        struct dirent *entry;
        if( (pDIR=opendir(path.c_str())) ){
                while((entry = readdir(pDIR))){
                        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                        proccessImg(path,entry->d_name);
                }
                closedir(pDIR);
        }
}

int main(int argc,char **argv)
{
  // Initialize ImageMagick install location for Windows
  InitializeMagick(*argv);

    try {
        //proccessImg(argv[1]);
        listFile(argv[1]);
    }
    catch( exception &error_ ){
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
    }
    return 0;
}
