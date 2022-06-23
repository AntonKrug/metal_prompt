# Introduction

This project is not a standalone library, but an ‘overlay’ blob 
which can be added on top of your bare-metal project and provide the users with a minimalist terminal prompt. The prompt can be used for automated testing or interactive use by the end-user. Users can interactively type commands through their serial UART (or another transport interface) and invoke C function callbacks associated with the commands. It's possible to pass through an argument to the command/callbacks and read back its return values too.

Project goals: 
- Bare-metal only
- Equally suited for interactive use and automated testing
- Low overhead 
- Portability 

## Bare-metal only
If you need a Linux shell, then this is not the right project. It will never be in the roadmap to become one. And it will never try to be a fully-fledged bare-metal prompt either, adding too many features could affect portability goals or the low overhead goals. It could be seen as a very simple callback invoker with some additional (and optional) features around it.

## Interactive use
The project has dedicated features for interactive use to improve user-friendliness, which might make look like a more feature-rich prompt. 

- Autocomplete commands when pressing `TAB`.
- Build-in help which displays associated argument and return types for each command.
- Command history (pressing up arrow to recall the last command).
- `ls` command to print all possible commands.
- Small help command explaining the usage of the prompt.
- Memory dump commands (byte aligned and native int aligned) to interactive poke memory regions without halting the target with a JTAG (or other) debugger.

## Automated testing
To make automated testing easier with this prompt:
- It was meant to be used with the Robot framework, which allows developers to write human-friendly tests.
- Colours can be disabled either on runtime or compile-time, to make it easier to be parsed by scripts.
- New sets of commands can be either 
  - Added manually into the `commands_enabled.c` (not very automation-friendly).
  - Or by modifying the `commands_enabled.c` with a script. There are unique and fixed marker comments in the code which can be used by scripts to detect the right sections of the code to modify (slightly more automation-friendly).
  - Or by providing the right defines (described later in this readme) when compiling which doesn't need any change of the metal prompt code. And allows rapid change of commands with just changing the defines and recompiling the project.
- Ability to change transport interfaces without changing the code and only by providing different defines at compile time.
- `configuration` command which can describe back how the project was compiled, telling apart debug/release builds, detect some optimization defines etc... This can be useful as some tests might change their results depending on the optimization levels. For example using `-O2` or higher on RISC-V devices which have F-extension might start using fused floating-point instructions which do multiplication and addition in one step and only one round of rounding (no pun intended). In some edge cases this provides extra 1-bit precision and over time can accumulate to very different results. Tests might want to have different `expect` values depending on how the target was compiled. `configuration` command can provide some of this information back to the test framework on runtime.
- Namespaces, allow attaching the commands with a unique group name, allowing many different groups of commands to overlap without conflict, for example `gpio.self_test` and `i2c.self_test`. Duplicate entries are allowed (with minimal overhead) which can overlap into already existing namespaces. For example, this can be useful for having the gpio tests attached twice, with an empty namespace (which overlaps with the bundled commands) while having it attached the second time under an official `gpio` namespace. This allows interactive users to type less and invoke the tests interactively and quickly (example: `self_test`), while at the same time being able to write tests which will use the official namespace (example: `gpio.self_test`).

## Low overhead
- No heap, this however means that buffers are preallocated as globals, or on the stack.
- No printf, on embedded devices 
- Able to disable functionality which will not be used (see the`config.h`):
  - Support/parsing of unneeded arguments and return types can be disabled (when all types are disabled then only void callback(void); is supported).
  - And selectively can disable every other optional feature.
  - This compile-time flexibility has a negative effect on the source code readability as it polluted the code with lots of `#ifdef` statements. This is not a positive feature, but noteworthy side effect and reason for some long-term decisions.
- Forced optimization (by default `-Os`) on all metal prompt functions, the application can be run in the debug mode (including user's callbacks) and the user can debug his application, but generally doesn't want to debug the prompt itself. Even if the whole project will be compiled with `-O0`, then still the prompt's functions will use `-Os`. If this is undesired then just comment out one line in the `config.h` file (or it can be replaced with a different optimization setting).

## Portability
- Prompt should run on 8-bit to 64-bit targets. Focusing on RISC embedded devices such as RISC-V, Arm and AVR.
- Allowing users to write their transport protocol implementations and be able to use different types of transports and not just UART. I hope the API is generic enough to make transports such as telnet doable.
- Despite it being portable between the targets (testing mostly 32-bit and 64-bit targets), it's not currently portable between compilers, it was made for `gcc`. This could be extended to other compilers in the future, but I have no plans to do so (it should be possible, but I'm not planning to impelemnt it).

