set_project("Noelle")

-- add_rules("mode.debug", "mode.release", "mode.profile")
add_rules("mode.debug")
set_languages("cxx20")

add_defines("SYSTEM_EXPORTS")

includes("Vendor/glfw")
includes("Vendor/imgui")

includes("Engine/System")
includes("Engine/Editor")

includes("Samples/Sample1")
includes("Samples/Sample2")
