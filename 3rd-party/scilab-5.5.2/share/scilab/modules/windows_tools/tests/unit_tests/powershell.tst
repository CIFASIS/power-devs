// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
if getos() == 'Windows' then
  cd('SCI/bin');
  r = powershell('ls');
  if r == [] then pause,end
end