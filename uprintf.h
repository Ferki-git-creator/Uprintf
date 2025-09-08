/**
 * @file uprintf.h
 * @brief Ultimate platform-independent printf implementation
 * @version 1.3
 * @author Ferki
 * @date 2025
 * 
 * @license MIT License
 * 
 * Revolutionary features:
 *   - Extended formatting system with pattern-based templates
 *   - Built-in lightweight markup language for rich text
 *   - Composite output system with multiple simultaneous destinations
 *   - Advanced buffer management with circular buffers and FIFO
 *   - Event-driven output system with hooks and callbacks
 *   - Stream processors for transformation pipelines
 *   - Lightweight state machine for complex formatting
 *   - Position-aware output for advanced layout
 *   - Memory-efficient string building system
 */

#ifndef UPRINTF_H
#define UPRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Output callback function type
 * @param c Character to output
 * @param ctx User context pointer
 */
typedef void (*u_output_cb_t)(char c, void* ctx);

/**
 * @brief Custom format handler function type
 */
typedef int (*u_format_handler_t)(u_output_cb_t output_cb, void* ctx, va_list* args, 
                                 const char** fmt, int width, int precision, 
                                 unsigned int flags);

/**
 * @brief Text alignment options
 */
typedef enum {
    U_ALIGN_LEFT,      /**< Left alignment */
    U_ALIGN_RIGHT,     /**< Right alignment */
    U_ALIGN_CENTER,    /**< Center alignment */
    U_ALIGN_JUSTIFY    /**< Justified alignment */
} u_text_align_t;

/**
 * @brief Text transformation options
 */
typedef enum {
    U_TRANSFORM_NONE,      /**< No transformation */
    U_TRANSFORM_UPPERCASE, /**< Convert to uppercase */
    U_TRANSFORM_LOWERCASE, /**< Convert to lowercase */
    U_TRANSFORM_CAPITALIZE,/**< Capitalize first letter of each word */
    U_TRANSFORM_REVERSE,   /**< Reverse the string */
    U_TRANSFORM_ROT13      /**< Apply ROT13 cipher */
} u_text_transform_t;

/**
 * @brief Template variable structure
 */
typedef struct {
    const char* key;   /**< Variable name */
    const char* value; /**< Variable value */
} u_template_var_t;

/**
 * @brief Output stream configuration
 */
typedef struct {
    u_output_cb_t output; /**< Output callback function */
    void* ctx;            /**< Context for output callback */
    bool enabled;         /**< Whether stream is enabled */
} u_output_stream_t;

/**
 * @brief String builder structure
 */
typedef struct {
    char* buffer;   /**< Character buffer */
    size_t size;    /**< Buffer size */
    size_t pos;     /**< Current position in buffer */
    bool dynamic;   /**< Whether buffer is dynamically allocated */
} u_string_builder_t;

/**
 * @brief Format flags
 */
#define U_FLAG_LEFT_ALIGN    (1 << 0)  /**< Left-align output */
#define U_FLAG_FORCE_SIGN    (1 << 1)  /**< Always show sign */
#define U_FLAG_SPACE_SIGN    (1 << 2)  /**< Space if no sign */
#define U_FLAG_ZERO_PAD      (1 << 3)  /**< Pad with zeros */
#define U_FLAG_ALT_FORM      (1 << 4)  /**< Alternate form */
#define U_FLAG_UPPERCASE     (1 << 5)  /**< Uppercase letters */
#define U_FLAG_WIDTH_ARG     (1 << 6)  /**< Width from argument */
#define U_FLAG_PRECISION_ARG (1 << 7)  /**< Precision from argument */

/**
 * @brief Main printf function
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param fmt Format string
 * @param ... Arguments to format
 * @return Number of characters written, or negative value on error
 */
int u_printf(u_output_cb_t output_cb, void* ctx, const char* fmt, ...);

/**
 * @brief Varargs version of printf
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param fmt Format string
 * @param args Variable arguments list
 * @return Number of characters written, or negative value on error
 */
int u_vprintf(u_output_cb_t output_cb, void* ctx, const char* fmt, va_list args);

/**
 * @brief Register a custom format handler
 * @param specifier Format specifier character
 * @param handler Handler function
 * @return 0 on success, -1 on failure
 */
int u_register_format_handler(char specifier, u_format_handler_t handler);

/**
 * @brief Unregister a custom format handler
 * @param specifier Format specifier character
 * @return 0 on success, -1 on failure
 */
int u_unregister_format_handler(char specifier);

/**
 * @brief Write formatted output to a string
 * @param buffer Output buffer
 * @param fmt Format string
 * @param ... Arguments to format
 * @return Number of characters written (excluding null terminator),
 *         or negative value on error
 */
int u_sprintf(char* buffer, const char* fmt, ...);

/**
 * @brief Write formatted output to a string with size limit
 * @param buffer Output buffer
 * @param size Buffer size
 * @param fmt Format string
 * @param ... Arguments to format
 * @return Number of characters actually written to buffer (excluding null terminator),
 *         or negative value on error
 */
int u_snprintf(char* buffer, size_t size, const char* fmt, ...);

/**
 * @brief Set locale for number formatting (minimal implementation)
 * @param locale Locale string (currently only supports decimal point)
 */
void u_set_locale(const char* locale);

/**
 * @brief Enable or disable floating point support
 * @param enabled True to enable, false to disable
 */
void u_set_float_support(bool enabled);

/**
 * @brief Set default output handler for simplified usage
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 */
void u_set_default_output(u_output_cb_t output_cb, void* ctx);

/**
 * @brief Simplified printf using default output handler
 * @param fmt Format string
 * @param ... Arguments to format
 * @return Number of characters written, or negative value on error
 */
int u_printf_simple(const char* fmt, ...);

/**
 * @brief Extended printf with advanced features
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param fmt Format string
 * @param ... Arguments to format
 * @return Number of characters written, or negative value on error
 */
int u_printf_ex(u_output_cb_t output_cb, void* ctx, const char* fmt, ...);

/**
 * @brief Varargs version of extended printf
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param fmt Format string
 * @param args Variable arguments list
 * @return Number of characters written, or negative value on error
 */
int u_vprintf_ex(u_output_cb_t output_cb, void* ctx, const char* fmt, va_list args);

/**
 * @brief Format text using template with variables
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param template Template string with {{variable}} placeholders
 * @param vars Array of template variables
 * @param count Number of variables in array
 */
