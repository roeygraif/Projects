#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <string>
#include <cstring>
#include <exception>


namespace mtm {
    class Exception: public std::exception{
        protected:
            std::string _msg;

        public:
            explicit Exception(std::string name):
                _msg("A game related error has occurred: " + name){}

            virtual ~Exception() noexcept = default;

            const char* what() const noexcept{
                return this->_msg.c_str();
            }
    };
}

#endif