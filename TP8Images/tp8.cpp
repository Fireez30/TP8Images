#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "CBlock.h"

using namespace std;

namespace {
    vector <unsigned> HistogrammeBase (ImageBase Img) {

        vector <unsigned> Histo (256);

        for (unsigned i (0); i < Img.getHeight(); i++)
            for (unsigned j (0); j < Img.getWidth(); j++)
                for (unsigned k (0); k < 256; ++k) {
                    if (Img[i][j] == k) {Histo[k]++;
                        break;}
                }

        return Histo;
    }

    void HistogrammeBanque (string FileName) {
        ImageBase Img;
        char *cstr = new char[FileName.length()];
        strcpy(cstr, FileName.c_str());
        Img.load(cstr);

        vector <unsigned> Histo (256);
        string tmp = FileName.substr(0, FileName.size() - 4);
        ofstream Dat (tmp + ".dat");

        for (unsigned i (0); i < Img.getHeight(); i++)
            for (unsigned j (0); j < Img.getWidth(); j++)
                for (unsigned k (0); k < 256; ++k) {
                    if (Img[i][j] == k) {Histo[k]++;
                        break;}
                }
        for (unsigned i (0); i < Histo.size(); ++i)
            Dat << Histo[i] << endl;
    }

    void WriteHistos(vector<string> FileName){
        for (int i = 0; i < FileName.size(); i++){
            cout << FileName[i] << endl;
            HistogrammeBanque(FileName[i]);
        }
    }

    double ChiSquared(vector<unsigned> histoImage, vector<unsigned> histoBDD){
        double dist = 0;

        for (int i = 0; i < histoImage.size(); i++){
            if (histoImage[i] + histoBDD[i] == 0)
                dist += pow(histoBDD[i] - histoImage[i], 2);
            else
                dist += pow(histoBDD[i] - histoImage[i], 2) / (double) (histoImage[i] + histoBDD[i]);
        }

        return dist;
    }

