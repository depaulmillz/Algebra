//
// Created by dePaul Miller on 3/22/21.
//

#ifndef ALGEBRA_GF2EXTENSION_HH
#define ALGEBRA_GF2EXTENSION_HH

#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <unordered_set>

class GF2To8Element;

class GF2To8Metadata {
public:
    GF2To8Metadata() : log_(new unsigned[256]),
                       ilog_(new uint8_t[256]) {
        initLogTable();
    }

    ~GF2To8Metadata() {

    }

    void initLogTable() {
        const uint16_t PrimitivePolynomial = 0b100011101; // x^8 + x^4 + x^3 + x^2 + 1

        std::unordered_set<uint8_t> s;

        // x^0 = 1
        // log (x^0) = 0
        // ilog(0) = 1;
        log_[0] = ~0; // To cause segfault ideally

        log_[1] = 0;
        ilog_[0] = 1;

        uint8_t val = 0b10; // start with x
        for (int i = 1; i < 255; i++) {
            assert(val != 0);
            if (s.find(val) != s.end()) {
                std::cerr << "Repeat at x^" << i << std::endl;
                exit(2);
            }

            s.insert(val);

            log_[val] = i;
            ilog_[i] = val;

            if (val & 128) {
                // this will occur if value will need to be modded
                uint16_t widenedVal = val;
                widenedVal = (widenedVal << 1);
                widenedVal = widenedVal ^ PrimitivePolynomial; // elm * x  - prim to get modulus
                val = (uint8_t) widenedVal;
            } else {
                // this occurs if we can simply bitshift
                val = (val << 1);
            }
        }
        assert(val == 1);
        //log_[val] = 255;
        //ilog_[255] = val;
    }

    friend class GF2To8Element;

private:
    unsigned *log_;
    uint8_t *ilog_;
};

class GF2To8Element {
public:
    explicit GF2To8Element(GF2To8Metadata m) : polynomial(0), metadata(m) {

    }

    GF2To8Element(uint8_t p, GF2To8Metadata m) : polynomial(p), metadata(m) {

    }

    ~GF2To8Element() {

    }

    inline explicit operator uint8_t() const {
        return polynomial;
    }

    inline GF2To8Element operator+(const GF2To8Element &rhs) {
        return GF2To8Element(polynomial ^ rhs.polynomial, metadata);
    }

    inline GF2To8Element operator-(const GF2To8Element &rhs) {
        return GF2To8Element(polynomial ^ rhs.polynomial, metadata);
    }

    inline GF2To8Element operator*(const GF2To8Element &rhs) {

        if (this->polynomial == 0 || rhs.polynomial == 0) {
            return GF2To8Element(0, metadata);
        }

        // polynomial = x ^ log(polynomial)
        // rhs.polynomial = x ^ log(rhs.polynomial)
        // polynomial * rhs.polynomial = (x ^ log(polynomial)) * (x ^ log(rhs.polynomial))
        // polynomial * rhs.polynomial = x ^ (log(polynomial) + log(rhs.polynomial))

        uint8_t res = metadata.ilog_[(metadata.log_[polynomial] + metadata.log_[rhs.polynomial]) % 255];
        return GF2To8Element(res, metadata);
    }

    inline GF2To8Element operator/(const GF2To8Element &rhs) {

        if (rhs.polynomial == 0) {
            throw std::runtime_error("Divide by 0");
        }

        // polynomial = x ^ log(polynomial)
        // rhs.polynomial = x ^ log(rhs.polynomial)
        // polynomial / rhs.polynomial = (x ^ log(polynomial)) * inverse(x ^ log(rhs.polynomial))
        // inverse(x ^ log(rhs.polynomial))) * (x ^ log(rhs.polynomial)) = 1
        // x ^ (255 - log(rhs.polynomial)) is the inverse

        unsigned logOfInv = 255 - metadata.log_[rhs.polynomial];
        uint8_t res = metadata.ilog_[(metadata.log_[polynomial] + logOfInv) % 255];
        return GF2To8Element(res, metadata);
    }

    inline bool operator==(GF2To8Element &rhs) const {
        return polynomial == rhs.polynomial;
    }

private:
    uint8_t polynomial;
    GF2To8Metadata metadata;
};


#endif //ALGEBRA_GF2EXTENSION_HH