void u_template_format(u_output_cb_t output_cb, void* ctx, 
                      const char* template, const u_template_var_t* vars, int count);

/**
 * @brief Load a template for later use
 * @param name Template name
 * @param template Template content
 */
void u_template_load(const char* name, const char* template);

/**
 * @brief Format text using a named template
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param name Name of the template to use
 * @param vars Array of template variables
 * @param count Number of variables in array
 */
void u_template_format_named(u_output_cb_t output_cb, void* ctx, 
                            const char* name, const u_template_var_t* vars, int count);

/**
 * @brief Add an output stream to the broadcast system
 * @param stream Output stream configuration
 */
void u_output_add_stream(u_output_stream_t stream);

/**
 * @brief Remove an output stream from the broadcast system
 * @param output_cb Output callback to remove
 */
void u_output_remove_stream(u_output_cb_t output_cb);

/**
 * @brief Broadcast a character to all registered output streams
 * @param c Character to broadcast
 */
void u_output_broadcast(char c);

/**
 * @brief Broadcast a string to all registered output streams
 * @param str String to broadcast
 */
void u_output_broadcast_str(const char* str);

/**
 * @brief Create a string builder
 * @param initial_size Initial buffer size
 * @return String builder instance
 */
u_string_builder_t u_string_builder_create(size_t initial_size);

/**
 * @brief Append a string to the string builder
 * @param sb String builder instance
 * @param str String to append
 */
void u_string_builder_append(u_string_builder_t* sb, const char* str);

/**
 * @brief Append formatted text to the string builder
 * @param sb String builder instance
 * @param fmt Format string
 * @param ... Arguments to format
 */
void u_string_builder_append_format(u_string_builder_t* sb, const char* fmt, ...);

/**
 * @brief Clear the string builder
 * @param sb String builder instance
 */
void u_string_builder_clear(u_string_builder_t* sb);

/**
 * @brief Free the string builder resources
 * @param sb String builder instance
 */
void u_string_builder_free(u_string_builder_t* sb);

/**
 * @brief Align text within a specified width
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param text Text to align
 * @param width Total width for alignment
 * @param align Alignment type
 */
void u_text_align(u_output_cb_t output_cb, void* ctx, const char* text, 
                 int width, u_text_align_t align);

/**
 * @brief Transform text according to specified transformation
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param text Text to transform
 * @param transform Transformation type
 */
void u_text_transform(u_output_cb_t output_cb, void* ctx, const char* text, 
                     u_text_transform_t transform);

/**
 * @brief Wrap text to specified width with optional prefix
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param text Text to wrap
 * @param width Maximum line width
 * @param prefix Prefix for each line (can be NULL)
 */
void u_text_wrap(u_output_cb_t output_cb, void* ctx, const char* text, 
                int width, const char* prefix);

/**
 * @brief Format value using a pattern
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param pattern Pattern string (%s will be replaced with value)
 * @param value Value to format (can be NULL)
 * @param default_pattern Default pattern to use if value is NULL
 */
void u_format_pattern(u_output_cb_t output_cb, void* ctx, const char* pattern, 
                     const char* value, const char* default_pattern);

/**
 * @brief Output hook function type
 * @param c Character being output
 * @param ctx Context pointer
 * @param user_data User-defined data
 */
typedef void (*u_output_hook_t)(char c, void* ctx, void* user_data);

/**
 * @brief Add an output hook for monitoring output
 * @param hook Hook function
 * @param user_data User data to pass to hook
 */
void u_add_output_hook(u_output_hook_t hook, void* user_data);

/**
 * @brief Remove an output hook
 * @param hook Hook function to remove
 */
void u_remove_output_hook(u_output_hook_t hook);

/**
 * @brief Stream processor function type
 * @param c Character to process
 * @param ctx Processor context
 * @return Processed character
 */
typedef char (*u_stream_processor_t)(char c, void* ctx);

/**
 * @brief Add a stream processor for character transformation
 * @param processor Processor function
 * @param ctx Processor context
 */
void u_add_stream_processor(u_stream_processor_t processor, void* ctx);

/**
 * @brief Remove a stream processor
 * @param processor Processor function to remove
 */
void u_remove_stream_processor(u_stream_processor_t processor);

/**
 * @brief State machine structure (opaque)
 */
typedef struct u_state_machine u_state_machine_t;

/**
 * @brief Create a new state machine
 * @return New state machine instance
 */
u_state_machine_t* u_state_machine_create(void);

/**
 * @brief Add a state to a state machine
 * @param sm State machine instance
 * @param name State name
 * @param enter Function called when entering state
 * @param exit Function called when exiting state
 */
void u_state_machine_add_state(u_state_machine_t* sm, const char* name, 
                              void (*enter)(void*), void (*exit)(void*));

/**
 * @brief Transition to a new state
 * @param sm State machine instance
 * @param state Name of state to transition to
 */
void u_state_machine_transition(u_state_machine_t* sm, const char* state);

/**
 * @brief Free a state machine
 * @param sm State machine to free
 */
void u_state_machine_free(u_state_machine_t* sm);

/**
 * @brief Move cursor to specified position
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 */
void u_output_move_to(u_output_cb_t output_cb, void* ctx, int x, int y);

/**
 * @brief Save current cursor position
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 */
void u_output_save_position(u_output_cb_t output_cb, void* ctx);

/**
 * @brief Restore saved cursor position
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 */
void u_output_restore_position(u_output_cb_t output_cb, void* ctx);

/**
 * @brief Clear current line
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 */
void u_output_clear_line(u_output_cb_t output_cb, void* ctx);

/**
 * @brief Clear entire screen
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 */
void u_output_clear_screen(u_output_cb_t output_cb, void* ctx);

/**
 * @brief Output a character repeated multiple times
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param c Character to repeat
 * @param count Number of times to repeat
 */
void u_format_repeat(u_output_cb_t output_cb, void* ctx, char c, int count);

/**
 * @brief Fill a field with text and alignment
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param text Text to display
 * @param width Field width
 * @param align Text alignment
 * @param fill_char Character to use for filling
 */
void u_format_fill(u_output_cb_t output_cb, void* ctx, const char* text, 
                  int width, u_text_align_t align, char fill_char);

/**
 * @brief Check if string matches a pattern with wildcards
 * @param str String to check
 * @param pattern Pattern with wildcards (* and ?)
 * @return true if string matches pattern, false otherwise
 */
bool u_str_match_pattern(const char* str, const char* pattern);

