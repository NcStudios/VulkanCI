#include <vulkan/vulkan.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

#ifndef VKCI_API_VERSION
#error Must build test with VKCI_API_VERSION defined
#endif

constexpr auto requestedVersionString = std::string_view{VKCI_API_VERSION};
constexpr auto validationLayers = std::string_view{"VK_LAYER_KHRONOS_validation"};
constexpr auto swiftshaderDeviceName = std::string_view{"SwiftShader Device (LLVM 10.0.0)"};

int main()
{
    auto version = uint32_t{};
    vkEnumerateInstanceVersion(&version);
    const auto actualVersionString = std::to_string(VK_API_VERSION_MAJOR(version)) + "."
                                   + std::to_string(VK_API_VERSION_MINOR(version)) + "."
                                   + std::to_string(VK_API_VERSION_PATCH(version));

    const auto expectedVersionString = []()
    {
        if (std::count(requestedVersionString.begin(), requestedVersionString.end(), '.') == 3)
        {
            return requestedVersionString.substr(0, requestedVersionString.rfind('.'));
        }

        return requestedVersionString;
    }();

    if (actualVersionString != expectedVersionString)
    {
        std::cerr << "Failed: Unexpected Vulkan version. expected: " << expectedVersionString << ", actual: " << actualVersionString << '\n';
        return 1;
    }

    auto layerCount = uint32_t{};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    auto availableLayers = std::vector<VkLayerProperties>(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    const auto layerPos = std::find_if(availableLayers.cbegin(), availableLayers.cend(), [](const auto layer)
    {
        return validationLayers == layer.layerName;
    });

    if (layerPos == availableLayers.cend())
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
        std::cerr << "Failed: did not find any physical devices\n";
        return 1;
    }

    auto devices = std::vector<VkPhysicalDevice>(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    auto foundSwiftShader = false;
    for (auto device : devices)
    {
        auto properties = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(device, &properties);
        if (swiftshaderDeviceName == properties.deviceName)
        {
            foundSwiftShader = true;
            break;
        }
    }

    if (!foundSwiftShader)
    {
        std::cerr << "Failed: did not find SwiftShader\n";
        return 1;
    }

    std::cout << "Passed\n";
    return 0;
}
