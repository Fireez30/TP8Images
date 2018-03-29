#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "CBlock.h"

using namespace std;

namespace Test {
    void TestMalloc (ImageBase & ImgIn) {
        ImageBase ImgOut (ImgIn.getWidth() / 2, ImgIn.getHeight() / 2, ImgIn.getColor());
        ImageBase ImgOut2 (ImgIn.getWidth() / 4, ImgIn.getHeight() / 4, ImgIn.getColor());
        ImgOut2.~ImageBase();
        ImageBase ImgOut3 (ImgIn.getWidth() / 6, ImgIn.getHeight() / 6, ImgIn.getColor());
        ImageBase ImgOut4 (ImgIn.getWidth() / 8, ImgIn.getHeight() / 8, ImgIn.getColor());
        ImageBase ImgOut5 (ImgIn.getWidth() / 9, ImgIn.getHeight() / 9, ImgIn.getColor());

        cout << "patate" << endl;
    }

    void TestImgIn (ImageBase & ImgIn) {
        ImgIn.save("TestingImgIn.pgm");
        exit(-2);
    }

    void NewApplyBloc (ImageBase & ImgIn, vector<CBlock> & blocs) {
        unsigned NumBloc = 0;
        unsigned NbBlocLine = 0;
        unsigned Modulo = blocs[0].getxMax() - blocs[0].getxMin();
        for (unsigned i (0); i < ImgIn.getHeight(); i++) {
            for (unsigned j (0); j < ImgIn.getWidth(); j++) {
                if (j > 0 && j % Modulo == 0) NumBloc++;

                string ImgName = blocs[NumBloc + NbBlocLine].getImageUtileName();
                ImageBase Bloc;

                char *cstr = new char[ImgName.length()];
                strcpy(cstr, ImgName.c_str());
                Bloc.load(cstr);

                ImgIn[i][j] = Bloc[i % Modulo][j % Modulo];
            }
            NumBloc = 0;
            if (i > 0 && i % Modulo == 0) NbBlocLine = NbBlocLine + (ImgIn.getHeight() / Modulo);
        }
    }

    void NewResize (ImageBase & ImgIn) {

    }
}

namespace {
    void LoadFiles(vector<string> & FileName, int number){
        cout << "Ecriture des fichiers dans le vector de noms " << endl;
        for (unsigned i (0); i < number; i++)
            FileName[i] = "BanqueImage/" + to_string(i) + ".pgm";
    }

    vector<CBlock> divisionImage(ImageBase & ImgIn, int pas){
        cout << "Division de l image en blocs " << endl;
        vector<CBlock> result;

        for (unsigned i (0); i < ImgIn.getHeight(); i += pas)
            for (unsigned j (0); j < ImgIn.getWidth(); j += pas){
                CBlock tmp (i, i + pas, j, j + pas);
                result.push_back(tmp);
            }

        return result;
    }

