#include <array>
#include <string_view>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>
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
    void initWindow();
    void initVulkan();

    bool m_isRunning;
    GLFWwindow* m_window;

    std::unique_ptr<vkr::Context> m_context;
    std::unique_ptr<vkr::Instance> m_instance;
};