import archive.ctd as ctd


def main():
    print("Running tests")
    ans = ctd.run_ctd_test(0xde, 0x10)
    print(hex(ans))


if __name__ == "__main__":
    main()