from registers import XED_REGISTERS


def generate_registers() -> dict:
    """
    TODO: docs
    """
    output = {}
    for i, register_name in enumerate(XED_REGISTERS):
        value = i # TODO; replace with an interesting binary generator (open for discussion)
        output.update({register_name: value})
    return output

def compile_for_sgx_registers(registers: dict) -> dict:
    """
    TODO: docs
    """
    output = ""
    for register_name, value in registers.items():
        output += f"\tsgx.{register_name} = 0x{value};\n"
    return output

def generate_copy_code() -> str:
    output = ""
    for register_name in XED_REGISTERS:
        output += f"\tcontext.{register_name} = sgx.{register_name};\n"
    return output

if __name__ == "__main__":
    with open("template.c" , 'r') as f:
        template_code = f.read()
    
    registers: dict = generate_registers()
    
    output = template_code.replace("    /* <--FILL-PLACEHOLDER--> */", compile_for_sgx_registers(registers))
    
    output = output.replace("    /* <--COPY-PLACEHOLDER--> */", generate_copy_code())
    
    with open("output.c" , 'w+') as f:
        f.write(output)