# The name

`Metal Prompt` is a play with words that this is a minimalistic bare-metal 
prompt. Other names, such as the `MiniPrompt` and `bare-prompt` names are already taken by different projects, so this is the best name I could think of while being unique (do not like overlapping names of tools which then are confusing the google searches). 

# How to use it - user perspective

Using it as a user means just typing the commands, (depending on if enabled in `config.h`) the autocomplete `TAB` can be used on an empty line to list all possible commands. Commands which have no namespace can be used directly `my_command`, while commands within a namespace can be used like this: `my_namespace.my_command`. Here is the list of all bundled commands (depending on the `config.h` many of them might not be present):

- `clear` Sends the VT100 escape character to clear the screen.
- `configuration` Self-reports if the target is running debug/release configuration and what optimizations are used.
- `dump_byte` Argument needs to be a memory address, following from this address another 160 bytes are read (aligned to byte) and displayed.
- `dump_word` Argument needs to be a memory address, following from this address another 160 bytes are read (aligned to unsigned int) and displayed. On 8-bit targets this means 16-bit alignment while on other targets this might 16-bit alignment.
- `help` It will display short help what can be done from within the prompt (depending what is enabled in the `config.h`).
- `ls` List all possible commands, same as pressing `TAB` on empty prompt.
- `print_in_dec` Commands which return `unsigned int`, `uint32_t` or `uint64_t` have their return values printed back on the prompt. This value can be either formatted in decimal or hexadecimal. Argument `0x1` will use the decimal format and `0x0` will use the hexadecimal format. 
- `set_color` If the colours are not desired, then they can be disabled with `set_color 0x0` and later enabled `set_color 0x1`. However more effective is to disable the colour support from `config.h` which will disable them on compile-time and will make runtime leaner.
- `set_benchmark` When systick feature is enabled then benchmarking mode can be enabled with `set_benchmark 0x1` and then each invoked command will report how many ticks it took to execute
- `uptime` When systick feature is enabled then it will report how many ticks of the systick happened since the powerup.
- `quit` Will quit the prompt's loop and return to the original application.

# How to use it - developer perspective

To start using the prompt
- Include a transport implementation `#include "metal_prompt/transport/implementation.h"` which will contain all transport inmplementations (but all disabled with `ifdefs`)
- A specific transport needs to be considered and it's define defined (preferably in your managed project settings, or makefile). Currently, one from only these two implementations can be enabled:
  - `M_P_TRANSPORT_IMPLEMENTATION_PF_FABRIC_CORE_UART_APB`
  - `M_P_TRANSPORT_IMPLEMENTATION_PFSOC_MSS_UART`
- That transport then needs to be configured, for example for UART the baud rate, stop bits, parity.
- Then a reference to that transport instance needs to be given to the metal prompt (example `m_p_prompt(&myUartInstance);`), which will start the main loop (and depending on the `config.h` might never exit). The signature of the `m_p_prompt` changes depending on what transport was enabled. And theoretically, the whole configuration can happen inside the `m_p_prompt` so some transport implementations might not require any configuration before invoking `m_p_prompt`

Example:

```c

// Use the global all-in-one implementation 
#include "metal_prompt/transport/implementation.h"

int main() {
    // configure UART instance, baud rate, parity, stop bits...

    m_p_prompt(&myUartInstance);

    // do something after the m_p_prompt quits,
    // if the config.h allows the prompt to quit

    return 0;
}

```

Note: Don't forget to check `config.h` to see what can be disabled/enabled and what values can be tweaked. 

When running the prompt, type `help` and `ls` commands to see more, commands who expect argument `char *` can be given string straight away with spaces and without quotes, for example `lcd.print Hello world!`. While commands with a numeric argument type accept only a single argument, which must be in the hex format (case insensitive), for example `math.double_number 0xaf`.

# Adding new commands
First, the callbacks for the commands have to be written. The callbacks either can have void argument, or 1 argument from the list of supported types, see `config.h`.
The user either can use the types as they are and let the compilation fail with an error if some will use disabled features, or have #ifdefs guarding the fancier commands and if they will not be supported by the `config.h` then they will not be added, letting the compilation always succeed but have a different set of commands on runtime.

