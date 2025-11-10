#include "gcd.h"

namespace gcd {
int64_t gcd(int64_t first, int64_t second) {
    if (first < 0) {
        first *= -1;
    }
    if(second < 0) {
        second *= -1;
    }
    int64_t temp = second;
    if(first < second) {
        second = first;
        first = temp;
    }
    if((first == 0) && (second == 0)) {
        return 0;
    } else if(first == 0) {
        return second;
    } else if(second == 0) {
        return first;
    }

    while(first >= second) {
        if(first % second == 0) {
            return second;
        } else {
            temp = first % second;
            first = second;
            second = temp;
        }
    
    }
    return 1;
}
} // namespace gcd