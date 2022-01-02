#include <MainGame.h>
#include <vulkan/vulkan_raii.hpp>
MainGame::MainGame()
{
    m_isRunning = true;
    m_window = nullptr;
}
MainGame::~MainGame()
{}
void MainGame::init()
{
    initWindow();
    initVulkan();
}
void MainGame::run()
{

}
void MainGame::update()
{

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
    auto applicationInfo = vk::ApplicationInfo {

    };
    auto createInfo = vk::InstanceCreateInfo {
        .
    };
    m_instance = vkr::Instance(vk::InstanceCreateInfo{
        .
    });
}