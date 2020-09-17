/*
 * Copyright 2020 Joerg Bartnick: 
 * Based on the Brainfuck interpreter by
 * 
 * Copyright 2016 Fabian Mastenbroek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef HTML_H
#define HTML_H

#define HTML_TAPE_SIZE 30000
/* 1: EOF leaves cell unchanged; 0: EOF == 0; 1: EOF ==  1 */
#define HTML_EOF_BEHAVIOR 1

#define HTML_TOKEN_PLUS 't'
#define HTML_TOKEN_MINUS 'm'
#define HTML_TOKEN_PREVIOUS 'H'
#define HTML_TOKEN_NEXT 'L'
#define HTML_TOKEN_OUTPUT 'T'
#define HTML_TOKEN_INPUT 'M'
#define HTML_TOKEN_LOOP_START 'h'
#define HTML_TOKEN_LOOP_END 'l'
#ifdef HTML_EXTENSION_DEBUG
#define HTML_TOKEN_BREAK '#'
#else
#define HTML_TOKEN_BREAK -10
#endif

#define READLINE_HIST_SIZE 20

/**
 * Represents a html instruction.
 */
typedef struct HtmlInstruction
{
    /**
	 * The difference between the value of the byte at the currect pointer and
	 *   the value we want.
	 */
    int difference;
    /**
	 * The type of this instruction.
	 */
    char type;
    /**
	 * The next instruction in the linked list.
	 */
    struct HtmlInstruction *next;
    /**
	 * The previous instruction in the linked list.
	 */
    struct HtmlInstruction *previous;
    /**
	 * The first instruction of a loop if this instruction is a loop. Otherwise
	 * 	<code>NULL</code>
	 */
    struct HtmlInstruction *loop;
} HtmlInstruction;

/**
 * The state structure contains the head and the root of the linked list containing
 * 	the instructions of the program.
 */
typedef struct HtmlState
{
    /**
	 * The root instruction of the instruction linked list.
	 */
    struct HtmlInstruction *root;
    /**
	 * The head instruction of the instruction linked list.
	 */
    struct HtmlInstruction *head;
} HtmlState;

/**
 * The callback that will be invoked when the HTML_TOKEN_OUTPUT token is found.
 * 
 * @param chr The value of the current cell.
 */
typedef int (*HtmlOutputHandler)(int chr);

/**
 * The callback that will be invoked when the HTML_TOKEN_INPUT token is found.
 * 
 * @return The character that is read.
 */
typedef char (*HtmlInputHandler)(void);

/**
 * This structure is used as a layer between a html program and
 * 	the outside. It allows control over input, output and memory.
 */
typedef struct HtmlExecutionContext
{
    /**
	 * The callback that will be invoked when the HTML_TOKEN_OUTPUT token is found.
	 */
    HtmlOutputHandler output_handler;
    /**
	 * The callback that will be invoked when the HTML_TOKEN_INPUT token is found.
	 */
    HtmlInputHandler input_handler;
    /**
	 * An array containing the memory cells the program can use.
	 */
    unsigned char *tape;
    /**
	 * Index into <code>tape</code>. Modified during execution.
	 */
    int tape_index;
    /**
	 * size of the tape in number of cells.
	 */
    size_t tape_size;
    /**
	 * A flag that, if set to true, indicates that execution should stop.
	 */
    int shouldStop;
} HtmlExecutionContext;

/**
 * Creates a new state.
 */
HtmlState *html_state();

/**
 * Creates a new context.
 *
 * @param size The size of the tape.
 */
HtmlExecutionContext *html_context(int);

/**
 * Removes the given instruction from the linked list.
 * 
 * @param state The state
 * @param instruction The instruction to remove.
 * @return The instruction that is removed.
 */
HtmlInstruction *html_remove(struct HtmlState *, struct HtmlInstruction *);

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
HtmlInstruction *html_add(struct HtmlState *state, struct HtmlInstruction *);

