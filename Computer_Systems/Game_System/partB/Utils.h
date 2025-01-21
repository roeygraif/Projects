#ifndef UTILS_H
#define UTILS_H


#include <set>
#include "Auxiliaries.h"


namespace mtm{
    struct GridPointsCompare {
        bool operator()(const GridPoint& gp1, const GridPoint& gp2) const {
            if(gp1.row != gp2.row){
                return gp1.row < gp2.row;
            }

            return gp1.col < gp2.col;
        }
    };

    class Utils{
        private:
            static void getGridPointsInRadius_aux(const GridPoint& center,
                                                   int radius,
                                                   std::set<GridPoint, mtm::GridPointsCompare> &in_radius);

        public:
            static int roundUpDivision(int x, int y);

            static char to_upper(char c);

            static char to_lower(char c);

            static std::set<GridPoint, mtm::GridPointsCompare> getGridPointsInRadius(const GridPoint& center,
                                                                                      int radius);

    };
}

#endif