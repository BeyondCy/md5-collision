#include <iostream>
#include <vector>
#include "tools.h"
//#include "Md5Compress.h"

void using_s00_find_block1(uint32 block[], const uint32 IV[])
{
	uint32 Q[68] = { IV[0], IV[3], IV[2], IV[1] };

	std::vector<uint32> q_vec_mac(8);
	for (unsigned k = 0; k < q_vec_mac.size(); ++k)
		q_vec_mac[k] = ((k << 5) ^ (k << 12) ^ (k << 25)) & 0x08002020;
	
	std::vector<uint32> q_vec_mac_9(1 << 9);
	for (unsigned k = 0; k < q_vec_mac_9.size(); ++k)
		q_vec_mac_9[k] = ((k << 1) ^ (k << 3) ^ (k << 6) ^ (k << 8) ^ (k << 11) ^ (k << 14) ^ (k << 18)) & 0x04310d12;
	
	while (1) 
	{
		uint32 aa = Q[FLAG] & 0x80000000;
		//std::cout << aa << std::endl;
		Q[FLAG + 2] = (changing() & 0x49a0e73e) | 0x221f00c1 | aa;
		Q[FLAG + 3] = (changing() & 0x0000040c) | 0x3fce1a71 | (Q[FLAG + 2] & 0x8000e000);
		Q[FLAG + 4] = (changing() & 0x00000004) | (0xa5f281a2 ^ (Q[FLAG + 3] & 0x80000008));
		Q[FLAG + 5] = (changing() & 0x00000004) | 0x67fd823b;
		Q[FLAG + 6] = (changing() & 0x00001044) | 0x15e5829a;
		Q[FLAG + 7] = (changing() & 0x00200806) | 0x950430b0;
		Q[FLAG + 8] = (changing() & 0x60050110) | 0x1bd29ca2 | (Q[FLAG + 7] & 0x00000004);
		Q[FLAG + 9] = (changing() & 0x40044000) | 0xb8820004;
		Q[FLAG + 10] = 0xf288b209 | (Q[FLAG + 9] & 0x00044000);
		Q[FLAG + 11] = (changing() & 0x12888008) | 0x85712f57;
		Q[FLAG + 12] = (changing() & 0x1ed98d7f) | 0xc0023080 | (~Q[FLAG + 11] & 0x00200000);
		Q[FLAG + 13] = (changing() & 0x0efb1d77) | 0x1000c008;
		Q[FLAG + 14] = (changing() & 0x0fff5d77) | 0xa000a288;
		Q[FLAG + 15] = (changing() & 0x0efe7ff7) | 0xe0008000 | (~Q[FLAG + 14] & 0x00010000);
		Q[FLAG + 16] = (changing() & 0x0ffdffff) | 0xf0000000 | (~Q[FLAG + 15] & 0x00020000);

		MD5_REVERSE_STEP(5, 0x4787c62a, 12);
		MD5_REVERSE_STEP(6, 0xa8304613, 17);
		MD5_REVERSE_STEP(7, 0xfd469501, 22);
		MD5_REVERSE_STEP(11, 0x895cd7be, 22);
		MD5_REVERSE_STEP(14, 0xa679438e, 17);
		MD5_REVERSE_STEP(15, 0x49b40821, 22);

		const uint32 tt17 = GG(Q[FLAG + 16], Q[FLAG + 15], Q[FLAG + 14]) + Q[FLAG + 13] + 0xf61e2562;
		const uint32 tt18 = Q[FLAG + 14] + 0xc040b340 + block[6];
		const uint32 tt19 = Q[FLAG + 15] + 0x265e5a51 + block[11];
		
		const uint32 tt0 = FF(Q[FLAG + 0], Q[FLAG - 1], Q[FLAG - 2]) + Q[FLAG - 3] + 0xd76aa478;
		const uint32 tt1 = Q[FLAG - 2] + 0xe8c7b756;		
		const uint32 q1a = 0x02020801 | (Q[FLAG + 0] & 0x80000000);
		
		unsigned counter = 0;
		while (counter < (1 << 12))
		{
			++counter;
			//std::cout << counter << std::endl;
			uint32 q1 = q1a | (changing() & 0x7dfdf7be);
			uint32 m1 = Q[FLAG+2] - q1;
			m1 = RR(m1, 12) - FF(q1, Q[FLAG+0], Q[FLAG-1]) - tt1;

			const uint32 q16 = Q[FLAG+16];
			uint32 q17 = tt17 + m1;
			q17 = RL(q17, 5) + q16;
			if (0x80000000 != ((q17^q16) & 0x80008008)) 
				continue;
			if (0 != (q17 & 0x00020000)) 
				continue;

			uint32 q18 = GG(q17, q16, Q[FLAG+15]) + tt18;
			q18 = RL(q18, 9); q18 += q17;
			if (0x80020000 != ((q18^q17) & 0xa0020000)) 
				continue;

			uint32 q19 = GG(q18, q17, q16) + tt19;
			q19 = RL(q19, 14); q19 += q18;
			if (0x80000000 != (q19 & 0x80020000)) 
				continue;

			uint32 m0 = q1 - Q[FLAG + 0];
			m0 = RR(m0, 7) - tt0;

			uint32 q20 = GG(q19, q18, q17) + q16 + 0xe9b6c7aa + m0;
			q20 = RL(q20, 20); q20 += q19;
			if (0x00040000 != ((q20^q19) & 0x80040000))	
				continue;
			
			Q[FLAG + 1] = q1;
			Q[FLAG + 17] = q17;
			Q[FLAG + 18] = q18;
			Q[FLAG + 19] = q19;
			Q[FLAG + 20] = q20;

			block[0] = m0;
			block[1] = m1;			

			MD5_REVERSE_STEP(5, 0x4787c62a, 12);
			uint32 q21 = GG(Q[FLAG+20], Q[FLAG+19], Q[FLAG+18]) + Q[FLAG+17] + 0xd62f105d + block[5];
			q21 = RL(q21, 5); q21 += Q[FLAG+20];
			if (((q21^Q[FLAG+20]) & 0x80020000) != 0) 
				continue;
			Q[FLAG+21] = q21;			

			counter = 0;
			break;
		}
		if (counter != 0)
			continue;

		const uint32 q9b = Q[FLAG + 9];
		const uint32 q10b = Q[FLAG + 10];

		MD5_REVERSE_STEP(2, 0x242070db, 17);
		MD5_REVERSE_STEP(3, 0xc1bdceee, 22);
		MD5_REVERSE_STEP(4, 0xf57c0faf, 7);
		MD5_REVERSE_STEP(7, 0xfd469501, 22);

		const uint32 tt10 = Q[FLAG + 7] + 0xffff5bb1;
		const uint32 tt22 = GG(Q[FLAG + 21], Q[FLAG + 20], Q[FLAG + 19]) + Q[FLAG + 18] + 0x02441453;
		const uint32 tt23 = Q[FLAG + 19] + 0xd8a1e681 + block[15];
		const uint32 tt24 = Q[FLAG + 20] + 0xe7d3fbc8 + block[4];
	 
		for (unsigned k10 = 0; k10 < (1 << 3); ++k10)
		{
			uint32 q10 = q10b | (q_vec_mac[k10]&0x08000020);
			uint32 m10 = RR(Q[FLAG+11]-q10,17);
			uint32 q9 = q9b | (q_vec_mac[k10]&0x00002000);

			m10 -= FF(q10, q9, Q[FLAG+8]) + tt10;

			uint32 aa = Q[FLAG + 21];
			uint32 dd = tt22+m10; dd = RL(dd, 9) + aa;
			if ((dd & 0x80000000) == 0) 
				continue;			

			uint32 bb = Q[FLAG + 20];
			uint32 cc = tt23 + GG(dd, aa, bb); 
			if ((cc & 0x20000) != 0) 
				continue;
			cc = RL(cc, 14) + dd;
			if ((cc & 0x80000000) != 0) 
				continue;

			bb = tt24 + GG(cc, dd, aa); 
			bb = RL(bb, 20) + cc;
			if ((bb & 0x80000000) == 0) 
				continue;
			//std::cout << "ddd!d" << std::endl;
			block[10] = m10;
			Q[FLAG + 9] = q9;
			Q[FLAG + 10] = q10;
			MD5_REVERSE_STEP(13, 0xfd987193, 12);

			for (unsigned k9 = 0; k9 < (1 << 9); ++k9)
			{
				uint32 a = aa;
				uint32 b = bb;
				uint32 c = cc;
				uint32 d = dd;
				Q[FLAG + 9] = q9 ^ q_vec_mac_9[k9];
				MD5_REVERSE_STEP(8, 0x698098d8, 7);
				MD5_REVERSE_STEP(9, 0x8b44f7af, 12);
				MD5_REVERSE_STEP(12, 0x6b901122, 7);

				MD5_STEP(GG, a, b, c, d, block[9], 0x21e1cde6, 5);
				MD5_STEP(GG, d, a, b, c, block[14], 0xc33707d6, 9);
				MD5_STEP(GG, c, d, a, b, block[3], 0xf4d50d87, 14);
				MD5_STEP(GG, b, c, d, a, block[8], 0x455a14ed, 20);
				MD5_STEP(GG, a, b, c, d, block[13], 0xa9e3e905, 5);
				MD5_STEP(GG, d, a, b, c, block[2], 0xfcefa3f8, 9);
				MD5_STEP(GG, c, d, a, b, block[7], 0x676f02d9, 14);
				MD5_STEP(GG, b, c, d, a, block[12], 0x8d2a4c8a, 20);
				MD5_STEP(HH, a, b, c, d, block[5], 0xfffa3942, 4);
				MD5_STEP(HH, d, a, b, c, block[8], 0x8771f681, 11);

				c += HH(d, a, b) + block[11] + 0x6d9d6122;
				if ((c & (1 << 15)) != 0)
					continue;
				c = (c<<16 | c>>16) + d;

				//std::cout << "ddd" << std::endl;
				
				MD5_STEP(HH, b, c, d, a, block[14], 0xfde5380c, 23);
				MD5_STEP(HH, a, b, c, d, block[1], 0xa4beea44, 4);
				MD5_STEP(HH, d, a, b, c, block[4], 0x4bdecfa9, 11);
				MD5_STEP(HH, c, d, a, b, block[7], 0xf6bb4b60, 16);
				MD5_STEP(HH, b, c, d, a, block[10], 0xbebfbc70, 23);
				MD5_STEP(HH, a, b, c, d, block[13], 0x289b7ec6, 4);
				MD5_STEP(HH, d, a, b, c, block[0], 0xeaa127fa, 11);
				MD5_STEP(HH, c, d, a, b, block[3], 0xd4ef3085, 16);
				MD5_STEP(HH, b, c, d, a, block[6], 0x04881d05, 23);
				MD5_STEP(HH, a, b, c, d, block[9], 0xd9d4d039, 4);
				MD5_STEP(HH, d, a, b, c, block[12], 0xe6db99e5, 11);
				MD5_STEP(HH, c, d, a, b, block[15], 0x1fa27cf8, 16);
				MD5_STEP(HH, b, c, d, a, block[2], 0xc4ac5665, 23);
				if (((b ^ d) & 0x80000000) != 0)
					continue;

				MD5_STEP(II, a, b, c, d, block[0], 0xf4292244, 6);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, d, a, b, c, block[7], 0x432aff97, 10);
				if (((b ^ d) >> 31) == 0) 
					continue;
				MD5_STEP(II, c, d, a, b, block[14], 0xab9423a7, 15);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, b, c, d, a, block[5], 0xfc93a039, 21);
				if (((b ^ d) >> 31) != 0) 
					continue;
				MD5_STEP(II, a, b, c, d, block[12], 0x655b59c3, 6);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, d, a, b, c, block[3], 0x8f0ccc92, 10);
				if (((b ^ d) >> 31) != 0) 
					continue;
				MD5_STEP(II, c, d, a, b, block[10], 0xffeff47d, 15);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, b, c, d, a, block[1], 0x85845dd1, 21);
				if (((b ^ d) >> 31) != 0) 
					continue;
				MD5_STEP(II, a, b, c, d, block[8], 0x6fa87e4f, 6);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, d, a, b, c, block[15], 0xfe2ce6e0, 10);
				if (((b ^ d) >> 31) != 0) 
					continue;
				MD5_STEP(II, c, d, a, b, block[6], 0xa3014314, 15);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, b, c, d, a, block[13], 0x4e0811a1, 21);
				if (((b ^ d) >> 31) == 0) 
					continue;
				MD5_STEP(II, a, b, c, d, block[4], 0xf7537e82, 6);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, d, a, b, c, block[11], 0xbd3af235, 10);
				if (((b ^ d) >> 31) != 0) 
					continue;
				MD5_STEP(II, c, d, a, b, block[2], 0x2ad7d2bb, 15);
				if (((a ^ c) >> 31) != 0) 
					continue;
				MD5_STEP(II, b, c, d, a, block[9], 0xeb86d391, 21);

				std::cout << "..." << std::flush;

				uint32 block2[16];
				uint32 IV1[4], IV2[4];
				for (int t = 0; t < 4; ++t)
				{
					IV1[t] = IV[t];
					IV2[t] = IV[t] + (1 << 31);
				}
				IV2[1] -= (1 << 25);
				IV2[2] -= (1 << 25);
				IV2[3] -= (1 << 25);

				for (int t = 0; t < 16; ++t)
					block2[t] = block[t];
				block2[4] += 1 << 31;
				block2[11] += 1 << 15;
				block2[14] += 1 << 31;

				md5_compress(IV1, block);
				md5_compress(IV2, block2);
				if (IV2[0] == IV1[0] && IV2[1] == IV1[1] && IV2[2] == IV1[2] && IV2[3] == IV1[3])
					return;
				if (IV2[0] != IV1[0])
						std::cout << "!" << std::flush;
			}
		}
	}
}
