# 4th-Year-All-Lab

This repository contains the practical work, assignments, reference material,
and previous examination questions for the fourth-year computer science
lab courses.

## Repository contents

| Area | Main topics | Documentation |
|---|---|---|
| [Artificial Intelligence Lab](./Artificial%20Intelligence%20Lab/) | Neural networks, regression, CNNs, transfer learning, image classification, and AI assignments | [AI Lab README](./Artificial%20Intelligence%20Lab/README.md) |
| [Blockchain Lab](./Blockchain%20Lab/) | Hashing, Bitcoin transactions, digital signatures, Merkle trees, RSA, and peer-to-peer socket examples | [Blockchain Lab README](./Blockchain%20Lab/README.md) |
| [Microcontroller Interfacing Lab](./Microcontroller%20Interfacing%20Lab/) | Bare-metal STM32F446RE communication using I2C, SPI, and UART | [Microcontroller Lab README](./Microcontroller%20Interfacing%20Lab/README.md) |
| [Object Oriented Design And Design Patterns](./Object%20Oriented%20Design%20And%20Design%20Patterns/) | Requirements analysis, UML, CRC cards, Java design patterns, and previous lab problems | [OOD Lab 2026 README](./Object%20Oriented%20Design%20And%20Design%20Patterns/Lab%202026/README.md) |
| [Parallel Processing Lab](./Parallel%20Processing%20Lab/) | C and C++ exercises for parallel programming and MPI setup | [MPI Windows Setup Guide](./Parallel%20Processing%20Lab/MPI_Windows_Setup_Guide.md) |
| [Previous Year Lab Questions (2024)](./Previous%20Year%20Lab%20Questions%20(2024)/) | 2024 question papers for all five laboratory courses | Image-based question papers |

## Course guide

### Artificial Intelligence Lab

The AI materials are primarily written in Python and TensorFlow/Keras. They
include:

- neural-network basics such as polynomial regression, DNNs, and MNIST CNNs;
- CIFAR-10 and leaf image classification;
- MobileNetV2 and VGG16 transfer-learning examples;
- assignment solutions for polynomial regression and MNIST classification;
- older practice work including A* search, the 8-puzzle, Pima diabetes
  classification, OpenCV processing, and FCNN/CNN comparisons; and
- 2026 lab scripts, question images, and solution references.

See the [AI Lab README](./Artificial%20Intelligence%20Lab/README.md) for
environment setup and Jupyter instructions.

### Blockchain Lab

The blockchain and cryptography materials include:

- basic hashing, SHA-256, MD5, and Merkle-tree examples;
- Bitcoin address generation and transaction construction;
- proof-of-work mining and a 51% attack demonstration;
- digital signatures and RSA signature verification;
- membership examples;
- shared-file Bitcoin transaction signing and verification; and
- socket-based bootnode, sender, and receiver examples.

See the [Blockchain Lab README](./Blockchain%20Lab/README.md) for Python
environment and package requirements.

### Microcontroller Interfacing Lab

This folder contains direct-register STM32F446RE NUCLEO examples for
communication between two boards:

- [I2C examples](./Microcontroller%20Interfacing%20Lab/I2C/) with master and
  slave programs;
- [SPI examples](./Microcontroller%20Interfacing%20Lab/SPI/) with master and
  slave programs;
- [UART examples](./Microcontroller%20Interfacing%20Lab/UART/) with master and
  slave programs;
- [I2C code explanation](./Microcontroller%20Interfacing%20Lab/I2C_Code_Explain.md);
- [important bit positions](./Microcontroller%20Interfacing%20Lab/Important_Bit_Position.md);
  and
- STM32F446RE register references in
  [Resources and Manual](./Microcontroller%20Interfacing%20Lab/Resources%20and%20Manual/).

### Object-Oriented Design and Design Patterns

The 2026 OOD lab contains three Java projects:

1. **Agricultural Automation System (AAS)** - farm monitoring, irrigation
   strategies, sensors, and notifications.
2. **Dhaka Urban Management System (DUMS)** - complaint management, urban
   services, route selection, and traffic integration.
3. **Smart Urban Community Management System (SUCMS)** - university accounts,
   registration, evaluation, notifications, and library integration.

The examples demonstrate Singleton, Factory, Observer, Strategy, and Adapter
patterns. The folder also contains experiment briefs, solution documents, lab
outline material, diagrams, and [previous OOD lab problems](./Object%20Oriented%20Design%20And%20Design%20Patterns/Previous%20Year%20Lab/).

Read the [OOD Lab 2026 README](./Object%20Oriented%20Design%20And%20Design%20Patterns/Lab%202026/README.md)
for the project structure and Java run commands.

### Parallel Processing Lab

The parallel-processing folder contains:

- C implementations of the current numbered lab problems;
- older C++ implementations in
  [parallel_processing_lab_old](./Parallel%20Processing%20Lab/parallel_processing_lab_old/);
- [the exam lab list](./Parallel%20Processing%20Lab/Lab%20List%20For%20Exam.txt);
- Visual Studio Code C/C++ configuration in
  [`.vscode`](./Parallel%20Processing%20Lab/.vscode/); and
- [MPI setup instructions for Windows](./Parallel%20Processing%20Lab/MPI_Windows_Setup_Guide.md).

### Previous Year Lab Questions

[Previous Year Lab Questions (2024)](./Previous%20Year%20Lab%20Questions%20(2024)/)
contains one question image for each course:

- Artificial Intelligence Lab
- Blockchain Lab
- Microcontroller Interfacing Lab
- Object Oriented Design and Design Patterns Lab
- Parallel Processing Lab

## File types used

The repository includes:

- **Python** - AI, blockchain, and cryptography exercises;
- **Jupyter notebooks** - interactive AI experiments and assignments;
- **C/C++** - microcontroller and parallel-processing programs;
- **Java** - object-oriented design and design-pattern demonstrations;
- **PDFs** - experiment briefs, lab outlines, solutions, and reference manuals;
- **JPEG/PNG images** - diagrams, lab questions, and supporting visuals; and
- **Markdown and text files** - setup guides, explanations, and lab lists.

## Getting started

1. Open the README for the lab you want to use.
2. Install only that lab's required tools and dependencies.
3. Follow the lab-specific setup and run instructions.
4. Keep generated build files, virtual environments, and downloaded datasets
   outside the repository when possible.

Each lab is independent; there is no single project-wide build command.
