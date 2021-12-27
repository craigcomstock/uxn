/*
Copyright (c) 2021 Devine Lu Linvega
Copyright (c) 2021 Andrew Alderwick

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

void mouse_xy(Device *d, Uint16 x, Uint16 y);
void mouse_z(Device *d, Uint8 z);
void mouse_down(Device *d, Uint8 mask);
void mouse_up(Device *d, Uint8 mask);