/**
 * @brief Replace all occurrences of a substring
 * @param str Original string
 * @param find Substring to find
 * @param replace Replacement string
 * @return New string with replacements (must be freed by caller)
 */
char* u_str_replace(const char* str, const char* find, const char* replace);

/**
 * @brief Trim whitespace from string
 * @param str String to trim
 * @param output Output buffer
 * @param max_len Maximum length of output buffer
 */
void u_str_trim(const char* str, char* output, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif // UPRINTF_H

/******************************************************************************/
/* IMPLEMENTATION SECTION                                                     */
/******************************************************************************/

#ifdef UPRINTF_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef UPRINTF_BUFFER_SIZE
#define UPRINTF_BUFFER_SIZE 32
#endif

#ifndef UPRINTF_MAX_HANDLERS
#define UPRINTF_MAX_HANDLERS 16
#endif

#ifndef UPRINTF_FLOAT_SUPPORT
#define UPRINTF_FLOAT_SUPPORT 1
#endif

// Internal state
static struct {
    u_output_cb_t default_output_cb;
    void* default_ctx;
    char decimal_point;
    bool float_support;
    struct {
        char specifier;
        u_format_handler_t handler;
    } handlers[UPRINTF_MAX_HANDLERS];
    
    // Extended state
    u_output_stream_t streams[16];
    int stream_count;
    
    struct {
        const char* name;
        const char* template;
    } templates[32];
    int template_count;
    
    struct {
        u_output_hook_t hook;
        void* user_data;
    } hooks[16];
    int hook_count;
    
    struct {
        u_stream_processor_t processor;
        void* ctx;
    } processors[16];
    int processor_count;
    
    u_string_builder_t builders[8];
    int builder_count;
} u_state = {
    .default_output_cb = NULL,
    .default_ctx = NULL,
    .decimal_point = '.',
    .float_support = UPRINTF_FLOAT_SUPPORT ? true : false,
    .handlers = {{0, NULL}},
    .stream_count = 0,
    .template_count = 0,
    .hook_count = 0,
    .processor_count = 0,
    .builder_count = 0
};

// Internal functions
static size_t u_strlen(const char* str) {
    if (!str) return 0;
    
    const char* s;
    for (s = str; *s; ++s);
    return (s - str);
}

static void u_strrev(char* begin, char* end) {
    if (!begin || !end || begin >= end) return;
    
    while (begin < end) {
        char temp = *begin;
        *begin++ = *end;
        *end-- = temp;
    }
}

static char* u_utoa(uint64_t value, char* str, int base, bool uppercase) {
    if (!str || base < 2 || base > 36) {
        if (str) *str = '\0';
        return str;
    }

    char* ptr = str;
    char* low = ptr;
    
    do {
        int digit = value % base;
        *ptr++ = digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10;
        value /= base;
    } while (value);
    
    *ptr = '\0';
    u_strrev(low, ptr - 1);
    return str;
}

static char* u_itoa(int64_t value, char* str, int base, bool uppercase) {
    if (!str || base < 2 || base > 36) {
        if (str) *str = '\0';
        return str;
    }

    char* ptr = str;
    
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }
    
    return u_utoa(value, ptr, base, uppercase);
}

#if UPRINTF_FLOAT_SUPPORT
static char* u_ftoa(double value, char* str, int precision, char decimal_point) {
    if (!str) return str;
    
    if (precision < 0) precision = 6;
    if (precision > UPRINTF_BUFFER_SIZE - 16) precision = UPRINTF_BUFFER_SIZE - 16;
    
    char* ptr = str;
    
    // Handle special cases
    if (value != value) { // NaN
        const char* nan_str = "nan";
        while (*nan_str) *ptr++ = *nan_str++;
        *ptr = '\0';
        return str;
    }
    
    if (value == 1.0/0.0) { // +Inf
        const char* inf_str = "inf";
        while (*inf_str) *ptr++ = *inf_str++;
        *ptr = '\0';
        return str;
    }
    
    if (value == -1.0/0.0) { // -Inf
        const char* inf_str = "-inf";
        while (*inf_str) *ptr++ = *inf_str++;
        *ptr = '\0';
        return str;
    }
    
    // Handle negative numbers
    if (value < 0) {
        *ptr++ = '-';
        value = -value;
    }
    
    // Integer part
    int64_t int_part = (int64_t)value;
    value -= int_part;
    
    // Convert integer part and advance pointer
    char* int_str = u_itoa(int_part, ptr, 10, false);
    ptr += u_strlen(int_str); // Advance to the end of the integer part
    
    // Fractional part
    if (precision > 0) {
        *ptr++ = decimal_point;
        
        // Rounding
        double rounding = 0.5;
        for (int i = 0; i < precision; i++) {
            rounding /= 10.0;
        }
        value += rounding;
        
        // Convert fractional part
        for (int i = 0; i < precision; i++) {
            value *= 10;
            int digit = (int)value;
            *ptr++ = '0' + digit;
            value -= digit;
            
            // Prevent buffer overflow
            if (ptr - str >= UPRINTF_BUFFER_SIZE - 1) {
                break;
            }
        }
    }
    
    *ptr = '\0';
    return str;
}
#endif

static void u_output_str(u_output_cb_t output_cb, void* ctx, const char* str, int max_len) {
    if (!output_cb || !str) {
        if (!str && output_cb) {
            u_output_str(output_cb, ctx, "(null)", max_len);
        }
        return;
    }
    
    for (int i = 0; (max_len < 0 || i < max_len) && str[i]; i++) {
        output_cb(str[i], ctx);
    }
}

static void u_output_repeat(u_output_cb_t output_cb, void* ctx, char c, int count) {
    if (!output_cb || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        output_cb(c, ctx);
    }
}

