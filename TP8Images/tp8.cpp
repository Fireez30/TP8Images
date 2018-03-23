#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include "CBlock.h"

using namespace std;


void LoadFiles(vector<string> & FileName, int number){
    for (int i = 0;i  < number; i++){
        FileName[i] = to_string(i) + ".pgm";
    }
}

vector<CBlock> divisionImage(ImageBase & ImgIn){
    vector<CBlock> result;
    for (int i = 0; i < ImgIn.getHeight(); i += 16)
        for (int j = 0; j < ImgIn.getWidth(); j += 16){
            CBlock tmp (i, i + 16, j, j + 16);
            result.push_back(tmp);
        }

    return result;
}

int main(int argc, char **argv)
{

    char cNomImgEcrite[250];
    //char cNomImgLue[250];

    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return -1;
    }

    //sscanf(argv[1],"%s",cNomImgLue) ;
    sscanf(argv[2],"%s",cNomImgEcrite);

    vector<string> FileName (10001);
    vector<CBlock> blocs;

    LoadFiles(FileName,10001);

    /*for (int i = 0; i < FileName.size(); i++){
        cout << "Fichier : " << FileName[i] << endl;
    }*/

    ImageBase ImgIn;

    //string str = "BanqueImage/"+FileName[100];
    string str = "MHX.pgm";
    char *cstr = new char[str.length()];
    strcpy(cstr, str.c_str());

    ImgIn.load(cstr);
    //ImgIn.load(cNomImgLue);

    ImageBase ImgOut(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());

    blocs = divisionImage(ImgIn);
    cout << "division finie " << endl;
    for(int y = 0; y < blocs.size(); y++)
    {
        blocs[y].CritereWithMoyenne(ImgIn);
        for (int i = blocs[y].getxMin(); i < blocs[y].getxMax(); i ++)
               for (int j = blocs[y].getyMin(); j < blocs[y].getyMax(); j ++){
                    ImgOut[i][j] = blocs[y].getCritere();
               }
    }
    cout << "save" << endl;
    ImgOut.save(cNomImgEcrite);

    return 0;

}
