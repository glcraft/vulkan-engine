add_rules("mode.debug", "mode.release")

add_requires("vulkan-headers", "vulkan-hpp", "vulkan-validation-layers", "glfw")

target("vulkan-engine")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("vulkan-headers", "vulkan-hpp", "vulkan-validation-layers", "glfw")
