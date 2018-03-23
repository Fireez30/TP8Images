#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

/*void erosion(ImageBase imIn,ImageBase & imOut){

    for(int x = 1; x < imIn.getHeight()-1; ++x)
        for(int y = 1; y < imIn.getWidth()-1; ++y)
        {
            if (imIn[x][y] == 0)
            {
                imOut[x][y] = 0;
                imOut[x-1][y-1] = 0;
                imOut[x+1][y+1] = 0;
                imOut[x-1][y] = 0;
                imOut[x+1][y] = 0;
                imOut[x][y-1] = 0;
                imOut[x][y+1] = 0;
                imOut[x+1][y-1] = 0;
                imOut[x-1][y+1] = 0;
            }
        }

}
*/

int seuil(int argc, char **argv) {

    char cNomImgLue[250], cNomImgEcrite[250];
    int seuil;

    if (argc != 4)
    {
        cout << "Usage: ImageIn.pgm valeurSeuil" << endl;
        return -1;
    }
    sscanf(argv[1],"%s",cNomImgLue) ;
    sscanf(argv[2],"%s",cNomImgEcrite);
    sscanf(argv[3],"%d",&seuil);


    ImageBase imIn;
    imIn.load(cNomImgLue);

    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

    for(int x=0;x < imIn.getHeight();x++)
        for(int y=0;y < imIn.getWidth();y++)
        {
            if (imIn[x][y] < seuil)
                imOut[x][y] = 0;
            else imOut[x][y] = 255;
        }

    imOut.save(cNomImgEcrite);

    return 0;
}

vector <unsigned> Histogramme (int argc, char **argv, bool Display) {

    vector <unsigned> Histo (256);
    char cNomImgLue[250];

    if (argc != 2)
    {
        cout << "Usage: ImageIn.ppm" << endl;
        return Histo;
    }

    sscanf(argv[1],"%s",cNomImgLue);

    ImageBase Img;
    Img.load(cNomImgLue);

    for (unsigned i (0); i < Img.getHeight(); i++)
        for (unsigned j (0); j < Img.getWidth(); j++)
            for (unsigned k (0); k < 256; ++k) {
                if (Img[i][j] == k) Histo[k]++;
            }


    if (Display)
        for (unsigned i (0); i < Histo.size(); ++i)
            cout << (unsigned) i << " " << (unsigned) Histo[i] << endl;

    cout << "fin histo" << endl;
    return Histo;
}

double getMinbis(vector<unsigned> p){
    for (unsigned i = 1; i < p.size();i++){
        if (p[i - 1]== 0 && p[i] != 0){
            return i;
        }
    }
    return 0;
}

void setMax(vector<unsigned> p,double seuilmax){
    int i = p.size();
    while (p[i] > seuilmax)
    {
        p[i] = seuilmax;
        i--;
    }
}

void setMin(vector<unsigned> p,double seuilmin){
    int i = 0;
    while (p[i] < seuilmin)
    {
        p[i] = seuilmin;
        i++;
    }
}

double getMaxbis(vector<unsigned> p){
    for (unsigned i = p.size()-1; i >= 0;i--){
        if (p[i + 1]== 0 && p[i] != 0){
            return i;
        }
    }
    return 255;
}


vector<vector<unsigned>> HistogrammeCouleur(int argc,char** argv){
    vector <unsigned> Rouge (256);
    vector <unsigned> Vert (256);
    vector <unsigned> Bleu (256);

    vector<vector<unsigned>> resultat;

    char cNomImgLue[250];

    if (argc != 2)
    {
        cout << "Usage: ImageIn.ppm" << endl;
    }

    sscanf(argv[1],"%s",cNomImgLue);

    ImageBase Img;
    Img.load(cNomImgLue);


    for (unsigned i (0); i < Img.getHeight(); i++)
        for (unsigned j (0); j < Img.getWidth(); j++)
            for (unsigned k (0); k < 256; ++k) {
                if (Img[i*3][j*3] == k) Rouge[k]++;
                if (Img[i*3][j*3+1] == k) Vert[k]++;
                if (Img[i*3][j*3+2] == k) Bleu[k]++;
            }

    for (unsigned i (0); i < Rouge.size(); ++i)
        cout << (unsigned) i << " " << (unsigned) Rouge[i] << " " << (unsigned) Vert[i] << " " << (unsigned) Bleu[i] << endl;

    resultat.push_back(Rouge);
    resultat.push_back(Vert);
    resultat.push_back(Bleu);

    return resultat;
}

