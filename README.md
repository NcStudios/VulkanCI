# VulkanCI

[![test](https://github.com/NcStudios/VulkanCI/actions/workflows/test.yml/badge.svg)](https://github.com/NcStudios/VulkanCI/actions/workflows/test.yml)

This action performs minimal setup to enable building and running Vulkan applications on GitHub-hosted runners. It installs the Vulkan headers, loader, and validation layers, as well as [SwiftShader](https://github.com/google/swiftshader) (a software-only Vulkan implementation), and sets up the GitHub environment with the necessary variables.

## Usage
```yaml
- uses: NcStudios/VulkanCI@v1.0
  with:
    # VulkanSDK version to use
    # Must be a supported version
    # The default value is 1.4.328.0
    sdkVersion: 1.4.328.0

    # Optional path to installation path
    # The default value is '${{ github.workspace }}`
    installPath: 'myArtifacts'
```

## Outputs
- `vulkan-install-path` Path where the VulkanSDK was installed
- `swiftshader-install-path` Path where SwiftShader was installed

Variables can be accessed through the step outputs: `${{ steps.<id>.outputs.swiftshader-install-path }}`.

In addition to the output variables, the action updates the following contexts to allow files and libraries to be found:
- `github.env`
  - `VULKAN_SDK`
  - `VULKAN_SDK_VERSION`
  - `VK_LAYER_PATH`
  - `VK_DRIVER_FILES` (points to a SwiftShader manifest at `swiftshader-install-path/vk_swiftshader_icd.json`)
  - `LD_LIBRARY_PATH` (Linux only)
  - `DYLD_LIBRARY_PATH` (macOS only)
- `github.path`
  - `$VULKAN_SDK/bin` (Windows only)

## Supported VulkanSDK Versions
- 1.4.328.0
- 1.4.321.0
- 1.4.309.0
- 1.3.283.0
- 1.3.280.0
- 1.3.275.0

## Supported Runners
Tests currently run on:
- `windows-2025` (x64)
- `ubuntu-24.04` (x64)
- `macOS-15` (ARM64)
- `macOS-15-intel` (x86_64)

macOS binaries are available for both ARM64 (Apple Silicon) and x86_64 (Intel). The action automatically detects the runner architecture and downloads the appropriate artifacts.

  | Runner         | Architecture | SDK Artifact                             | SwiftShader Artifact             |
  |----------------|--------------|------------------------------------------|----------------------------------|
  | windows-2025   | x64          | vulkanSDK-{version}-windows-x64.zip      | swiftshader-windows-x64.zip      |
  | ubuntu-24.04   | x64          | vulkanSDK-{version}-ubuntu-24.04-x64.zip | swiftshader-ubuntu-24.04-x64.zip |
  | macOS-15       | ARM64        | vulkanSDK-{version}-macos-15-arm64.zip   | swiftshader-macos-15-arm64.zip   |
  | macOS-15-intel | x86_64       | vulkanSDK-{version}-macos-15-x86_64.zip  | swiftshader-macos-15-x86_64.zip  |

## Known Limitations
Due to significant build times, pre-built artifacts are downloaded from the corresponding release of this action. This requires the action version to be specified as a tag - targeting a SHA will not work.

The Vulkan loader is built with `LOADER_USE_UNSAFE_FILE_SEARCH=ON` to more reliably allow file discovery through environment variables. To state the obvious, the artifacts provided here are not intended for use outside of testing environments.



