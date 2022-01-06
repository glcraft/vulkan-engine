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
    vkr::Queue m_graphicsQueue;
    vkr::Queue m_presentQueue;
};