void ExpansionCouleur(int argc, char** argv){
    char cNomImgLue[250];
    char cNomImgEcrite[250];

    vector<vector<unsigned>> histo;


    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        exit(1);
    }


    sscanf(argv[1],"%s",cNomImgLue);
    sscanf(argv[2],"%s",cNomImgEcrite);


    ImageBase Img;
    Img.load(cNomImgLue);

    ImageBase imOut(Img.getWidth(), Img.getHeight(), Img.getColor());

    histo = HistogrammeCouleur(argc,argv);
    cout << "histo fait" << endl;

    double minrouge = getMinbis(histo[0]);
    double maxrouge = getMaxbis(histo[0]);

    double minvert = getMinbis(histo[1]);
    double maxvert = getMaxbis(histo[1]);

    double minbleu = getMinbis(histo[2]);
    double maxbleu = getMaxbis(histo[2]);

    double alpharouge= - ((255*minrouge)/(maxrouge-minrouge));
    double betarouge = 255 / (maxrouge-minrouge);
    cout << "Alpha rouge:" << alpharouge << endl;
    cout << "Beta rouge: " << betarouge << endl;


    double alphavert= - ((255*minvert)/(maxvert-minvert));
    double betavert = 255 / (maxvert-minvert);
    cout << "Alpha vert:" << alphavert << endl;
    cout << "Beta vert: " << betavert << endl;


    double alphableu= - ((255*minbleu)/(maxbleu-minbleu));
    double betableu = 255 / (maxbleu-minbleu);
    cout << "Alpha bleu:" << alphableu << endl;
    cout << "Beta bleu: " << betableu << endl;


    for(int x = 0; x < Img.getHeight(); ++x)
        for(int y = 0; y < Img.getWidth(); ++y) {
            imOut[x*3][y*3] = alpharouge + betarouge * Img[x*3][y*3];
            imOut[x*3][y*3+1] = alphavert + betavert * Img[x*3][y*3+1];
            imOut[x*3][y*3+2] = alphableu + betableu * Img[x*3][y*3+2];
        }

    imOut.save(cNomImgEcrite);
}

void SeuillageExtrema(int argc,char** argv){
    char cNomImgLue[250];
    char cNomImgEcrite[250];
    double SminR,SmaxR,SminV,SmaxV,SminB,SmaxB;

    vector<vector<unsigned>> histo;


    if (argc != 9)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm SminR SmaxR SminV SmaxV SminB SmaxB" << endl;
        exit(1);
    }


    sscanf(argv[1],"%s",cNomImgLue);
    sscanf(argv[2],"%s",cNomImgEcrite);
    /* sscanf(argv[3],"%d",SminR);
    sscanf(argv[4],"%d",SmaxR);
    sscanf(argv[5],"%d",SminV);
    sscanf(argv[6],"%d",SmaxV);
    sscanf(argv[7],"%d",SminB);
    sscanf(argv[8],"%d",SmaxB);
*/
    SminR = 35;
    SmaxR = 210;
    SminV = 30;
    SmaxV = 225;
    SminB = 40;
    SmaxB = 220;

    ImageBase Img;
    Img.load(cNomImgLue);

    ImageBase imOut(Img.getWidth(), Img.getHeight(), Img.getColor());

    histo = HistogrammeCouleur(argc,argv);
    cout << "histo fait" << endl;

    setMin(histo[0],SminR);
    setMin(histo[0],SmaxR);

    setMin(histo[1],SminV);
    setMin(histo[1],SmaxV);

    setMin(histo[2],SminB);
    setMin(histo[2],SmaxB);

    ImageBase seuil(Img.getWidth(), Img.getHeight(), Img.getColor());

    for(int x = 0; x < Img.getHeight(); ++x)
        for(int y = 0; y < Img.getWidth(); ++y) {
            if (Img[x*3][y*3] < SminR) seuil[x*3][y*3]= SminR;
            else if (Img[x*3][y*3] > SmaxR) seuil[x*3][y*3]= SmaxR;
            else seuil[x*3][y*3]=Img[x*3][y*3];

            if (Img[x*3][y*3+1] < SminV) seuil[x*3][y*3+1]= SminV;
            else if (Img[x*3][y*3+1] > SmaxV) seuil[x*3][y*3+1]= SmaxV;
            else seuil[x*3][y*3+1]=Img[x*3][y*3+1];

            if (Img[x*3][y*3+2] < SminB) seuil[x*3][y*3+2]= SminB;
            else if (Img[x*3][y*3+2] > SmaxB) seuil[x*3][y*3+2]= SmaxB;
            else seuil[x*3][y*3+2]=Img[x*3][y*3+2];;
        }

    seuil.save("Seuil.ppm");

    double alpharouge= - ((255*SminR)/(SmaxR-SminR));
    double betarouge = 255 / (SmaxR-SminR);
    cout << "Alpha rouge:" << alpharouge << endl;
    cout << "Beta rouge: " << betarouge << endl;


    double alphavert= - ((255*SminV)/(SmaxV-SminV));
    double betavert = 255 / (SmaxV-SminV);
    cout << "Alpha vert:" << alphavert << endl;
    cout << "Beta vert: " << betavert << endl;


    double alphableu= - ((255*SminB)/(SmaxB-SminB));
    double betableu = 255 / (SmaxB-SminB);
    cout << "Alpha bleu:" << alphableu << endl;
    cout << "Beta bleu: " << betableu << endl;


    for(int x = 0; x < Img.getHeight(); ++x)
        for(int y = 0; y < Img.getWidth(); ++y) {
            imOut[x*3][y*3] = alpharouge + betarouge * Img[x*3][y*3];
            imOut[x*3][y*3+1] = alphavert + betavert * Img[x*3][y*3+1];
            imOut[x*3][y*3+2] = alphableu + betableu * Img[x*3][y*3+2];
        }

    imOut.save(cNomImgEcrite);
}