    void WriteDistances(vector<unsigned> histoImage, vector<string> FileNames){
        ofstream File ("distances.csv", ios::out);
        if (File){
            cout << "Fichier ouvert ! " << endl;
        }
        else {
            cout << "Erreur lors de l'ouverture" << endl;
            exit(-1);
        }
        for (int i = 0; i < FileNames.size(); i++){
            ifstream Data(FileNames[i].substr(0, FileNames[i].size() - 4)+".dat", ios::in);
            if (Data){
                cout << "Fichier ouvert ! " << endl;
            }
            else {
                cout << "Erreur lors de l'ouverture" << endl;
                exit(-1);
            }

            vector<unsigned> histoBDD (256);
            for (int j = 0; j < histoBDD.size(); j++){
                Data >> histoBDD[j];
            }
            Data.close();

            double dist = ChiSquared(histoImage,histoBDD);
            cout << dist << endl;
            File << FileNames[i] << " " << dist << endl;

            histoBDD.clear();
        }

        File.close();
    }

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
            cpti++;
            cptj = 0;
        }

        string FileName = Name.substr(0, Name.size() - 4) + "RESIZED" + to_string(ImgOut.getHeight()) + ".pgm";

        char *cstr = new char[FileName.length()];
        strcpy(cstr, FileName.c_str());
        ImgOut.save(cstr);

        return FileName;
    }

    string resizeImageToBlockSize(string Name, CBlock Bloc){
        string FileName = "BanqueImage/0.pgm";

        ImageBase Img;
        char *cstr = new char[Name.length()];
        strcpy(cstr, Name.c_str());
        Img.load(cstr);

        while (Img.getWidth() > Bloc.getxMax() - Bloc.getxMin()){

            FileName = resizeImageBy2(Img, Bloc.getImageUtileName());

            Img.reset();
            char *cstr2 = new char[FileName.length()];
            strcpy(cstr2, FileName.c_str());
            Img.load(cstr2);
        }

        return FileName;
    }

    void applyBlockToImage(ImageBase & ImgIn, CBlock Bloc){
        ImageBase resized;
        ImageBase ImgOut;
        ImgOut.copy(ImgIn);

        string FileName = resizeImageToBlockSize(Bloc.getImageUtileName(), Bloc);
        char *cstr2 = new char[FileName.length()];
        strcpy(cstr2, FileName.c_str());
        resized.load(cstr2);

        int cmpimageX = 0;
        int cmpimageY = 0;

        for (int i = Bloc.getxMin(); i < Bloc.getxMax(); i++){
            for (int j = Bloc.getyMin(); j < Bloc.getyMax(); j++){
                ImgOut[i][j] = resized[cmpimageX][cmpimageY];
                cmpimageY++;
            }
            cmpimageX++;
            cmpimageY=0;
        }

        ImgOut.save("result1.pgm");
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
            cout << FileName[i] << endl;
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

int main(int argc, char **argv) {

    char cNomImgLu[250];
    char cNomImgEcrite[250];
    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return -1;
    }
    sscanf(argv[1],"%s",cNomImgLu);
    sscanf(argv[2],"%s",cNomImgEcrite);

    vector<string> FileName (10001);
    LoadFiles(FileName, 10001);

    ImageBase ImgIn;
    ImgIn.load(cNomImgLu);

    vector<CBlock> blocs = divisionImage(ImgIn, 16);

    vector<pair<string, double>> Ms = getAllMoy();

    CreateBlockImage(ImgIn, cNomImgEcrite, blocs);

    for (int i = 0; i < blocs.size(); i++){
        cout << "bloc " << i << endl;
        blocs[i].DistanceWithMoyenne(Ms);
        applyBlockToImage(ImgIn, blocs[i]);
        ImgIn.save("patate.pgm");
        ImgIn.load("result1.pgm");
    }

    ImgIn.save("Result Moyenne 16x16.pgm");

    ImgIn.load(cNomImgLu);


    for (int i = 0; i < blocs.size(); i++){
        cout << "bloc " << i << endl;
        blocs[i].CritereWithHistogramme(ImgIn);
        blocs[i].DistanceWithHistogramme(FileName);
    }

    for (int i = 0; i < blocs.size(); i++){
        cout << "Applique le bloc : " << i << " a l'image ! " << endl;
        applyBlockToImage(ImgIn, blocs[i]);
        ImgIn.save("patate.pgm");
        ImgIn.load("result1.pgm");
    }

    ImgIn.save("Result Histogramme 16x16.pgm");

    ImgIn.load(cNomImgLu);

    if (ImgIn.getHeight() % 32 == 0 && ImgIn.getWidth() % 32 == 0) {

        blocs = divisionImage(ImgIn, 32);

        CreateBlockImage(ImgIn, cNomImgEcrite, blocs);

        for (int i = 0; i < blocs.size(); i++){
            cout << "bloc " << i << endl;
            blocs[i].DistanceWithMoyenne(Ms);
            applyBlockToImage(ImgIn, blocs[i]);
            ImgIn.save("patate.pgm");
            ImgIn.load("result1.pgm");
        }

        ImgIn.save("Result Moyenne 32x32.pgm");

        ImgIn.load(cNomImgLu);


        for (int i = 0; i < blocs.size(); i++){
            cout << "bloc " << i << endl;
            blocs[i].CritereWithHistogramme(ImgIn);
            blocs[i].DistanceWithHistogramme(FileName);
        }

        for (int i = 0; i < blocs.size(); i++){
            cout << "Applique le bloc : " << i << " a l'image ! " << endl;
            applyBlockToImage(ImgIn, blocs[i]);
            ImgIn.save("patate.pgm");
            ImgIn.load("result1.pgm");
        }

        ImgIn.save("Result Histogramme 32x32.pgm");

        ImgIn.load(cNomImgLu);
    }

    blocs = divisionImage(ImgIn, 8);

    CreateBlockImage(ImgIn, cNomImgEcrite, blocs);

    for (int i = 0; i < blocs.size(); i++){
        cout << "bloc " << i << endl;
        blocs[i].DistanceWithMoyenne(Ms);
        applyBlockToImage(ImgIn, blocs[i]);
        ImgIn.save("patate.pgm");
        ImgIn.load("result1.pgm");
    }

    ImgIn.save("Result Moyenne 8x8.pgm");

    ImgIn.load(cNomImgLu);

    for (int i = 0; i < blocs.size(); i++){
        cout << "bloc " << i << endl;
        blocs[i].CritereWithHistogramme(ImgIn);
        blocs[i].DistanceWithHistogramme(FileName);
    }

    for (int i = 0; i < blocs.size(); i++){
        cout << "Applique le bloc : " << i << " a l'image ! " << endl;
        applyBlockToImage(ImgIn, blocs[i]);
        ImgIn.save("patate.pgm");
        ImgIn.load("result1.pgm");
    }

    ImgIn.save("Result Histogramme 8x8.pgm");

    return 0;

}

