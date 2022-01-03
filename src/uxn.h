/*
Copyright (c) 2021 Devine Lu Linvega

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned short Uint16;
typedef signed short Sint16;
typedef unsigned int Uint32;

#define PAGE_PROGRAM 0x0100

/* clang-format off */

#define DEVPEEK16(o, x) { (o) = (d->dat[(x)] << 8) + d->dat[(x) + 1]; }
#define DEVPOKE16(x, y) { d->dat[(x)] = (y) >> 8; d->dat[(x) + 1] = (y); }

/* clang-format on */

typedef struct {
	Uint8 ptr;
	Uint8 dat[256];
} Stack;

typedef struct {
	Uint16 ptr;
	Uint8 *dat;
} Memory;

typedef struct Device {
	struct Uxn *u;
	Uint8 addr, dat[16], *mem;
	Uint16 vector;
	Uint8 (*dei)(struct Device *d, Uint8);
	void (*deo)(struct Device *d, Uint8);
} Device;

typedef struct Uxn {
	Stack wst, rst;
	Memory ram;
	Device dev[16];
} Uxn;

Uint16 peek16(Uint8 *m, Uint16 a);

int uxn_boot(Uxn *c, Uint8 *memory);
int uxn_eval(Uxn *u, Uint16 vec);
int uxn_halt(Uxn *u, Uint8 error, char *name, int id);
Device *uxn_port(Uxn *u, Uint8 id, Uint8 (*deifn)(Device *, Uint8), void (*deofn)(Device *, Uint8));
