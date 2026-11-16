# Fuzzing

## AFL++

### Install

Follow the official repo install guide: [INSTALL.md](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/INSTALL.md)

### Running AFL++

To avoid Ubuntu hiding crashes (which makes AFL++ see "hangs" but not crashes), first run:

```bash
sudo afl-system-config
```

At this point, crashes should be seen by AFL++.

### Things to use / know

- **ASAN** needs to be enabled: `AFL_USE_ASAN=1`
- It is possible to use different sanitizer to detect vulns, not just ASAN. Look at LLVM docs: [1](https://clang.llvm.org/docs/AddressSanitizer.html) [2](https://clang.llvm.org/docs/MemorySanitizer.html) [3](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html) [4](https://github.com/google/sanitizers/wiki/AddressSanitizerFlags), use this link to see how to properly use them: https://securitylab.github.com/resources/fuzzing-software-2/
- many things are only available on the LLVM instrumentation, so try to stick with that with the most recent clang
- Coverage (edge coverage) is collected by default with the normal instrumentation, e.g. `afl-clang-lto`.
- Use `afl-clang-lto`, not `afl-clang-fast`. Link-time optimization has one defect to keep in mind: `dlopen()` may miss coverage or crash if libraries load dynamically post-forkserver, unless using `AFL_PRELOAD`.
- `-m none` is already the default. On older versions, set the flag to remove the target's memory allocation limit (needed because of ASAN's shadow memory usage).
- `-x PATH_TO_DICT` specifies the dictionary file.
- `-D` enables deterministic mutations (only for the master fuzzer in parallel master/slave fuzzing).
- Persistent mode use/re-use a single process, giving better performance **BUT ONLY WITH STATELESS TARGET**.
- Closed-source program fuzzing is possible with QEMU mode and its features (persistent mode, QASAN, etc.).

### Partial instrumentation
This is useful for guiding the fuzzer, especially in modular programs with differnt files. We can use the list to compile only interesting files. When the fuzzer reach uninstrumented part of the code, it will "go back on track". like if a mutation touch a magic byte and instead of testing libreary png it starts to use jpeg.
Instrument only part of the target at compile time:

- `AFL_LLVM_ALLOWLIST=file` — instrument **only** what matches.
- `AFL_LLVM_DENYLIST=file` — instrument everything **except** what matches.

> These used to be called `AFL_LLVM_WHITELIST` and `AFL_LLVM_BLACKLIST`.

The file looks like this:

```
# instrument only the parser code
src: parser/*.c
src: lexer.c
fun: parse_header
fun: decode_*
```

Matching rules:

- `src:` entries match the end of the source file path, so `lexer.c` matches `/whatever/path/lexer.c`.
- `fun:` entries match function names.
- Wildcards are supported.
- Plain lines with only a filename also work (the older format).

### Runtime-selective coverage

There is also runtime-selective coverage via the `__AFL_COVERAGE()` / `__AFL_COVERAGE_ON()` / `__AFL_COVERAGE_OFF()` macros in the harness.

Everything stays instrumented, but coverage is only recorded during the parts of execution you choose — for example, only while the target function is running and not during setup. This can be combined with partial instrumentation.

Reference: `instrumentation/README.instrument_list.md` in the AFL++ repo.

### Custom coverage
 [FuzzFactory](https://github.com/rohanpadhye/FuzzFactory). FuzzFactory defines itself as “an extension of AFL that generalizes coverage-guided fuzzing to domain-specific testing goals”. It maximaze inputs score based on stuff like: Inputs that minimize the number of **failed assertions**, Inputs that maximize **dynamic memory allocations**, Maximization of **execution paths lengths**
 
### Fuzzing command-line parameters
parameters that are dynamic or flags that need to be fuzz as well could be tested with a trick. Essentially is to append the input parameters in the first part of the file as described here:
https://securitylab.github.com/resources/fuzzing-challenges-solutions-1/
in the page, they also mentioned that "Command-line arguments can also be added to the dictionary as tokens". 

### Structure-aware fuzzing
Essentially is the usage of cutom mutators that describe the grammar. Good if looking for going deeper, bypassing initail lexial / sintax checks E.g. finding sematics bugs in SQL. Good for protocol, good and in some cases for stateful systems. 
Not good for highly modular software; when dealing with costant values; when dealing with checksum that cannot be disables/bypassed. Look also at: https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/custom_mutators.md

### Custom oracles
AFL++ only detects crashes/violations. One possible trick is to manually insert crashing code like a memcpy to a null pointer, in a part of the code that is not supposed to be executed. For instance a "priviledge" part of the code that is not accessible from unpriviledge positions. If the fuzzer reach that part the crashing memcpy is executed and the vuln detected.  

### TRICKS
- the Laf-intel plugin compile the program splitting the string comparison as multi-single byte comparisons, allowing afl to guess one byte at a time, making more likely to pass through a string comparison: https://securitylab.github.com/resources/fuzzing-challenges-solutions-1/ 
- FuzzFactory, plugin that uses waypoints to better direct the fuzzers based on user provided metric
- MODE:
	- **Sub-process** (AFL_NO_FORKSRV=1), create a sub process, slow but useful if  SUT performs init before main
	- **Fork server**, fork for every testcase just before the main. Only available when fuzzing via standard input
	- **Persistent mode** (fastest option, x10 than forkserver), every testcase run in the same single process (look at doc how to enable it). Is important that the harness resets the state of the SUT clearly after each execution 
- **Multi-core** fuzzing allows run 1 master/primary and multiple slaves/secondary. It should be possible to have differnt confing like the master using AdressSanitaziner and slaves don't.
- **CMPLOG/RedQueen** `AFL_LLVM_CMPLOG=1` best constraint solver for fuzzing (like for magic bytes, checksums, leght fields, multibyte comparison... )
