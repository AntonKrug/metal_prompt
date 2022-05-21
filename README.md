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

#include “test_interface/transport/pfsoc_mss_uart.h”

int main() {

    // configure UART instance

    test_interface_cmd_line(&myUartInstance, “name of my application”);

    

    // do something after the cmd line is quit

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

