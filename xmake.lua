set_project("Noelle")

add_rules("mode.debug", "mode.release", "mode.profile")
set_languages("cxx20")

add_defines("SYSTEM_EXPORTS")

set_targetdir("lib")

includes("Vendor/glfw")
includes("Extras/glad")
includes("Vendor/imgui")

includes("Engine/System")
includes("Engine/Editor")

includes("Samples/Sample1")
includes("Samples/Sample2")