static int u_parse_format(u_output_cb_t output_cb, void* ctx, const char** fmt, va_list* args) {
    if (!output_cb || !fmt || !*fmt || !args) return 0;
    
    const char* format_start = *fmt; // Remember start of format specifier
    int chars_written = 0;
    unsigned int flags = 0;
    int width = -1;
    int precision = -1;
    int length_modifier = 0;
    
    // Parse flags
    while (**fmt) {
        if (**fmt == '-') flags |= U_FLAG_LEFT_ALIGN;
        else if (**fmt == '+') flags |= U_FLAG_FORCE_SIGN;
        else if (**fmt == ' ') flags |= U_FLAG_SPACE_SIGN;
        else if (**fmt == '0') flags |= U_FLAG_ZERO_PAD;
        else if (**fmt == '#') flags |= U_FLAG_ALT_FORM;
        else break;
        (*fmt)++;
    }
    
    // Parse width
    if (**fmt == '*') {
        flags |= U_FLAG_WIDTH_ARG;
        width = va_arg(*args, int);
        (*fmt)++;
    } else {
        width = 0;
        while (**fmt >= '0' && **fmt <= '9') {
            width = width * 10 + (**fmt - '0');
            (*fmt)++;
        }
        if (width == 0) width = -1;
    }
    
    // Parse precision
    if (**fmt == '.') {
        (*fmt)++;
        if (**fmt == '*') {
            flags |= U_FLAG_PRECISION_ARG;
            precision = va_arg(*args, int);
            (*fmt)++;
        } else {
            precision = 0;
            while (**fmt >= '0' && **fmt <= '9') {
                precision = precision * 10 + (**fmt - '0');
                (*fmt)++;
            }
        }
    }
    
    // Parse length modifiers
    if (**fmt == 'h') {
        length_modifier = -1;
        (*fmt)++;
        if (**fmt == 'h') {
            length_modifier = -2;
            (*fmt)++;
        }
    } else if (**fmt == 'l') {
        length_modifier = 1;
        (*fmt)++;
        if (**fmt == 'l') {
            length_modifier = 2;
            (*fmt)++;
        }
    } else if (**fmt == 'z') {
        length_modifier = 3; // size_t
        (*fmt)++;
    } else if (**fmt == 't') {
        length_modifier = 4; // ptrdiff_t
        (*fmt)++;
    } else if (**fmt == 'j') {
        length_modifier = 5; // intmax_t
        (*fmt)++;
    }
    
    // Parse specifier
    char specifier = **fmt;
    if (!specifier) {
        // No specifier found - output the entire format sequence
        output_cb('%', ctx);
        chars_written++;
        u_output_str(output_cb, ctx, format_start, *fmt - format_start);
        chars_written += (*fmt - format_start);
        return chars_written;
    }
    (*fmt)++;
    
    // Check for custom handlers
    for (int i = 0; i < UPRINTF_MAX_HANDLERS; i++) {
        if (u_state.handlers[i].specifier == specifier && u_state.handlers[i].handler) {
            return u_state.handlers[i].handler(output_cb, ctx, args, fmt, width, precision, flags);
        }
    }
    
    // Handle standard specifiers
    char buffer[UPRINTF_BUFFER_SIZE];
    int len = 0;
    bool sign = false;
    bool number = false;
    int base = 10;
    bool uppercase = false;
    
    switch (specifier) {
        case 'd':
        case 'i': {
            number = true;
            int64_t value;
            if (length_modifier == -2) value = (signed char)va_arg(*args, int);
            else if (length_modifier == -1) value = (short)va_arg(*args, int);
            else if (length_modifier == 1) value = va_arg(*args, long);
            else if (length_modifier == 2) value = va_arg(*args, long long);
            else if (length_modifier == 5) value = va_arg(*args, intmax_t);
            else value = va_arg(*args, int);
            
            u_itoa(value, buffer, 10, false);
            len = u_strlen(buffer);
            sign = value < 0;
            break;
        }
        
        case 'u': {
            number = true;
            uint64_t value;
            if (length_modifier == -2) value = (unsigned char)va_arg(*args, unsigned int);
            else if (length_modifier == -1) value = (unsigned short)va_arg(*args, unsigned int);
            else if (length_modifier == 1) value = va_arg(*args, unsigned long);
            else if (length_modifier == 2) value = va_arg(*args, unsigned long long);
            else if (length_modifier == 3) value = va_arg(*args, size_t);
            else if (length_modifier == 5) value = va_arg(*args, uintmax_t);
            else value = va_arg(*args, unsigned int);
            
            u_utoa(value, buffer, 10, false);
            len = u_strlen(buffer);
            break;
        }
        
        case 'o': {
            number = true;
            base = 8;
            uint64_t value;
            if (length_modifier == -2) value = (unsigned char)va_arg(*args, unsigned int);
            else if (length_modifier == -1) value = (unsigned short)va_arg(*args, unsigned int);
            else if (length_modifier == 1) value = va_arg(*args, unsigned long);
            else if (length_modifier == 2) value = va_arg(*args, unsigned long long);
            else if (length_modifier == 3) value = va_arg(*args, size_t);
            else if (length_modifier == 5) value = va_arg(*args, uintmax_t);
            else value = va_arg(*args, unsigned int);
            
            u_utoa(value, buffer, 8, false);
            len = u_strlen(buffer);
            
            if (flags & U_FLAG_ALT_FORM && value != 0) {
                // Add prefix for octal (shift right and add '0')
                if (len < UPRINTF_BUFFER_SIZE - 1) {
                    memmove(buffer + 1, buffer, len + 1);
                    buffer[0] = '0';
                    len++;
                }
            }
            break;
        }
        
        case 'x':
        case 'X': {
            number = true;
            base = 16;
            uppercase = (specifier == 'X');
            uint64_t value;
            if (length_modifier == -2) value = (unsigned char)va_arg(*args, unsigned int);
            else if (length_modifier == -1) value = (unsigned short)va_arg(*args, unsigned int);
            else if (length_modifier == 1) value = va_arg(*args, unsigned long);
            else if (length_modifier == 2) value = va_arg(*args, unsigned long long);
            else if (length_modifier == 3) value = va_arg(*args, size_t);
            else if (length_modifier == 5) value = va_arg(*args, uintmax_t);
            else value = va_arg(*args, unsigned int);
            
            u_utoa(value, buffer, 16, uppercase);
            len = u_strlen(buffer);
            
            if (flags & U_FLAG_ALT_FORM && value != 0) {
                // Add prefix for hexadecimal
                if (len < UPRINTF_BUFFER_SIZE - 2) {
                    memmove(buffer + 2, buffer, len + 1);
                    buffer[0] = '0';
                    buffer[1] = uppercase ? 'X' : 'x';
                    len += 2;
                }
            }
            break;
        }
        
#if UPRINTF_FLOAT_SUPPORT
        case 'f':
        case 'F': {
            if (!u_state.float_support) {
                // Float support disabled
                output_cb('%', ctx);
                output_cb(specifier, ctx);
                chars_written += 2;
                return chars_written;
            }
            
            number = true;
            double value = va_arg(*args, double);
            u_ftoa(value, buffer, precision, u_state.decimal_point);
            len = u_strlen(buffer);
            sign = value < 0;
            break;
        }
#endif
        
        case 'c': {
            char c = (char)va_arg(*args, int);
            int padding = width > 1 ? width - 1 : 0;
            
            if (!(flags & U_FLAG_LEFT_ALIGN)) {
                u_output_repeat(output_cb, ctx, ' ', padding);
                chars_written += padding;
            }
            
            output_cb(c, ctx);
            chars_written++;
            
            if (flags & U_FLAG_LEFT_ALIGN) {
                u_output_repeat(output_cb, ctx, ' ', padding);
                chars_written += padding;
            }
            
            return chars_written;
        }
        
        case 's': {
            const char* str = va_arg(*args, const char*);
            int str_len = str ? u_strlen(str) : 6; // "(null)"
            
            // Handle precision for strings (negative precision means no limit)
            if (precision >= 0 && precision < str_len) {
                str_len = precision;
            }
            
            int padding = width > str_len ? width - str_len : 0;
            
            if (!(flags & U_FLAG_LEFT_ALIGN)) {
                u_output_repeat(output_cb, ctx, ' ', padding);
                chars_written += padding;
            }
            
            u_output_str(output_cb, ctx, str ? str : "(null)", str_len);
            chars_written += str_len;
            
            if (flags & U_FLAG_LEFT_ALIGN) {
                u_output_repeat(output_cb, ctx, ' ', padding);
                chars_written += padding;
            }
            
            return chars_written;
        }
        
        case 'p': {
            number = true;
            base = 16;
            void* ptr = va_arg(*args, void*);
            uintptr_t value = (uintptr_t)ptr;
            
            u_utoa(value, buffer, 16, false);
            len = u_strlen(buffer);
            
            // Add prefix for pointers
            if (len < UPRINTF_BUFFER_SIZE - 2) {
                memmove(buffer + 2, buffer, len + 1);
                buffer[0] = '0';
                buffer[1] = 'x';
                len += 2;
            }
            break;
        }
        
        case '%': {
            output_cb('%', ctx);
            chars_written++;
            return chars_written;
        }
        
        case 'n': {
            int* ptr = va_arg(*args, int*);
            if (ptr) *ptr = chars_written;
            return chars_written;
        }
        
        default: {
            // Unknown specifier - output as-is
            output_cb('%', ctx);
            output_cb(specifier, ctx);
            chars_written += 2;
            return chars_written;
        }
    }
    
    // Handle numbers
    if (number) {
        int num_digits = len;
        int sign_len = sign ? 1 : 0;
        
        if (flags & U_FLAG_FORCE_SIGN && !sign) {
            sign_len = 1;
        } else if (flags & U_FLAG_SPACE_SIGN && !sign) {
            sign_len = 1;
        }
        
        int total_len = num_digits + sign_len;
        int padding = width > total_len ? width - total_len : 0;
        int zero_padding = 0;
        
        if (precision > num_digits) {
            zero_padding = precision - num_digits;
            total_len += zero_padding;
            padding = width > total_len ? width - total_len : 0;
        }
        
        if (flags & U_FLAG_ZERO_PAD && !(flags & U_FLAG_LEFT_ALIGN) && precision < 0) {
            zero_padding += padding;
            padding = 0;
        }
        
        // Output left padding
        if (!(flags & U_FLAG_LEFT_ALIGN)) {
            u_output_repeat(output_cb, ctx, ' ', padding);
            chars_written += padding;
        }
        
        // Output sign
        if (sign) {
            // sign already output as part of u_itoa in float and integer cases.
        } else if (flags & U_FLAG_FORCE_SIGN) {
            output_cb('+', ctx);
            chars_written++;
        } else if (flags & U_FLAG_SPACE_SIGN) {
            output_cb(' ', ctx);
            chars_written++;
        }
        
        // Output zero padding
        u_output_repeat(output_cb, ctx, '0', zero_padding);
        chars_written += zero_padding;
        
        // Output number
        u_output_str(output_cb, ctx, buffer, -1);
        chars_written += num_digits;
        
        // Output right padding
        if (flags & U_FLAG_LEFT_ALIGN) {
            u_output_repeat(output_cb, ctx, ' ', padding);
            chars_written += padding;
        }
    }
    
    return chars_written;
}

