#ifndef CBLOCK_H
#define CBLOCK_H

#include "ImageBase.h"
#include <iostream>


class CBlock
{
    private:
        int m_xMin;
        int m_xMax;
        int m_yMin;
        int m_yMax;
        double m_Critere = 0.0 ;

    public:
        CBlock();
        CBlock(int xMin = 0, int xMax = 0, int yMin = 0, int yMax = 0);

        int getxMin() const;
        int getxMax() const;
        int getyMin() const;
        int getyMax() const;
        double getCritere() const;

        void setXMin(int xMin);
        void setXMax(int xMax);
        void setYMin(int yMin);
        void setYMax(int yMax);
        void setCritere(double Critere);

        void CritereWithMoyenne (ImageBase & Img);
        void DistanceWithHistogramme (ImageBase & Img);
};

#endif // CBLOCK_H
