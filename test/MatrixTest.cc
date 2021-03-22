//
// Created by dePaul Miller on 3/22/21.
//

#include <Matrix.hh>
#include <GFPrime.hh>
#include <cassert>
#include <GF2Extension.hh>

int main() {

    GFPrimeMetadata<2> m;
    m.initLogTable(1);
    const GFPrimeElement<2> zero(0, m);
    const GFPrimeElement<2> one(1, m);

    Matrix<GFPrimeElement<2>> a(2, 2, zero);
    Matrix<GFPrimeElement<2>> b(2, 2, zero);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (i == j)
                a[i][j] = one;
            else
                a[i][j] = zero;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            b[i][j] = one;

    auto c = a.multiply(b);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            assert(b[i][j] == c[i][j]);


    GF2To8Metadata metadata;
    const GF2To8Element Zero(0, metadata);
    const GF2To8Element One(1, metadata);

    Matrix<GF2To8Element> d(2, 2, Zero);
    Matrix<GF2To8Element> e(2, 2, Zero);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (i == j)
                d[i][j] = One;
            else
                d[i][j] = Zero;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            e[i][j] = GF2To8Element((i * j) % 255, metadata);

    auto f = d.multiply(e);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            assert(e[i][j] == f[i][j]);

    return 0;
}