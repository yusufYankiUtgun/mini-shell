# kell 👨🏻‍🦲

A minimal Unix shell written in C.

## Features

- Pipes (`|`) — chain multiple commands
- Input/Output redirection (`<`, `>`)
- Append redirection (`>>`)
- Built-in commands
- Signal handling

## Requirements

- Linux
- GCC

## Usage

```
gcc *.c -o kell 
./kell
```

## Examples

```sh
ls | grep .c
echo hello | cat
cat < input.txt
echo hello > output.txt
echo world >> output.txt
```

## Project Structure

| File                                                            | Description                            |
| --------------------------------------------------------------- | -------------------------------------- |
| `kell.c` / `kell.h`                                             | Main shell loop                        |
| `lexer.c` / `lexer.h`                                           | Tokenizes user input                   |
| `parser.c` / `parser.h`                                         | Builds command linked list from tokens |
| `execute_pipeline.c` / `execute_pipeline.h`                     | Handles pipeline execution             |
| `execute_redirected_command.c` / `execute_redirected_command.h` | Handles I/O redirection                |
| `handle_builtins.c` / `handle_builtins.h`                       | Built-in command handling              |
| `get_input.c` / `get_input.h`                                   | Reads user input                       |
| `resize.c` / `resize.h`                                         | Dynamic array resizing helper          |
