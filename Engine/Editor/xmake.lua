target("Editor")
    set_kind("binary")
    add_deps(
        "libGLFW",
        "libBGFX",
        "libBX",
        "libBImg",
        "libImGui",
        "libGlad",
        "SDL3",
        "AssetPipeline",
        -- inner
        "System",
        "Runtime"
    )

    add_headerfiles(
        "**.h",
        "**.hpp",
        "**.inl"
    )
    add_files(
        "**.cpp"
    )

    add_includedirs(
        "$(projectdir)/Vendor/glfw/include/",
        "$(projectdir)/Vendor/imgui/",
        "$(projectdir)/Extras/glad/include/",
        "$(projectdir)/Vendor/freetype/include/",
        "$(projectdir)/Vendor/SDL/include/",
        "$(projectdir)/Vendor/bgfx/include/",
        "$(projectdir)/Vendor/bimg/include/",
        "$(projectdir)/Vendor/bx/include/",
        "$(projectdir)/Vendor/bimg/3rdparty/",
        -- inner
        "ImGui/",
        "Layer/",
        "Rendering/",
        "Window/",
        "$(projectdir)/Engine/DataStruct/",
        "$(projectdir)/Engine/Runtime/",
        "$(projectdir)/Engine/System/",
        "$(projectdir)/Engine/Graphic/",

        "$(projectdir)/Vendor/AssetPipeline/public/",
        "$(projectdir)/Engine/Math/"
    )

    add_links(
        "SDL3"
    )

    add_linkdirs("$(projectdir)/lib/")

    add_defines("CDEDITOR_RESOURCES_ROOT_PATH=\"" .. (os.projectdir():gsub("\\", "\\\\")) .. "/Res/" .. "\"")
    add_defines("__STDC_LIMIT_MACROS"
                , "__STDC_FORMAT_MACROS"
                , "__STDC_CONSTANT_MACROS"
                , "NDEBUG"
                , "WIN32"
                , "_WIN32"
                , "_HAS_EXCEPTIONS=0"
                , "_HAS_ITERATOR_DEBUGGING=0"
                , "_ITERATOR_DEBUG_LEVEL=0"
                , "_SCL_SECURE=0"
                , "_SECURE_SCL=0"
                , "_SCL_SECURE_NO_WARNINGS"
                , "_CRT_SECURE_NO_WARNINGS"
                , "_CRT_SECURE_NO_DEPRECATE"
                , "BX_CONFIG_DEBUG")
    add_defines("CD_PLATFORM_WINDOWS")
