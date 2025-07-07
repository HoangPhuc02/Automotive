# AUTOSAR Classic Software Modules

This workspace contains implementations of various AUTOSAR modules and STM32F103 peripherals.

## Project Structure

- **0. Setup Makefile** - Basic project setup with Makefile templates
- **1. GPIO** - Digital Input/Output implementations
  - **DIO** - Digital I/O driver implementation
  - **PORT** - Port driver implementation
- **2. ADC** - Analog to Digital Converter implementations
  - **examples** - Various ADC usage examples
- **AutosarLib** - AUTOSAR-compliant library implementations

## ADC Examples

The ADC module includes several example implementations demonstrating different ADC configurations:

- Group-based ADC sampling with hardware trigger
- Continuous sampling with DMA transfer
- Single channel ADC with software trigger
- And more

### Building and Running Examples

You can build a specific example using:

```bash
# Navigate to the ADC directory
cd "2. ADC"

# For Unix/Linux/WSL:
make example EXAMPLE=example_name

# For Windows (PowerShell):
make example-win EXAMPLE=example_name
```

Where `example_name` is the filename without the `.c` extension.

#### VS Code Integration

For VS Code users, you can use the included tasks:

1. Open the command palette (`Ctrl+Shift+P`)
2. Select "Tasks: Run Task"
3. Choose "Build Example" or "Build and Flash Example"
4. Enter the example name when prompted

See the `examples/README.md` file for more details on available examples and usage.

## Compiling and Flashing

The project uses a Makefile-based build system:

- `make all` - Compiles the project and generates .bin and .hex files
- `make flash` - Flashes the compiled binary to the target using OpenOCD
- `make clean` - Removes all build artifacts

## Development Environment

- ARM GCC Toolchain
- Make
- OpenOCD for flashing
- Visual Studio Code with C/C++ extensions
