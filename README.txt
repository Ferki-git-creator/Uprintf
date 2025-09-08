===============================================================================
|                           UPRINTF LIBRARY v1.3                              |
===============================================================================

Universal platform-independent printf implementation for C with advanced features.

Author: Ferki
License: MIT
Version: 1.3

------------------------------------------------------------------------------=
DESCRIPTION:
------------------------------------------------------------------------------=

uprintf is a single-header, platform-independent printf implementation that
works from bare-metal microcontrollers to modern operating systems without any
platform dependencies or #ifdef directives. Version 1.3 adds revolutionary
new features while maintaining full backward compatibility.

Features:
- Full printf functionality (%d, %i, %u, %o, %x, %X, %f, %F, %c, %s, %p, %%)
- Support for flags: '-', '+', ' ', '0', '#'
- Width and precision specification (including * for both)
- Length modifiers: h, hh, l, ll, z, t, j
- Floating point support with configurable precision (including NaN, Inf handling)
- Plugin system for custom format specifiers
- Buffer functions (sprintf, snprintf)
- No dynamic memory allocation (optional for advanced features)
- Header-only implementation
- Locale support for decimal point
- Configurable features via preprocessor definitions
- Enhanced error checking and safety features
- Bare metal compatible with zero dependencies

NEW IN VERSION 1.3:
- Template system with variable substitution
- Multi-output streaming to multiple destinations simultaneously
- Advanced string building with dynamic allocation support
- Text alignment and transformation functions
- Text wrapping and formatting utilities
- Pattern-based formatting with fallbacks
- Event system with output hooks
- Stream processors for character transformation
- Lightweight state machine for complex workflows
- Position-aware output for advanced terminal control
- Memory-efficient formatting utilities
- String manipulation utilities (pattern matching, replacement, trimming)

------------------------------------------------------------------------------=
USAGE:
------------------------------------------------------------------------------=

1. Add to your project:
   #define UPRINTF_IMPLEMENTATION
   #include "uprintf.h"

2. Implement a character output function:
   void my_output_cb(char c, void* ctx) {
       // Send character to UART, file, buffer, etc.
   }

3. Use the printf functions:
   u_printf(my_output_cb, NULL, "Value: %d", 42);
   u_sprintf(buffer, "Result: %s", "text");

------------------------------------------------------------------------------=
EXAMPLES:
------------------------------------------------------------------------------=

Basic usage:
  u_printf(uart_output_cb, NULL, "Temperature: %.1fC\n", 23.5);

Template system:
  u_template_format(output, ctx, "Hello {{name}}!", 
                   (u_template_var_t[]){{"name", "World"}}, 1);

Multi-output streaming:
  u_output_add_stream((u_output_stream_t){uart_output, NULL, true});
  u_output_add_stream((u_output_stream_t){file_output, file, true});
  u_output_broadcast_str("This goes to all streams!\n");

String building:
  u_string_builder_t sb = u_string_builder_create(256);
  u_string_builder_append_format(&sb, "Value: %d", 42);
  u_string_builder_append(&sb, " units");

Text formatting:
  u_text_align(output, ctx, "Text", 10, U_ALIGN_CENTER);
  u_text_transform(output, ctx, "hello", U_TRANSFORM_CAPITALIZE);

Advanced formatting:
  u_format_pattern(output, ctx, "Value: %s", "test", "No value");
  u_text_wrap(output, ctx, long_text, 80, "> ");

Custom format handler:
  int timer_handler(u_output_cb_t output_cb, void* ctx, va_list* args, 
                   const char** fmt, int width, int precision, 
                   unsigned int flags) {
      uint32_t ticks = get_ticks();
      char buffer[16];
      u_utoa(ticks, buffer, 10, false);
      // ... output formatting
      return chars_written;
  }

  u_register_format_handler('T', timer_handler);
  u_printf(uart_output_cb, NULL, "Ticks: %T\n");

Buffer output:
  char buf[100];
  u_sprintf(buf, "Value: %04x", 255);
  u_snprintf(buf, sizeof(buf), "Trimmed: %.5s", "very long string");

Bare metal example:
  void uart_putchar(char c, void* ctx) {
      while (!(USART1->SR & USART_SR_TXE));
      USART1->DR = c;
  }
  
  void main() {
      u_set_default_output(uart_putchar, NULL);
      u_printf_simple("System started: %d\n", 42);
  }

------------------------------------------------------------------------------=
CONFIGURATION:
------------------------------------------------------------------------------=

Preprocessor definitions:
  UPRINTF_IMPLEMENTATION - Define in one source file to enable implementation
  UPRINTF_BUFFER_SIZE    - Set internal buffer size (default: 32)
  UPRINTF_MAX_HANDLERS   - Set maximum custom handlers (default: 16)
  UPRINTF_FLOAT_SUPPORT  - Enable/disable float support (default: 1)

Runtime configuration:
  u_set_locale()         - Set decimal point character
  u_set_float_support()  - Enable/disable float support at runtime
  u_set_default_output() - Set default output handler

------------------------------------------------------------------------------=
API REFERENCE:
------------------------------------------------------------------------------=

