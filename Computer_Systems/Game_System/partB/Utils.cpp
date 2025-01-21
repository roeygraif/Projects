
#include <set>
#include "Utils.h"


namespace mtm {

    int Utils::roundUpDivision(int x, int y){
        return (x + y - 1) / y;
    }

    char Utils::to_upper(char c){
        if(c >= 'a' && c <= 'z'){
            return c - 'a' + 'A';
        }
        return c;
    }

    char Utils::to_lower(char c){
        if(c >= 'A' && c <= 'Z'){
            return c - 'A' + 'a';
        }
        return c;
    }

    std::set<GridPoint, mtm::GridPointsCompare> Utils::getGridPointsInRadius(const GridPoint& center, int radius){
        std::set<GridPoint, mtm::GridPointsCompare> in_radius;
        Utils::getGridPointsInRadius_aux(center, radius, in_radius);

        return in_radius;
    }

    void mtm::Utils::getGridPointsInRadius_aux(const GridPoint &center, int radius,
                                               std::set <GridPoint, GridPointsCompare> &in_radius)
    {
        if(radius < 0 ){
            return;
        }

        in_radius.insert(GridPoint(center));

        if(radius == 0){
            return;
        }

        GridPoint up    (center.row + 1, center.col);
        GridPoint down  (center.row - 1, center.col);
        GridPoint left  (center.row, center.col - 1);
        GridPoint right (center.row, center.col + 1);

        getGridPointsInRadius_aux(up,      radius - 1, in_radius);
        getGridPointsInRadius_aux(down,    radius - 1, in_radius);
        getGridPointsInRadius_aux(left,    radius - 1, in_radius);
        getGridPointsInRadius_aux(right,   radius - 1, in_radius);
    }
}


