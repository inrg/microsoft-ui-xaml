# Windows UI Library

[![Follow WinUI on Twitter](https://img.shields.io/twitter/follow/windowsui.svg?label=Follow%20WinUI%20on%20Twitter&style=social)](https://twitter.com/intent/follow?screen_name=windowsui)

The Windows UI Library (WinUI) provides official native Microsoft UI controls and features for Windows [UWP apps](https://docs.microsoft.com/windows/uwp/index).

WinUI is the easiest way to build great [Fluent Design](https://docs.microsoft.com/windows/uwp/design/fluent-design-system/) experiences for Windows.

WinUI can be used in any Windows 10 UWP XAML app, or in a Xamarin.Forms app running on Windows 10 using [native view embedding](https://docs.microsoft.com/xamarin/xamarin-forms/platform/native-views).

## WinUI 3.0 Announcement

At the Microsoft Build conference in May 2019 we [shared our plans for WinUI 3.0](https://mybuild.techcommunity.microsoft.com/sessions/77008), which will greatly expand the scope of WinUI to include the full native Windows UI platform. 

For more info check out the updated [**roadmap**](https://github.com/microsoft/microsoft-ui-xaml/blob/master/docs/roadmap.md) and join the discussion in the [**3.0 discussion issue**](https://github.com/microsoft/microsoft-ui-xaml/issues/717).

## Using WinUI
You can download and use WinUI packages in your app using the NuGet package manager: see the [Getting Started with the Windows UI Library](https://docs.microsoft.com/uwp/toolkits/winui/getting-started) page for more information.

### Packages

| NuGet Package | Build Status | Latest Versions | Documentation |
| --- | --- | --- | --- |
| [Microsoft.UI.Xaml](https://www.nuget.org/packages/Microsoft.UI.Xaml) <br /> Controls and Fluent Design for UWP apps | [![Build Status](https://dev.azure.com/ms/microsoft-ui-xaml/_apis/build/status/WinUI-Public-MUX-CI?branchName=master)](https://dev.azure.com/ms/microsoft-ui-xaml/_build/latest?definitionId=20?branchName=master) | [![latest stable version](https://img.shields.io/nuget/v/Microsoft.UI.Xaml.svg)](https://www.nuget.org/packages/Microsoft.UI.Xaml) <br /> [![latest prerelease version](https://img.shields.io/nuget/vpre/Microsoft.UI.Xaml.svg)](https://www.nuget.org/packages/Microsoft.UI.Xaml/absoluteLatest) | [2.1 release](https://docs.microsoft.com/uwp/toolkits/winui/release-notes/winui-2.1) |
| [Microsoft.UI.Xaml.Core.Direct](https://www.nuget.org/packages/Microsoft.UI.Xaml.Core.Direct) <br /> Low-level APIs for middleware components | | [![latest prerelease version](https://img.shields.io/nuget/vpre/Microsoft.UI.Xaml.Core.Direct.svg)](https://www.nuget.org/packages/Microsoft.UI.Xaml.Core.Direct/absoluteLatest) | [2.0 prerelease](https://docs.microsoft.com/uwp/api/microsoft.ui.xaml.core.direct) |

You can also build a WinUI package yourself from source. See [Contributing to the Windows UI Library](CONTRIBUTING.md) for more information on building and contributing to WinUI.

## Documentation

**WinUI usage documentation**:  
https://docs.microsoft.com/uwp/toolkits/winui

**Release notes**:  
https://docs.microsoft.com/uwp/toolkits/winui/release-notes/

**Sample code**:  
To view the WinUI controls in an interactive format, check out the Xaml Controls Gallery:
* Get the XAML Controls Gallery app from the [Microsoft Store](https://www.microsoft.com/store/productId/9MSVH128X2ZT)
* Get the source code on [GitHub](https://github.com/Microsoft/Xaml-Controls-Gallery)

## Contributing to WinUI
The WinUI team welcomes feedback and contributions!

For information on how to contribute please see [Contributing to the Windows UI Library](CONTRIBUTING.md).

## WinUI features

### Benefits

The WinUI Library provides some useful benefits when building apps for Windows 10:

1. **Helps you stay up to date**  
WinUI helps keep your app up to date with the latest versions of key controls and features of [UWP XAML](https://docs.microsoft.com/windows/uwp/xaml-platform/xaml-overview) and the [Fluent Design System](https://www.microsoft.com/design/fluent)

2. **Provides backward compatibility**  
WinUI is backward-compatible with a wide range of Windows 10 versions: you can start building and shipping apps with new XAML features immediately as soon as they're released, even if your users aren't on the latest version of Windows 10

3. **Makes it simpler to build version adaptive apps**  
You don't need version checks or conditional XAML markup to use WinUI controls or features: WinUI automatically adapts to the user's OS version

### Version support

The Microsoft.UI.Xaml 2.2 NuGet package requires your project to have TargetPlatformVersion &gt;= 10.0.18362.0 and TargetPlatformMinVersion &gt;= 10.0.14393.0 when building. 

Your app's users can be on any of the following supported Windows versions:
* Windows Insider Previews
* May 2019 Update (18362 aka "19H1")
* October 2018 Update (17763 aka "Redstone 5")
* April 2018 Update (17134 aka "Redstone 4")
* Fall Creators Update (16299 aka "Redstone 3")
* Creators Update (15063 aka "Redstone 2")
* Anniversary Update (14393 aka "Redstone 1")

Some features may have a reduced or slightly different user experience on older versions, particularly on builds before 15063. This should not impact overall usability.

## Roadmap

For info on the WinUI release schedule and high level plans please see the [Windows UI Library Roadmap](docs/roadmap.md).

## Data/Telemetry

This project collects usage data and sends it to Microsoft to help improve our products and services. Read Microsoft's [privacy statement to learn more](https://privacy.microsoft.com/privacystatement).

For more information on telemetry implementation see the [developer guide](docs/developer_guide.md#Telemetry).
