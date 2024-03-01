target("libGlad")
    set_kind("static")
    add_headerfiles(
        "include/glad/glad.h",
        "include/KHR/khrplatform.h"
    )
    add_includedirs("include/")
    add_files("src/glad.c")

    set_group("Vendor")
