#include <array>
#include <optional>
#include <memory>
#include <string_view>
#include <vulkan/vulkan_raii.hpp>
#include <GLFW/glfw3.h>
namespace vkr = vk::raii;

class MainGame 
{
public:
    static constexpr uint32_t WINDOW_WIDTH = 800;
    static constexpr uint32_t WINDOW_HEIGHT = 600;

    #ifdef DEBUG
        static constexpr bool enableValidationLayers = true;
    #else
        static constexpr bool enableValidationLayers = false;
    #endif
    static constexpr auto validationLayers = std::array{
        "VK_LAYER_KHRONOS_validation"
    };
    static constexpr auto deviceExtensions = std::array{
        VK_KHR_SURFACE_EXTENSION_NAME,
    };


    MainGame();
    ~MainGame();
    void init();
    void run();
    void update();
    void render();
    bool running() { return m_isRunning; }
    void quit() { m_isRunning = false; }
private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;

        SwapChainSupportDetails(const vkr::PhysicalDevice& physicalDevice, const vkr::SurfaceKHR& surface) {
            capabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
            formats = physicalDevice.getSurfaceFormatsKHR(*surface);
            presentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
        }
        bool isComplete() {
            return !formats.empty() && !presentModes.empty();
        }
        vk::SurfaceFormatKHR chooseSwapSurfaceFormat() {
            if (formats.size() == 1 && formats[0].format == vk::Format::eUndefined) {
                return {
                    .format = vk::Format::eB8G8R8A8Unorm,
                    .colorSpace = formats[0].colorSpace,
                };
            }
            for (const auto& format : formats) {
                if (format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                    return format;
                }
            }
            return formats[0];
        }
    };

    void initWindow();
    void initVulkan();
    //Vulkan inits
    void createInstance();
    void createSurface();
    void initDevices();
    bool isDeviceSuitable(const vkr::PhysicalDevice& device);
    QueueFamilyIndices findQueueFamily(const vkr::PhysicalDevice&);


    bool m_isRunning;
    GLFWwindow* m_window;

    std::unique_ptr<vkr::Context> m_context;
    std::unique_ptr<vkr::Instance> m_instance;
    std::unique_ptr<vkr::SurfaceKHR> m_surface;

    std::unique_ptr<vkr::PhysicalDevice> m_physicalDevice;
    QueueFamilyIndices m_queueFamilyIndices;
    std::unique_ptr<vkr::Device> m_device;
    std::unique_ptr<vkr::Queue> m_graphicsQueue;
    std::unique_ptr<vkr::Queue> m_presentQueue;
};