#pragma once
typedef struct {
	const char* name;
	int id;
} EXT_TABLE;

const EXT_TABLE ext_table[67] = {
	{"mdpe",  0x1A},
	{"qar",   0xF1},
	{"vrdv",  0x21},
	{"vrd",   0x20},
	{"mgm",   0x30},
	{"mds",   0x05},
	{"row",   0x6E},
	{"spk",   0x1D},
	{"cap",   0x35},
	{"rat",   0x6B},
	{"mtfa",  0x0A},
	{"eqp",   0x64},
	{"psq",   0xFF},
	{"dcd",   0x1B},
	{"mtst",  0x18},
	{"gcx",   0x02},
	{"cvd",   0x10},
	{"bgp",   0x38},
	{"ohd",   0x1E},
	{"tri",   0x03},
	{"rpd",   0x16},
	{"mdp",   0x13},
	{"vlm",   0x65},
	{"vcpg",  0x24},
	{"kms",   0x15},
	{"la2",   0x5F},
	{"ptcp",  0x33},
	{"vcp",   0x23},
	{"fcx",   0x17},
	{"ola",   0x6D},
	{"rcm",   0x6C},
	{"lt2",   0x06},
	{"olang", 0x5D},
	{"mtsq",  0x09},
	{"pcmp",  0x36},
	{"vram",  0x61},
	{"mdh",   0x04},
	{"mmd",   0x1F},
	{"bin",   0x01},
	{"mdpb",  0x19},
	{"img",   0x69},
	{"mdc",   0x13},
	{"vib",   0x6A},
	{"zon",   0x12},
	{"cddl",  0x34},
	{"txp",   0x14},
	{"vrdt",  0x22},
	{"nav",   0x0F},
	{"cmf",   0x63},
	{"png",   0x68},
	{"la3",   0x5E},
	{"lst",   0x66},
	{"dar",   0xF0},
	{"ypk",   0x1C},
	{"rlc",   0x32},
	{"mtra",  0x6F},
	{"geom",  0x0C},
	{"cv2",   0x07},
	{"prx",   0x31},
	{"mtar",  0x08},
	{"eft",   0x11},
	{"slot",  0x60},
	{"mdl",   0x13},
	{"mtcm",  0x0B},
	{"sep",   0x37},
	{"mdb",   0x13},
	{"cnf",   0xF2},
};

inline
const char* getExtForID(uint8_t id) {
	for (int i = 0; i < 67; i++) {
		if (ext_table[i].id == id)
			return ext_table[i].name;
	}
	return "";
}

inline
uint8_t getIDforExt(const char* ext) {
	for (int i = 0; i < 67; i++) {
		if (!strcmp(ext_table[i].name, ext))
			return ext_table[i].id;
	}
	return -1;
}