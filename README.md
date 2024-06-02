# VulkanCI

[![test](https://github.com/NcStudios/VulkanCI/actions/workflows/test.yml/badge.svg)](https://github.com/NcStudios/VulkanCI/actions/workflows/test.yml)

This action performs minimal setup to enable building and running Vulkan applications on GitHub-hosted runners. It installs the Vulkan headers, loader, and validation layers, as well as [SwiftShader](https://github.com/google/swiftshader) (a software-only Vulkan implementation), and sets up the GitHub environment with the necessary variables.

## Usage
```yaml
- uses: NcStudios/VulkanCI@v1.0
  with:
    # VulkanSDK version to use
    # Must be a supported version
    # The default value is 1.3.283.0
    sdkVersion: 1.3.283.0

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
- 1.3.283.0
- 1.3.280.0
- 1.3.275.0
- 1.3.261.1

## Supported Runners
The intention is to support all standard GitHub-hosted runners. Tests currently run on:
- `windows-2019`
- `windows-2022`
- `ubuntu20.04`
- `ubuntu22.04`
- `ubuntu24.04`
- `macOS-12`
- `macOS-13`
- `macOS-14`

## Known Limitations
Due to significant build times, pre-built artifacts are downloaded from the corresponding release of this action. This requires the action version to be specified as a tag - targeting a SHA will not work.

The Vulkan loader is built with `LOADER_USE_UNSAFE_FILE_SEARCH=ON` to more reliably allow file discovery through environment variables. To state the obvious, the artifacts provided here are not intended for use outside of testing environments.

Currently, all macOS binaries are built for x86_64.
