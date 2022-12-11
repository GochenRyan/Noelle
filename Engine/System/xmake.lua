target("System")
    set_kind("shared")
    add_defines("SYSTEM_EXPORTS")
    add_includedirs("$(projectdir)/Extras/TBB/include")
    add_linkdirs("$(projectdir)/Extras/TBB/Lib")
    add_headerfiles("*.h")
    add_files("*.cpp")
    set_group("Engine")
    after_build(function(target)
        os.cp("$(projectdir)/Extras/TBB/Lib/tbb.dll", target:targetdir())
        os.cp("$(projectdir)/Extras/TBB/Lib/tbb_debug.dll", target:targetdir())
        os.cp("$(projectdir)/Extras/TBB/Lib/tbbmalloc.dll", target:targetdir())
        os.cp("$(projectdir)/Extras/TBB/Lib/tbbmalloc_debug.dll", target:targetdir())
        os.cp("$(projectdir)/Extras/dbghelp.dll", target:targetdir())
    end)