import ctd


def main():
    print("Running tests")
    ans = ctd.run_ctd_test(0xdeadbeefcafebabe)
    print(hex(ans))


if __name__ == "__main__":
    main()