void Egalisation(int argc,char** argv){
    char cNomImgLue[250];
    char cNomImgEcrite[250];

    vector<unsigned> histo;
    vector<double> f;
    vector<double> F;


    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        exit(1);
    }


    sscanf(argv[1],"%s",cNomImgLue);
    sscanf(argv[2],"%s",cNomImgEcrite);


    ImageBase Img;
    Img.load(cNomImgLue);

    ImageBase imOut(Img.getWidth(), Img.getHeight(), Img.getColor());

    histo = Histogramme(2,argv,0);
    for (int i =0; i < histo.size();i++){
        f.push_back((double) histo[i] /  Img.getTotalSize());
    }

    for (int i = 0;i<f.size();i++){
        //   cout << i << " " << f[i] << endl;
    }

    F.push_back(f[0]);

    for (int i=1;i< f.size();i++){
        F.push_back(F[i - 1] + f[i]);
    }

    for (int i = 0;i<F.size();i++){
        cout << i << " " << F[i] << endl;
    }

    for (unsigned i (0); i < Img.getHeight(); i++)
        for (unsigned j (0); j < Img.getWidth(); j++)
            imOut[i][j] = 255 * F[Img[i][j]];

    imOut.save(cNomImgEcrite);

}


void Expansion(int argc, char** argv){
    char cNomImgLue[250];
    char cNomImgEcrite[250];


    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        exit(1);
    }


    sscanf(argv[1],"%s",cNomImgLue);
    sscanf(argv[2],"%s",cNomImgEcrite);


    ImageBase Img;
    Img.load(cNomImgLue);

    ImageBase imOut(Img.getWidth(), Img.getHeight(), Img.getColor());

    vector<unsigned> histo = Histogramme(argc,argv,0);


    double min = getMinbis(histo);
    cout << "min fin " << endl;
    double max = getMaxbis(histo);
    cout << "max fin" << endl;

    cout << "minmax fini" << endl;
    double alpha = - ((255*min)/(max-min));
    double beta = 255 / (max-min);

    cout << "Alpha :" << alpha << endl;
    cout << "Beta : " << beta << endl;



    for (unsigned i (0); i < Img.getHeight(); i++)
        for (unsigned j (0); j < Img.getWidth(); j++)
            imOut[i][j] = alpha + beta * Img[i][j];

    cout << "boucle finie" << endl;
    imOut.save(cNomImgEcrite);
}