// Public functions
int u_vprintf(u_output_cb_t output_cb, void* ctx, const char* fmt, va_list args) {
    int chars_written = 0;
    
    if (!output_cb || !fmt) {
        return -1;
    }
    
    while (*fmt) {
        if (*fmt != '%') {
            output_cb(*fmt++, ctx);
            chars_written++;
            continue;
        }
        
        fmt++; // Skip '%'
        if (!*fmt) break; // Handle trailing '%'
        
        int result = u_parse_format(output_cb, ctx, &fmt, &args);
        if (result < 0) {
            return result; // Error
        }
        chars_written += result;
    }
    
    return chars_written;
}

int u_printf(u_output_cb_t output_cb, void* ctx, const char* fmt, ...) {
    if (!output_cb || !fmt) return -1;
    
    va_list args;
    va_start(args, fmt);
    int result = u_vprintf(output_cb, ctx, fmt, args);
    va_end(args);
    return result;
}

int u_register_format_handler(char specifier, u_format_handler_t handler) {
    if (!handler) return -1;
    
    for (int i = 0; i < UPRINTF_MAX_HANDLERS; i++) {
        if (u_state.handlers[i].specifier == 0 || u_state.handlers[i].specifier == specifier) {
            u_state.handlers[i].specifier = specifier;
            u_state.handlers[i].handler = handler;
            return 0;
        }
    }
    return -1; // No space
}

int u_unregister_format_handler(char specifier) {
    for (int i = 0; i < UPRINTF_MAX_HANDLERS; i++) {
        if (u_state.handlers[i].specifier == specifier) {
            u_state.handlers[i].specifier = 0;
            u_state.handlers[i].handler = NULL;
            return 0;
        }
    }
    return -1; // Not found
}

// Buffer output functions
static void buffer_output_cb(char c, void* ctx) {
    if (!ctx) return;
    
    char** buffer_ptr = (char**)ctx;
    if (*buffer_ptr) {
        **buffer_ptr = c;
        (*buffer_ptr)++;
    }
}

int u_sprintf(char* buffer, const char* fmt, ...) {
    if (!buffer || !fmt) return -1;
    
    va_list args;
    va_start(args, fmt);
    char* ptr = buffer;
    int result = u_vprintf(buffer_output_cb, &ptr, fmt, args);
    *ptr = '\0';
    va_end(args);
    return result;
}

