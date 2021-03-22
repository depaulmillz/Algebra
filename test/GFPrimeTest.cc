//
// Created by dePaul Miller on 3/22/21.
//

#include <GFPrime.hh>
#include <cassert>

int main(){

    GFPrimeMetadata<2> metadata;
    metadata.initLogTable(1);

    GFPrimeElement<2> x(0, metadata);
    GFPrimeElement<2> y(1, metadata);

    auto z = x + y;

    assert(z == y);

    z = z + z;
    assert(z == x);

    z = y * y;
    assert(z == y);

    z = z * x;
    assert(z == x);

    z = y.log(); // log (1) = 0;

    assert(z == x);

    return 0;
}