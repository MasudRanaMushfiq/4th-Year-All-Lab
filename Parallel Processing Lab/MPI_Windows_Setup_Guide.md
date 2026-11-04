# MPI C Programs on Windows

This folder is configured to compile and run MPI C programs on Windows using
Microsoft MPI and the 64-bit WinLibs MinGW-w64 compiler.

The source file `problem_1.c` was not modified. The Windows build configuration
is stored in `.vscode/tasks.json` and `.vscode/c_cpp_properties.json`.

## Installation commands used

The package manager was checked first:

```powershell
winget --version
```

The first runtime command was attempted exactly as follows:

```powershell
winget install --id=Microsoft.msmpi -e --accept-source-agreements --accept-package-agreements
```

It initially timed out while contacting the Microsoft Store source. The working
commands explicitly selected the `winget` source:

```powershell
winget install --id=Microsoft.msmpi -e --source winget --accept-source-agreements --accept-package-agreements
winget install --id=Microsoft.msmpisdk -e --source winget --accept-source-agreements --accept-package-agreements
winget install --id=BrechtSanders.WinLibs.POSIX.UCRT -e --source winget --accept-source-agreements --accept-package-agreements
```

The first two commands install Microsoft MPI. The third installs the 64-bit
WinLibs MinGW-w64 compiler because the original GCC installation was 32-bit.

## Download, storage, and hardware requirements

Approximate package download information:

| Component | Approximate download | Installed content |
|---|---:|---|
| MS-MPI runtime | 7.8 MB | `mpiexec.exe` and `msmpi.dll` |
| MS-MPI SDK | 2.3 MB | `mpi.h` and MPI libraries |
| WinLibs x64 archive | large portable ZIP; reserve about 500 MB or more | GCC, MinGW-w64, linker, GDB, Make, and related tools |

The first two sizes are based on the installer metadata. The WinLibs package
is a compressed archive and its exact download size can change between
releases, so reserve at least 500 MB of free download/storage capacity for it
and its extracted files. Additional temporary space may be needed while
`winget` downloads and extracts the archive.

This setup does not require a special GPU or server. A normal 64-bit Windows
10/11 laptop with at least 4 GB RAM and approximately 1 GB of free disk space
is sufficient for these small MPI programs. More free disk space is useful for
the compiler archive, temporary extraction files, and future projects.

## Checks performed after installation

Check the MPI launcher:

```powershell
mpiexec -help
```

Expected output starts with:

```text
Microsoft MPI Startup Program [Version 10.1.12498.52]
```

Check the compiler and confirm it is 64-bit:

```powershell
gcc --version
gcc -dumpmachine
```

Expected target:

```text
x86_64-w64-mingw32
```

Check the installed SDK files:

```powershell
Test-Path "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"
Test-Path "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64\msmpi.lib"
```

Both commands should print:

```text
True
```

Check the runtime PATH entry:

```powershell
where.exe mpiexec
```

It should locate:

```text
C:\Program Files\Microsoft MPI\Bin\mpiexec.exe
```

After installation or PATH changes, close and reopen CMD, Git Bash, or VS Code.
A reboot is normally not required.

## Installed software

### Microsoft MPI runtime

Package:

```text
Microsoft.msmpi
```

Executable:

```text
C:\Program Files\Microsoft MPI\Bin\mpiexec.exe
```

### Microsoft MPI SDK

Package:

```text
Microsoft.msmpisdk
```

MPI include path:

```text
C:\Program Files (x86)\Microsoft SDKs\MPI\Include
```

MPI 64-bit library path:

```text
C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64
```

The SDK provides `mpi.h` and `msmpi.lib`.

### C compiler

Compiler:

```text
WinLibs MinGW-w64 GCC 16.2.0
```

The compiler target must be 64-bit:

```text
x86_64-w64-mingw32
```

The WinLibs `mingw64\bin` folder is added to the user PATH.

