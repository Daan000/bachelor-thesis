import subprocess
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CLI = ROOT / "cli"

CLI_CASES = [
    {
        "name": "absolute 64-bit memory operand",
        "arguments": ("0", "0x48a1efbeadde00000000"),
        "expected_output": (
            "instr_data       : 48a1efbeadde00000000000000000000",
            "instr_data <<  0 : 48a1efbeadde00000000000000000000",
            "[CTD] rv = 0; ans=0x8",
            "[XED] i = mov rax, qword ptr [0xdeadbeef]",
            "[XED] xed_memop0 = 0xdeadbeef",
        ),
    },
    {
            "name": "absolute 64-bit memory operand",
            "arguments": ("0", "0x48035c9110"),
            "expected_output": (
                "instr_data       : 48035c91100000000000000000000000",
                "instr_data <<  0 : 48035c91100000000000000000000000",
                "[CTD] rv = 1; ans=0x1e",
                "[XED] i = add rbx, qword ptr [rcx+rdx*4+0x10]",
                "[XED] xed_memop0 = 0x10",
            ),
    },
]


class CliRegressionTests(unittest.TestCase):
    def test_cli_cases(self):
        for case in CLI_CASES:
            with self.subTest(case=case["name"]):
                result = subprocess.run(
                    [str(CLI), *case["arguments"]],
                    cwd=ROOT,
                    capture_output=True,
                    text=True,
                    check=False,
                )

                self.assertEqual(
                    result.returncode,
                    0,
                    msg=f"CLI exited with {result.returncode}.\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}",
                )
                for expected in case["expected_output"]:
                    self.assertIn(expected, result.stdout)


if __name__ == "__main__":
    unittest.main()