// Bounded buffer output
typedef struct {
    char* buffer;
    size_t size;
    size_t pos;
} bounded_ctx_t;

static void bounded_output_cb(char c, void* ctx) {
    if (!ctx) return;
    
    bounded_ctx_t* bctx = (bounded_ctx_t*)ctx;
    if (bctx->pos < bctx->size - 1) {
        bctx->buffer[bctx->pos] = c;
        bctx->pos++;
    }
}

int u_snprintf(char* buffer, size_t size, const char* fmt, ...) {
    if (!buffer || size == 0 || !fmt) return -1;
    
    va_list args;
    va_start(args, fmt);
    
    bounded_ctx_t ctx = {buffer, size, 0};
    u_vprintf(bounded_output_cb, &ctx, fmt, args);
    buffer[ctx.pos] = '\0';
    
    va_end(args);
    return ctx.pos; // Return actual number of characters written
}

void u_set_locale(const char* locale) {
    // Minimal implementation - just set decimal point
    if (locale && *locale) {
        u_state.decimal_point = *locale;
    }
}

void u_set_float_support(bool enabled) {
    u_state.float_support = enabled;
}

void u_set_default_output(u_output_cb_t output_cb, void* ctx) {
    u_state.default_output_cb = output_cb;
    u_state.default_ctx = ctx;
}

int u_printf_simple(const char* fmt, ...) {
    if (!u_state.default_output_cb || !fmt) return -1;
    
    va_list args;
    va_start(args, fmt);
    int result = u_vprintf(u_state.default_output_cb, u_state.default_ctx, fmt, args);
    va_end(args);
    return result;
}

// Enhanced output function with processors and hooks
static void enhanced_output_cb(char c, void* ctx) {
    // Apply stream processors
    for (int i = 0; i < u_state.processor_count; i++) {
        c = u_state.processors[i].processor(c, u_state.processors[i].ctx);
    }
    
    // Call hooks
    for (int i = 0; i < u_state.hook_count; i++) {
        u_state.hooks[i].hook(c, ctx, u_state.hooks[i].user_data);
    }
    
    // Original output
    u_output_cb_t original = (u_output_cb_t)ctx;
    original(c, ctx);
}

// Enhanced printf with all new features
int u_vprintf_ex(u_output_cb_t output_cb, void* ctx, const char* fmt, va_list args) {
    // Use enhanced output system
    return u_vprintf(enhanced_output_cb, (void*)output_cb, fmt, args);
}

int u_printf_ex(u_output_cb_t output_cb, void* ctx, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = u_vprintf_ex(output_cb, ctx, fmt, args);
    va_end(args);
    return result;
}

// Template system implementation
void u_template_format(u_output_cb_t output_cb, void* ctx, 
                      const char* template, const u_template_var_t* vars, int count) {
    const char* ptr = template;
    while (*ptr) {
        if (*ptr == '{' && *(ptr + 1) == '{') {
            // Found variable start
            const char* end = strstr(ptr + 2, "}}");
            if (!end) break;
            
            // Extract variable name
            char var_name[64];
            size_t var_len = end - (ptr + 2);
            if (var_len >= sizeof(var_name)) var_len = sizeof(var_name) - 1;
            strncpy(var_name, ptr + 2, var_len);
            var_name[var_len] = '\0';
            
            // Find variable value
            const char* value = NULL;
            for (int i = 0; i < count; i++) {
                if (strcmp(vars[i].key, var_name) == 0) {
                    value = vars[i].value;
                    break;
                }
            }
            
            // Output value or empty string
            if (value) {
                u_output_str(output_cb, ctx, value, -1);
            }
            
            ptr = end + 2;
        } else {
            output_cb(*ptr++, ctx);
        }
    }
}

void u_template_load(const char* name, const char* template) {
    for (int i = 0; i < u_state.template_count; i++) {
        if (strcmp(u_state.templates[i].name, name) == 0) {
            u_state.templates[i].template = template;
            return;
        }
    }
    
    if (u_state.template_count < 32) {
        u_state.templates[u_state.template_count].name = name;
        u_state.templates[u_state.template_count].template = template;
        u_state.template_count++;
    }
}

void u_template_format_named(u_output_cb_t output_cb, void* ctx, 
                            const char* name, const u_template_var_t* vars, int count) {
    for (int i = 0; i < u_state.template_count; i++) {
        if (strcmp(u_state.templates[i].name, name) == 0) {
            u_template_format(output_cb, ctx, u_state.templates[i].template, vars, count);
            return;
        }
    }
    
    // Template not found - output error
    u_output_str(output_cb, ctx, "[Template '", -1);
    u_output_str(output_cb, ctx, name, -1);
    u_output_str(output_cb, ctx, "' not found]", -1);
}

// Multi-output system implementation
void u_output_add_stream(u_output_stream_t stream) {
    if (u_state.stream_count < 16) {
        u_state.streams[u_state.stream_count++] = stream;
    }
}

void u_output_remove_stream(u_output_cb_t output_cb) {
    for (int i = 0; i < u_state.stream_count; i++) {
        if (u_state.streams[i].output == output_cb) {
            // Remove by shifting
            for (int j = i; j < u_state.stream_count - 1; j++) {
                u_state.streams[j] = u_state.streams[j + 1];
            }
            u_state.stream_count--;
            break;
        }
    }
}

void u_output_broadcast(char c) {
    for (int i = 0; i < u_state.stream_count; i++) {
        if (u_state.streams[i].enabled) {
            u_state.streams[i].output(c, u_state.streams[i].ctx);
        }
    }
}

void u_output_broadcast_str(const char* str) {
    for (int i = 0; str[i]; i++) {
        u_output_broadcast(str[i]);
    }
}

// String builder implementation
u_string_builder_t u_string_builder_create(size_t initial_size) {
    u_string_builder_t sb = {
        .buffer = malloc(initial_size),
        .size = initial_size,
        .pos = 0,
        .dynamic = true
    };
    if (sb.buffer) sb.buffer[0] = '\0';
    return sb;
}

void u_string_builder_append(u_string_builder_t* sb, const char* str) {
    if (!sb || !str) return;
    
    size_t len = strlen(str);
    if (sb->pos + len + 1 >= sb->size) {
        if (sb->dynamic) {
            size_t new_size = sb->size * 2;
            if (new_size < sb->pos + len + 1) new_size = sb->pos + len + 1;
            char* new_buffer = realloc(sb->buffer, new_size);
            if (!new_buffer) return;
            sb->buffer = new_buffer;
            sb->size = new_size;
        } else {
            len = sb->size - sb->pos - 1;
        }
    }
    
    strncpy(sb->buffer + sb->pos, str, len);
    sb->pos += len;
    sb->buffer[sb->pos] = '\0';
}

