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

#include "ao/render/brep/mt.hpp"
#include "ao/render/brep/dual.hpp"
#include "ao/render/brep/scaffold.hpp"
#include "util/shapes.hpp"

using namespace Kernel;

TEST_CASE("TetMarcher: operator() on sphere")
{
    Evaluator e(sphere(1));

    // Create a padded scaffolding for the tree
    Region<3> r({-1, -1, -1}, {1, 1, 1});
    const auto scaffold = Scaffold<3>(&e, r, 2, true);

    // Create the quadtree on the scaffold (without subdividing farther)
    auto xtree = XTree<3>(&e, scaffold, 1000);

    TetMarcher mt(&e);
    Dual<3>::walk(xtree, mt);

    float rmin = 2;
    float rmax = 0;
    for (auto pt : mt.verts)
    {
        rmin = fmin(rmin, pt.norm());
        rmax = fmax(rmax, pt.norm());
    }
    REQUIRE(rmin > 0.99);
    REQUIRE(rmax < 1.01);
}

