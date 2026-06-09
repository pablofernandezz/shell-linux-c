# Custom Linux Shell in C 🐚

> A minimalist, interactive command-line interpreter (shell) built from scratch in C for Unix/Linux environments.

## 📖 About The Project

This project is a custom interactive shell designed to replicate the core functionalities of a Unix command-line interface. Developed as an academic project focused on **Operating Systems and Concurrency**, it explores low-level system communication, process management, and memory handling within Linux.

Instead of relying on high-level wrappers, this shell interacts directly with the Linux kernel using standard POSIX system calls to manage processes and handle concurrent execution.

## ✨ Key Features

* **Command Parsing:** Robust tokenization of user inputs into executable commands and arguments.
* **Process Management:** Execution of standard system programs by creating child processes (`fork`) and replacing the process image (`exec`).
* **Concurrency Control:** Support for foreground and background task execution, managing execution flows using process synchronization (`wait`/`waitpid`).
* **Built-in Commands:** Native implementation of essential shell built-ins such as `cd` (change directory), `help`, and `exit`.
* **System Call Integration:** Direct usage of Linux API functions for low-level system and resource control.

## 🛠️ Tech Stack

* **Language:** C (C99/C11 standard)
* **Environment:** Unix/Linux
* **Core APIs:** POSIX / Linux System Calls (`fork`, `execvp`, `waitpid`, `chdir`, etc.)

## 🚀 Getting Started

To compile and run the custom shell on your Linux machine, follow these steps.

### Prerequisites

* A Linux environment (Ubuntu, Debian, etc.)
* GCC compiler and GNU Make toolchain installed.

You can install the build essentials on Ubuntu/Debian using:
```bash
sudo apt update && sudo apt install build-essential
```
## Installation & Compilation
Clone the repository:
```bash
git clone https://github.com/pablofernandezz/shell-linux-c.git
```
Navigate to the project directory:
```bash
cd shell-linux-c
```
Compile the source code using gcc:
```bash
gcc -Wall -Wextra -O2 main.c -o custom_shell
(Note: you can simply run make instead).
```

## 💻 Usage
Launch the shell by executing the compiled binary:

```bash
./custom_shell
```
Once inside, you will see the custom prompt. You can execute standard Linux commands or use the built-in functions:

```Bash
ls -la
cd /var/log
pwd
help
exit
```

## 👨‍💻 Author
Pablo Fernández Martí
LinkedIn: www.linkedin.com/in/pablo-fernandez-marti-526320415
GitHub: @pablofernandezz
This project was developed as part of the Computer Engineering degree curriculum at Universidade da Coruña (UDC).