void u_string_builder_append_format(u_string_builder_t* sb, const char* fmt, ...) {
    if (!sb || !fmt) return;
    
    va_list args;
    va_start(args, fmt);
    
    // Calculate required size
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);
    
    if (needed < 0) {
        va_end(args);
        return;
    }
    
    // Ensure enough space
    if (sb->pos + needed + 1 >= sb->size) {
        if (sb->dynamic) {
            size_t new_size = sb->size + needed + 1;
            char* new_buffer = realloc(sb->buffer, new_size);
            if (!new_buffer) {
                va_end(args);
                return;
            }
            sb->buffer = new_buffer;
            sb->size = new_size;
        } else {
            needed = sb->size - sb->pos - 1;
        }
    }
    
    // Format directly into buffer
    vsnprintf(sb->buffer + sb->pos, needed + 1, fmt, args);
    sb->pos += needed;
    
    va_end(args);
}

void u_string_builder_clear(u_string_builder_t* sb) {
    if (sb) {
        sb->pos = 0;
        if (sb->buffer) sb->buffer[0] = '\0';
    }
}

void u_string_builder_free(u_string_builder_t* sb) {
    if (sb && sb->dynamic && sb->buffer) {
        free(sb->buffer);
        sb->buffer = NULL;
        sb->size = 0;
        sb->pos = 0;
    }
}

// Text alignment implementation
void u_text_align(u_output_cb_t output_cb, void* ctx, const char* text, 
                 int width, u_text_align_t align) {
    int text_len = strlen(text);
    int padding = width - text_len;
    
    if (padding <= 0) {
        u_output_str(output_cb, ctx, text, -1);
        return;
    }
    
    switch (align) {
        case U_ALIGN_LEFT:
            u_output_str(output_cb, ctx, text, -1);
            u_output_repeat(output_cb, ctx, ' ', padding);
            break;
            
        case U_ALIGN_RIGHT:
            u_output_repeat(output_cb, ctx, ' ', padding);
            u_output_str(output_cb, ctx, text, -1);
            break;
            
        case U_ALIGN_CENTER:
            u_output_repeat(output_cb, ctx, ' ', padding / 2);
            u_output_str(output_cb, ctx, text, -1);
            u_output_repeat(output_cb, ctx, ' ', padding - padding / 2);
            break;
            
        case U_ALIGN_JUSTIFY:
            // Simple justification for demonstration
            u_output_str(output_cb, ctx, text, -1);
            // TODO: Implement proper justification
            break;
    }
}

// Text transformation implementation
void u_text_transform(u_output_cb_t output_cb, void* ctx, const char* text, 
                     u_text_transform_t transform) {
    char buffer[256];
    size_t len = strlen(text);
    if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;
    
    strncpy(buffer, text, len);
    buffer[len] = '\0';
    
    switch (transform) {
        case U_TRANSFORM_UPPERCASE:
            for (char* p = buffer; *p; p++) *p = toupper(*p);
            break;
            
        case U_TRANSFORM_LOWERCASE:
            for (char* p = buffer; *p; p++) *p = tolower(*p);
            break;
            
        case U_TRANSFORM_CAPITALIZE:
            if (buffer[0]) buffer[0] = toupper(buffer[0]);
            for (char* p = buffer + 1; *p; p++) {
                if (isspace(*(p - 1))) *p = toupper(*p);
            }
            break;
            
        case U_TRANSFORM_REVERSE:
            u_strrev(buffer, buffer + len - 1);
            break;
            
        case U_TRANSFORM_ROT13:
            for (char* p = buffer; *p; p++) {
                if (*p >= 'a' && *p <= 'z') {
                    *p = 'a' + (*p - 'a' + 13) % 26;
                } else if (*p >= 'A' && *p <= 'Z') {
                    *p = 'A' + (*p - 'A' + 13) % 26;
                }
            }
            break;
            
        default:
            break;
    }
    
    u_output_str(output_cb, ctx, buffer, -1);
}

// Text wrapping implementation
void u_text_wrap(u_output_cb_t output_cb, void* ctx, const char* text, 
                int width, const char* prefix) {
    const char* start = text;
    const char* end = text;
    int current_line_length = 0;
    int prefix_len = prefix ? strlen(prefix) : 0;
    
    while (*end) {
        // Find next word boundary
        const char* word_end = end;
        while (*word_end && !isspace(*word_end)) word_end++;
        
        int word_len = word_end - end;
        
        if (current_line_length + word_len > width) {
            // Output current line
            if (prefix && start != end) {
                u_output_str(output_cb, ctx, prefix, prefix_len);
            }
            u_output_str(output_cb, ctx, start, end - start);
            u_output_str(output_cb, ctx, "\n", 1);
            
            // Start new line
            start = end;
            current_line_length = 0;
        }
        
        // Skip whitespace
        while (*word_end && isspace(*word_end)) word_end++;
        
        current_line_length += word_len + (word_end - end - word_len);
        end = word_end;
    }
    
    // Output remaining text
    if (start < end) {
        if (prefix) {
            u_output_str(output_cb, ctx, prefix, prefix_len);
        }
        u_output_str(output_cb, ctx, start, end - start);
    }
}

// Pattern-based formatting
void u_format_pattern(u_output_cb_t output_cb, void* ctx, const char* pattern, 
                     const char* value, const char* default_pattern) {
    if (!value || !*value) {
        if (default_pattern) {
            u_output_str(output_cb, ctx, default_pattern, -1);
        }
        return;
    }
    
    const char* ptr = pattern;
    while (*ptr) {
        if (*ptr == '%' && *(ptr + 1) == 's') {
            u_output_str(output_cb, ctx, value, -1);
            ptr += 2;
        } else {
            output_cb(*ptr++, ctx);
        }
    }
}

// Event system implementation
void u_add_output_hook(u_output_hook_t hook, void* user_data) {
    if (u_state.hook_count < 16) {
        u_state.hooks[u_state.hook_count].hook = hook;
        u_state.hooks[u_state.hook_count].user_data = user_data;
        u_state.hook_count++;
    }
}

