target("Editor")
    set_kind("binary")

    add_includedirs(
        "$(projectdir)/Engine/System",
        "$(projectdir)/Vendor/imgui",
        "$(projectdir)/Vendor/glfw/include",
        "$(projectdir)/Extras/glad/include"
    )

    add_subdirs(
        "$(projectdir)/Engine/System",
        "$(projectdir)/Vendor/imgui",
        "$(projectdir)/Vendor/glfw",
        "$(projectdir)/Extras/glad"
    )

    if is_plat("windows") then
        add_links("$(buildir)/windows/$(arch)/$(mode)/*.lib")
    end

    add_headerfiles("*.h")
    add_headerfiles("ImGui/*.h")
    add_files("Application.cpp")
    add_files("ImGui/*.cpp")
    set_group("Engine")