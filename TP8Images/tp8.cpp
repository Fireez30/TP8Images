#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include "CBlock.h"

using namespace std;

namespace {
    void LoadFiles(vector<string> & FileName, int number){
        for (unsigned i (0); i < number; i++)
            FileName[i] = to_string(i) + ".pgm";
    }

    vector<CBlock> divisionImage(ImageBase & ImgIn){
        vector<CBlock> result;

        for (unsigned i (0); i < ImgIn.getHeight(); i += 16)
            for (unsigned i (0); j < ImgIn.getWidth(); j += 16){
                CBlock tmp (i, i + 16, j, j + 16);
                result.push_back(tmp);
            }

        return result;
    }

    void CreateBlockImage (ImageBase & Img, const char & cNomImgEcrite[], vector<CBlock> & blocs) {
        ImageBase ImgOut(Img.getWidth(), Img.getHeight(), Img.getColor());

        for(int y = 0; y < blocs.size(); y++)
        {
            blocs[y].CritereWithMoyenne(Img);
            for (int i = blocs[y].getxMin(); i < blocs[y].getxMax(); i ++)
                for (int j = blocs[y].getyMin(); j < blocs[y].getyMax(); j ++){
                    ImgOut[i][j] = blocs[y].getCritere();
                }
        }
        ImgOut.save(cNomImgEcrite);
    }
}

int main(int argc, char **argv)
{
    char cNomImgEcrite[250];
    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return -1;
    }
    sscanf(argv[2],"%s",cNomImgEcrite);

    vector<string> FileName (10001);
    LoadFiles(FileName,10001);

    ImageBase ImgIn;
    string str = "MHX.pgm";
    char *cstr = new char[str.length()];
    strcpy(cstr, str.c_str());
    ImgIn.load(cstr);

    vector<CBlock> blocs = divisionImage(ImgIn);
    CreateBlockImage(ImgIn, cNomImgEcrite, blocs);


    return 0;

}
