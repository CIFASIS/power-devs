
function result = atomsInstall(packages,allusers)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

endfunction
