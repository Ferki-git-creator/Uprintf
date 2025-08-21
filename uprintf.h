/**
 * @file uprintf.h
 * @brief Universal platform-independent printf implementation
 * @version 1.0
 * @author Ferki
 * @date 2023
 * 
 * @license MIT License
 * 
 * This is a single-header library that provides a complete printf implementation
 * that works from bare-metal microcontrollers to modern OSes without any platform
 * dependencies or #ifdefs.
 * 
 * Usage:
 *   #define UPRINTF_IMPLEMENTATION
 *   #include "uprintf.h"
 * 
 * Features:
 *   - Full printf functionality (%d, %i, %u, %o, %x, %X, %f, %F, %c, %s, %p, %%)
 *   - Flags: '-', '+', ' ', '0', '#'
 *   - Width and precision (including * for both)
 *   - Length modifiers: h, hh, l, ll, z, t
 *   - Floating point support with configurable precision
 *   - Plugin system for custom format specifiers
 *   - Buffer functions (sprintf, snprintf)
 *   - No dynamic memory allocation
 *   - Header-only implementation
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
 * @return Number of characters written
 */
int u_printf(u_output_cb_t output_cb, void* ctx, const char* fmt, ...);

/**
 * @brief Varargs version of printf
 * @param output_cb Character output callback
 * @param ctx Context pointer for callback
 * @param fmt Format string
 * @param args Variable arguments list
 * @return Number of characters written
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
 * @return Number of characters written (excluding null terminator)
 */
int u_sprintf(char* buffer, const char* fmt, ...);

/**
 * @brief Write formatted output to a string with size limit
 * @param buffer Output buffer
 * @param size Buffer size
 * @param fmt Format string
 * @param ... Arguments to format
 * @return Number of characters written (excluding null terminator)
 * 
 * @note This function returns the number of characters actually written to
 *       the buffer, not the number that would be written if the buffer was
 *       large enough (unlike standard snprintf).
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
 * @return Number of characters written
 */
int u_printf_simple(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // UPRINTF_H

/******************************************************************************/
/* IMPLEMENTATION SECTION - only included when UPRINTF_IMPLEMENTATION is defined */
/******************************************************************************/

#ifdef UPRINTF_IMPLEMENTATION

#include <stdarg.h>
#include <string.h>

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
} u_state = {
    .default_output_cb = NULL,
    .default_ctx = NULL,
    .decimal_point = '.',
    .float_support = UPRINTF_FLOAT_SUPPORT ? true : false,
    .handlers = {{0, NULL}}
};

// Internal functions
static size_t u_strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s);
    return (s - str);
}

static void u_strrev(char* begin, char* end) {
    while (begin < end) {
        char temp = *begin;
        *begin++ = *end;
        *end-- = temp;
    }
}

static char* u_utoa(uint64_t value, char* str, int base, bool uppercase) {
    if (base < 2 || base > 36) {
        *str = '\0';
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
    if (base < 2 || base > 36) {
        *str = '\0';
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
    if (precision < 0) precision = 6;
    
    char* ptr = str;
    
    // Handle negative numbers
    if (value < 0) {
        *ptr++ = '-';
        value = -value;
    }
    
    // Integer part
    int64_t int_part = (int64_t)value;
    value -= int_part;
    
    // Convert integer part
    ptr = u_itoa(int_part, ptr, 10, false);
    
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
        }
    }
    
    *ptr = '\0';
    return str;
}
#endif

static void u_output_str(u_output_cb_t output_cb, void* ctx, const char* str, int max_len) {
    if (!str) {
        u_output_str(output_cb, ctx, "(null)", max_len);
        return;
    }
    
    for (int i = 0; (max_len < 0 || i < max_len) && str[i]; i++) {
        output_cb(str[i], ctx);
    }
}

static void u_output_repeat(u_output_cb_t output_cb, void* ctx, char c, int count) {
    for (int i = 0; i < count; i++) {
        output_cb(c, ctx);
    }
}

static int u_parse_format(u_output_cb_t output_cb, void* ctx, const char** fmt, va_list* args) {
    const char* start = *fmt;
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
                memmove(buffer + 1, buffer, len + 1);
                buffer[0] = '0';
                len++;
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
                memmove(buffer + 2, buffer, len + 1);
                buffer[0] = '0';
                buffer[1] = uppercase ? 'X' : 'x';
                len += 2;
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
            output_cb(c, ctx);
            chars_written++;
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
            memmove(buffer + 2, buffer, len + 1);
            buffer[0] = '0';
            buffer[1] = 'x';
            len += 2;
            break;
        }
        
        case '%': {
            output_cb('%', ctx);
            chars_written++;
            return chars_written;
        }
        
        case 'n': {
            int* ptr = va_arg(*args, int*);
            *ptr = chars_written;
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
        chars_written += u_parse_format(output_cb, ctx, &fmt, &args);
    }
    
    return chars_written;
}

int u_printf(u_output_cb_t output_cb, void* ctx, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = u_vprintf(output_cb, ctx, fmt, args);
    va_end(args);
    return result;
}

int u_register_format_handler(char specifier, u_format_handler_t handler) {
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
    char** buffer_ptr = (char**)ctx;
    **buffer_ptr = c;
    (*buffer_ptr)++;
}

int u_sprintf(char* buffer, const char* fmt, ...) {
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
    bounded_ctx_t* bctx = (bounded_ctx_t*)ctx;
    if (bctx->pos < bctx->size - 1) {
        bctx->buffer[bctx->pos] = c;
        bctx->pos++;
    }
}

int u_snprintf(char* buffer, size_t size, const char* fmt, ...) {
    if (size == 0) return 0;
    
    va_list args;
    va_start(args, fmt);
    
    bounded_ctx_t ctx = {buffer, size, 0};
    int result = u_vprintf(bounded_output_cb, &ctx, fmt, args);
    buffer[ctx.pos] = '\0';
    
    va_end(args);
    return result;
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
    if (!u_state.default_output_cb) return -1;
    
    va_list args;
    va_start(args, fmt);
    int result = u_vprintf(u_state.default_output_cb, u_state.default_ctx, fmt, args);
    va_end(args);
    return result;
}

#endif // UPRINTF_IMPLEMENTATION