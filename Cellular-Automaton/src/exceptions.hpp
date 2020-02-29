#include <stdexcept>


class OpenGlError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};