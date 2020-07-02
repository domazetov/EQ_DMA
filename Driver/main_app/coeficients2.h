unsigned int p[] =
    {
        0x0023fd66, 0x0023fd66, 0x0023fd66, 0x002ffe44, 0x00400000,
        0x0055586a, 0x0071cf54, 0x0071cf54, 0x0071cf54, 0x0071cf54};

unsigned int pr[] =
    {
        0x00000005, 0x0000000a, 0x00000013, 0x00000023, 0x00000046,
        0x00000075, 0x000000a3, 0x000000e8, 0x0000015c};

int64_t audio[] =
    {
        0x0000000000000001, 0x0000000000000002, 0x0000000000000003, 0x0000000000000004, 0x0000000000000005, 
		0x0000000000000006, 0x0000000000000007, 0x0000000000000008, 0x0000000000000009, 0x000000000000000a, 
		0x000000000000000b, 0x000000000000000c, 0x000000000000000d, 0x000000000000000e, 0x000000000000000f, 
		0x0000000000000010, 0x0000000000000011, 0x0000000000000012, 0x0000000000000013, 0x0000000000000014, 
		0x0000000000000015, 0x0000000000000016, 0x0000000000000017, 0x0000000000000018, 0x0000000000000019, 
		0x000000000000001a, 0x000000000000001b, 0x000000000000001c, 0x000000000000001d, 0x000000000000001e, 
		0x000000000000001f, 0x0000000000000020, 0x0000000000000021, 0x0000000000000022, 0x0000000000000023, 
		0x0000000000000024, 0x0000000000000025, 0x0000000000000026, 0x0000000000000027, 0x0000000000000028, 
		0x0000000000000029, 0x000000000000002a, 0x000000000000002b, 0x000000000000002c, 0x000000000000002d, 
		0x000000000000002e, 0x000000000000002f, 0x0000000000000030, 0x0000000000000031, 0x0000000000000032, 
		0x0000000000000033, 0x0000000000000034, 0x0000000000000035, 0x0000000000000036, 0x0000000000000037, 
		0x0000000000000038, 0x0000000000000039, 0x000000000000003a, 0x000000000000003b, 0x000000000000003c, 
		0x000000000000003d, 0x000000000000003e, 0x000000000000003f, 0x0000000000000040, 0x0000000000000041, 
		0x0000000000000042, 0x0000000000000043, 0x0000000000000044, 0x0000000000000045, 0x0000000000000046, 
		0x0000000000000047, 0x0000000000000048, 0x0000000000000049, 0x000000000000004a, 0x000000000000004b, 
		0x000000000000004c, 0x000000000000004d, 0x000000000000004e, 0x000000000000004f, 0x0000000000000050, 
		0x0000000000000051, 0x0000000000000052, 0x0000000000000053, 0x0000000000000054, 0x0000000000000055, 
		0x0000000000000056, 0x0000000000000057, 0x0000000000000058, 0x0000000000000059, 0x000000000000005a, 
		0x000000000000005b, 0x000000000000005c, 0x000000000000005d, 0x000000000000005e, 0x000000000000005f, 
		0x0000000000000060, 0x0000000000000061, 0x0000000000000062, 0x0000000000000063, 0x0000000000000064, 
		0x0000000000000065, 0x0000000000000066, 0x0000000000000067, 0x0000000000000068, 0x0000000000000069, 
		0x000000000000006a, 0x000000000000006b, 0x000000000000006c, 0x000000000000006d, 0x000000000000006e, 
		0x000000000000006f, 0x0000000000000070, 0x0000000000000071, 0x0000000000000072, 0x0000000000000073, 
		0x0000000000000074, 0x0000000000000075, 0x0000000000000076, 0x0000000000000077, 0x0000000000000078, 
		0x0000000000000079, 0x000000000000007a, 0x000000000000007b, 0x000000000000007c, 0x000000000000007d, 
		0x000000000000007e, 0x000000000000007f, 0x0000000000000080, 0x0000000000000081, 0x0000000000000082, 
		0x0000000000000083, 0x0000000000000084, 0x0000000000000085, 0x0000000000000086, 0x0000000000000087, 
		0x0000000000000088, 0x0000000000000089, 0x000000000000008a, 0x000000000000008b, 0x000000000000008c, 
		0x000000000000008d, 0x000000000000008e, 0x000000000000008f, 0x0000000000000090, 0x0000000000000091, 
		0x0000000000000092, 0x0000000000000093, 0x0000000000000094, 0x0000000000000095, 0x0000000000000096, 
		0x0000000000000097, 0x0000000000000098, 0x0000000000000099, 0x000000000000009a, 0x000000000000009b, 
		0x000000000000009c, 0x000000000000009d, 0x000000000000009e, 0x000000000000009f, 0x00000000000000a0, 
		0x00000000000000a1, 0x00000000000000a2, 0x00000000000000a3, 0x00000000000000a4, 0x00000000000000a5, 
		0x00000000000000a6, 0x00000000000000a7, 0x00000000000000a8, 0x00000000000000a9, 0x00000000000000aa, 
		0x00000000000000ab, 0x00000000000000ac, 0x00000000000000ad, 0x00000000000000ae, 0x00000000000000af, 
		0x00000000000000b0, 0x00000000000000b1, 0x00000000000000b2, 0x00000000000000b3, 0x00000000000000b4, 
		0x00000000000000b5, 0x00000000000000b6, 0x00000000000000b7, 0x00000000000000b8, 0x00000000000000b9, 
		0x00000000000000ba, 0x00000000000000bb, 0x00000000000000bc, 0x00000000000000bd, 0x00000000000000be, 
		0x00000000000000bf, 0x00000000000000c0, 0x00000000000000c1, 0x00000000000000c2, 0x00000000000000c3, 
		0x00000000000000c4, 0x00000000000000c5, 0x00000000000000c6, 0x00000000000000c7, 0x00000000000000c8, 
		0x00000000000000c9, 0x00000000000000ca, 0x00000000000000cb, 0x00000000000000cc, 0x00000000000000cd, 
		0x00000000000000ce, 0x00000000000000cf, 0x00000000000000d0, 0x00000000000000d1, 0x00000000000000d2, 
		0x00000000000000d3, 0x00000000000000d4, 0x00000000000000d5, 0x00000000000000d6, 0x00000000000000d7, 
		0x00000000000000d8, 0x00000000000000d9, 0x00000000000000da, 0x00000000000000db, 0x00000000000000dc, 
		0x00000000000000dd, 0x00000000000000de, 0x00000000000000df, 0x00000000000000e0, 0x00000000000000e1, 
		0x00000000000000e2, 0x00000000000000e3, 0x00000000000000e4, 0x00000000000000e5, 0x00000000000000e6, 
		0x00000000000000e7, 0x00000000000000e8, 0x00000000000000e9, 0x00000000000000ea, 0x00000000000000eb, 
		0x00000000000000ec, 0x00000000000000ed, 0x00000000000000ee, 0x00000000000000ef, 0x00000000000000f0, 
		0x00000000000000f1, 0x00000000000000f2, 0x00000000000000f3, 0x00000000000000f4, 0x00000000000000f5, 
		0x00000000000000f6, 0x00000000000000f7, 0x00000000000000f8, 0x00000000000000f9, 0x00000000000000fa, 
		0x00000000000000fb, 0x00000000000000fc, 0x00000000000000fd, 0x00000000000000fe, 0x00000000000000ff, 
		0x0000000000000100, 0x0000000000000101, 0x0000000000000102, 0x0000000000000103, 0x0000000000000104, 
		0x0000000000000105, 0x0000000000000106, 0x0000000000000107, 0x0000000000000108, 0x0000000000000109, 
		0x000000000000010a, 0x000000000000010b, 0x000000000000010c, 0x000000000000010d, 0x000000000000010e, 
		0x000000000000010f, 0x0000000000000110, 0x0000000000000111, 0x0000000000000112, 0x0000000000000113, 
		0x0000000000000114, 0x0000000000000115, 0x0000000000000116, 0x0000000000000117, 0x0000000000000118, 
		0x0000000000000119, 0x000000000000011a, 0x000000000000011b, 0x000000000000011c, 0x000000000000011d, 
		0x000000000000011e, 0x000000000000011f, 0x0000000000000120, 0x0000000000000121, 0x0000000000000122, 
		0x0000000000000123, 0x0000000000000124, 0x0000000000000125, 0x0000000000000126, 0x0000000000000127, 
		0x0000000000000128, 0x0000000000000129, 0x000000000000012a, 0x000000000000012b, 0x000000000000012c, 
		0x000000000000012d, 0x000000000000012e, 0x000000000000012f, 0x0000000000000130, 0x0000000000000131, 
		0x0000000000000132, 0x0000000000000133, 0x0000000000000134, 0x0000000000000135, 0x0000000000000136, 
		0x0000000000000137, 0x0000000000000138, 0x0000000000000139, 0x000000000000013a, 0x000000000000013b, 
		0x000000000000013c, 0x000000000000013d, 0x000000000000013e, 0x000000000000013f, 0x0000000000000140, 
		0x0000000000000141, 0x0000000000000142, 0x0000000000000143, 0x0000000000000144, 0x0000000000000145, 
		0x0000000000000146, 0x0000000000000147, 0x0000000000000148, 0x0000000000000149, 0x000000000000014a, 
		0x000000000000014b, 0x000000000000014c, 0x000000000000014d, 0x000000000000014e, 0x000000000000014f, 
		0x0000000000000150, 0x0000000000000151, 0x0000000000000152, 0x0000000000000153, 0x0000000000000154, 
		0x0000000000000155, 0x0000000000000156, 0x0000000000000157, 0x0000000000000158, 0x0000000000000159, 
		0x000000000000015a, 0x000000000000015b, 0x000000000000015c, 0x000000000000015d, 0x000000000000015e, 
		0x000000000000015f, 0x0000000000000160, 0x0000000000000161, 0x0000000000000162, 0x0000000000000163, 
		0x0000000000000164, 0x0000000000000165, 0x0000000000000166, 0x0000000000000167, 0x0000000000000168, 
		0x0000000000000169, 0x000000000000016a, 0x000000000000016b, 0x000000000000016c, 0x000000000000016d, 
		0x000000000000016e, 0x000000000000016f, 0x0000000000000170, 0x0000000000000171, 0x0000000000000172, 
		0x0000000000000173, 0x0000000000000174, 0x0000000000000175, 0x0000000000000176, 0x0000000000000177, 
		0x0000000000000178, 0x0000000000000179, 0x000000000000017a, 0x000000000000017b, 0x000000000000017c, 
		0x000000000000017d, 0x000000000000017e, 0x000000000000017f, 0x0000000000000180, 0x0000000000000181, 
		0x0000000000000182, 0x0000000000000183, 0x0000000000000184, 0x0000000000000185, 0x0000000000000186, 
		0x0000000000000187, 0x0000000000000188, 0x0000000000000189, 0x000000000000018a, 0x000000000000018b, 
		0x000000000000018c, 0x000000000000018d, 0x000000000000018e, 0x000000000000018f, 0x0000000000000190, 
		0x0000000000000191, 0x0000000000000192, 0x0000000000000193, 0x0000000000000194, 0x0000000000000195, 
		0x0000000000000196, 0x0000000000000197, 0x0000000000000198, 0x0000000000000199, 0x000000000000019a, 
		0x000000000000019b, 0x000000000000019c, 0x000000000000019d, 0x000000000000019e, 0x000000000000019f, 
		0x00000000000001a0, 0x00000000000001a1, 0x00000000000001a2, 0x00000000000001a3, 0x00000000000001a4, 
		0x00000000000001a5, 0x00000000000001a6, 0x00000000000001a7, 0x00000000000001a8, 0x00000000000001a9, 
		0x00000000000001aa, 0x00000000000001ab, 0x00000000000001ac, 0x00000000000001ad, 0x00000000000001ae, 
		0x00000000000001af, 0x00000000000001b0, 0x00000000000001b1, 0x00000000000001b2, 0x00000000000001b3, 
		0x00000000000001b4, 0x00000000000001b5, 0x00000000000001b6, 0x00000000000001b7, 0x00000000000001b8, 
		0x00000000000001b9, 0x00000000000001ba, 0x00000000000001bb, 0x00000000000001bc, 0x00000000000001bd, 
		0x00000000000001be, 0x00000000000001bf, 0x00000000000001c0, 0x00000000000001c1, 0x00000000000001c2, 
		0x00000000000001c3, 0x00000000000001c4, 0x00000000000001c5, 0x00000000000001c6, 0x00000000000001c7, 
		0x00000000000001c8, 0x00000000000001c9, 0x00000000000001ca, 0x00000000000001cb, 0x00000000000001cc, 
		0x00000000000001cd, 0x00000000000001ce, 0x00000000000001cf, 0x00000000000001d0, 0x00000000000001d1, 
		0x00000000000001d2, 0x00000000000001d3, 0x00000000000001d4, 0x00000000000001d5, 0x00000000000001d6, 
		0x00000000000001d7, 0x00000000000001d8, 0x00000000000001d9, 0x00000000000001da, 0x00000000000001db, 
		0x00000000000001dc, 0x00000000000001dd, 0x00000000000001de, 0x00000000000001df, 0x00000000000001e0, 
		0x00000000000001e1, 0x00000000000001e2, 0x00000000000001e3, 0x00000000000001e4, 0x00000000000001e5, 
		0x00000000000001e6, 0x00000000000001e7, 0x00000000000001e8, 0x00000000000001e9, 0x00000000000001ea, 
		0x00000000000001eb, 0x00000000000001ec, 0x00000000000001ed, 0x00000000000001ee, 0x00000000000001ef, 
		0x00000000000001f0, 0x00000000000001f1, 0x00000000000001f2, 0x00000000000001f3, 0x00000000000001f4, 
		0x00000000000001f5, 0x00000000000001f6, 0x00000000000001f7, 0x00000000000001f8, 0x00000000000001f9, 
		0x00000000000001fa, 0x00000000000001fb, 0x00000000000001fc, 0x00000000000001fd, 0x00000000000001fe, 
		0x00000000000001ff, 0x0000000000000200, 0x0000000000000201, 0x0000000000000202, 0x0000000000000203, 
		0x0000000000000204, 0x0000000000000205, 0x0000000000000206, 0x0000000000000207, 0x0000000000000208, 
		0x0000000000000209, 0x000000000000020a, 0x000000000000020b, 0x000000000000020c, 0x000000000000020d, 
		0x000000000000020e, 0x000000000000020f, 0x0000000000000210, 0x0000000000000211, 0x0000000000000212, 
		0x0000000000000213, 0x0000000000000214, 0x0000000000000215, 0x0000000000000216, 0x0000000000000217, 
		0x0000000000000218, 0x0000000000000219, 0x000000000000021a, 0x000000000000021b, 0x000000000000021c, 
		0x000000000000021d, 0x000000000000021e, 0x000000000000021f, 0x0000000000000220, 0x0000000000000221, 
		0x0000000000000222, 0x0000000000000223, 0x0000000000000224, 0x0000000000000225, 0x0000000000000226, 
		0x0000000000000227, 0x0000000000000228, 0x0000000000000229, 0x000000000000022a, 0x000000000000022b, 
		0x000000000000022c, 0x000000000000022d, 0x000000000000022e, 0x000000000000022f, 0x0000000000000230, 
		0x0000000000000231, 0x0000000000000232, 0x0000000000000233, 0x0000000000000234, 0x0000000000000235, 
		0x0000000000000236, 0x0000000000000237, 0x0000000000000238, 0x0000000000000239, 0x000000000000023a, 
		0x000000000000023b, 0x000000000000023c, 0x000000000000023d, 0x000000000000023e, 0x000000000000023f, 
		0x0000000000000240, 0x0000000000000241, 0x0000000000000242, 0x0000000000000243, 0x0000000000000244, 
		0x0000000000000245, 0x0000000000000246, 0x0000000000000247, 0x0000000000000248, 0x0000000000000249, 
		0x000000000000024a, 0x000000000000024b, 0x000000000000024c, 0x000000000000024d, 0x000000000000024e, 
		0x000000000000024f, 0x0000000000000250, 0x0000000000000251, 0x0000000000000252, 0x0000000000000253, 
		0x0000000000000254, 0x0000000000000255, 0x0000000000000256, 0x0000000000000257, 0x0000000000000258, 
		0x0000000000000259, 0x000000000000025a, 0x000000000000025b, 0x000000000000025c, 0x000000000000025d, 
		0x000000000000025e, 0x000000000000025f, 0x0000000000000260, 0x0000000000000261, 0x0000000000000262, 
		0x0000000000000263, 0x0000000000000264, 0x0000000000000265, 0x0000000000000266, 0x0000000000000267, 
		0x0000000000000268, 0x0000000000000269, 0x000000000000026a, 0x000000000000026b, 0x000000000000026c, 
		0x000000000000026d, 0x000000000000026e, 0x000000000000026f, 0x0000000000000270, 0x0000000000000271, 
		0x0000000000000272, 0x0000000000000273, 0x0000000000000274, 0x0000000000000275, 0x0000000000000276, 
		0x0000000000000277, 0x0000000000000278, 0x0000000000000279, 0x000000000000027a, 0x000000000000027b, 
		0x000000000000027c, 0x000000000000027d, 0x000000000000027e, 0x000000000000027f, 0x0000000000000280, 
		0x0000000000000281, 0x0000000000000282, 0x0000000000000283, 0x0000000000000284, 0x0000000000000285, 
		0x0000000000000286, 0x0000000000000287, 0x0000000000000288, 0x0000000000000289, 0x000000000000028a, 
		0x000000000000028b, 0x000000000000028c, 0x000000000000028d, 0x000000000000028e, 0x000000000000028f, 
		0x0000000000000290, 0x0000000000000291, 0x0000000000000292, 0x0000000000000293, 0x0000000000000294, 
		0x0000000000000295, 0x0000000000000296, 0x0000000000000297, 0x0000000000000298, 0x0000000000000299, 
		0x000000000000029a, 0x000000000000029b, 0x000000000000029c, 0x000000000000029d, 0x000000000000029e, 
		0x000000000000029f, 0x00000000000002a0, 0x00000000000002a1, 0x00000000000002a2, 0x00000000000002a3, 
		0x00000000000002a4, 0x00000000000002a5, 0x00000000000002a6, 0x00000000000002a7, 0x00000000000002a8, 
		0x00000000000002a9, 0x00000000000002aa, 0x00000000000002ab, 0x00000000000002ac, 0x00000000000002ad, 
		0x00000000000002ae, 0x00000000000002af, 0x00000000000002b0, 0x00000000000002b1, 0x00000000000002b2, 
		0x00000000000002b3, 0x00000000000002b4, 0x00000000000002b5, 0x00000000000002b6, 0x00000000000002b7, 
		0x00000000000002b8, 0x00000000000002b9, 0x00000000000002ba, 0x00000000000002bb, 0x00000000000002bc, 
		0x00000000000002bd, 0x00000000000002be, 0x00000000000002bf, 0x00000000000002c0, 0x00000000000002c1, 
		0x00000000000002c2, 0x00000000000002c3, 0x00000000000002c4, 0x00000000000002c5, 0x00000000000002c6, 
		0x00000000000002c7, 0x00000000000002c8, 0x00000000000002c9, 0x00000000000002ca, 0x00000000000002cb, 
		0x00000000000002cc, 0x00000000000002cd, 0x00000000000002ce, 0x00000000000002cf, 0x00000000000002d0, 
		0x00000000000002d1, 0x00000000000002d2, 0x00000000000002d3, 0x00000000000002d4, 0x00000000000002d5, 
		0x00000000000002d6, 0x00000000000002d7, 0x00000000000002d8, 0x00000000000002d9, 0x00000000000002da, 
		0x00000000000002db, 0x00000000000002dc, 0x00000000000002dd, 0x00000000000002de, 0x00000000000002df, 
		0x00000000000002e0, 0x00000000000002e1, 0x00000000000002e2, 0x00000000000002e3, 0x00000000000002e4, 
		0x00000000000002e5, 0x00000000000002e6, 0x00000000000002e7, 0x00000000000002e8, 0x00000000000002e9, 
		0x00000000000002ea, 0x00000000000002eb, 0x00000000000002ec, 0x00000000000002ed, 0x00000000000002ee, 
		0x00000000000002ef, 0x00000000000002f0, 0x00000000000002f1, 0x00000000000002f2, 0x00000000000002f3, 
		0x00000000000002f4, 0x00000000000002f5, 0x00000000000002f6, 0x00000000000002f7, 0x00000000000002f8, 
		0x00000000000002f9, 0x00000000000002fa, 0x00000000000002fb, 0x00000000000002fc, 0x00000000000002fd, 
		0x00000000000002fe, 0x00000000000002ff, 0x0000000000000300, 0x0000000000000301, 0x0000000000000302, 
		0x0000000000000303, 0x0000000000000304, 0x0000000000000305, 0x0000000000000306, 0x0000000000000307, 
		0x0000000000000308, 0x0000000000000309, 0x000000000000030a, 0x000000000000030b, 0x000000000000030c, 
		0x000000000000030d, 0x000000000000030e, 0x000000000000030f, 0x0000000000000310, 0x0000000000000311, 
		0x0000000000000312, 0x0000000000000313, 0x0000000000000314, 0x0000000000000315, 0x0000000000000316, 
		0x0000000000000317, 0x0000000000000318, 0x0000000000000319, 0x000000000000031a, 0x000000000000031b, 
		0x000000000000031c, 0x000000000000031d, 0x000000000000031e, 0x000000000000031f, 0x0000000000000320, 
		0x0000000000000321, 0x0000000000000322, 0x0000000000000323, 0x0000000000000324, 0x0000000000000325, 
		0x0000000000000326, 0x0000000000000327, 0x0000000000000328, 0x0000000000000329, 0x000000000000032a, 
		0x000000000000032b, 0x000000000000032c, 0x000000000000032d, 0x000000000000032e, 0x000000000000032f, 
		0x0000000000000330, 0x0000000000000331, 0x0000000000000332, 0x0000000000000333, 0x0000000000000334, 
		0x0000000000000335, 0x0000000000000336, 0x0000000000000337, 0x0000000000000338, 0x0000000000000339, 
		0x000000000000033a, 0x000000000000033b, 0x000000000000033c, 0x000000000000033d, 0x000000000000033e, 
		0x000000000000033f, 0x0000000000000340, 0x0000000000000341, 0x0000000000000342, 0x0000000000000343, 
		0x0000000000000344, 0x0000000000000345, 0x0000000000000346, 0x0000000000000347, 0x0000000000000348, 
		0x0000000000000349, 0x000000000000034a, 0x000000000000034b, 0x000000000000034c, 0x000000000000034d, 
		0x000000000000034e, 0x000000000000034f, 0x0000000000000350, 0x0000000000000351, 0x0000000000000352, 
		0x0000000000000353, 0x0000000000000354, 0x0000000000000355, 0x0000000000000356, 0x0000000000000357, 
		0x0000000000000358, 0x0000000000000359, 0x000000000000035a, 0x000000000000035b, 0x000000000000035c, 
		0x000000000000035d, 0x000000000000035e, 0x000000000000035f, 0x0000000000000360, 0x0000000000000361, 
		0x0000000000000362, 0x0000000000000363, 0x0000000000000364, 0x0000000000000365, 0x0000000000000366, 
		0x0000000000000367, 0x0000000000000368, 0x0000000000000369, 0x000000000000036a, 0x000000000000036b, 
		0x000000000000036c, 0x000000000000036d, 0x000000000000036e, 0x000000000000036f, 0x0000000000000370, 
		0x0000000000000371, 0x0000000000000372, 0x0000000000000373, 0x0000000000000374, 0x0000000000000375, 
		0x0000000000000376, 0x0000000000000377, 0x0000000000000378, 0x0000000000000379, 0x000000000000037a, 
		0x000000000000037b, 0x000000000000037c, 0x000000000000037d, 0x000000000000037e, 0x000000000000037f, 
		0x0000000000000380, 0x0000000000000381, 0x0000000000000382, 0x0000000000000383, 0x0000000000000384, 
		0x0000000000000385, 0x0000000000000386, 0x0000000000000387, 0x0000000000000388, 0x0000000000000389, 
		0x000000000000038a, 0x000000000000038b, 0x000000000000038c, 0x000000000000038d, 0x000000000000038e, 
		0x000000000000038f, 0x0000000000000390, 0x0000000000000391, 0x0000000000000392, 0x0000000000000393, 
		0x0000000000000394, 0x0000000000000395, 0x0000000000000396, 0x0000000000000397, 0x0000000000000398, 
		0x0000000000000399, 0x000000000000039a, 0x000000000000039b, 0x000000000000039c, 0x000000000000039d, 
		0x000000000000039e, 0x000000000000039f, 0x00000000000003a0, 0x00000000000003a1, 0x00000000000003a2, 
		0x00000000000003a3, 0x00000000000003a4, 0x00000000000003a5, 0x00000000000003a6, 0x00000000000003a7, 
		0x00000000000003a8, 0x00000000000003a9, 0x00000000000003aa, 0x00000000000003ab, 0x00000000000003ac, 
		0x00000000000003ad, 0x00000000000003ae, 0x00000000000003af, 0x00000000000003b0, 0x00000000000003b1, 
		0x00000000000003b2, 0x00000000000003b3, 0x00000000000003b4, 0x00000000000003b5, 0x00000000000003b6, 
		0x00000000000003b7, 0x00000000000003b8, 0x00000000000003b9, 0x00000000000003ba, 0x00000000000003bb, 
		0x00000000000003bc, 0x00000000000003bd, 0x00000000000003be, 0x00000000000003bf, 0x00000000000003c0, 
		0x00000000000003c1, 0x00000000000003c2, 0x00000000000003c3, 0x00000000000003c4, 0x00000000000003c5, 
		0x00000000000003c6, 0x00000000000003c7, 0x00000000000003c8, 0x00000000000003c9, 0x00000000000003ca, 
		0x00000000000003cb, 0x00000000000003cc, 0x00000000000003cd, 0x00000000000003ce, 0x00000000000003cf, 
		0x00000000000003d0, 0x00000000000003d1, 0x00000000000003d2, 0x00000000000003d3, 0x00000000000003d4, 
		0x00000000000003d5, 0x00000000000003d6, 0x00000000000003d7, 0x00000000000003d8, 0x00000000000003d9, 
		0x00000000000003da, 0x00000000000003db, 0x00000000000003dc, 0x00000000000003dd, 0x00000000000003de, 
		0x00000000000003df, 0x00000000000003e0, 0x00000000000003e1, 0x00000000000003e2, 0x00000000000003e3, 
		0x00000000000003e4, 0x00000000000003e5, 0x00000000000003e6, 0x00000000000003e7, 0x00000000000003e8, 
		0x00000000000003e9, 0x00000000000003ea, 0x00000000000003eb, 0x00000000000003ec, 0x00000000000003ed, 
		0x00000000000003ee, 0x00000000000003ef, 0x00000000000003f0, 0x00000000000003f1, 0x00000000000003f2, 
		0x00000000000003f3, 0x00000000000003f4, 0x00000000000003f5, 0x00000000000003f6, 0x00000000000003f7, 
		0x00000000000003f8, 0x00000000000003f9, 0x00000000000003fa, 0x00000000000003fb, 0x00000000000003fc, 
		0x00000000000003fd, 0x00000000000003fe, 0x00000000000003ff, 0x0000000000000400};
