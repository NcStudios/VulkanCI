#include <vulkan/vulkan.h>

#include <stdint.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <vector>

constexpr auto validationLayers = std::string_view{"VK_LAYER_KHRONOS_validation"};
constexpr auto swiftshaderDeviceName = std::string_view{"SwiftShader Device (LLVM 10.0.0)"};

int main()
{
    auto version = uint32_t{};
    vkEnumerateInstanceVersion(&version);

    // @todo assert version

    std::cout << "Vulkan Version "
              << VK_VERSION_MAJOR(version) << '.'
              << VK_VERSION_MINOR(version) << '.'
              << VK_VERSION_PATCH(version)
              << " (" << version << ")\n";

    auto layerCount = uint32_t{};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    auto availableLayers = std::vector<VkLayerProperties>(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    if (!std::ranges::contains(availableLayers, validationLayers, &VkLayerProperties::layerName))
    {
        std::cerr << "Failed: did not find validation layers\n";
        return 1;
    }

    auto appInfo = VkApplicationInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = version;
    auto createInfo = VkInstanceCreateInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    const auto layers = std::array{ validationLayers.data() };
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();
    auto instance = VkInstance{};
    if (auto result = vkCreateInstance(&createInfo, nullptr, &instance); result)
    {
        std::cerr << "Failed: vkCreateInstance failed with: " << result << '\n';
        return 1;
    }

    auto deviceCount = uint32_t{};
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        std::cerr << "Failed: did not find a physical device\n";
        return 1;
    }

    auto devices = std::vector<VkPhysicalDevice>(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    auto deviceProperties = std::vector<VkPhysicalDeviceProperties>{};
    std::ranges::transform(devices, std::back_inserter(deviceProperties), [](auto device)
    {
        auto properties = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(device, &properties);
        return properties;
    });

    if (!std::ranges::contains(deviceProperties, swiftshaderDeviceName, &VkPhysicalDeviceProperties::deviceName))
    {
        std::cerr << "Failed: did not find SwiftShader\n";
        return 1;
    }

    std::cout << "Passed\n";
    return 0;
}
