// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// <-- Non-regression test for bug 353 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/353/
//
// <-- Short Description -->
// The csvRead function does not manage the range.
// =================================g============================================

path = SCI+"/modules/spreadsheet/tests/unit_tests/";

// Read only rows/columns in range
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double" , [] , [], [2 1 3 2]);
expected = [
0.10000000000000001 1.1000000000000001
0.10000000000000001 0.10000000000000001
];
assert_checkequal(r, expected);
