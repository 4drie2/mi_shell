# Minishell - Our Shell ☭

Code written with sweat, tears, and a bit of love (okay, maybe not love, but definitely effort).

A custom Unix shell implementation developed in C as part of the 42 school curriculum.

![Minishell Logo](https://s1.qwant.com/thumbr/474x315/3/d/3d8ed8c83719322da94e15b8721ab037dd577b95916cc4812af39f6459e659/th.jpg?u=https%3A%2F%2Ftse.mm.bing.net%2Fth%3Fid%3DOIP.vBzv5fn5KZA9CFWF7P3feAHaE7%26pid%3DApi&q=0&b=1&p=0&a=0)

## Features

This shell implements several features found in traditional Unix shells:

- Command execution with arguments
- Built-in commands: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`
- Environment variable management
- Input/output redirections (`<`, `>`, `>>`)
- Pipes (`|`) for command chaining
- Heredocs (`<<`) for multi-line input
- Signal handling (Ctrl+C, Ctrl+D)
- Quote handling (single and double quotes)
- Error management with appropriate exit codes

## Architecture

The project is organized into several components:

```
minishell/
├── parsing/       # Command line parsing and heredoc handling
├── exec/          # Command execution and built-ins
│   └── builtins/  # Implementation of built-in commands
└── utils/         # Helper functions and utilities
```

## Implementation Details

- **Parsing**: Tokenizes input, handles quotes, and builds a command structure
- **Execution**: Handles command execution, path resolution, and pipe creation
- **Signal Handling**: Custom signal handlers for interactive shell experience
- **Memory Management**: Careful allocation and freeing of resources
- **Error Handling**: Proper error messages and exit codes

## Building and Running

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git

# Build the project
cd minishell
make

# Run the shell
./minishell
```

## Development Team

- [abidaux](https://github.com/abidaux) - Core parsing and heredoc implementation
- [pthuilli](https://github.com/Nlkuser) - Built-ins and excec handling


## Challenges & Reflections (but boring)

While Minishell might seem like a straightforward project at first glance, its true challenge lies in the tedious and extensive implementation required. The project is quite lengthy and demands meticulous attention to detail across multiple components.

What makes this project particularly challenging is not necessarily complex algorithms, but rather:
- The sheer volume of different edge cases to handle
- Ensuring proper memory management across the entire program
- Coordinating different parts of the shell to work seamlessly together

This project taught us valuable lessons in task distribution, project organization, and collaborative coding. We learned to divide responsibilities effectively, maintain consistent coding standards across team members, and integrate various components into a cohesive final product.

Unlike more algorithmically challenging projects, Minishell tests your endurance, patience, and ability to manage a large codebase with many interconnected parts.


## Learning Outcomes

Through this project, we gained a deep understanding of:
- Shell behavior and command interpretation
- Process creation and management
- File descriptor manipulation
- Signal handling in Unix environments
- Complex memory management
