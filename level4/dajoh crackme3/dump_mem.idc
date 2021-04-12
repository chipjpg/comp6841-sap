auto fname      = "dump_mem.bin";
auto address    = 0x0134E468;
auto size       = 0x400;
auto file= fopen(fname, "wb");

savefile(file, 0, address, size);
fclose(file);