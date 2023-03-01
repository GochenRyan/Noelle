target("Editor")
    set_kind("binary")
    add_deps(
        "libGLFW",
        "libImGui",
        "libGlad",
        -- inner
        "System"
    )

    add_headerfiles(
        "*.h",
        "ImGui/*.h"
    )
    add_files(
        "*.cpp",
        "ImGui/*.cpp"
    )

    add_includedirs(
        "$(projectdir)/Vendor/glfw/include/",
        "$(projectdir)/Vendor/imgui/",
        "$(projectdir)/Extras/glad/include/",
        -- inner
        "ImGui/",
        "$(projectdir)/Engine/System/"
    )

    add_linkdirs("$(projectdir)/lib/")
