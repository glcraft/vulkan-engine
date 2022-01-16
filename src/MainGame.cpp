#include <MainGame.h>
#include <vulkan/vulkan_raii.hpp>
#include <set>
MainGame::MainGame()
{
    m_isRunning = true;
    m_window = nullptr;
}
MainGame::~MainGame()
{
    m_presentQueue.reset();
    m_graphicsQueue.reset();
    m_device.reset();
    m_surface.reset();
    m_instance.reset();
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
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (std::string_view layerName : MainGame::validationLayers) {
        if (std::find_if(availableLayers.begin(), availableLayers.end(),
                         [layerName](const vk::LayerProperties& layerProperties) {
                             return layerProperties.layerName == layerName;
                         }) == availableLayers.end()) {
            return false;
        }
    }
    return true;
}

void MainGame::initVulkan()
{
    m_context = std::make_unique<vkr::Context>();

    createInstance();
    createSurface();
    initDevices();
}

void MainGame::createInstance()
{
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
void MainGame::createSurface()
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(**m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("échec de la création de la window surface!");
    }
    m_surface = std::make_unique<vkr::SurfaceKHR>(m_instance, std::move(surface));
}
void MainGame::initDevices()
{
    auto phy_devices = m_instance->enumeratePhysicalDevices();
    if (phy_devices.empty())
        throw std::runtime_error("Pas de carte graphique supportant Vulkan!");
    for (const auto& device : phy_devices) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = std::make_unique<vkr::PhysicalDevice>(std::move(device));
            break;
        }
    }

    if (!m_physicalDevice) {
        throw std::runtime_error("aucun GPU ne peut exécuter ce programme!");
    }
    float queuePriority = 1.0f;
    std::set<uint32_t> uniqueQueueFamilies = {m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.presentFamily.value()};
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        queueCreateInfos.push_back(
            vk::DeviceQueueCreateInfo {
                .queueFamilyIndex = queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &queuePriority,
            }
        );
    }
    vk::PhysicalDeviceFeatures deviceFeatures = {};
    vk::DeviceCreateInfo deviceCreateInfo = {
        .queueCreateInfoCount = queueCreateInfos.size(),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = 0,
        .pEnabledFeatures = &deviceFeatures,
    };
    if constexpr (enableValidationLayers) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }
    m_device = std::make_unique<vkr::Device>(std::move(m_physicalDevice->createDevice(deviceCreateInfo)));

    m_graphicsQueue = std::make_unique<vkr::Queue>(m_device->getQueue(m_queueFamilyIndices.graphicsFamily.value(), 0));
    m_presentQueue = std::make_unique<vkr::Queue>(m_device->getQueue(m_queueFamilyIndices.presentFamily.value(), 0));
}
MainGame::QueueFamilyIndices MainGame::findQueueFamily(const vkr::PhysicalDevice& device)
{
    MainGame::QueueFamilyIndices indices;

    auto queueFamilies = device.getQueueFamilyProperties();
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }
        if (device.getSurfaceSupportKHR(i, **m_surface)) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }
    return indices;
}

bool MainGame::isDeviceSuitable(const vkr::PhysicalDevice& device)
{
    // auto queueFamilies = device.getQueueFamilyProperties();
    
    // auto extensions = device.enumerateDeviceExtensionProperties();
    // auto features = device.getFeatures();
    return findQueueFamily(device).isComplete();
}