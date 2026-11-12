#pragma once
#include <stdbool.h>

typedef struct loom loom_t;

/**
 * @brief Create a new loom with a given size and expected width
 * @param instr_size the maximum amount of instructions in the loom
 * @param width the expected width of the rows produced by the loom
 * @return An empty loop with room for `instr_size` instructions, or NULL if `instr_size` or `width` is 0
 */
loom_t *loom_create(size_t mem_size, size_t width);

/**
 * @brief Destroy a loom
 * @param loom the loom to destroy
 */
void loom_destroy(loom_t *loom);

/**
 * @brief Add a new instruction to a loom
 * @param loom the loom to add an instruction to
 * @param instruction the instruction to add
 * @return true if the instruction could be added, false otherwise
 * @note An instruction can be added if it is well-formed and
 * produces a row of the expected width of the loom. The loom may
 * not retain a pointer to the passed instruction
 */
bool loom_add_instruction(loom_t *loom, char *instruction);

/**
 * @brief Weave a tapestry with the current instructions
 * @param loom the loom whose instructions should be weaved
 * @return a NULL-terminated array of strings gotten by weaving
 * the instructions of the loom in order
 */
char **loom_weave(loom_t *loom);