add_rules("mode.debug", "mode.release")

local vulkan_validation_layers = is_plat("linux") and "vulkan-validation-layers" or "vulkan-validationlayers"

add_requires("vulkan-headers", "vulkan-hpp", "glfw")
if is_mode("debug") then
    add_requires(vulkan_validation_layers)
end

target("vulkan-engine")
    set_kind("binary")
    set_languages("cxx20")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("vulkan-headers", "vulkan-hpp", "glfw")
    add_defines("VULKAN_HPP_NO_CONSTRUCTORS")
    if is_mode("debug") then
        add_packages(vulkan_validation_layers)
        add_defines("DEBUG")
    end
    on_load(function (target)
        if is_plat("linux", "macosx") then
            target:add("links", "pthread", "m", "dl")
        end
    end)

