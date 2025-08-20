===============================================================================
|                           UPRINTF LIBRARY                                  |
===============================================================================

Universal platform-independent printf implementation for C.

Author: Ferki
License: MIT

------------------------------------------------------------------------------=
DESCRIPTION:
------------------------------------------------------------------------------=

uprintf is a single-header, platform-independent printf implementation that
works from bare-metal microcontrollers to modern operating systems without any
platform dependencies or #ifdef directives.

Features:
- Full printf functionality (%d, %i, %u, %o, %x, %X, %f, %F, %c, %s, %p, %%)
- Support for flags: '-', '+', ' ', '0', '#'
- Width and precision specification (including * for both)
- Length modifiers: h, hh, l, ll, z, t, j
- Floating point support with configurable precision
- Plugin system for custom format specifiers
- Buffer functions (sprintf, snprintf)
- No dynamic memory allocation
- Header-only implementation
- Locale support for decimal point
- Configurable features via preprocessor definitions

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

Handler registration:
  int u_register_format_handler(char specifier, u_format_handler_t handler)
  int u_unregister_format_handler(char specifier)

Configuration:
  void u_set_locale(const char* locale)
  void u_set_float_support(bool enabled)
  void u_set_default_output(u_output_cb_t output_cb, void* ctx)
  int u_printf_simple(const char* fmt, ...)

Note on u_snprintf: This function returns the number of characters actually
written to the buffer, not the number that would be written if the buffer was
large enough (unlike standard snprintf).

------------------------------------------------------------------------------=
LICENSE:
------------------------------------------------------------------------------=

MIT License

Copyright (c) 2023 Ferki

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