#include "CBlock.h"

CBlock::CBlock() {}

CBlock::CBlock(int xMin, int xMax, int yMin, int yMax)
    : m_xMin (xMin), m_xMax (xMax), m_yMin (yMin), m_yMax (yMax) {}

void CBlock::CritereWithMoyenne (ImageBase & Img) {

    double Total = 0.0;

    for (unsigned i (m_xMin); i < m_xMax; ++i)
        for (unsigned j (m_yMin); j < m_yMax; ++j)
            Total += Img[i][j];

    //std::cout << "total : " << Total << std::endl;
    m_Critere = Total / 256.0;
}

void CBlock::DistanceWithHistogramme (ImageBase & Img) {
    //ToDO
}


void CBlock::setCritere(double Critere)
{
    m_Critere = Critere;
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
