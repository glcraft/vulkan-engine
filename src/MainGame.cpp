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

bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (std::string_view layerName : MainGame::validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (layerName ==  layerProperties.layerName) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }
    return true;
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
    if constexpr (enableValidationLayers) {
        if (!checkValidationLayerSupport())
            throw std::runtime_error("les validations layers sont activées mais ne sont pas disponibles!");
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    m_instance = std::make_unique<vkr::Instance>(std::move(m_context->createInstance(createInfo)));
}