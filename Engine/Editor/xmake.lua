target("Editor")
    set_kind("binary")

    add_includedirs("$(projectdir)/Engine/System")
    add_includedirs("$(projectdir)/Vendor/imgui")
    add_includedirs("$(projectdir)/Vendor/glfw/include")
    add_includedirs("$(projectdir)/Extras/glad/include")
    add_linkdirs("$(projectdir)/Vendor/imgui/examples/libs")

    add_headerfiles("*.h")
    add_headerfiles("ImGui/*.h")
    add_files("Application.cpp")
    add_files("ImGui/*.cpp")
    set_group("Engine")