#ifndef ERRORS_H
#define ERRORS_H
#include <iostream>
#include <string>

class PlayAudioErr: public std::exception {
    private:
        std::string _message;
    public:
        explicit PlayAudioErr(const std::string& mes) : _message{mes} {}
        const char* what() const noexcept {
            return _message.c_str();
        }
};
#endif //ERRORS_H
