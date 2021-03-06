/*
Ao: a CAD kernel for modeling with implicit functions
Copyright (C) 2017  Matt Keeter

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "catch.hpp"

#include "ao/tree/cache.hpp"

using namespace Kernel;

TEST_CASE("Deduplication of variables")
{
    auto t = Cache::instance();

    auto xa = t->X();
    auto xb = t->X();
    REQUIRE(xa == xb);

    auto ya = t->Y();
    REQUIRE(xa != ya);
}

TEST_CASE("Deduplication of constants")
{
    auto t = Cache::instance();

    auto ca = t->constant(3.14);
    auto cb = t->constant(3.14);
    REQUIRE(ca == cb);

    auto cc = t->constant(4);
    REQUIRE(ca != cc);
}

TEST_CASE("Deduplication of operations")
{
    auto t = Cache::instance();

    auto oa = t->operation(Opcode::ADD, t->X(), t->constant(1));
    auto ob = t->operation(Opcode::ADD, t->X(), t->constant(1));
    REQUIRE(oa == ob);

    auto oc = t->operation(Opcode::ADD, t->X(), t->constant(2));
    REQUIRE(oa != oc);
}

TEST_CASE("Collapsing of identites")
{
    auto t = Cache::instance();

    SECTION("Addition")
    {
        auto oa = t->operation(Opcode::ADD, t->X(), t->constant(0));
        REQUIRE(oa == t->X());

        auto ob = t->operation(Opcode::ADD, t->constant(0), t->X());
        REQUIRE(ob == t->X());
    }

    SECTION("Subtraction")
    {
        auto oa = t->operation(Opcode::SUB, t->X(), t->constant(0));
        REQUIRE(oa == t->X());

        auto ob = t->operation(Opcode::SUB, t->constant(0), t->X());
        REQUIRE(ob->op == Opcode::NEG);
        REQUIRE(ob->lhs == t->X());
    }

    SECTION("Multiplication")
    {
        auto oa = t->operation(Opcode::MUL, t->X(), t->constant(1));
        REQUIRE(oa == t->X());

        auto ob = t->operation(Opcode::MUL, t->constant(1), t->X());
        REQUIRE(ob == t->X());

        auto oc = t->operation(Opcode::MUL, t->X(), t->constant(0));
        REQUIRE(oc->op == Opcode::CONST);
        REQUIRE(oc->value == 0);

        auto od = t->operation(Opcode::MUL, t->constant(0), t->X());
        REQUIRE(od->op == Opcode::CONST);
        REQUIRE(od->value == 0);
    }

    SECTION("Exponentiation")
    {
        auto oa = t->operation(Opcode::POW, t->X(), t->constant(1));
        REQUIRE(oa == t->X());

        auto ob = t->operation(Opcode::NTH_ROOT, t->X(), t->constant(1));
        REQUIRE(ob == t->X());
    }
}

TEST_CASE("Collapsing constants")
{
    auto t = Cache::instance();

    auto a = t->operation(Opcode::ADD, t->constant(4), t->constant(3));
    REQUIRE(a->op == Opcode::CONST);
    REQUIRE(a->value == 7);

    auto b = t->operation(Opcode::NEG, t->constant(4));
    REQUIRE(b->op == Opcode::CONST);
    REQUIRE(b->value == -4);
}

TEST_CASE("Cache::var")
{
    auto t = Cache::instance();
    auto a = t->var();
    auto b = t->var();
    REQUIRE(a != b);
}

TEST_CASE("Cache::checkCommutative")
{
    auto t = Cache::instance();

    SECTION("No balancing")
    {
        auto a = t->operation(Opcode::MIN, t->X(), t->Y());
        REQUIRE(a->rank == 1);
    }

    SECTION("LHS")
    {
        auto b = t->operation(Opcode::MIN, t->X(),
                 t->operation(Opcode::MIN, t->Y(),
                 t->operation(Opcode::MIN, t->Z(), t->constant(1.0f))));
        REQUIRE(b->rank == 2);
    }

    SECTION("RHS")
    {
        auto b = t->operation(Opcode::MIN, t->operation(Opcode::MIN,
                 t->operation(Opcode::MIN, t->Z(), t->constant(1.0f)),
                     t->X()), t->Y());
        REQUIRE(b->rank == 2);
    }
}
