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

### Parser
The parser is responsible for checking the syntax errors the input might have and creating an ast (abstract syntax tree)

To make all of this, we first need to set up some priorities and rules. These is set up by the grammar of the language we working on.
Since we are making a shell, we can take the [bash grammar](https://cmdse.github.io/pages/appendix/bash-grammar.html) as a reference.

By reading the grammar we already can see some rules

Here's the most important ones(adapted to the project):
- `<SIMPLE-COMMAND-ELEMENT> ::= <WORD>
                              |  <REDIRECTION>`
- `<REDIRECTION> ::= '<' <WORD>
                    | '>' <WORD>
                    | '<<' <WORD>
                    | '>>' <WORD>`
- `<COMMAND> ::=  <SIMPLE-COMMAND>`
- `<PIPELINE> ::= <PIPELINE>
              '|' <PIPELINE>
              |  <COMMAND>`

These rules must be strictly followed. Any misconfiguration will result in a `syntax error`

By looking at these 4 rules we can already see some rules depends on others to be correct.

Analizing it more carafully we can denote this behaviour

The `<PIPELINE>` is recursive until the last `<COMMAND>`

The `<COMMAND>` only requires a `<WORD>` but can have more things, like a `<REDIRECTION>`

The `<REDIRECTION>` only requires a `redir char` and a `<WORD>`

This already sets up a prioritie on how to construct our tree

This 2 rules, `<PIPELINE>` and `<REDIRECTION>` require a `<COMMAND>` to be syntactically correct, so the first thing our tree builds is the `<COMMAND>` rule.

The `<COMMAND>` rule will also check if it as to build a `<REDIRECTION>` rule or if it is only `<WORD>`

Done building the `<COMMAND>`, now we will need to check the input to see if it as build a `pipe`. If the response is yes, then we need to build a `<PIPELINE>`

To build a `<PIPELINE>` we will follow the grammar. We will put the built `<COMMAND>` to the left side of the `pipe` and call again the tree builder to build `<COMMAND>` until it's the last one

A thing to understand from here is the last rule will have the most priority and the first will have the least priority in the execution.

Now, with the parser done we can go for the next step

### Expander
The expander is responsible for expanding the values of the enviroment variables.



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