Main functions:
  int u_printf(u_output_cb_t output_cb, void* ctx, const char* fmt, ...)
  int u_vprintf(u_output_cb_t output_cb, void* ctx, const char* fmt, va_list args)
  int u_sprintf(char* buffer, const char* fmt, ...)
  int u_snprintf(char* buffer, size_t size, const char* fmt, ...)
  int u_printf_ex(u_output_cb_t output_cb, void* ctx, const char* fmt, ...)
  int u_vprintf_ex(u_output_cb_t output_cb, void* ctx, const char* fmt, va_list args)

Handler registration:
  int u_register_format_handler(char specifier, u_format_handler_t handler)
  int u_unregister_format_handler(char specifier)

Configuration:
  void u_set_locale(const char* locale)
  void u_set_float_support(bool enabled)
  void u_set_default_output(u_output_cb_t output_cb, void* ctx)
  int u_printf_simple(const char* fmt, ...)

Template system:
  void u_template_format(u_output_cb_t output_cb, void* ctx, const char* template, 
                        const u_template_var_t* vars, int count)
  void u_template_load(const char* name, const char* template)
  void u_template_format_named(u_output_cb_t output_cb, void* ctx, 
                              const char* name, const u_template_var_t* vars, int count)

Multi-output system:
  void u_output_add_stream(u_output_stream_t stream)
  void u_output_remove_stream(u_output_cb_t output_cb)
  void u_output_broadcast(char c)
  void u_output_broadcast_str(const char* str)

String building:
  u_string_builder_t u_string_builder_create(size_t initial_size)
  void u_string_builder_append(u_string_builder_t* sb, const char* str)
  void u_string_builder_append_format(u_string_builder_t* sb, const char* fmt, ...)
  void u_string_builder_clear(u_string_builder_t* sb)
  void u_string_builder_free(u_string_builder_t* sb)

Text processing:
  void u_text_align(u_output_cb_t output_cb, void* ctx, const char* text, 
                   int width, u_text_align_t align)
  void u_text_transform(u_output_cb_t output_cb, void* ctx, const char* text, 
                       u_text_transform_t transform)
  void u_text_wrap(u_output_cb_t output_cb, void* ctx, const char* text, 
                  int width, const char* prefix)

Pattern formatting:
  void u_format_pattern(u_output_cb_t output_cb, void* ctx, const char* pattern, 
                       const char* value, const char* default_pattern)

Event system:
  void u_add_output_hook(u_output_hook_t hook, void* user_data)
  void u_remove_output_hook(u_output_hook_t hook)

Stream processors:
  void u_add_stream_processor(u_stream_processor_t processor, void* ctx)
  void u_remove_stream_processor(u_stream_processor_t processor)

State machine:
  u_state_machine_t* u_state_machine_create(void)
  void u_state_machine_add_state(u_state_machine_t* sm, const char* name, 
                                void (*enter)(void*), void (*exit)(void*))
  void u_state_machine_transition(u_state_machine_t* sm, const char* state)
  void u_state_machine_free(u_state_machine_t* sm)

Position-aware output:
  void u_output_move_to(u_output_cb_t output_cb, void* ctx, int x, int y)
  void u_output_save_position(u_output_cb_t output_cb, void* ctx)
  void u_output_restore_position(u_output_cb_t output_cb, void* ctx)
  void u_output_clear_line(u_output_cb_t output_cb, void* ctx)
  void u_output_clear_screen(u_output_cb_t output_cb, void* ctx)

Utility functions:
  void u_format_repeat(u_output_cb_t output_cb, void* ctx, char c, int count)
  void u_format_fill(u_output_cb_t output_cb, void* ctx, const char* text, 
                    int width, u_text_align_t align, char fill_char)
  bool u_str_match_pattern(const char* str, const char* pattern)
  char* u_str_replace(const char* str, const char* find, const char* replace)
  void u_str_trim(const char* str, char* output, size_t max_len)

Note on u_snprintf: This function returns the number of characters actually
written to the buffer, not the number that would be written if the buffer was
large enough (unlike standard snprintf).

------------------------------------------------------------------------------=
VERSION HISTORY:
------------------------------------------------------------------------------=

v1.3 (Current)
- Added template system with variable substitution
- Added multi-output streaming to multiple destinations
- Added advanced string building with dynamic allocation support
- Added text alignment and transformation functions
- Added text wrapping and formatting utilities
- Added pattern-based formatting with fallbacks
- Added event system with output hooks
- Added stream processors for character transformation
- Added lightweight state machine for complex workflows
- Added position-aware output for terminal control
- Added memory-efficient formatting utilities
- Added string manipulation utilities
- Enhanced error checking and safety features
- Improved documentation and examples

v1.2
- Enhanced error checking and null pointer protection
- Improved floating point handling with NaN and Inf support
- Fixed u_snprintf return value to match actual written characters
- Better handling of unknown format specifiers
- Buffer overflow protection in all conversion functions
- Additional safety checks throughout the codebase

v1.1
- Added width support for %c format specifier
- Fixed various edge cases in format parsing
- Improved platform independence

v1.0
- Initial release with basic printf functionality

------------------------------------------------------------------------------=
SUPPORT THE PROJECT
------------------------------------------------------------------------------=

If you find this library useful, please consider supporting its development:

  [ https://ko-fi.com/ferki ]

Your support helps maintain and improve this software!

------------------------------------------------------------------------------=
LICENSE:
------------------------------------------------------------------------------=

MIT License

Copyright (c) 2023-2024 Ferki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===============================================================================