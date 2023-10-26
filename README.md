# TestProgramDemo

This demo showcases a Test Program that simulates a typical Automated Test Equipment (ATE) programming environment. The primary objective is to demonstrate the interplay between Simulate Test System's ATE Simulation software and the Visual Studio debugging environment.

1. **Clone the Repository**: Either clone or download this project and open it in Visual Studio.
2. **Account Setup**: Create an account on [Simulate Test](https://app.simulatetest.com) and log in.
3. **Configuration**: Update `Settings.h` in the mock test program to align with the user key provided during account registration. Here's a code snippet illustrating how you can do this:
   ```cpp
   #pragma once
   #define STS_USER_KEY "YOUR_USER_KEY_HERE"    // <-- Replace with your user key
   #define STS_HOSTNAME "app.simulatetest.com"

   #ifdef DLL_WRAP_EXPORTS
   #define WRAP_API extern "C" __declspec(dllexport)
   #else
   #define WRAP_API extern "C" __declspec(dllimport)
   #endif

   WRAP_API void STS_LoadSettings(const char* key, const char* hostname);
   ```
   4. **File Upload**: Transfer the 4 designated files from this project's `Demo` directory to the drop box on the [Simulate Test homepage](https://app.simulatetest.com). The files include:
   - `Schematic.PDF`
   - `IPC2581.xml`
   - `Tester1_PinMapping.xlsx`
   - `DUT_Info.xlsx`
5. **ATE Simulation**: On the website, navigate to the "ATE Simulation" section.
6. **Execution**: Launch the Visual Studio project. Once started, click on "Load Lot" followed by "Test".
7. **Validation**: On a successful connection, results should populate in the mock datalog as well as on the "ATE Simulation" webpage.
8. **Documentation**: For a comprehensive understanding and navigation guide, visit [Simulate Test Documentation](https://docs.simulatetest.com).
