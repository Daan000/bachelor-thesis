void dump_hex(uint8_t *buf, int len)
{
    for (int i = 0; i < len; i++)
        printf("%02x", *(buf + i));
    printf("\n");
}