Then the commands need to be referenced in a m_p_command array (null-terminated), each entry is one command:
- The first element is the name of the command, then describing its capabilities
- The second element describes what types it will return and what type will be the argument
- The last element is the callback, which needs to be attached to the corresponding entry. For example, the `.uint32_chars` is a pointer to a callback which will return `uint32_t` and has `char *` as an argument. So that is the matching pointer for the `M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_CHARS)` types.

Here is an `example.c` implementing 2 callbacks, `something` and `args`:

```c
#include "example.h"

void something() {
  // do something
}

uint32_t args(char *input_string) {
  return 10u;
}

const m_p_command my_commands_list[] = {
  { "something", M_P_CMD_TYPES(M_P_TYPE_VOID,   M_P_TYPE_VOID),  .void_void    = { &something} },
  { "args",      M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_CHARS), .uint32_chars = { &args } },
  { NULL }  // Terminator of the list
};
```

Corresponding `example.h` just exposes it to the outside world:

```c
#ifndef EXAMPLE_COMMANDS_H_
#define EXAMPLE_COMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "metal_prompt/structures.h"
extern const m_p_command my_commands_list[];

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_COMMANDS_H_ */
```

More examples can be seen in `commands_bundled.c` and  `commands_bundled.h`.

# Attaching new commands 
The previous section described the commands, but they are not attached to the prompt yet, they need to be attached with their own names. Then the commands can be used `my_namespace.something`, when empty "" namespace is given then the commands can be invoked directly `something`. This is done inside the `commands_enabled.c` which can be edited directly there or left untouched but controlled with defines. For example to attach the above `example.c` we would need to compile the project with the following three defines:
- `M_P_ADD_CUSTOM_COMMAND_INCLUDE="\"example.h\""`
- `M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_NAME="namespace"`
- `M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_LIST=my_commands_list`

Then the prompt will be able to run commands with `namespace.something`.
If the defines are not suitable for the user, then the `commands_enabled.c` can be edited with a script as well (see the code, there are dedicated and fixed comments acting as anchors for the script to find).

# Systick and benchmarks
Inside the `config.h` a systick/uptime feature can be added. First it is required to include metal_prompt's systick handler `#include "metal_prompt/systick.h"` and then call find the application/system systick handler and call metal_prompt's handler from within it. `m_p_systick_handler();`.

After this is done then `uptime` and `set_benchmark 0x1` commands will be usable. Uptime will show how many ticks were executed since power up and when benchmark mode is enabled `set_benchmark 0x1` then every command will report how long it took for it to be executed (can be useful for slower tests which might need to be benchmarked).

If the system doesn't have systick, then it can be disabled in the `config.h` and everything else will work just fine, it's not crucial for the prompt to work.

# Ugliness
Because some goals had negative side effects. I will go over the non-pretty parts of this prompt

## Lots of ifdefs
Because almost everything can be enabled/disabled on compile-time, to has very low overhead and be portable so even small targets can run it. This means that there is lots of #ifdes which doesn't make the code pretty.

## Lots of strcpy,strcat
There are places in the code begging for a printf to be used, but our goal was to avoid it having low overhead. Again code readability suffered with this.

## Avoiding heap
In some places maybe malloc and free would be more suiting instead of hardcoded buffers, but many bare-metal applications live without heap and I didn't want this prompt to impose new dependencies and requirements on the targets.

## Mixing types

Using the fixed-width types, such as `uint32_t` together with the native types 
`unsigned int` (which can have a different size depending on the platform) is 
usually discouraged, but here it’s intentional. When the 32-bit numbers are 
needed then `uint32_t` is used and when a native type is needed, then 
`unsigned int` is used. In case of `unsigned int` it can be small as 16-bit 
(depending on the target), but that is ok if it's used to pass boolean or small values.

If the code would just use the `uint32_t` everywhere instead of `unsigned int` then it would cause a lot of overhead on smaller 8-bit targets. And if a `uint16_t` would be used everywhere instead, then on bigger platforms it could cause a waste of resources. This is because of the project's focus on RISC embedded targets, which might not have dedicated instructions to deal with smaller registers and could waste resources emulating the behaviour of a smaller type. Therefore this project uses in some places intentionally `unsigned int` to allow its size to change depending on the target, which usually by coding standards is not recomended.


