//
// Created by dePaul Miller on 3/22/21.
//

#ifndef ALGEBRA_GFPRIME_HH
#define ALGEBRA_GFPRIME_HH

#include <stdexcept>

template<unsigned Prime>
class GFPrimeElement;

template<unsigned Prime>
class GFPrimeMetadata {
public:
    GFPrimeMetadata() : log_(new unsigned[Prime]),
                        ilog_(new unsigned[Prime]) {

    }

    ~GFPrimeMetadata() {

    }

    inline void initLogTable(unsigned generator) {
        // log of 1 is 0
        log_[1] = 0;
        ilog_[0] = 1;
        unsigned val = generator;
        for (int i = 1; i < Prime - 1; i++) {
            log_[val] = i;
            ilog_[i] = val;
            val = val * val;
        }
    }

    friend class GFPrimeElement<Prime>;

private:
    unsigned *log_;
    unsigned *ilog_;
};

template<unsigned Prime>
class GFPrimeElement {
public:

    GFPrimeElement() {

    }

    explicit GFPrimeElement(GFPrimeMetadata<Prime> m) : elm(0), metadata(m) {

    }

    GFPrimeElement(unsigned x, GFPrimeMetadata<Prime> m) : elm(x % Prime), metadata(m) {

    }

    ~GFPrimeElement() {

    }

    inline explicit operator unsigned() const {
        return elm;
    }

    inline GFPrimeElement<Prime> operator+(const GFPrimeElement<Prime> &rhs) const {
        return GFPrimeElement<Prime>((elm + rhs.elm) % Prime, metadata);
    }

    inline GFPrimeElement<Prime> &operator+=(const GFPrimeElement<Prime> &rhs) {
        elm = (elm + rhs.elm) % Prime;
        return *this;
    }


    inline GFPrimeElement<Prime> operator-(const GFPrimeElement<Prime> &rhs) const {
        return GFPrimeElement<Prime>((elm - rhs.elm) % Prime, metadata);
    }

    inline GFPrimeElement<Prime> &operator-=(const GFPrimeElement<Prime> &rhs) {
        elm = (elm - rhs.elm) % Prime;
        return *this;
    }


    inline GFPrimeElement<Prime> operator*(const GFPrimeElement<Prime> &rhs) const {
        unsigned m = elm * rhs.elm;
        if (m < Prime) {
            return GFPrimeElement<Prime>(m, metadata);
        }
        return GFPrimeElement<Prime>(m % Prime, metadata);
    }

    inline GFPrimeElement<Prime> &operator*=(const GFPrimeElement<Prime> &rhs) {
        unsigned m = elm * rhs.elm;
        elm = m;
        if (m >= Prime) {
            elm = elm % Prime;
        }
        return *this;
    }

    inline GFPrimeElement<Prime> multWithLogTable(const GFPrimeElement<Prime> &rhs) const {
        // generator ^ logrhs * generator ^ thislog = generator ^ (logrhs + thislog)
        unsigned logrhs = metadata.log_[rhs.elm];
        unsigned thislog = metadata.log_[elm];
        return GFPrimeElement<Prime>(metadata.ilog_[logrhs + thislog], metadata);
    }

    inline GFPrimeElement<Prime> operator/(const GFPrimeElement<Prime> &rhs) const {
        // generator ^ logrhs = rhs.elm;
        // generator ^ (Prime - 1) = 1;
        // generator ^ (Prime - 1 - logrhs) * generator ^ logrhs = generator ^ (Prime - 1);
        // ===> generator ^ (Prime - 1 - logrhs) is rhs.elm inverse

        if (rhs.elm == 0) {
            throw std::runtime_error("Divide by 0");
        }

        unsigned logrhs = metadata.log_[rhs.elm];
        unsigned inverse = metadata.ilog_[Prime - 1 - logrhs];
        return (*this * inverse); // this can be done with lookup as well
    }

    inline GFPrimeElement<Prime> log() const {
        return GFPrimeElement<Prime>(metadata.log_[elm], metadata);
    }

    inline bool operator==(GFPrimeElement<Prime> &rhs) const {
        return elm == rhs.elm;
    }

private:
    unsigned elm;
    GFPrimeMetadata<Prime> metadata;
};

#endif //ALGEBRA_GFPRIME_HH
