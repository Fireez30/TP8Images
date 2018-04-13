#include "CBlock.h"


std::string CBlock::getImageUtileName() const
{
    return m_ImageUtileName;
}

void CBlock::setImageUtileName(const std::string &ImageUtileName)
{
    m_ImageUtileName = ImageUtileName;
}

CBlock::CBlock() {}

CBlock::CBlock(int xMin, int xMax, int yMin, int yMax)
    : m_xMin (xMin), m_xMax (xMax), m_yMin (yMin), m_yMax (yMax) {
    std::vector <unsigned> Histo (256);
    m_Histogramme = Histo;
}

void CBlock::CritereWithMoyenne (ImageBase & Img) {

    double Total = 0.0;

    for (unsigned i (m_xMin); i < m_xMax; ++i)
        for (unsigned j (m_yMin); j < m_yMax; ++j)
            Total += Img[i][j];

    m_Critere = Total / (double) ((m_xMax - m_xMin) * (m_yMax - m_yMin));
}


void CBlock::DistanceWithMoyenne(const std::vector<std::pair<std::__cxx11::string, double> > &ImgList)
{
    double ClosestAverage = ImgList[0].second;
    std::string ImgUtile = ImgList[0].first;

    for (unsigned i (1); i < ImgList.size(); i++)
        if (abs(ClosestAverage - m_Critere) > abs(ImgList[i].second - m_Critere)) {
            ClosestAverage = ImgList[i].second;
            ImgUtile = ImgList[i].first;
        }

    char *cstr = new char[ImgUtile.length()];
    strcpy(cstr, ImgUtile.c_str());

    m_ImageUtileName = ImgUtile;
    m_moyenneImageUtile = ClosestAverage;
}

std::vector<unsigned> CBlock::getHistogramme() const
{
    return m_Histogramme;
}

void CBlock::setHistogramme(const std::vector<unsigned> &Histogramme)
{
    m_Histogramme = Histogramme;
}

std::vector<unsigned> CBlock::HistogrammeFromImage(std::string & FileName){
    /*ImageBase Img;
    char *cstr = new char[FileName.length()];
    strcpy(cstr, FileName.c_str());
    Img.load(cstr);


    std::vector <unsigned> Histotmp (256);

    for (unsigned i (0); i < Img.getHeight(); i++)
        for (unsigned j (0); j < Img.getWidth(); j++)
            for (unsigned k (0); k < 256; ++k)
                if (Img[i][j] == k) {
                    Histotmp[k]++;
                    break;
                }*/

    std::string tmp = FileName.substr(0, FileName.size() - 4);
    std::ifstream File (tmp + ".dat");
    std::vector <unsigned> Histotmp (256);

    for (unsigned k (0); k < 256; ++k)
        File >> Histotmp[k];

    //std::cout << Histotmp[0] << std::endl;

    return Histotmp;
}

void CBlock::CritereWithHistogramme (ImageBase & Img) {
    for (unsigned i (m_xMin); i < m_xMax; i++)
        for (unsigned j (m_yMin); j < m_yMax; j++)
            for (unsigned k (0); k < 256; ++k)
                if (Img[i][j] == k) {
                    m_Histogramme[k]++;
                    break;
                }
}

double CBlock::ChiSquared(const std::vector<unsigned> histoBDD){
    double dist = 0.0;

    for (int i = 0; i < histoBDD.size(); i++){
        if (m_Histogramme[i] + histoBDD[i] == 0)
            dist += (double) pow(histoBDD[i] - m_Histogramme[i], 2);
        else
            dist += (double) pow(histoBDD[i] - m_Histogramme[i], 2) / (double) (m_Histogramme[i] + histoBDD[i]);
    }

    return dist;
}

void CBlock::DistanceWithHistogramme(std::vector<std::string> & FileNames)
{
    for (int i = 0; i < FileNames.size(); i++){
        //std::cout << "DistanceWithHistogramme avec image bdd numero : " << i << std::endl;


        std::vector<unsigned> histoActuel = HistogrammeFromImage(FileNames[i]);
        double dist = ChiSquared(histoActuel);
        if (m_distanceImageUtile > dist) {
            m_distanceImageUtile = dist;
            m_ImageUtileName = FileNames[i];
            if (dist == 0.0) break;
        }
    }
}

int CBlock::getxMin() const
{
    return m_xMin;
}

int CBlock::getxMax() const
{
    return m_xMax;
}

int CBlock::getyMin() const
{
    return m_yMin;
}

int CBlock::getyMax() const
{
    return m_yMax;
}

double CBlock::getCritere() const
{
    return m_Critere;
}

int CBlock::getMoyenneImageUtile() const
{
    return m_moyenneImageUtile;
}


void CBlock::setXMin(int xMin)
{
    m_xMin = xMin;
}

void CBlock::setXMax(int xMax)
{
    m_xMax = xMax;
}

void CBlock::setYMin(int yMin)
{
    m_yMin = yMin;
}

void CBlock::setYMax(int yMax)
{
    m_yMax = yMax;
}

void CBlock::setCritere(double Critere)
{
    m_Critere = Critere;
}

void CBlock::setMoyenneImageUtile(int moyenneImageUtile)
{
    m_moyenneImageUtile = moyenneImageUtile;
}
