# VulkanCI

[![test](https://github.com/NcStudios/VulkanCI/actions/workflows/test.yml/badge.svg)](https://github.com/NcStudios/VulkanCI/actions/workflows/test.yml)

This action performs minimal environment setup for running Vulkan applications on GitHub-hosted runners. It downloads a pre-built package containing the Vulkan headers, loader, and validation layers as well as SwiftShader, serving as software-only Vulkan implementation.

## Usage
```yaml
- uses: NcStudios/VulkanCI@v1.0
  with:
    # VulkanSDK version to use
    # Must be a supported version
    sdkVersion: 1.3.261.1

    # Optional path to installation path
    # The default value is '${{ github.workspace }}`
    installPath: 'myArtifacts'
```

## Output Variables
- `vulkan-sdk-install-path` Path where the VulkanSDK was installed
- `swiftshader-install-path` Path where SwiftShader was installed

Variables can be accessed through the step outputs: `${{ steps.<id>.outputs.swiftshader-install-path }}`.

In addition to the output variables, the action updates the following contexts to allow files and libraries to be found:
- `github.env`
  - `VULKAN_SDK`
  - `VULKAN_SDK_VERSION`
  - `VK_LAYER_PATH`
  - `VK_DRIVER_FILES` (points to a SwiftShader manifest at `swiftshader-install-path/vk_swiftshader_icd.json`)
  - `LD_LIBRARY_PATH` (Appends on Linux only)
  - `DYLD_LIBRARY_PATH` (Appends on macOS only)
- `github.path`
  - `$VULKAN_SDK/bin` (Windows only)

## Supported VulkanSDK Versions
- 1.3.261.1

## Known Limitations
Due to limitations in SwiftShader, macOS binaries are built for x86_64.

While SwiftShader is a fully conforming Vulkan 1.3 implementation, it typically does not support a wide range of extensions. YMMV.
