from scripts.register_fill.registers import XED_REGISTERS


def generate_registers() -> dict:
    """
    Generate the *values* for the XED registers.
    e.g. {"RAX": 0x20, ...}
    """
    output = {}
    for i, register_name in enumerate(XED_REGISTERS):
        value = i+1 # TODO; replace with an interesting binary generator (open for discussion)
        output.update({register_name: value})
    return output

def compile_for_sgx_registers(registers: dict) -> dict:
    """
    Compile the values for the registers ({"RAX": 0x20, ...}) to C code ("sgx.RAX = 0x20;")
    """
    lines = []
    for register_name, value in registers.items():
        lines.append(f"\tsgx->{register_name} = 0x{value};")
    return '\n'.join(lines)

def generate_copy_code() -> str:
    lines = []
    for register_name in XED_REGISTERS:
        lines.append( f"\tcontext->{register_name} = sgx->{register_name};" )
    return '\n'.join(lines)

if __name__ == "__main__":
    with open("scripts/register_fill/template.c" , 'r') as f:
        template_code = f.read()
    
    registers: dict = generate_registers()
    
    output = template_code.replace("    /* <--FILL-PLACEHOLDER--> */", compile_for_sgx_registers(registers))
    
    output = output.replace("    /* <--COPY-PLACEHOLDER--> */", generate_copy_code())
    
    with open("src/register_fill.c" , 'w+') as f:
        f.write(output)