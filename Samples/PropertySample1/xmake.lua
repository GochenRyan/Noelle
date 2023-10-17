target("PropertySample1")
    set_kind("binary")
    add_headerfiles("**.h")
    add_files("*.cpp")
    add_deps(
        "System",
        "Graphic",
        "Runtime",
        "spdlog"
    )

    add_includedirs(
        "$(projectdir)/Engine/DataStruct/",
        "$(projectdir)/Engine/System/",
        "$(projectdir)/Engine/Graphic",
        "$(projectdir)/Engine/Runtime",
        "$(projectdir)/Vendor/spdlog/include/"
    )

    add_linkdirs("$(projectdir)/lib/")
    set_group("Samples")