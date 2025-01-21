#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include "Exception.h"


namespace mtm {
    class IllegalArgument: virtual public Exception{
        public:
            IllegalArgument(): Exception("IllegalArgument") {}
    };

    class IllegalCell: virtual public Exception{
        public:
            IllegalCell(): Exception("IllegalCell") {}
    };

    class CellEmpty: virtual public Exception{
        public:
            CellEmpty(): Exception("CellEmpty") {}
    };

    class MoveTooFar: virtual public Exception{
        public:
            MoveTooFar(): Exception("MoveTooFar") {}
    };

    class CellOccupied: virtual public Exception{
        public:
            CellOccupied(): Exception("CellOccupied") {}
    };

    class OutOfRange: virtual public Exception{
        public:
            OutOfRange(): Exception("OutOfRange") {}
    };
    
    class OutOfAmmo: virtual public Exception{
        public:
            OutOfAmmo(): Exception("OutOfAmmo") {}
    };
    
    class IllegalTarget: virtual public Exception{
        public:
            IllegalTarget(): Exception("IllegalTarget") {}
    };
}

#endif