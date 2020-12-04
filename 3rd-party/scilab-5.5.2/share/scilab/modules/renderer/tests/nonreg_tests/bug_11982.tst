// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 11982 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11982
//
// <-- Short Description -->
// Ticks computations was made asynchronously
drawlater();
plot2d();
a=gca();
computed=a.x_ticks.locations;
assert_checkfalse(computed == (0:0.1:1));
