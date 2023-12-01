#pragma once

// Environment
extern unsigned int PIXEL_WIDTH;
extern unsigned int PIXEL_HEIGHT;
extern unsigned int GLOBAL_SEED;

/**
 * @brief Sets the global seed for random number generation.
 *
 * This function sets the global seed for random number generation. The global seed is used
 * as the initial value for the random number generator, ensuring that the sequence of random
 * numbers generated is deterministic.
 *
 * @param globalSeed The global seed value to set.
 */
void setGlobalSeed(unsigned int globalSeed);

/**
 * @brief Set the Scr Height object
 * 
 * @param scrHeight 
 */
void setScrHeight(unsigned int scrHeight);

/**
 * @brief Set the Scr Width object
 * 
 * @param scrWidth 
 */
void setScrWidth(unsigned int scrWidth);
