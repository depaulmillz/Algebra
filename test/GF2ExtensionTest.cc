//
// Created by dePaul Miller on 3/22/21.
//

#include <GF2Extension.hh>
#include <unordered_set>
#include <iostream>

int main(){
    GF2To8Metadata metadata;

    GF2To8Element one(1, metadata);
    GF2To8Element zero(0, metadata);
    GF2To8Element x(0b10, metadata);

    GF2To8Element shouldBe0 = one + one;

    assert(shouldBe0 == zero);

    GF2To8Element val = x;

    std::unordered_set<uint8_t> s;

    for(int i = 1; i < 255; i++){

        if(s.find((uint8_t) val) != s.end()) {
            return 1;
        }

        s.insert((uint8_t)val);
        val = val * x;
    }

    assert(one == val);

    GF2To8Element elm(10, metadata);

    auto elmDivOne = elm / one;

    assert(elmDivOne == elm);

    auto Inv = one / elm;

    auto Ident = elm * Inv;

    assert(Ident == one);
    Ident = Inv * elm;

    assert(Ident == one);

    return 0;
}