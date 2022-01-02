add_rules("mode.debug", "mode.release")

add_requires("vulkan-headers", "vulkan-hpp", "glfw")
if is_mode("debug") then
    add_requires("vulkan-validationlayers")
end

target("vulkan-engine")
    set_kind("binary")
    set_languages("cxx20")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("vulkan-headers", "vulkan-hpp", "glfw")
    add_defines("VULKAN_HPP_NO_CONSTRUCTORS")
    if is_mode("debug") then
        add_packages("vulkan-validationlayers")
        add_defines("DEBUG")
    end