unsigned getMax(vector<unsigned> p){
    unsigned max = p[0];
    for (unsigned i = 0; i < p.size(); i++){
        if (p[i] > max)
            max = p[i];
    }
    return max;
}

unsigned getMin(vector<unsigned> p){
    unsigned min = p[0];
    for (unsigned i = 0; i < p.size(); i++){
        if (p[i] < min)
            min = p[i];
    }
    return min;
}



void Dilatation (int argc, char **argv) {
    char cNomImgLue[250], cNomImgEcrite[250];

    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return;
    }
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    ImageBase imIn;
    imIn.load(cNomImgLue);

    //ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

    for(int x = 0; x < imIn.getHeight(); ++x)
        for(int y = 0; y < imIn.getWidth(); ++y)
        {
            vector <unsigned> pixValue;
            if (x != 0) {
                pixValue.push_back(imIn[x-1][y]);
            }
            if (y != 0) {
                pixValue.push_back(imIn[x][y-1]);
            }
            if (x != imIn.getHeight() - 1) {
                pixValue.push_back(imIn[x+1][y]);
            }
            if (y != imIn.getWidth() - 1) {
                pixValue.push_back(imIn[x][y+1]);
            }

            pixValue.push_back(imIn[x][y]);

            imOut[x][y] = getMin(pixValue);

        }
    imOut.save(cNomImgEcrite);
}

void Erosion (int argc, char **argv) {
    char cNomImgLue[250], cNomImgEcrite[250];

    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return;
    }
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    ImageBase imIn;
    imIn.load(cNomImgLue);

    //ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

    for(int x = 0; x < imIn.getHeight(); ++x)
        for(int y = 0; y < imIn.getWidth(); ++y)
        {
            vector <unsigned> pixValue;
            if (x != 0) {
                pixValue.push_back(imIn[x-1][y]);
            }
            if (y != 0) {
                pixValue.push_back(imIn[x][y-1]);
            }
            if (x != imIn.getHeight() - 1) {
                pixValue.push_back(imIn[x+1][y]);
            }
            if (y != imIn.getWidth() - 1) {
                pixValue.push_back(imIn[x][y+1]);
            }

            pixValue.push_back(imIn[x][y]);

            imOut[x][y] = getMax(pixValue);

        }
    imOut.save(cNomImgEcrite);
}

void fermeture(int argc, char **argv){
    Dilatation(argc,argv);
    argv[1]=argv[2];
    argv[2]="fermeture.pgm";
    Erosion(argc,argv);
}

void ouverture(int argc,char **argv){
    Erosion(argc,argv);
    argv[1]=argv[2];
    argv[2]="ouverture.pgm";
    Dilatation(argc,argv);
}

void difference(int argc,char **argv){

    char cNomImgLue[250], cNomImgEcrite[250], cNomImgDilat[250];

    if (argc != 4)
    {
        cout << "Usage: ImageIn.pgm ImageDilate.pgm ImageOut.pgm" << endl;
        return;
    }
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgDilat);
    sscanf (argv[3],"%s",cNomImgEcrite);

    ImageBase imIn;
    imIn.load(cNomImgLue);
    ImageBase imDil;
    imDil.load(cNomImgDilat);

    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

    for(int x = 0; x < imIn.getHeight(); ++x)
        for(int y = 0; y < imIn.getWidth(); ++y)
            if (imIn[x][y] == imDil[x][y])
                imOut[x][y] = 255;
            else imOut[x][y] = 0;

    imOut.save(cNomImgEcrite);

}

void TP4Q1(int argc,char** argv){
    char cNomImgLue[250], cNomImgEcrite[250];

    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return;
    }
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    ImageBase Img;
    Img.load(cNomImgLue);

    //ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    ImageBase imOut(Img.getWidth(), Img.getHeight(), !Img.getColor());

    for(int x = 0; x < Img.getHeight(); ++x)
        for(int y = 0; y < Img.getWidth(); ++y) {
            imOut[x][y] = (0.299 * Img[x*3][y*3]) + (0.587 * Img[x*3][y*3+1]) + (0.114 * Img[x*3][y*3+2]);
        }

    imOut.save(cNomImgEcrite);
}

