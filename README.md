# Introduction

This project is not intended to be used standalone, but as a ‘overlay’ blob 
which can be added to your bare metal project and provide the users with a 
minimalist terminal prompt. The prompt can have few commands associated with 
your function pointers (callbacks) and either invoke regular features of your 
application, or trigger some tests which can be automated with frameworks such 
as robot. The goal is to provide a lightweight prompt for scripted automated 
tests and a human friendly interactive use-case. It’s structured in such a way 
that in the future different transport layers could be implemented, but 
currently it’s just using a UART.

# Targets

Mainly focusesd on GCC bare-metal projects, tested with Arm and RISC-V targets,
both 32-bit and 64-bit. This project currently is not suitable for very low-end
devices such as 8-bit with 1KB of RAM. Create issue if a 8-bit port of this 
project would be useful to somebody and I could make that port.

This project should work with other compilers after some tweaking, but by 
default it's using GCC's features. 


# Mixing types

Using the fixed-width types, such as `uint32_t` together with the native types 
`unsigned int` (which can have different size depending on the platform) is 
usually discouraged, but here it’s intentional. When the 32-bit numbers are 
needed then `uint32_t` is used and when a native type is needed, then 
`unsigned int` is used. In case of `unsigned int` it can be small as 16-bit 
(depending on the target), but that is ok, usually it’s used for small values 
in for loops anyway and the intention is to use native width of the platform. 
On X86 this might be different but on CISC platforms, but this is bare-metal 
project so it’s intedend to be used on small embedded devices, which are usually 
RISC based.

The RISC devices rarely have many extra instructions and might not handle 
non-native sizes of the types with single instructions. Using a 32-bit variable 
on a 64-bit RISC target might cause overhead when the target is unecesary 
emulating a 32-bit register within a 64-bit register. With RISC targets, 
these variables often will not save memory as they often fit into the large 
set of RISC’s registers and are scoped in such way that they never leave the 
registers. In some cases, there is no memory footprint improvement for using 
a smaller type, yet it might cause runtime overhead. Fixing the width of 
variables to uint64_t is not a solution because then it is causing overhead 
on 32-bit targets where two 32-bit registers will be utilized just to hold 
a value which can fit into a 16-bit variable.

Therefore, some variables are intentionally typed into the native built-in 
types and their width left to change depending on the targets. Usually this 
is undesired and the cause of portability bugs, but here it’s deliberate.

# The name

`Metal Prompt` is a play with words that this is a minimalistic bare-metal 
prompt. The `MiniPrompt` and `bare-prompt` names are already taken. They and are 
`bash` prompts for Linux, while this prompt is meant to be used on non-Linux 
targets and have the same name as existing project would create confusion. 

# How to use it

To start using the prompt a specific transport needs to be included, then 
configuring your UART instance to a baud rate in your application and then pass 
on the instance. Or make your own transport implementation which will configure 
the instance inside itself. Letting the application to configure everything is 
more versatile.

```c

// Use some specific transport implementation
#include “test_interface/transport/pfsoc_mss_uart.h”

int main() {

    // configure UART instance, baud rate, parity, stop bits...

    m_p_prompt(&myUartInstance);

    // do something after the cmd line quit (if the config.h allows the cmd_line
    // to quit)

    return 0;
}

```

The `config.h` has multiple options to enable/disable some features or tweak 
aspects of the test interface prompt. 

# Adding commands

When adding own commands, first your project has to describe your callbacks 
in a structure which then can be added to the `enabled_commads.c`. Then they 
will be parsed and accessible from the command prompt. Look into 
`bundled_commands.c` To see example how the command structure is populated 
with callbacks which can have different arguments as inputs and different 
return types.

# Constrains

- Doesn’t assume a `heap` to be present in the system, therefore the buffers 
are allocated globally instead of `malloc`. 

- Is avoiding use of `printf/sprintf/scanf` functions as they can be pretty 
heavy from bare-metal perspective. Therefore, a section of code doing formatted 
output might look awkward as it’s begging for a `printf` to be used instead. 

- The code doesn’t need access to a systick interrupt timer function, but if 
one is provided, then `uptime` and `benchmark` features can be used from 
the command prompt.

