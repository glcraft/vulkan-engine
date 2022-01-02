#include <MainGame.h>
#include <vulkan/vulkan_raii.hpp>
MainGame::MainGame()
{
    m_isRunning = true;
    m_window = nullptr;
}
MainGame::~MainGame()
{
    glfwDestroyWindow(m_window);

    glfwTerminate();
}
void MainGame::init()
{
    initWindow();
    initVulkan();
}
void MainGame::run()
{
    while (m_isRunning)
    {
        update();
        render();
    }
}
void MainGame::update()
{
    glfwPollEvents();
    m_isRunning = !glfwWindowShouldClose(m_window);
}
void MainGame::render()
{

}

void MainGame::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);

}
void MainGame::initVulkan()
{
    m_context = std::make_unique<vkr::Context>();

    vk::ApplicationInfo applicationInfo = {
        .pApplicationName = "Vulkan Engine",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
    };
    auto createInfo = vk::InstanceCreateInfo {
        .pApplicationInfo = &applicationInfo,
        
    };
    m_instance = std::make_unique<vkr::Instance>(std::move(m_context->createInstance(createInfo)));
}