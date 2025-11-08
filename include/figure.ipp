#include "figure.hpp"

namespace figure {

template<Scalar T>
Figure<T>::operator double() const { 
    return Area(); 
}

template<Scalar T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& figure) {
    figure.Print(os);
    return os;
}

template<Scalar T>
std::istream& operator>>(std::istream& is, Figure<T>& figure) {
    figure.Read(is);
    return is;
}

}