    void CreateBlockImage (ImageBase & Img, char* cNomImgEcrite, vector<CBlock> & blocs) {
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

    string resizeImageBy2(ImageBase & ImgIn, string Name){
        ImageBase ImgOut (ImgIn.getWidth() / 2, ImgIn.getHeight() / 2, ImgIn.getColor());

        int cpti = 0;
        int cptj = 0;

        for (unsigned i (0); i < ImgIn.getHeight() - 1; i += 2){

            for (unsigned j (0); j < ImgIn.getWidth() - 1; j += 2){
                double moyenne = (ImgIn[i][j] + ImgIn[i][j+1] + ImgIn[i+1][j] + ImgIn[i+1][j+1]) / 4.0;
                ImgOut[cpti][cptj] = moyenne;
                cptj++;
            }
            //cout << (int) ImgOut[cpti][cptj] << endl;
            cpti++;
            cptj = 0;
        }

        string FileName = Name.substr(0, Name.size() - 5) + to_string(ImgOut.getHeight()) + ".pgm";
        char *cstr = new char[FileName.length()];
        strcpy(cstr, FileName.c_str());
        ImgOut.save(cstr);
        return FileName;
    }

    string resizeImageToBlockSize(ImageBase ImgIn, CBlock Bloc){
        string FileName;
        while (ImgIn.getWidth() > Bloc.getxMax() - Bloc.getxMin()){
            FileName = resizeImageBy2(ImgIn, Bloc.getImageUtileName());
            char *cstr = new char[FileName.length()];
            strcpy(cstr, FileName.c_str());
            ImgIn.load(cstr);
        }
        return FileName;
    }

    string applyBlockToImage(ImageBase ImgIn, CBlock Bloc){
        ImageBase use;
        ImageBase resized;

        string UseName = Bloc.getImageUtileName();
        char *cstr = new char[UseName.length()];
        strcpy(cstr, UseName.c_str());
        use.load(cstr);


        string FileName = resizeImageToBlockSize(use, Bloc);
        char *cstr2 = new char[FileName.length()];
        strcpy(cstr2, FileName.c_str());
        resized.load(cstr2);


        int cmpimageX = 0;
        int cmpimageY = 0;

        for (int i = Bloc.getxMin(); i < Bloc.getxMax(); i++){
            for (int j = Bloc.getyMin(); j < Bloc.getyMax(); j++){
                ImgIn[i][j] = resized[cmpimageX][cmpimageY];
                cmpimageY++;
            }
            cmpimageX++;
            cmpimageY=0;
        }

        ImgIn.save("result.pgm");
    }

    void ecritureMoyennesBanque(const vector<string> & FileName){
        cout << "Ecriture des moyennes ! " << endl;
        ofstream File ("moyennes.csv", ios::out);
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
        ifstream File("moyennes.csv", ios::in);
        if (File){
            cout << "Fichier ouvert ! " << endl;
        }
        else {
            cout << "Erreur lors de l'ouverture" << endl;
            exit(-1);
        }

        while (!File.eof()){
            string name;
            int moyenne;

            File >> name >> moyenne;
            if (name == pic)
                return moyenne;
        }

        cout << "Image non trouvée ! " << endl;
        return -1;
    }

    vector<pair<string, double>> getAllMoy () {
        cout << "Recuperation des moyennes " << endl;
        ifstream File("moyennes.csv", ios::in);
        if (File){
            cout << "Fichier ouvert ! " << endl;
        }
        else {
            cout << "Erreur lors de l'ouverture" << endl;
            exit(-1);
        }

        vector<pair<string, double>> AllMoy;
        pair<string, double> CurrentPair;

        while (!File.eof()) {

            File >> CurrentPair.first >> CurrentPair.second;

            AllMoy.push_back(CurrentPair);
        }
        cout << "Fin recuperation des moyennes " << endl;
        return AllMoy;
    }

}

int main(int argc, char **argv)
{
    char cNomImgEcrite[250];
    if (argc != 2)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return -1;
    }
    sscanf(argv[1],"%s",cNomImgEcrite);

    vector<string> FileName (10001);
    LoadFiles(FileName,10001);

    /*string ts;
    cout << "Mise a jour des moyennes necessaire ? oui ou non" << endl;
    cin >> ts;

    if (ts == "oui"){
        ecritureMoyennesBanque(FileName);
    }
*/
    ImageBase ImgIn;
    string str = "MHX.pgm";
    char *cstr = new char[str.length()];
    strcpy(cstr, str.c_str());
    ImgIn.load(cstr);

    vector<CBlock> blocs = divisionImage(ImgIn,16);

    vector<pair<string,double>> Ms = getAllMoy();

    //Test::TestMalloc(ImgIn);



    for (int i = 0; i < blocs.size(); i++){
        blocs[i].DistanceWithMoyenne(Ms);
        applyBlockToImage(ImgIn, blocs[i]);
    }

    //ImgIn.save("result.pgm");

    return 0;

}
