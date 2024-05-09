#pragma once
#include <system_error>
#include <exception>

namespace Networking {
    class TESTEXCEPTION : public std::exception {
    private:
        char* message;

    public:
        TESTEXCEPTION(char* msg) : message(msg) {}
        char* what() {
            return message;
        }
    };
}