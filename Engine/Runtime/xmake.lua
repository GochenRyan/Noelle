target("Runtime")
    set_kind("shared")

    add_deps(
        "System"
    )

    add_defines("RUNTIME_EXPORTS")
    add_includedirs(
        "$(projectdir)/Engine/Runtime",
        "$(projectdir)/Engine/System/")
    add_headerfiles("**.h", "**.inl")
    add_files("**.cpp")
    add_linkdirs("$(projectdir)/lib/")

    set_group("Engine")