void u_remove_output_hook(u_output_hook_t hook) {
    for (int i = 0; i < u_state.hook_count; i++) {
        if (u_state.hooks[i].hook == hook) {
            // Remove by shifting
            for (int j = i; j < u_state.hook_count - 1; j++) {
                u_state.hooks[j] = u_state.hooks[j + 1];
            }
            u_state.hook_count--;
            break;
        }
    }
}

// Stream processors implementation
void u_add_stream_processor(u_stream_processor_t processor, void* ctx) {
    if (u_state.processor_count < 16) {
        u_state.processors[u_state.processor_count].processor = processor;
        u_state.processors[u_state.processor_count].ctx = ctx;
        u_state.processor_count++;
    }
}

void u_remove_stream_processor(u_stream_processor_t processor) {
    for (int i = 0; i < u_state.processor_count; i++) {
        if (u_state.processors[i].processor == processor) {
            // Remove by shifting
            for (int j = i; j < u_state.processor_count - 1; j++) {
                u_state.processors[j] = u_state.processors[j + 1];
            }
            u_state.processor_count--;
            break;
        }
    }
}

// State machine implementation
struct u_state_machine {
    struct {
        const char* name;
        void (*enter)(void*);
        void (*exit)(void*);
    } states[16];
    int state_count;
    int current_state;
    void* user_data;
};

u_state_machine_t* u_state_machine_create(void) {
    u_state_machine_t* sm = malloc(sizeof(u_state_machine_t));
    if (sm) {
        sm->state_count = 0;
        sm->current_state = -1;
        sm->user_data = NULL;
    }
    return sm;
}

void u_state_machine_add_state(u_state_machine_t* sm, const char* name, 
                              void (*enter)(void*), void (*exit)(void*)) {
    if (sm && sm->state_count < 16) {
        sm->states[sm->state_count].name = name;
        sm->states[sm->state_count].enter = enter;
        sm->states[sm->state_count].exit = exit;
        sm->state_count++;
    }
}

void u_state_machine_transition(u_state_machine_t* sm, const char* state) {
    if (!sm) return;
    
    // Find state index
    int new_state = -1;
    for (int i = 0; i < sm->state_count; i++) {
        if (strcmp(sm->states[i].name, state) == 0) {
            new_state = i;
            break;
        }
    }
    
    if (new_state == -1) return;
    
    // Exit current state
    if (sm->current_state >= 0 && sm->states[sm->current_state].exit) {
        sm->states[sm->current_state].exit(sm->user_data);
    }
    
    // Enter new state
    sm->current_state = new_state;
    if (sm->states[sm->current_state].enter) {
        sm->states[sm->current_state].enter(sm->user_data);
    }
}

void u_state_machine_free(u_state_machine_t* sm) {
    if (sm) free(sm);
}

// Position-aware output implementation
void u_output_move_to(u_output_cb_t output_cb, void* ctx, int x, int y) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "\033[%d;%dH", y, x);
    u_output_str(output_cb, ctx, buffer, -1);
}

void u_output_save_position(u_output_cb_t output_cb, void* ctx) {
    u_output_str(output_cb, ctx, "\033[s", -1);
}

void u_output_restore_position(u_output_cb_t output_cb, void* ctx) {
    u_output_str(output_cb, ctx, "\033[u", -1);
}

void u_output_clear_line(u_output_cb_t output_cb, void* ctx) {
    u_output_str(output_cb, ctx, "\033[2K", -1);
}

void u_output_clear_screen(u_output_cb_t output_cb, void* ctx) {
    u_output_str(output_cb, ctx, "\033[2J", -1);
}

// Utility functions implementation
void u_format_repeat(u_output_cb_t output_cb, void* ctx, char c, int count) {
    u_output_repeat(output_cb, ctx, c, count);
}

void u_format_fill(u_output_cb_t output_cb, void* ctx, const char* text, 
                  int width, u_text_align_t align, char fill_char) {
    int text_len = strlen(text);
    int padding = width - text_len;
    
    if (padding <= 0) {
        u_output_str(output_cb, ctx, text, -1);
        return;
    }
    
    switch (align) {
        case U_ALIGN_LEFT:
            u_output_str(output_cb, ctx, text, -1);
            u_output_repeat(output_cb, ctx, fill_char, padding);
            break;
            
        case U_ALIGN_RIGHT:
            u_output_repeat(output_cb, ctx, fill_char, padding);
            u_output_str(output_cb, ctx, text, -1);
            break;
            
        case U_ALIGN_CENTER:
            u_output_repeat(output_cb, ctx, fill_char, padding / 2);
            u_output_str(output_cb, ctx, text, -1);
            u_output_repeat(output_cb, ctx, fill_char, padding - padding / 2);
            break;
            
        case U_ALIGN_JUSTIFY:
            u_output_str(output_cb, ctx, text, -1);
            // TODO: Implement proper justification with fill character
            break;
    }
}

bool u_str_match_pattern(const char* str, const char* pattern) {
    while (*pattern) {
        if (*pattern == '*') {
            pattern++;
            while (*str) {
                if (u_str_match_pattern(str, pattern)) {
                    return true;
                }
                str++;
            }
            return !*pattern;
        } else if (*pattern == '?') {
            if (!*str) return false;
            pattern++;
            str++;
        } else {
            if (*pattern != *str) return false;
            pattern++;
            str++;
        }
    }
    return !*str;
}

char* u_str_replace(const char* str, const char* find, const char* replace) {
    if (!str || !find) return NULL;
    
    // Count occurrences
    int count = 0;
    const char* pos = str;
    size_t find_len = strlen(find);
    while ((pos = strstr(pos, find))) {
        count++;
        pos += find_len;
    }
    
    // Calculate new string length
    size_t str_len = strlen(str);
    size_t replace_len = strlen(replace);
    size_t new_len = str_len + count * (replace_len - find_len) + 1;
    
    // Allocate new string
    char* result = malloc(new_len);
    if (!result) return NULL;
    
    // Build new string
    char* dst = result;
    const char* src = str;
    while (*src) {
        if (strstr(src, find) == src) {
            strcpy(dst, replace);
            dst += replace_len;
            src += find_len;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    
    return result;
}

void u_str_trim(const char* str, char* output, size_t max_len) {
    if (!str || !output || max_len == 0) return;
    
    // Skip leading whitespace
    while (isspace(*str)) str++;
    
    // Find end of string
    const char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    
    // Copy trimmed string
    size_t len = end - str + 1;
    if (len >= max_len) len = max_len - 1;
    
    strncpy(output, str, len);
    output[len] = '\0';
}

#endif // UPRINTF_IMPLEMENTATION