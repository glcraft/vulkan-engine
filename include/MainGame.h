#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>
namespace vkr = vk::raii;

class MainGame 
{
public:
    static constexpr uint32_t WINDOW_WIDTH = 800;
    static constexpr uint32_t WINDOW_HEIGHT = 600;

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
    std::unique_ptr<vkr::Instance> m_instance;
};