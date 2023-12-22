target("Graphic")
    set_kind("shared")

    add_deps(
        "System",
        "spdlog"
    )

    add_defines("GRAPHIC_EXPORTS")
    add_headerfiles("**.h", "**.inl", "**.marc")
    add_files("**.cpp")

    add_includedirs(
        "$(projectdir)/Engine/DataStruct/",
        "$(projectdir)/Engine/Graphic/",
        "$(projectdir)/Engine/System/",
        "$(projectdir)/Vendor/spdlog/include/"
    )
    add_linkdirs("$(projectdir)/lib/")

    set_group("Engine")