#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>

#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

// Test output context for capturing output
typedef struct {
    char buffer[1024];
    size_t position;
    size_t max_size;
} test_ctx_t;

static void test_output_cb(char c, void* ctx) {
    test_ctx_t* context = (test_ctx_t*)ctx;
    if (context->position < context->max_size - 1) {
        context->buffer[context->position++] = c;
    }
    context->buffer[context->position] = '\0';
}

static void reset_test_ctx(test_ctx_t* ctx) {
    ctx->position = 0;
    ctx->buffer[0] = '\0';
    ctx->max_size = sizeof(ctx->buffer);
}

// Basic output test
static void test_basic_output() {
    test_ctx_t ctx;
    reset_test_ctx(&ctx);
    
    u_printf(test_output_cb, &ctx, "Hello, World!");
    assert(strcmp(ctx.buffer, "Hello, World!") == 0);
    printf("✓ Basic output test passed\n");
}

// Integer formatting tests
static void test_integer_formatting() {
    test_ctx_t ctx;
    
    // Test %d
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%d", 42);
    assert(strcmp(ctx.buffer, "42") == 0);
    
    // Test negative
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%d", -42);
    assert(strcmp(ctx.buffer, "-42") == 0);
    
    // Test width
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%5d", 42);
    assert(strcmp(ctx.buffer, "   42") == 0);
    
    // Test zero padding
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%05d", 42);
    assert(strcmp(ctx.buffer, "00042") == 0);
    
    // Test left align
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%-5d", 42);
    assert(strcmp(ctx.buffer, "42   ") == 0);
    
    printf("✓ Integer formatting tests passed\n");
}

// Unsigned integer tests
static void test_unsigned_formatting() {
    test_ctx_t ctx;
    
    // Test %u
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%u", 12345);
    assert(strcmp(ctx.buffer, "12345") == 0);
    
    // Test large value
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%u", 4294967295U);
    assert(strcmp(ctx.buffer, "4294967295") == 0);
    
    printf("✓ Unsigned integer tests passed\n");
}

// Hexadecimal tests
static void test_hex_formatting() {
    test_ctx_t ctx;
    
    // Test %x lowercase
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%x", 0xABCDEF);
    assert(strcmp(ctx.buffer, "abcdef") == 0);
    
    // Test %X uppercase
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%X", 0xABCDEF);
    assert(strcmp(ctx.buffer, "ABCDEF") == 0);
    
    // Test alt form
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%#x", 0xABC);
    assert(strcmp(ctx.buffer, "0xabc") == 0);
    
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%#X", 0xABC);
    assert(strcmp(ctx.buffer, "0XABC") == 0);
    
    printf("✓ Hexadecimal tests passed\n");
}

// Octal tests
static void test_octal_formatting() {
    test_ctx_t ctx;
    
    // Test %o
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%o", 511);
    assert(strcmp(ctx.buffer, "777") == 0);
    
    // Test alt form
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%#o", 511);
    assert(strcmp(ctx.buffer, "0777") == 0);
    
    printf("✓ Octal tests passed\n");
}

// Character tests
static void test_char_formatting() {
    test_ctx_t ctx;
    
    // Test %c
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%c", 'A');
    assert(strcmp(ctx.buffer, "A") == 0);
    
    // Test width
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%5c", 'A');
    assert(strcmp(ctx.buffer, "    A") == 0);
    
    printf("✓ Character tests passed\n");
}

// String tests
static void test_string_formatting() {
    test_ctx_t ctx;
    
    // Test %s
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%s", "hello");
    assert(strcmp(ctx.buffer, "hello") == 0);
    
    // Test width
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%10s", "hello");
    assert(strcmp(ctx.buffer, "     hello") == 0);
    
    // Test left align
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%-10s", "hello");
    assert(strcmp(ctx.buffer, "hello     ") == 0);
    
    // Test precision
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%.2s", "hello");
    assert(strcmp(ctx.buffer, "he") == 0);
    
    // Test null
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%s", NULL);
    assert(strcmp(ctx.buffer, "(null)") == 0);
    
    printf("✓ String tests passed\n");
}

