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
    : m_xMin (xMin), m_xMax (xMax), m_yMin (yMin), m_yMax (yMax) {}

void CBlock::CritereWithMoyenne (ImageBase & Img) {

    double Total = 0.0;

    for (unsigned i (m_xMin); i < m_xMax; ++i)
        for (unsigned j (m_yMin); j < m_yMax; ++j)
            Total += Img[i][j];

    m_Critere = Total / (double) ((m_xMax - m_xMin) * (m_yMax - m_yMin));
}

void CBlock::CritereWithHistogramme (ImageBase & Img) {
    //ToDO
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
