target("Sample3")
    set_kind("binary")
    add_files("*.cpp")
    add_deps(
        "System",
        "Graphic"
    )

    add_includedirs(
        "$(projectdir)/Engine/DataStruct/",
        "$(projectdir)/Engine/System/",
        "$(projectdir)/Engine/Graphic"
    )

    add_linkdirs("$(projectdir)/lib/")
    set_group("Samples")