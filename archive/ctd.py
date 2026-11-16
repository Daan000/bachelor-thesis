import subprocess


def run_ctd_test(offset, instruction):
    # Pass the command and its arguments as a list
    result = subprocess.run(["./ctd/main", str(hex(offset)), str(hex(instruction))], capture_output=True, text=True)

    if result.returncode != 0:
        raise ValueError(f"Returncode is {result.returncode}.")
    if len(result.stderr) > 0:
        raise ValueError(f"Standard error is {result.stderr}.")

    # Parse "rv = 2; ans=aaaacaca"
    last_line = result.stdout.split("\n")[-1]
    rv, ans = (
        int(last_line.split("; ")[0].split(" = ")[1]),
        int(last_line.split("; ")[1].split('=')[1], base=16)
    )
    # if rv != 0:
    #     raise ValueError(f"CTD Return Code was {rv}.")
    return ans


if __name__ == "__main__":
    run_ctd_test(0xdeadbeefcafebabe)