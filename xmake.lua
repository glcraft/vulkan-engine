add_rules("mode.debug", "mode.release")

add_requires("vulkan-headers", "vulkan-hpp", "vulkan-validation-layers", "glfw")

target("vulkan-engine")
    set_kind("binary")
    set_languages("cxx20")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_defines("VULKAN_HPP_NO_CONSTRUCTORS")
    if is_mode("debug") then
        add_defines("DEBUG")
        set_symbols("debug")
        set_optimize("none")
    end

