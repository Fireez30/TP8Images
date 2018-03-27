#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
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

    vector<CBlock> divisionImage(ImageBase & ImgIn,int pas){
        vector<CBlock> result;

        for (unsigned i (0); i < ImgIn.getHeight(); i += pas)
            for (unsigned j (0); j < ImgIn.getWidth(); j += pas){
                CBlock tmp (i, i + pas, j, j + pas);
                result.push_back(tmp);
            }

        return result;
    }

    void CreateBlockImage (ImageBase & Img,char* cNomImgEcrite, vector<CBlock> & blocs) {
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

    ImageBase resizeImageBy2(ImageBase & in){
        ImageBase Out (in.getWidth()/2,in.getHeight()/2,in.getColor());

        int cpti = 0;
        int cptj = 0;

        for (unsigned i (0); i < in.getHeight(); i += 2){
            for (unsigned j (0); j < in.getWidth() - 1; j += 2){
                int moyenne = (in[i][j] + in[i][j+1]) / 2;
                Out[cpti][cptj] = moyenne;
                cptj++;
            }
            cpti++;
        }

        return Out;
    }

    ImageBase resizeImageToBlockSize(ImageBase in,CBlock b){
        ImageBase tmp = in;
        ImageBase result;
        while (tmp.getWidth() > b.getxMax() - b.getxMin()){
            result = resizeImageBy2(tmp);
            tmp = result;
        }

        return result;
    }

    void applyBlockToImage(ImageBase & in,CBlock b){
        ImageBase use;
        ImageBase resized;

        use = b.getImageUtile();
        resized = resizeImageToBlockSize(use,b);

        int cmpimageX = 0;
        int cmpimageY = 0;

        for (int i = b.getxMin(); i < b.getxMax(); i++){
            cmpimageX++;
            for (int j = b.getyMin(); j < b.getyMax(); j++){
                in[i][j] = resized[cmpimageX][cmpimageY];
                cmpimageY++;
            }
        }
    }

    void ecritureMoyennesBanque(vector<string> FileName){
        ofstream File ("moyennes.csv",ios::out | ios::ate);
        if (File){
            cout << "Fichier ouvert ! " << endl;
        }
        else {
            cout << "Erreur lors de l'ouverture" << endl;
            exit(-1);
        }

        for (int i = 0; i < FileName.size(); i++){
            ImageBase ImgIn;
            double moyenne = 0;
            string str = FileName[i];
            char *cstr = new char[str.length()];
            strcpy(cstr, str.c_str());
            ImgIn.load(cstr);

            double Total = 0.0;

            for (unsigned i (0); i < ImgIn.getHeight(); ++i)
             for (unsigned j (0); j < ImgIn.getWidth(); ++j)
            Total += ImgIn[i][j];

            moyenne = Total / ImgIn.getTotalSize();

            
            File << FileName[i] << " " << moyenne << endl;
        }
        File.close();
    }

    int getMoyenneWithPicName(string pic){
        ifstream File("moyennes.csv",ios::in);
        if (File){
            cout << "Fichier ouvert ! " << endl;
        }
        else {
            cout << "Erreur lors de l'ouverture" << endl;
            exit(-1);
        }

        while (!testFichierVide){
            string name;
            int moyenne;

            File >> name >> moyenne;
            if (name == pic)
                return moyenne;
        }

        cout << "Image non trouvée ! " << endl;
        return -1;
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

    vector<CBlock> blocs = divisionImage(ImgIn,16);
    CreateBlockImage(ImgIn, cNomImgEcrite, blocs);


    return 0;

}
