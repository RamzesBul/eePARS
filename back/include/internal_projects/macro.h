#ifndef IPEE_MACRO_H
#define IPEE_MACRO_H

/**
 * @brief Stringify parameter.
 * 
 * @details
 * Converts parameter to string.
 */
#define stringify(param) #param

/**
 * @brief Return stringified name.
 */
#define name_of(param) stringify(param)

#endif // IPEE_MACRO_H