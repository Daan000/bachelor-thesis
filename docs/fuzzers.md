# Fuzzers

## Mening

**AFL++**:
- lijkt een zeer goed begin
- heel snel bij start
- enkel niet goed in constant time violations te spotten
    -> kan met `dudect` in het harnas, met een student t-test

**LibAFL**:
- Heel snel
- heel aanpasbaar, maar ook complex om aan te passen

**Custom Fuzzer**:
- Goed voor constant time violations te vinden ~ je kan een counter uitlezen
- Heel complex om te schrijven

## Conclusie

Start zeker met AFL++.

Indien we willen uitbreiden naar timing violations,
dan kunnen we dat herbekijken.