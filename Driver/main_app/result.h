unsigned int error_tolerance = 0x000020c5; // 0.001 unsigned int 
unsigned int result[] = { 0x00ff5b49,0x00ff3a16,0x00ff95ae,0x00ff9dae,0x00ffc6cd, 
0x00ffae1d,0x00ff9f7b,0x00ff72a3,0x00ff4c37,0x00ff2118, 
0x00ff1fe7,0x00ff1941,0x00ff3820,0x00ff60d3,0x00ff9499, 
0x00ffc0cc,0x00ffe2e5,0x00ffffff,0x00000229,0x0000067e, 
0x00ffedbe,0x00fff102,0x00ffcf3d,0x00ffda85,0x00ffc832, 
0x00fff5f0,0x00fffe5c,0x0000316b,0x000055c9,0x000071cd, 
0x00009491,0x00008d40,0x0000a407,0x00008d50,0x0000824f, 
0x00006ed1,0x00004682,0x000031b4,0x0000134e,0x00001279, 
0x000015cc,0x000026fd,0x00002eec,0x000044eb,0x00004164, 
0x00003f16,0x00002ca8,0x0000197d,0x00fffd91,0x00fffc1d, 
0x00ffeeea,0x0000031b,0x0000201c,0x0000439b,0x000077fe, 
0x00007ca1,0x00009ac4,0x00008a96,0x000094ff,0x000068f0, 
0x00004647,0x00001c1c,0x00fff1ea,0x00ffbf01,0x00ffacc0, 
0x00ff91b4,0x00ffa774,0x00ffb747,0x00ffda3a,0x00fffa8f, 
0x00001a57,0x0000447b,0x00005a41,0x00006107,0x0000602c, 
0x000032f0,0x000021b2,0x00ffe7c5,0x00ffd971,0x00ffc0d2, 
0x00ffbc76,0x00ffc2ee,0x00ffbb8a,0x00ffc363,0x00ffbcf7, 
0x00ffaff3,0x00ffc597,0x00ffb541,0x00ffdbdf,0x00ffd166, 
0x00fff739,0x00fffd37,0x000006b9,0x000002eb,0x00fffaa4, 
0x00ffee78,0x00ffda93,0x00ffe1f9,0x00ffc9ee,0x00ffdff6, 
0x00ffda11,0x00fff863,0x000002c3,0x00001639,0x00000b57, 
0x000007a9,0x00fff30c,0x00ffd2c6,0x00ffc13f,0x00ffa925, 
0x00ffa94d,0x00ff99f7,0x00ffa74e,0x00ff9bc8,0x00ffa3af, 
0x00ffb0c0,0x00ffc522,0x00ffd56a,0x00ffec2f,0x00fff2a8, 
0x0000074a,0x00fff60a,0x00ffff1e,0x00ffdfc0,0x00ffd45c, 
0x00ffc590,0x00ffb1ae,0x00ffc60f,0x00ffb45f,0x00ffd09e, 
0x00ffd25c,0x00ffd63b,0x00ffc163,0x00ffdab9,0x00ffd255, 
0x00fff1fd,0x00fff981,0x000003c4,0x00fff5fa,0x00ffe608, 
0x00ffca16,0x00ffb515,0x00ffa3d7,0x00ff9e11,0x00ffa90d, 
0x00ffbf4a,0x00ffd77f,0x00ffdafe,0x00fff12f,0x00ffd792, 
0x00ffe178,0x00ffd2db,0x00ffd85f,0x00ffe834,0x000002ca, 
0x00001bd2,0x000039c1,0x00004dc6,0x000059f6,0x000077c3, 
0x000077e3,0x00007f3a,0x000055cc,0x0000423c,0x00003159, 
0x000022be,0x0000351c,0x000045f5,0x00006a55,0x00007b2c, 
0x0000a003,0x000095ee,0x0000a2d2,0x00008a05,0x00007651, 
0x000045d1,0x0000065a,0x00ffd61c,0x00ffad9d,0x00ff8d9a, 
0x00ff91f1,0x00ff9e8f,0x00ffc9d9,0x00ffe480,0x00001676, 
0x00002a31,0x0000433b,0x00004016,0x000045a2,0x000038ce, 
0x00005499,0x00004f64,0x0000536c,0x00004dec,0x00003750, 
0x00001bcf,0x00000660,0x00fff51b,0x00ffcc00,0x00ffa959, 
0x00ff99a2,0x00ffa03f,0x00ffda80,0x00000607,0x000043ea, 
0x00003142,0x00004026,0x0000206b,0x00001bfc,0x00fff658, 
0x00ffc4f8,0x00ff9b68,0x00ff5448,0x00ff1e9e,0x00feeb9d, 
0x00fecae3,0x00fef82c,0x00ff2814,0x00ff8230,0x00ffcbd8, 
0x000000e1,0x00001004,0x00fff9f7,0x00ffce0b,0x00ff9446, 
0x00ff79dd,0x00ff74a7,0x00ffab49,0x00ffee12,0x00002293, 
0x000043f1,0x00003c1c,0x000024ba,0x00001379,0x00000ab9, 
0x00fff1ca,0x00fff618,0x00ffe855,0x00fff921,0x00fff6ce, 
0x000001f8,0x00001e70,0x00002631,0x00002af9,0x00003839, 
0x00002c6f,0x000037f4,0x00002f25,0x00002718,0x0000303c, 
0x00001d30,0x000017c3,0x00001251,0x0000077f,0x00fffbe0, 
0x00ffdfdc,0x00ffcffc,0x00ffbaac,0x00ffb618,0x00ffbcee, 
0x00ffe3e7,0x0000026d,0x0000394f,0x00003757,0x00003193, 
0x000019e7,0x00ffe615,0x00ffc562,0x00ff9f81,0x00ff9e5a, 
0x00ffa5f2,0x00ffd327,0x00fffbb0,0x00001f74,0x00003412, 
0x00002e8e,0x00003265,0x000002b4,0x00ffeb1a,0x00ffa014, 
0x00ff6707,0x00ff2c98,0x00ff022c,0x00ff078c,0x00ff17a5, 
0x00ff5864,0x00ffa4bf,0x00ffef67,0x00002567,0x0000447c, 
0x00004ffa,0x00004453,0x00003050,0x00002932,0x00002b5c, 
0x0000458f,0x00004ccf,0x0000755f,0x00009621,0x0000a676, 
0x0000cc15,0x0000e37f,0x0000ec26,0x0000ed99,0x0000e781, 
0x0000c70f,0x00009ad4,0x00006a26,0x000041d8,0x00002d52, 
0x0000185d,0x000007e7,0x00ffe7fc,0x00ffdb89,0x00ffc348, 
0x00ffa6b9,0x00ff9f12,0x00ff8b2a,0x00ff8a8f,0x00ff712b, 
0x00ff81db,0x00ff616d,0x00ff6295,0x00ff4c71,0x00ff4190, 
0x00ff2cae,0x00ff3171,0x00ff3d41,0x00ff67f1,0x00ff904a, 
0x00ffbf12,0x00ffe42a,0x00ffe71a,0x00ffc9a9,0x00ffa7ab, 
0x00ff85d5,0x00ff6f90,0x00ff8508,0x00ff955c,0x00ffe2bd, 
0x00001ac4,0x00005c46,0x00008ca8,0x00009653,0x000090c1, 
0x0000741e,0x00005163,0x00003e6a,0x0000291f,0x00002bfe, 
0x00002e3b,0x0000534f,0x0000683b,0x00009637,0x00009919, 
0x0000b2c5,0x00009eef,0x0000a751,0x0000c602,0x0000d0aa, 
0x0000f53b,0x00010ba4,0x000133bb,0x00014bcc,0x00015e1b, 
0x00014746,0x0001121a,0x0000c734,0x0000743d,0x00001e4f, 
0x00ffddbf,0x00ffb12b,0x00ff9ce0,0x00ffa7e4,0x00ffb67a, 
0x00ffb3d2,0x00ffc7b7,0x00ffb6c9,0x00ffae06,0x00ff645d, 
0x00ff40e5,0x00ff0c21,0x00feffc9,0x00fef090,0x00ff0c87, 
0x00ff1a46,0x00ff4414,0x00ff3120,0x00ff1f69,0x00ff01e3, 
0x00feebf6,0x00fecf2c,0x00fec1c0,0x00fec9f3,0x00fecfc4, 
0x00fef6f9,0x00ff0a93,0x00ff3a54,0x00ff5c40,0x00ff8972, 
0x00ffbec6,0x00fffbce,0x000044e7,0x00007844,0x0000c283, 
0x0000d4c6,0x0000f22a,0x0000d852,0x0000ce01,0x0000a207, 
0x000096ec,0x0000955f,0x00008e5b,0x0000ab06,0x00009e00, 
0x0000c13b,0x0000b915,0x0000b7e4,0x0000c185,0x00009d29, 
0x00009ab0,0x00007268,0x000066b3,0x0000322c,0x00002471, 
0x000010de,0x00001a07,0x00003b26,0x00006685,0x00009510, 
0x0000bc8c,0x0000bca4,0x0000b353,0x000078e3,0x00004cb2, 
0x0000143e,0x00fffe16,0x00ffdd6b,0x00ffd53b,0x00ffcb9c, 
0x00ffc1ec,0x00ffb226,0x00ffa385,0x00ff9881,0x00ff92a2, 
0x00ff7f9c,0x00ff783c,0x00ff69c6,0x00ff6454,0x00ff5c5c, 
0x00ff6de1,0x00ff7dcf,0x00ff92bf,0x00ffac12,0x00ffc217, 
0x00ffd615,0x00ffd5b7,0x00ffd004,0x00ffbc10,0x00ffc6e9, 
0x00ffc71b,0x00ff6ee4,0x00ff7085,0x00ff70ac,0x00ff739c, 
0x00ff7642,0x00ff8525,0x00ffadb4,0x00ffda18,0x00000d30, 
0x000015ce,0x0000307c,0x000033d9,0x000030a1,0x0000333c, 
0x00000a98,0x00fff0ea,0x00ffd2ce,0x00ffadff,0x00ff9b09, 
0x00ffa006,0x00ffa4fc,0x00ffdb5f,0x000014dd,0x0000428d, 
0x00006914,0x000084d0,0x000083c0,0x00007abe,0x00006987, 
0x000059df,0x0000508e,0x0000636b,0x00006eea,0x00008ebf, 
0x000087da,0x0000a438,0x0000807d,0x00008a27,0x0000689a, 
0x00006f8e,0x000066b5,0x00006f9a,0x00007b1c,0x00008cd9, 
0x0000b1d4,0x0000bc52,0x0000c6eb,0x0000c5f3,0x0000caf0, 
0x0000b2fe,0x000091d1,0x00006cc5,0x00003080,0x0000084e, 
0x00fff1a6,0x00ffd885,0x00ffc747,0x00ffb12f,0x00ff95e6, 
0x00ff7fcb,0x00ff6074,0x00ff3614,0x00ff30c3,0x00ff0eaa, 
0x00ff082c,0x00fecf90,0x00feb2bc,0x00fe77cd,0x00fe4f7e, 
0x00fe5096,0x00fe7196,0x00fecb07,0x00ff093b,0x00ff59f7, 
0x00ff80a4,0x00ff9c2b,0x00ff9087,0x00ff9641,0x00ff7e27, 
0x00ff6b29,0x00ff5c8f,0x00ff711f,0x00ff8480,0x00ffb14b, 
0x00fff089,0x00002c24,0x000064af,0x000081fe,0x0000827c, 
0x00007964,0x00004f6a,0x00003214,0x000024e4,0x0000397b, 
0x00006e4f,0x0000b22b,0x0000ee46,0x00013000,0x000160c6, 
0x00017e24,0x00018eb3,0x000183e9,0x00017537,0x000167d1, 
0x000149fe,0x00012c6a,0x000115bf,0x0000ed03,0x0000d756, 
0x0000b939,0x00009d81,0x000063b5,0x000050a1,0x00002d31, 
0x00002c68,0x00002f16,0x000028ec,0x000026e6,0x00001032, 
0x00fff02f,0x00ffd621,0x00ffa743,0x00ff8bec,0x00ff722f, 
0x00ff6043,0x00ff6842,0x00ff5b54,0x00ff7742,0x00ff5767, 
0x00ff5b73,0x00ff4034,0x00ff3028,0x00ff2096,0x00ff178c, 
0x00ff1c1c,0x00ff32a1,0x00ff4355,0x00ff4f20,0x00ff566d, 
0x00ff7959,0x00ffa4ab,0x00ffe449,0x000018e2,0x00004ee1, 
0x0000667a,0x000077da,0x00006209,0x000046e0,0x00000d1e, 
0x00ffec51,0x00ffde68,0x00fff68b,0x000013a3,0x00003e4b, 
0x00005ce8,0x0000601c,0x00005f67,0x00004a06,0x00002ac4, 
0x000013dc,0x00fff27e,0x0000079c,0x0000053e,0x00003049, 
0x00004d95,0x00005cd4,0x0000752d,0x00006ed8,0x00004a14, 
0x00001a6e,0x00ffe0db,0x00ffad04,0x00ff9ab4,0x00ff9b1d, 
0x00ffbd80,0x00ffcca8,0x00ffdc40,0x00ffd7f4,0x00ffb307, 
0x00ffa4ca,0x00ff7e75,0x00ff8c11,0x00ff9e4e,0x00ffe0b0, 
0x00001f3b,0x00006d4c,0x0000910d,0x0000b59a,0x0000b79a, 
0x00008b22,0x00006ed7,0x00000ff4,0x00ffdbe3,0x00ff7b0b, 
0x00ff72eb,0x00ff63ab,0x00ff9308,0x00ffbb37,0x00ffecaa, 
0x000006bb,0x000000ea,0x00fff6f3,0x00ffcb4a,0x00ffb354, 
0x00ffac8f,0x00ffada0,0x00ffdf51,0x00ffe0e9,0x00fffddf, 
0x00ffd371,0x00ffc735,0x00ffa760,0x00ffa518,0x00ffb9c2, 
0x00ffd32e,0x00001432,0x00003956,0x000063b1,0x00006de6, 
0x00006516,0x000032b0,0x00000784,0x00ffc607,0x00ffa459, 
0x00ff8c3f,0x00ff9e0c,0x00ffc995,0x00000481,0x00003e3a, 
0x000063ce,0x00006272,0x000061b7,0x0000357b,0x00001cd6, 
0x00fff847,0x00fff378,0x00fff082,0x00001da4,0x00003536, 
0x00007ba8,0x00009b4a,0x0000c8f8,0x0000c8a0,0x0000c595, 
0x0000b085,0x00008fba,0x00006e0e,0x00005310,0x000055d3, 
0x00005d9d,0x00008597,0x0000a8a4,0x0000cb01,0x0000e1f3, 
0x0000d9e9,0x0000c28e,0x00008f06,0x000066c1,0x00002111, 
0x00fff607,0x00ffcec0,0x00ffb703,0x00ff8e85,0x00ff8850, 
0x00ff5180,0x00ff5dd4,0x00ff3f23,0x00ff5624,0x00ff5d87, 
0x00ff6f9a,0x00ff8d18,0x00ff973a,0x00ffa7d4,0x00ff9d16, 
0x00ff9946,0x00ff765b,0x00ff62eb,0x00ff49d2,0x00ff4f31, 
0x00ff5276,0x00ff5574,0x00ff66c8,0x00ff5c15,0x00ff6375, 
0x00ff4fa4,0x00ff3811,0x00ff357e,0x00ff2271,0x00ff349e, 
0x00ff3814,0x00ff7178,0x00ffa53c,0x00ffeea2,0x0000363e, 
0x00007838,0x00009507,0x00009956,0x0000764a,0x000044fd, 
0x000001bb,0x00ffc443,0x00ff94bd,0x00ff7b4b,0x00ff92b9, 
0x00ffb6eb,0x00fffd58,0x0000378c,0x0000602a,0x00009d5a, 
0x00009711,0x0000b8ad,0x00009113,0x000088b2,0x00005b03, 
0x00005749,0x00004ba8,0x00005ca0,0x00006a6c,0x00009485, 
0x0000b793,0x0000c7c9,0x0000cea3,0x0000b899,0x0000a4aa, 
0x00006ea4,0x00004636,0x000000e1,0x00ffd1ac,0x00ff8644, 
0x00ff7d29,0x00ff6637,0x00ff8bf0,0x00ffa6dc,0x00ffd270, 
0x00fff371,0x00000566,0x00000841,0x00fffac8,0x00fff3a4, 
0x00ffde51,0x00ffed9a,0x00ffefbb,0x0000096a,0x00001cc4, 
0x00002c53,0x0000269b,0x00001378,0x00fff38b,0x00ffd039, 
0x00ffa6f1,0x00ff7fa0,0x00ff5fb0,0x00ff5777,0x00ff628c, 
0x00ff73ce,0x00ff8f00,0x00ff976e,0x00ffa427,0x00ff9303, 
0x00ff9ad1,0x00ff8fdb,0x00ff9ff9,0x00ffb1d8,0x00ffeac1, 
0x00001a81,0x000036ae,0x0000420f,0x000027db,0x0000044a, 
0x00ffe6f8,0x00ffcb65,0x00ffc8ea,0x00ffe790,0x00000256, 
0x000046a5,0x000079ef,0x00009801,0x0000a89f,0x00007ece, 
0x00005e85,0x00002bf1,0x00000979,0x000003c1,0x00fffd28, 
0x0000188c,0x0000289f,0x0000428d,0x00004eb8,0x000060f1, 
0x00004d74,0x00004911,0x000027aa,0x00000756,0x00ffe99b, 
0x00ffbad6,0x00ffa985,0x00ffa2d2,0x00ff97ea,0x00ff9341, 
0x00ff84cc,0x00ff771b,0x00ff69c4,0x00ff5f8b,0x00ff605d, 
0x00ff6f1e,0x00ff84ed,0x00ffc701,0x00fffa54,0x0000334b, 
0x000042f7,0x00005090,0x00002f5b,0x000016e8,0x00ffe15b, 
0x00ffac53,0x00ff7895,0x00ff69e6,0x00ff6a0b,0x00ff9cf9, 
0x00ffdcfa,0x00002d5c,0x00007e75,0x0000b8d9,0x0000dc67, 
0x0000d52e,0x0000a294,0x00007980,0x00004167,0x00003a62, 
0x00002de3,0x00004f7b,0x000070e6,0x00009b3b,0x0000c073, 
0x0000d70d,0x0000efea,0x0000e140,0x0000a3a0,0x00005895, 
0x00ffd960,0x00ff7500,0x00ff3d07,0x00ff2604,0x00ff61de, 
0x00ffa8d6,0x00001a1d,0x0000625a,0x00008ad3,0x00006e56, 
0x00003a11,0x00ffcfcc,0x00ff7ed9,0x00ff3bbc,0x00ff126b, 
0x00fefcf5,0x00ff13a9,0x00ff3167,0x00ff6039,0x00ff7cf5, 
0x00ff8b76,0x00ff90f6,0x00ff82c8,0x00ff7aea,0x00ff757f, 
0x00ff6c43,0x00ff7a49,0x00ff8d78,0x00ff97b1,0x00ffb1ba, 
0x00ffc00a,0x00ffd2ac,0x00ffe727,0x00ffeaaf,0x00001555, 
0x00001dfd,0x00004567,0x0000762b,0x00008d6c,0x0000ba63, 
0x0000b4c4,0x0000adee,0x00008e8d,0x000062ee,0x000035f6, 
0x000023b6,0x00001354,0x00002824,0x00003962,0x00006080, 
0x0000931f,0x0000a39c,0x0000ac34,0x0000888d,0x0000539a, 
0x00001643,0x00ffd893,0x00ffbfa8,0x00ffaef3,0x00ffc722, 
0x00ffd5d2,0x00fff81d,0x00000745,0x000003b4,0x00ffe5ca, 
0x00ffa31b,0x00ff6647,0x00ff204a,0x00fef682,0x00fef22b, 
0x00ff0115,0x00ff376a,0x00ff7454,0x00ffb64c,0x00fff81c, 
0x00002379,0x00003941,0x00003f81,0x00002fd0,0x000020a9, 
0x00fff429,0x00ffdc42,0x00ffb5e7,0x00ffbd19,0x00ffbb6d, 
0x00ffebcc,0x00000a63,0x000037fb,0x00004b43,0x00006919, 
0x00006af5,0x0000790a,0x00005d81,0x00005d86,0x000037f6, 
0x000039db,0x000034a7,0x000057eb,0x00007a86,0x0000b348, 
0x0000d0f8,0x0000eaeb,0x0000d197,0x0000a4fb,0x00006485, 
0x000026e8,0x00fffee3,0x00ffe740,0x00ffe599,0x00ffe8cb, 
0x00ffdbf6,0x00ffd8dd,0x00ffc308,0x00ffc17c,0x00ffb21f, 
0x00ffa347,0x00ff8a5c,0x00ff6c53,0x00ff55bd,0x00ff53c8,
0x00ff71af,0x00ffa59e,0x000009ef,0x000086e2};
