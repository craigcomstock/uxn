#include <stdio.h>
#include <stdlib.h>

#include "../uxn.h"
#include "system.h"

/*
Copyright (c) 2022-2023 Devine Lu Linvega, Andrew Alderwick

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

static const char *errors[] = {
	"underflow",
	"overflow",
	"division by zero"};

static void
system_print(Stack *s, char *name)
{
	Uint8 i;
	fprintf(stderr, "<%s>", name);
	for(i = 0; i < s->ptr; i++)
		fprintf(stderr, " %02x", s->dat[i]);
	if(!i)
		fprintf(stderr, " empty");
	fprintf(stderr, "\n");
}

void
system_inspect(Uxn *u)
{
	system_print(u->wst, "wst");
	system_print(u->rst, "rst");
}

int
uxn_halt(Uxn *u, Uint8 instr, Uint8 err, Uint16 addr)
{
	Uint8 *d = &u->dev[0x00];
	Uint16 handler = GETVEC(d);
	if(handler) {
		u->wst->ptr = 4;
		u->wst->dat[0] = addr >> 0x8;
		u->wst->dat[1] = addr & 0xff;
		u->wst->dat[2] = instr;
		u->wst->dat[3] = err;
		return uxn_eval(u, handler);
	} else {
		system_inspect(u);
		fprintf(stderr, "%s %s, by %02x at 0x%04x.\n", (instr & 0x40) ? "Return-stack" : "Working-stack", errors[err - 1], instr, addr);
	}
	return 0;
}

/* MMU */

Uint8 *
mmu_init(Mmu *m, Uint16 pages)
{
	m->length = pages;
	m->pages = (Uint8 *)calloc(0x10000 * pages, sizeof(Uint8));
	return m->pages;
}

void
mmu_copy(Uint8 *ram, Uint16 length, Uint16 src_page, Uint16 src_addr, Uint16 dst_page, Uint16 dst_addr)
{
	Uint16 i;
	for(i = 0; i < length; i++) {
		ram[dst_page * 0x10000 + dst_addr + i] = ram[src_page * 0x10000 + src_addr + i];
	}
}

void
mmu_eval(Uint8 *ram, Uint16 addr)
{
	Uint16 a = addr;
	Uint8 o = ram[a++];
	if(o == 1) {
		Uint16 length = (ram[a++] << 8) + ram[a++];
		Uint16 src_page = ((ram[a++] << 8) + ram[a++]) % 16, src_addr = (ram[a++] << 8) + ram[a++];
		Uint16 dst_page = ((ram[a++] << 8) + ram[a++]) % 16, dst_addr = (ram[a++] << 8) + ram[a];
		mmu_copy(ram, length, src_page, src_addr, dst_page, dst_addr);
	}
}

/* IO */

void
system_deo(Uxn *u, Uint8 *d, Uint8 port)
{
	Uint16 a;
	switch(port) {
	case 0x3:
		PEKDEV(a, 0x2);
		mmu_eval(u->ram, a);
		break;
	case 0xe:
		if(u->wst->ptr || u->rst->ptr) system_inspect(u);
		break;
	}
}