void flouPixel(ImageBase &Img, ImageBase &imOut, int x, int y){
    imOut[x*3][y*3] = (float) 1/9 * (Img[x-1*3][y-1*3] + Img[x-1*3][y*3] + Img[x-1*3][y+1*3] + Img[x*3][y-1*3] + Img[x*3][y*3] + Img[x*3][y+1*3] + Img[x+1*3][y-1*3] + Img[x+1*3][y*3] + Img[x+1*3][y+1*3]);
    imOut[x*3][y*3+1] = (float) 1/9 *  (Img[x-1*3][y-1*3+1] + Img[x-1*3][y*3+1] + Img[x-1*3][y+1*3+1] + Img[x*3][y-1*3+1] + Img[x*3][y*3+1] + Img[x*3][y+1*3+1] + Img[x+1*3][y-1*3+1] + Img[x+1*3][y*3+1] + Img[x+1*3][y+1*3+1]);
    imOut[x*3][y*3+2] = (float) 1/9 *  (Img[x-1*3][y-1*3+2] + Img[x-1*3][y*3+2] + Img[x-1*3][y+1*3+2] + Img[x*3][y-1*3+2] + Img[x*3][y*3+2] + Img[x*3][y+1*3+2] + Img[x+1*3][y-1*3+2] + Img[x+1*3][y*3+2] + Img[x+1*3][y+1*3+2]);

}

void flouCouleur(int argc, char **argv){
    char cNomImgLue[250], cNomImgEcrite[250];

    if (argc != 3)
    {
        cout << "Usage: ImageIn.pgm ImageOut.pgm" << endl;
        return;
    }
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    ImageBase Img;
    Img.load(cNomImgLue);

    //ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    ImageBase imOut(Img.getWidth(), Img.getHeight(), Img.getColor());

    for(int x=1;x < Img.getHeight()-1;x++)
        for(int y=1;y < Img.getWidth()-1;y++) {
            flouPixel(Img,imOut,x,y);
        }

    imOut.save(cNomImgEcrite);

}

void flouPixelRelatif(ImageBase &Img, ImageBase &ImgBin, ImageBase &imOut, int x, int y){
    if (ImgBin[x][y] == 255){
        imOut[x*3][y*3] = (float) 1/9 * (Img[x-1*3][y-1*3] + Img[x-1*3][y*3] + Img[x-1*3][y+1*3] + Img[x*3][y-1*3] + Img[x*3][y*3] + Img[x*3][y+1*3] + Img[x+1*3][y-1*3] + Img[x+1*3][y*3] + Img[x+1*3][y+1*3]);
        imOut[x*3][y*3+1] = (float) 1/9 *  (Img[x-1*3][y-1*3+1] + Img[x-1*3][y*3+1] + Img[x-1*3][y+1*3+1] + Img[x*3][y-1*3+1] + Img[x*3][y*3+1] + Img[x*3][y+1*3+1] + Img[x+1*3][y-1*3+1] + Img[x+1*3][y*3+1] + Img[x+1*3][y+1*3+1]);
        imOut[x*3][y*3+2] = (float) 1/9 *  (Img[x-1*3][y-1*3+2] + Img[x-1*3][y*3+2] + Img[x-1*3][y+1*3+2] + Img[x*3][y-1*3+2] + Img[x*3][y*3+2] + Img[x*3][y+1*3+2] + Img[x+1*3][y-1*3+2] + Img[x+1*3][y*3+2] + Img[x+1*3][y+1*3+2]);
    }
}

void flouRelatif(int argc, char **argv){
    char cNomImgLue[250], cNomImgLue2[250], cNomImgEcrite[250];

    if (argc != 4)
    {
        cout << "Usage: ImageIn.pgm ImageIn2.pgm ImageOut.pgm" << endl;
        return;
    }
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgLue2) ;
    sscanf (argv[3],"%s",cNomImgEcrite);

    ImageBase Img;
    Img.load(cNomImgLue);

    ImageBase Binaire;
    Binaire.load(cNomImgLue2);

    //ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    ImageBase imOut(Img.getWidth(), Img.getHeight(), Img.getColor());

    for(int x=1;x < Img.getHeight()-1;x++)
        for(int y=1;y < Img.getWidth()-1;y++) {
            flouPixelRelatif(Img,Binaire,imOut,x,y);
        }

    imOut.save(cNomImgEcrite);

}



int main(int argc, char **argv)
{

    ouverture(argc,argv);
    return 0;



}
