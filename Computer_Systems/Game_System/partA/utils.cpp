#include <math.h>


namespace mtm {

    bool isValidMonth(int month) {
        return !(month < 1 || month > 12);
    }

    bool isValidDay(int day) {
        return !(day < 1 || day > 30);
    }

    bool isValidHour(double hour) {
        if (hour < 0 || hour > 23.5)
            return false;
        int minutes = hour - floor(hour);
        return (minutes == 0 || minutes == 0.5);
    }


    bool isValidLength(int length) {
        return length >= 0;
    }

    bool isRoundNumber(double num){
        return ((num - floor(num)) == 0);
    }

}