# Minishell

Minishell is a 42 school team project aimed at creating a basic shell program in C. It supports redirections, pipes, environment variable expansion, and several built-in commands: cd, echo, env, exit, export, pwd, and unset.

## 📥 Installation & Usage

### Clone the repository

git clone <repository-url>

### Compilation

cd minishell && make

### Running Minishell

./minishell

Once launched, a prompt will appear where you can enter commands to be executed.

##⚡Features

Minishell is a lightweight shell inspired by Bash, offering the following functionalities:

### ✅ General Features

- Custom prompt display

- Command history navigation using ↑ and ↓

- Execution of system commands (ls, cat, grep, etc.)

- Execution of local binaries (./minishell)

- Built-in Commands

  - echo (supports -n flag)

  - cd (accepts only relative or absolute paths)

  - pwd (prints working directory)

  - export (adds environment variables)

  - unset (removes environment variables)

  - env (displays environment variables)

  - exit (exits shell, optionally with an exit statua)

 - Pipes (|): Redirect output from one command to another

- 🔀 Redirections:

  - '>' Redirects output (overwrite mode)

  - '>>' Redirects output (append mode)

  - '<' Redirects input

  - '<<' Heredoc: Reads user input until DELIMITER is reached

- 🌎 Environment Variable Expansion

   - Expands environment variables (e.g., $USER, $VAR)

  - $? expands to the exit status of the last executed command

- ⌨️ Keyboard Shortcuts

  - Ctrl + C → Displays a new prompt line

  - Ctrl + D → Exits Minishell

- ❌ Limitations

   - Minishell does not support: \ ; && || or wildcards (*)