## Why the 64-bit compiler is required

The original compiler was:

```text
MinGW.org GCC-6.3.0
mingw32
```

It is a 32-bit compiler. Linking it with the MS-MPI x64 library caused errors
such as:

```text
undefined reference to `MPI_Init@8'
undefined reference to `MPI_Comm_rank@8'
undefined reference to `MPI_Finalize@0'
```

The problem was solved by installing WinLibs MinGW-w64 and ensuring that its
`mingw64\bin` directory appears before `C:\MinGW\bin` in PATH.

Check the active compiler with:

```bash
gcc --version
gcc -dumpmachine
```

The second command must print:

```text
x86_64-w64-mingw32
```

## Build command

Open a terminal in this folder:

```text
C:\Users\User\Downloads\All Lab\Parallel Processing Lab
```

Compile `problem_1.c` with:

```bash
gcc problem_1.c -o problem_1.exe -DMSMPI_NO_SAL \
-I"C:/Program Files (x86)/Microsoft SDKs/MPI/Include" \
-L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64" \
-lmsmpi
```

### Why `-DMSMPI_NO_SAL` is present

MS-MPI's `mpi.h` includes Microsoft's `sal.h` by default. MinGW does not
provide that Microsoft annotation header. Defining `MSMPI_NO_SAL` disables
those optional annotations while retaining the MPI API declarations.

## Run command

After a successful build, run four MPI processes:

```bash
mpiexec -n 4 ./problem_1.exe
```

If `mpiexec` is not on PATH, use its full path:

```bash
"/c/Program Files/Microsoft MPI/Bin/mpiexec.exe" -n 4 ./problem_1.exe
```

The expected output includes:

```text
P0: Right boundary sent = 30
P1: Left boundary received = 30
P1: Right boundary sent = 60
P2: Left boundary received = 60
P2: Right boundary sent = 90
P3: Left boundary received = 90
Boundary exchange completed successfully.
```

## Git Bash instructions

Use Unix-style paths in Git Bash:

```bash
cd "/c/Users/User/Downloads/All Lab/Parallel Processing Lab"
```

If Git Bash selects the old compiler, activate the installed x64 compiler for
the current terminal:

```bash
export WINLIBS="/c/Users/User/AppData/Local/Microsoft/WinGet/Packages/BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe/mingw64/bin"
export PATH="$WINLIBS:/c/Program Files/Microsoft MPI/Bin:$PATH"
hash -r
```

Then verify and build:

```bash
which gcc
gcc -dumpmachine
gcc problem_1.c -o problem_1.exe -DMSMPI_NO_SAL \
-I"C:/Program Files (x86)/Microsoft SDKs/MPI/Include" \
-L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64" \
-lmsmpi
```

Run:

```bash
"/c/Program Files/Microsoft MPI/Bin/mpiexec.exe" -n 4 ./problem_1.exe
```

Do not use `cd /d` in Git Bash. That is CMD syntax and produces:

```text
bash: cd: too many arguments
```

## Windows CMD instructions

In CMD, use:

```cmd
cd /d "C:\Users\User\Downloads\All Lab\Parallel Processing Lab"
"C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 4 problem_1.exe
```

The `/d` option is valid in CMD, but not in Git Bash.

## VS Code instructions

1. Reopen VS Code after installing or changing PATH.
2. Open `problem_1.c`.
3. Press **Ctrl+Shift+B**.
4. Run the default **Build MPI C program** task.
5. Run the generated executable from a terminal:

```bash
mpiexec -n 4 ./problem_1.exe
```

The build task uses:

```text
${file}
${fileDirname}\${fileBasenameNoExtension}.exe
```

Therefore, opening another MPI source file and pressing **Ctrl+Shift+B**
creates an executable with the matching filename.

## Problems encountered and solutions

### 1. `mpiexec` was not recognized

Error:

```text
mpiexec : The term 'mpiexec' is not recognized
```

Solution: add this folder to the user PATH:

```text
C:\Program Files\Microsoft MPI\Bin
```

Then reopen the terminal or VS Code.

### 2. `winget` timed out

The first runtime installation attempt contacted the Microsoft Store source
and failed with:

```text
WinHttpSendRequest: 12002
0x80072ee2
```

Solution: explicitly select the working `winget` source:

```powershell
winget install --id=Microsoft.msmpi -e --source winget --accept-source-agreements --accept-package-agreements
winget install --id=Microsoft.msmpisdk -e --source winget --accept-source-agreements --accept-package-agreements
```

### 3. `sal.h` was missing

Error:

```text
fatal error: sal.h: No such file or directory
```

Solution: add this compiler definition:

```text
-DMSMPI_NO_SAL
```

### 4. MPI symbols were undefined during linking

Errors included:

```text
undefined reference to `MPI_Init@8'
undefined reference to `MPI_Send@24'
undefined reference to `MPI_Barrier@4'
```