// Pointer tests
static void test_pointer_formatting() {
    test_ctx_t ctx;
    int x;
    
    // Test %p
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%p", &x);
    // Can't compare directly, but should contain 0x
    assert(strstr(ctx.buffer, "0x") != NULL);
    
    printf("✓ Pointer tests passed\n");
}

// Float tests (if enabled)
static void test_float_formatting() {
#if UPRINTF_FLOAT_SUPPORT
    test_ctx_t ctx;
    
    // Test %f
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%f", 3.14159);
    assert(strstr(ctx.buffer, "3.14159") != NULL);
    
    // Test precision
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%.2f", 3.14159);
    assert(strcmp(ctx.buffer, "3.14") == 0);
    
    // Test negative
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%f", -3.14159);
    assert(strstr(ctx.buffer, "-3.14159") != NULL);
    
    printf("✓ Float tests passed\n");
#endif
}

// Length modifier tests
static void test_length_modifiers() {
    test_ctx_t ctx;
    
    // Test short
    reset_test_ctx(&ctx);
    short s = 123;
    u_printf(test_output_cb, &ctx, "%hd", s);
    assert(strcmp(ctx.buffer, "123") == 0);
    
    // Test long
    reset_test_ctx(&ctx);
    long l = 123456789;
    u_printf(test_output_cb, &ctx, "%ld", l);
    assert(strcmp(ctx.buffer, "123456789") == 0);
    
    // Test long long
    reset_test_ctx(&ctx);
    long long ll = 123456789012345LL;
    u_printf(test_output_cb, &ctx, "%lld", ll);
    assert(strcmp(ctx.buffer, "123456789012345") == 0);
    
    printf("✓ Length modifier tests passed\n");
}

// Buffer function tests
static void test_buffer_functions() {
    char buffer[100];
    
    // Test sprintf
    int len = u_sprintf(buffer, "Hello, %s!", "World");
    assert(strcmp(buffer, "Hello, World!") == 0);
    assert(len == 13);
    
    // Test snprintf
    len = u_snprintf(buffer, 10, "Hello, %s!", "World");
    assert(strcmp(buffer, "Hello, Wo") == 0);
    assert(len == 9);
    
    printf("✓ Buffer function tests passed\n");
}

// Edge case tests
static void test_edge_cases() {
    test_ctx_t ctx;
    
    // Test empty string
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "");
    assert(strcmp(ctx.buffer, "") == 0);
    
    // Test percent sign
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%%");
    assert(strcmp(ctx.buffer, "%") == 0);
    
    // Test unknown specifier
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%z");
    assert(strcmp(ctx.buffer, "%z") == 0);
    
    printf("✓ Edge case tests passed\n");
}

// Custom format handler test
static int custom_handler(u_output_cb_t output_cb, void* ctx, va_list* args, 
                         const char** fmt, int width, int precision, 
                         unsigned int flags) {
    const char* str = "CUSTOM";
    u_output_str(output_cb, ctx, str, -1);
    return 6;
}

static void test_custom_handlers() {
    test_ctx_t ctx;
    
    // Register custom handler
    u_register_format_handler('K', custom_handler);
    
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%K");
    assert(strcmp(ctx.buffer, "CUSTOM") == 0);
    
    // Unregister
    u_unregister_format_handler('K');
    
    reset_test_ctx(&ctx);
    u_printf(test_output_cb, &ctx, "%K");
    assert(strcmp(ctx.buffer, "%K") == 0);
    
    printf("✓ Custom handler tests passed\n");
}

// Main test function
int main() {
    printf("Running uprintf tests...\n\n");
    
    test_basic_output();
    test_integer_formatting();
    test_unsigned_formatting();
    test_hex_formatting();
    test_octal_formatting();
    test_char_formatting();
    test_string_formatting();
    test_pointer_formatting();
    test_float_formatting();
    test_length_modifiers();
    test_buffer_functions();
    test_edge_cases();
    test_custom_handlers();
    
    printf("\nAll tests passed! \n");
    return 0;
}