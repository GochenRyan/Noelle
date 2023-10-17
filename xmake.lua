set_project("Noelle")

add_rules("mode.debug", "mode.release", "mode.profile")
set_languages("cxx20")
add_cxxflags("/Zc:__cplusplus")
add_ldflags("/NODEFAULTLIB:LIBCMTD")

if is_mode("debug") then
    add_cxflags("-D _DEBUG")
    set_runtimes("MDd")
end

set_targetdir("lib")

includes("Vendor/glfw")
includes("Extras/glad")
includes("Vendor/freetype")
includes("Vendor/SDL")
includes("Vendor/bx")
includes("Vendor/bimg")
includes("Vendor/bgfx")
includes("Vendor/imgui")
includes("Vendor/spdlog")

includes("Engine/System")
includes("Engine/Graphic")
includes("Engine/Runtime")
includes("Engine/Editor")

includes("Samples/Sample1")
includes("Samples/Sample2")
includes("Samples/Sample3")
includes("Samples/PropertySample1")
