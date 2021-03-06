#ifndef CBLOCK_H
#define CBLOCK_H

#include "ImageBase.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>

class CBlock
{
    private:
        int m_xMin;
        int m_xMax;
        int m_yMin;
        int m_yMax;
        double m_Critere = 0.0 ;
        int m_moyenneImageUtile = 0;
        std::string m_ImageUtileName;

    public:
        CBlock();
        CBlock(int xMin = 0, int xMax = 0, int yMin = 0, int yMax = 0);

        int getxMin() const;
        int getxMax() const;
        int getyMin() const;
        int getyMax() const;
        double getCritere() const;
        int getMoyenneImageUtile() const;
        std::string getImageUtileName() const;

        void setXMin(int xMin);
        void setXMax(int xMax);
        void setYMin(int yMin);
        void setYMax(int yMax);
        void setCritere(double Critere);
        void setMoyenneImageUtile(int moyenneImageUtile);
        void setImageUtileName(const std::string &ImageUtileName);

        void CritereWithMoyenne (ImageBase & Img);
        void CritereWithHistogramme (ImageBase & Img);

        void DistanceWithMoyenne (const std::vector<std::pair<std::string, double>> & ImgList);
};

#endif // CBLOCK_H
