# Minishell [42 Project]
This 42 project challenges us to recreate a command-line interpreter for Linux

## Table of Contents
1. [Overview](#overview)
2. [Project Structure](#project-structure)
3. [Dependencies](#dependencies)
4. [How to Use](#how-to-use)
5. [Code Examples](#code-examples)
6. [Contributing](#contributing)

## Overview
Minishell is a command-line interface that parses and executes prompts like any other shell.

### The Rules
- Not interpret unclosed quotes or special characters which are not required by the
subject such as `\` (backslash) or `;` (semicolon).
- Handle `’` (single quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence.
- Handle `"` (double quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence except for `$` (dollar sign).
- Implement Redirections
    - `<` redirect input
    - `>` redirect output
    - `<<` redirect input from a Here Document
    - `>>` redirect output in append mode
- Implement Pipes `|` and Pipelines
- Handle enviroment variables and expand their values
- Handle the exit status variable `$?`
- Handle `ctrl-D`, which closes `STDIN_FILENO`
- Handle signals
    - `ctrl-C`
    - `ctrl-\`
- Implement the following builtins
    - `echo` (with the option `-n`)
    - `cd` (with relative or absolute path)
    - `pwd` (with no options)
    - `export` (with no options)
    - `unset` (with no options)
    - `env` (with no options or arguments)
    - `exit` (with no options)

## Project Structure
- `minishell/`: Main directory containing the source code, header files, and Makefile.
    - `src/`: Contains all the source code
    - `include/`: Contains header files
    - `libft/` : Contains libft library
    - `Makefile`: Build automation file

## How it works
Our shell works in four steps: `lexer`>`parser`>`expander`>`execution`

### Lexer
The lexer is the first step our shell takes to parse the input

It checks for unclosed quotes and tokenizes the input

Input Example:
`ls -a | echo Hello > file`

After the tokenization, this input would look like this

format: (string, type of token)

- `ls` word
- `-a` word
- `|` pipe
- `echo` word
- `Hello` word
- `>` redir
- `file` word

After the tokenizer we can advance to the second step

### Parser and Expander
The parser is responsible for checking the syntax errors the input might have and creating an ast (abstract syntax tree)

To make all of this, we first need to set up some priorities and rules. These is set up by the grammar of the language we working on.
Since we are making a shell, we can take the [bash grammar](https://cmdse.github.io/pages/appendix/bash-grammar.html) as a reference.

Here's the most important rules(adapted to the project):
- `<SIMPLE-COMMAND-ELEMENT> ::= <WORD>
                              |  <REDIRECTION>`
- `<COMMAND> ::=  <SIMPLE-COMMAND>`
- `<REDIRECTION> ::= '<' <WORD>
                    | '>' <WORD>
                    | '<<' <WORD>
                    | '>>' <WORD>`
- `<PIPELINE> ::= <PIPELINE>
              '|' <PIPELINE>
              |  <COMMAND>`

We first build a `<SIMPLE-COMMAND-ELEMENT>`, then we check if the command has any tokens of type redir. If it has a redir type token then we build a `<REDIRECTION>`.
Also any enviroment variable is expanded in this moment.

After the `<SIMPLE-COMMAND-ELEMENT>` we check for a pipe type token. If it has a pipe token we call again our parser with the `<SIMPLE-COMMAND-ELEMENT>` built going to the rigth and the next `<SIMPLE-COMMAND-ELEMENT>` going to the left.

### Execution
The execution it's the part where the input is gonna be executted.

With our ast built it's now a matter of transversing the tree and executing the commands

## Dependencies
- Libft (aleready included)

## How to Use
1. Clone the repository.
2. Navigate to the `minishell` directory: `cd minishell`.
3. Compile the program: `make`.
4. Run the `minishell` binary: `./minishell`
5. Now you can use `minishell` as your command-line interpreter

## Code Examples
```bash
./minishell
minishell$ echo Hello Minishell
Hello Minishell
minishell$
```

## Contributing
I welcome contributions. If you have any bug fix or improvement, feel free to submit a pull request on the repository.
