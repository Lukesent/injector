
# Process Injector

CellJectah is a C++ library made for injecting dynamic link libraries (DLLs) into running processes on Windows, originally used for CSGO and remainded undetected for multiple years. The library provides a simple and convenient API for injecting a DLL into a target process, as well as for parsing information about running processes and loaded modules.

## Projects

There are two projects in this repository: `servicer` and `injector (CellJectah)`.

The Servicer  acts as an intermediary between the DLL and the target process, providing the necessary functionality to inject the DLL into the process.

CellJectah is an injector inserted into Servicer that then parses the Target DLL into a process to lower detection rates.

Target DLL -> CellJectah -> Servicer -> Windows Process

## Features

- Inject a DLL into a running process
- Parse information about running processes and loaded modules
- Eject a loaded module from a process
- Retrieve the file path of a given file in the same directory as the executable
- Easy-to-use API with detailed documentation
- Random strings to change signature. 
- XorStr to stop string reading from Target DLL, double encrypted to modify signature of TargetDLL.
- Mask injection with Middle-Man service that passes to target process.
- Could use a trusted process as the injector in memory.

## Getting Started

To use Process Injector in your project, you will need to download the source code and add the necessary files to your project. The library consists of a single header file (`Injector.hpp`) and a single source file (`Injector.cpp`).

To inject a DLL into a running process, you can create an instance of the `Injector` class and call the `inject` method, passing in the path to the DLL file and the process ID of the target process. For example:

```c++
Injector injector;
if (injector.inject(1234, "C:\\path\\to\\mydll.dll"))
{
    // DLL successfully injected
}
else
{
    // Injection failed
}
```

To parse information about running processes and loaded modules, you can use the static methods `parse_running_proccesses` and `parse_loaded_modules`, respectively. For example:

```c++
auto procs = Injector::parse_running_proccesses();
for (const auto & proc : procs)
{
    std::cout << "Process ID: " << proc.second.th32ProcessID << std::endl;
    std::cout << "Process name: " << proc.second.szExeFile << std::endl;
}

auto modules = Injector::parse_loaded_modules(1234);
for (const auto & module : modules)
{
    std::cout << "Module name: " << module.second.szModule << std::endl;
    std::cout << "Module path: " << module.second.szExePath << std::endl;
}
```

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

Process Injector was inspired by the [DLL Injector](https://github.com/JKornev/DLL-Injector) library by JKornev. Special thanks to the authors of the XorStr library , which is used for string obfuscation in this project.
