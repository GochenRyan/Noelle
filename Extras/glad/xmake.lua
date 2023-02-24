target("Glad")
    set_kind("static")
    set_languages("c")
    -- set_targetdir("$(projectdir)/libs/Glad")
    add_includedirs("include")
    add_headerfiles(
        "include/glad/glad.h",
        "include/KHR/khrplatform.h"
    )
    add_files("src/glad.c")
    set_group("Dependencies")