/**
 * Adds an instruction to the front of the instruction list.
 *
 * @param state The state.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
HtmlInstruction *html_add_first(struct HtmlState *state, struct HtmlInstruction *);

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param before The instruction you want to add another instruction before.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
HtmlInstruction *html_insert_before(struct HtmlState *, struct HtmlInstruction *,
                                    struct HtmlInstruction *);

/**
 * Adds an instruction to the instruction list.
 *
 * @param state The state.
 * @param after The instruction you want to add another instruction after.
 * @param instruction The instruction to add.
 * @return The instruction that is given.
 */
HtmlInstruction *html_insert_after(struct HtmlState *, struct HtmlInstruction *,
                                   struct HtmlInstruction *);

/**
 * Reads a character, converts it to an instruction and repeats until the EOF character
 * 	occurs and will then return a linked list containing all instructions.
 *
 * @param stream The stream to read from.
 * @param The head of the linked list containing the instructions.
 */
HtmlInstruction *html_parse_stream(FILE *);

/**
 * Reads a character, converts it to an instruction and repeats until the given character
 * 	occurs and will then return a linked list containing all instructions.
 *
 * @param stream The stream to read from.
 * @param until If this character is found in the stream, we will quit reading and return.
 * @param The head of the linked list containing the instructions.
 */
HtmlInstruction *html_parse_stream_until(FILE *, int);

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 *
 * @param str The string to read from.
 * @param The head of the linked list containing the instructions.
 */
HtmlInstruction *html_parse_string(char *);

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 *
 * @param str The string to read from.
 * @param begin The index you want to start parsing at.
 * @param end The index you want to stop parsing at.
 *	When <code>-1</code> is given, it will stop at the end of the string.
 * @param The head of the linked list containing the instructions.
 */
HtmlInstruction *html_parse_substring(char *, int, int);

/**
 * Reads a character, converts it to an instruction and repeats until the string ends
 *	and will then return a linked list containing all instructions.
 * This method is special because it uses the begin index as counter, so this variable
 * 	will increase.
 *
 * @param str The string to read from.
 * @param ptr The pointer to the integer holding the index you want to start parsing at.
 *	Since this will be used as counter, the value of the pointer will be increased.
 * @param end The index you want to stop parsing at.
 *	When <code>-1</code> is given, it will stop at the end of the string.
 * @param The head of the linked list containing the instructions.
 */
HtmlInstruction *html_parse_substring_incremental(char *, int *, int);

/**
 * Converts the given character to an instruction.
 *
 * @param c The character to convert.
 * @param The character that's converted into an instruction.
 */
HtmlInstruction *html_parse_character(char);

/**
 * Destroys the given instruction.
 * 
 * @param instruction The instruction to destroy.
 */
void html_destroy_instruction(struct HtmlInstruction *);

/**
 * Destroys a linked list containing instructions.
 * 
 * @param head The start of the instruction list.
 */
void html_destroy_instructions(struct HtmlInstruction *);

/**
 * Destroys a state.
 * 
 * @param state The state to destroy
 */
void html_destroy_state(struct HtmlState *);

/**
 * Destroys a context.
 * 
 * @param context The context to destroy
 */
void html_destroy_context(struct HtmlExecutionContext *);

/**
 * Executes the given linked list containing instructions.
 *
 * @param root The start of the linked list of instructions you want
 * 	to execute.
 * @param context The context of this execution that contains the tape and
 *	other execution related variables.
 */
void html_execute(struct HtmlInstruction *, struct HtmlExecutionContext *);

/**
 * Stops the currently running program referenced by the given execution context.
 *
 * @param context The context of this execution that contains the tape and
 *	other execution related variables.
 */
void html_execution_stop(HtmlExecutionContext *);

/**
 * Reads exactly one char from stdin.
 * @return The character read from stdin. 
 */
char html_getchar(void);

#endif /* HTML_H */