Cause: the old `mingw32` compiler was 32-bit while the selected MS-MPI
library was x64.

Solution: install and select WinLibs MinGW-w64, then confirm:

```text
x86_64-w64-mingw32
```

### 5. The executable was not found by `mpiexec`

Error:

```text
Error (2) The system cannot find the file specified.
```

Cause: compilation had failed, so `problem_1.exe` did not exist.

Solution: fix the compiler/linker error, build successfully, and confirm the
executable exists before running:

```bash
ls -l problem_1.exe
```

### 6. `msmpi.dll` is not found

If a future run reports that `msmpi.dll` is missing, add the runtime folder to
PATH for the current terminal:

Git Bash:

```bash
export PATH="/c/Program Files/Microsoft MPI/Bin:$PATH"
```

CMD:

```cmd
set "PATH=C:\Program Files\Microsoft MPI\Bin;%PATH%"
```

Then retry `mpiexec`.

### 7. PATH changes are not visible

Existing terminals keep their old environment. Close and reopen Git Bash,
CMD, or VS Code after installing software or changing PATH. A system reboot is
normally not required.

## General recipe for another MPI problem

For `problem_2.c`, build:

```bash
gcc problem_2.c -o problem_2.exe -DMSMPI_NO_SAL \
-I"C:/Program Files (x86)/Microsoft SDKs/MPI/Include" \
-L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64" \
-lmsmpi
```
### Or use 
```
"/c/Users/User/AppData/Local/Microsoft/WinGet/Packages/BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe/mingw64/bin/gcc.exe" \
problem_2.c -o problem_2.exe -DMSMPI_NO_SAL \
-I"C:/Program Files (x86)/Microsoft SDKs/MPI/Include" \
-L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64" \
-lmsmpi
```

Then run:

```bash
mpiexec -n 4 ./problem_2.exe
```

Change `4` to the process count required by that program.

## Important note about `problem_1.c`

The program performs MPI scattering and boundary exchange. Although its
comments mention computing a local result, the current source does not compute
or print a separate local result. It was intentionally left unchanged because
this setup only configures the Windows build and run environment.

## Linux verification and run commands

On a Linux laptop, check that the MPI compiler and launcher are installed:

```bash
which mpicc
mpicc --version
which mpirun
mpirun --version
```

Test that MPI can start multiple processes:

```bash
mpirun -np 2 hostname
```

The hostname should be printed twice. If these commands are not found on
Ubuntu or Debian, install MPI with:

```bash
sudo apt update
sudo apt install build-essential openmpi-bin libopenmpi-dev
```

Compile an MPI C file from its folder:

```bash
mpicc problem_4.c -o problem_4
```

Run the compiled file with the required number of processes:

```bash
mpirun -np 2 ./problem_4
```

For a file that uses the math library, compile with `-lm`:

```bash
mpicc problem_15.c -lm -o problem_15
mpirun -np 4 ./problem_15
```



