set_project("Noelle")

add_rules("mode.debug", "mode.release", "mode.profile")
set_languages("cxx20")
add_cxxflags("/Zc:__cplusplus")
add_ldflags("/NODEFAULTLIB:LIBCMTD")

set_targetdir("lib")

includes("Vendor/glfw")
includes("Extras/glad")
includes("Vendor/freetype")
includes("Vendor/SDL")
includes("Vendor/bx")
includes("Vendor/bimg")
includes("Vendor/bgfx")
includes("Vendor/imgui")

includes("Engine/System")
includes("Engine/Runtime")
includes("Engine/Editor")

includes("Samples/Sample1")
includes("Samples/Sample2")
