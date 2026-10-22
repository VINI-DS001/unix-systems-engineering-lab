# POSIX Mini Shell (Module 1)

A C17, POSIX-compliant minimalist Unix shell implementation developed as part of a Systems Engineering Lab project. This project demonstrates core operating system concepts, including process lifecycle management, inter-process communication (IPC) via pipelines, file I/O redirection, and asynchronous signal handling.

---

## Technical Overview & Architecture

The shell operates on the standard **Read-Eval-Print Loop (REPL)** pattern:

```text
[ Input (getline) ] ---> [ Parser (Tokenizer) ] ---> [ Built-in Handler ]
                                                           |
                                                   (If External Binary)
                                                           v
                                                  [ Process Pipeline ]
                                                 (fork / execvp / pipe)
```

1. **Read (REPL Loop):** Captures user input interactively via `getline()` while validating standard input connection via `isatty()`.
2. **Parsing & Tokenization:** Splits command strings by pipeline boundaries (`|`), extracts I/O redirection tokens (`<`, `>`, `>>`), and detects background execution (`&`).
3. **Execution Dispatch:**
   - **Built-ins (`cd`, `pwd`, `exit`):** Executed directly in the parent process to alter or inspect shell state.
   - **External Binaries:** Executed in dedicated child processes using `fork()`, `execvp()`, `dup2()`, and `pipe()`.

---

## Key POSIX System Calls & API Usage

| System Call / API | Module / Context | Purpose |
| :--- | :--- | :--- |
| `fork()` | Process Management | Duplicates the current shell process to spawn child command tasks. |
| `execvp()` | Process Execution | Replaces the child process image with the target executable, searching PATH. |
| `waitpid()` / `wait()` | Process Management | Reaps child process exit statuses in foreground/background execution. |
| `pipe()` | Inter-Process Communication | Creates unidirectional data channels for multi-stage command pipelines (`\|`). |
| `open()`, `dup2()`, `close()` | I/O Redirection | Overrides standard input/output file descriptors for `<`, `>`, and `>>`. |
| `sigaction()`, `signal()` | Signal Handling | Configures async handlers for `SIGINT` (`Ctrl+C`) and `SIGCHLD` (zombie prevention). |
| `isatty()` | Shell Interface | Ensures interactive prompt (`minishell>`) is rendered only when connected to a TTY. |

---

## Repository & File Structure

```text
shell/
├── include/
│   ├── builtins.h    # Header for internal shell built-ins
│   ├── parser.h      # Definitions for command and pipeline structures
│   └── shell.h       # Main REPL loop and pipeline execution prototypes
├── src/
│   ├── builtins.c    # Implementation of cd, pwd, exit
│   ├── main.c        # Entry point invoking shell_loop()
│   ├── parser.c      # Tokenizer for pipes, redirections, and arguments
│   └── shell.c       # Core process management, IPC pipes, and signal handlers
├── Makefile          # Build system configuration
└── README.md         # Project documentation
```

---

## Compilation & Execution Instructions

### Prerequisites
- GCC Compiler with C17 standard support.
- POSIX-compliant environment (Linux / AlmaLinux / macOS).

### Build Commands

To compile the source code using standard compilation flags (`-Wall -Wextra -Werror -pedantic -std=c17`):

```bash
make
```

To run the interactive shell:

```bash
./minishell
```

To clean up object files and compiled binaries:

```bash
make clean
```

---

## Supported Features & Usage Examples

### 1. Built-in Commands
```bash
minishell> pwd
/home/user/systems-lab/shell

minishell> cd ..
minishell> pwd
/home/user/systems-lab

minishell> exit
```

### 2. External Binary Execution
```bash
minishell> ls -la
minishell> uname -a
minishell> whoami
```

### 3. File I/O Redirection
```bash
# Output redirection (Truncate)
minishell> ls -la > files.txt

# Input redirection
minishell> grep "src" < files.txt

# Output redirection (Append)
minishell> echo "New Entry" >> files.txt
```

### 4. Inter-Process Pipelines (`|`)
```bash
# Single pipe stage
minishell> ls -la | grep src

# Multi-stage pipeline with redirection
minishell> cat /etc/passwd | grep root | wc -l > output.txt
```

### 5. Asynchronous Background Execution (`&`)
```bash
minishell> sleep 10 &
[Background process started with PID 12345]
minishell> 
```

### 6. Signal Handling
- **`Ctrl+C` (`SIGINT`):** Interrupts running foreground processes without terminating the parent `minishell`. When idle, prints a clean new prompt line.
- **`SIGCHLD` Handling:** Asynchronously reaps background child processes to prevent process table accumulation (*zombies*).

---

## Technical Limitations & Scope Constraints

1. **Tokenization Simplification:** `strtok()` handles space/tab delimiters but does not yet evaluate quoted strings containing literal spaces (e.g., `grep "hello world"`).
2. **Environment Expansion:** Variables like `$PATH` or `$HOME` are expanded natively by `execvp()` or `getenv()`, but general shell variable expansion (`$VAR`) is not evaluated inside raw strings.
3. **No Complex Control Operators:** Chained conditional execution (`&&`, `||`) and subshell groupings `()` are out of scope in this Module 1.