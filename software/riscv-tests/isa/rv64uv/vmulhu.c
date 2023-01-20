// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matteo Perotti <mperotti@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>

#include "vector_macros.h"

void TEST_CASE1(void) {
  VSET(16, e8, m2);
  VLOAD_8(v4, 0x3b, 0xb2, 0xd1, 0x3e, 0x39, 0x2c, 0x08, 0xc5, 0xbf, 0x54, 0x6c,
          0xde, 0x87, 0xcb, 0x7a, 0x83);
  VLOAD_8(v6, 0x55, 0xde, 0xf4, 0x14, 0x3c, 0xed, 0x47, 0x1b, 0xca, 0x0b, 0xc4,
          0xe3, 0xd8, 0x8f, 0xa0, 0x0d);
  asm volatile("vmulhu.vv v2, v4, v6");
  VCMP_U8(1, v2, 0x13, 0x9a, 0xc7, 0x04, 0x0d, 0x28, 0x02, 0x14, 0x96, 0x03,
          0x52, 0xc4, 0x71, 0x71, 0x4c, 0x06);

  VSET(16, e16, m2);
  VLOAD_16(v4, 0xe6e1, 0x02a1, 0x2911, 0xe3c3, 0xe141, 0x69e6, 0x4133, 0xf783,
           0x91ef, 0x1897, 0xf0bb, 0x0e07, 0xb8eb, 0x3f5a, 0x9f5d, 0xa626);
  VLOAD_16(v6, 0x4fcb, 0x8a38, 0xbaa0, 0x8a97, 0xe409, 0x558e, 0x582b, 0x62b1,
           0xf7bb, 0x181f, 0x2b5a, 0xdf85, 0x44f3, 0x27fe, 0x8412, 0xcda0);
  asm volatile("vmulhu.vv v2, v4, v6");
  VCMP_U16(2, v2, 0x47f6, 0x016b, 0x1df0, 0x7b4d, 0xc8a5, 0x2364, 0x1674,
           0x5f6b, 0x8d38, 0x0251, 0x28c4, 0x0c3f, 0x31cd, 0x09e5, 0x5237,
           0x8574);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xf129e694, 0x8dfc72a3, 0xc9911598, 0xd20083ec, 0xe7f36604,
           0x1ab510aa, 0xc290b86c, 0xa7e9a02e, 0x5c3f3bb3, 0x70a3dfae,
           0x16baad22, 0x21758cfb, 0x09033e60, 0x8b31075e, 0x6439b7bf,
           0xead33cf0);
  VLOAD_32(v6, 0x3f2ef56d, 0x12649032, 0x6c0a880b, 0x7cb2477a, 0x41525037,
           0x02a39cfa, 0xf7595181, 0x0c230035, 0x86cf9ea9, 0x0f66ddd3,
           0x13351370, 0xbe489ce5, 0x4127f488, 0xe6b5e1b3, 0xc6918270,
           0xccc8626a);
  asm volatile("vmulhu.vv v2, v4, v6");
  VCMP_U32(3, v2, 0x3b858c79, 0x0a3386a5, 0x55117fe4, 0x664a7ee4, 0x3b2f618b,
           0x00467bcb, 0xbbfd8432, 0x07f5e895, 0x3093e98c, 0x06c6dd00,
           0x01b49139, 0x18debc33, 0x024b3af0, 0x7d70f100, 0x4dbd9bdf,
           0xbbd823dc);

#if ELEN == 64
  VSET(16, e64, m2);
  VLOAD_64(v4, 0x4aee1e4f9775ff4b, 0x045a804c3d3e7dc0, 0x1a2f38060efcd306,
           0x34e09e5173ee6301, 0xd1f03c2e38769683, 0x1b1f454816d4ed10,
           0xed4a4f231da4abb3, 0xc87b025e6da277dc, 0x8da43ddf6feb6aae,
           0x7dcf10ced634db74, 0x736fd9583bc2de91, 0xa66de0036d350cbc,
           0x40bf5ec7afca9ec2, 0x5bb552a7b134ba79, 0x6ae5d02d7c121603,
           0x8a7621ad8d6f104a);
  VLOAD_64(v6, 0x8f2c0088bea2739e, 0x4ed8c54dad60d3cb, 0x51e715e5cf56b2e6,
           0xa1b1262536ea3c57, 0x67f334468e5cde4f, 0x8ae5618164bd63fd,
           0x2f8be93c1d7807c3, 0x0444a9f4ccff2a2c, 0x6cac5e35bf847d59,
           0x1d92c5117b87a392, 0x124597d21d757a4e, 0x4ec5a9fb5b8a6591,
           0xb5b4189dd6080734, 0xf75ddacea0effff6, 0x5c3cb19fbc1c7580,
           0xff93a562f06d3641);
  asm volatile("vmulhu.vv v2, v4, v6");
  VCMP_U64(4, v2, 0x29e7e403b1955330, 0x015742ce71e2c757, 0x08609392d9402e03,
           0x2165dabfb788d03d, 0x553f1a1e61409141, 0x0eb728a66479b5fb,
           0x2c125410c5448322, 0x0357b1cf05241ad9, 0x3c20a893e10635bb,
           0x0e8895d7f39e953c, 0x083d3ee38137c9b0, 0x3335fb506009220b,
           0x2df4dacbb013b6b0, 0x589d920140d7dd8c, 0x2683eed8bb77fa43,
           0x8a3b86d4dd8169cf);
#endif
};

void TEST_CASE2(void) {
  VSET(16, e8, m2);
  VLOAD_8(v4, 0x3b, 0xb2, 0xd1, 0x3e, 0x39, 0x2c, 0x08, 0xc5, 0xbf, 0x54, 0x6c,
          0xde, 0x87, 0xcb, 0x7a, 0x83);
  VLOAD_8(v6, 0x55, 0xde, 0xf4, 0x14, 0x3c, 0xed, 0x47, 0x1b, 0xca, 0x0b, 0xc4,
          0xe3, 0xd8, 0x8f, 0xa0, 0x0d);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vv v2, v4, v6, v0.t");
  VCMP_U8(5, v2, 0, 0x9a, 0, 0x04, 0, 0x28, 0, 0x14, 0, 0x03, 0, 0xc4, 0, 0x71,
          0, 0x06);

  VSET(16, e16, m2);
  VLOAD_16(v4, 0xe6e1, 0x02a1, 0x2911, 0xe3c3, 0xe141, 0x69e6, 0x4133, 0xf783,
           0x91ef, 0x1897, 0xf0bb, 0x0e07, 0xb8eb, 0x3f5a, 0x9f5d, 0xa626);
  VLOAD_16(v6, 0x4fcb, 0x8a38, 0xbaa0, 0x8a97, 0xe409, 0x558e, 0x582b, 0x62b1,
           0xf7bb, 0x181f, 0x2b5a, 0xdf85, 0x44f3, 0x27fe, 0x8412, 0xcda0);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vv v2, v4, v6, v0.t");
  VCMP_U16(6, v2, 0, 0x016b, 0, 0x7b4d, 0, 0x2364, 0, 0x5f6b, 0, 0x0251, 0,
           0x0c3f, 0, 0x09e5, 0, 0x8574);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xf129e694, 0x8dfc72a3, 0xc9911598, 0xd20083ec, 0xe7f36604,
           0x1ab510aa, 0xc290b86c, 0xa7e9a02e, 0x5c3f3bb3, 0x70a3dfae,
           0x16baad22, 0x21758cfb, 0x09033e60, 0x8b31075e, 0x6439b7bf,
           0xead33cf0);
  VLOAD_32(v6, 0x3f2ef56d, 0x12649032, 0x6c0a880b, 0x7cb2477a, 0x41525037,
           0x02a39cfa, 0xf7595181, 0x0c230035, 0x86cf9ea9, 0x0f66ddd3,
           0x13351370, 0xbe489ce5, 0x4127f488, 0xe6b5e1b3, 0xc6918270,
           0xccc8626a);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vv v2, v4, v6, v0.t");
  VCMP_U32(7, v2, 0, 0x0a3386a5, 0, 0x664a7ee4, 0, 0x00467bcb, 0, 0x07f5e895, 0,
           0x06c6dd00, 0, 0x18debc33, 0, 0x7d70f100, 0, 0xbbd823dc);

#if ELEN == 64
  VSET(16, e64, m2);
  VLOAD_64(v4, 0x4aee1e4f9775ff4b, 0x045a804c3d3e7dc0, 0x1a2f38060efcd306,
           0x34e09e5173ee6301, 0xd1f03c2e38769683, 0x1b1f454816d4ed10,
           0xed4a4f231da4abb3, 0xc87b025e6da277dc, 0x8da43ddf6feb6aae,
           0x7dcf10ced634db74, 0x736fd9583bc2de91, 0xa66de0036d350cbc,
           0x40bf5ec7afca9ec2, 0x5bb552a7b134ba79, 0x6ae5d02d7c121603,
           0x8a7621ad8d6f104a);
  VLOAD_64(v6, 0x8f2c0088bea2739e, 0x4ed8c54dad60d3cb, 0x51e715e5cf56b2e6,
           0xa1b1262536ea3c57, 0x67f334468e5cde4f, 0x8ae5618164bd63fd,
           0x2f8be93c1d7807c3, 0x0444a9f4ccff2a2c, 0x6cac5e35bf847d59,
           0x1d92c5117b87a392, 0x124597d21d757a4e, 0x4ec5a9fb5b8a6591,
           0xb5b4189dd6080734, 0xf75ddacea0effff6, 0x5c3cb19fbc1c7580,
           0xff93a562f06d3641);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vv v2, v4, v6, v0.t");
  VCMP_U64(8, v2, 0, 0x015742ce71e2c757, 0, 0x2165dabfb788d03d, 0,
           0x0eb728a66479b5fb, 0, 0x0357b1cf05241ad9, 0, 0x0e8895d7f39e953c, 0,
           0x3335fb506009220b, 0, 0x589d920140d7dd8c, 0, 0x8a3b86d4dd8169cf);
#endif
};

void TEST_CASE3(void) {
  VSET(16, e8, m2);
  VLOAD_8(v4, 0x5c, 0x3c, 0x86, 0x65, 0x41, 0x38, 0x20, 0x9e, 0x88, 0x28, 0x19,
          0xc2, 0x5f, 0xa3, 0x7c, 0xca);
  uint64_t scalar = 5;
  asm volatile("vmulhu.vx v2, v4, %[A]" ::[A] "r"(scalar));
  VCMP_U8(9, v2, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x03, 0x02, 0x00,
          0x00, 0x03, 0x01, 0x03, 0x02, 0x03);

  VSET(16, e16, m2);
  VLOAD_16(v4, 0x4e7f, 0xfe41, 0x1346, 0x6c1a, 0x38ce, 0x5fa7, 0x5e39, 0xf7a2,
           0x61aa, 0x0a3a, 0xfe0a, 0x30f1, 0x5852, 0xbb6b, 0x42f7, 0x58d9);
  scalar = 816;
  asm volatile("vmulhu.vx v2, v4, %[A]" ::[A] "r"(scalar));
  VCMP_U16(10, v2, 0x00fa, 0x032a, 0x003d, 0x0158, 0x00b5, 0x0130, 0x012c,
           0x0315, 0x0137, 0x0020, 0x0329, 0x009c, 0x0119, 0x0255, 0x00d5,
           0x011b);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0x15c6221c, 0x0d704417, 0x3d90ffd1, 0x4e168273, 0xc3bd5e20,
           0xd75f62df, 0x3002ed42, 0x74269b1d, 0xc77bc0dd, 0x36f2552d,
           0x71b5888c, 0x02eb291b, 0x790cb3b1, 0xa3cf03c4, 0x8f90730a,
           0xf41b555a);
  scalar = 7389998;
  asm volatile("vmulhu.vx v2, v4, %[A]" ::[A] "r"(scalar));
  VCMP_U32(11, v2, 0x00099748, 0x0005eb5c, 0x001b1e60, 0x00226562, 0x00563815,
           0x005eddef, 0x001525e2, 0x00332972, 0x0057de3b, 0x001833e9,
           0x0032161d, 0x0001491b, 0x003551d9, 0x00482775, 0x003f3ca7,
           0x006b8612);

  /* #if ELEN == 64 */
  /*   VSET(16, e64, m2); */
  /*   VLOAD_64(v4, 0x96304201a90be11f, 0x11654d4226322e4b, 0xe16e5cf2c1183b63,
   */
  /*            0x447b5f4710764817, 0xb62589a3d309672c, 0x5ddec2e6716fd0d3, */
  /*            0xf31034a096a6d0fa, 0x9cb4dca46ce577f7, 0x30cf2e2dc6773d82, */
  /*            0x6129247d49c42f4b, 0x3d9ee22336a4e216, 0x3c9b9d533797be90, */
  /*            0x0c0c54042a20ddc8, 0xf309bda968a3a583, 0x550697570a1e9645, */
  /*            0x5beaf5933973231f); */
  /*   scalar = 321156886679781445; */
  /*   asm volatile("vmulhu.vx v2, v4, %[A]" ::[A] "r"(scalar)); */
  /*   VCMP_U64(12, v2, 0x029d61da2f470da8, 0x004d882170361dd2,
   * 0x03ecbc09716942cd, */
  /*            0x013138661b0ea1a1, 0x032bd162449d3f20, 0x01a25fd52874e6a2, */
  /*            0x043b51fe85cf352c, 0x02ba6ebb77802a7c, 0x00d98a5bba81dc57, */
  /*            0x01b10a47f99f8c44, 0x0112a3d22e03b6e9, 0x010e20461059ad6b, */
  /*            0x0035b2b00a44dfe2, 0x043b352e6dc32a00, 0x017af48bc4f5ad70, */
  /*            0x0199ac3dc8053978); */
  /* #endif */
};

void TEST_CASE4(void) {
  VSET(16, e8, m2);
  VLOAD_8(v4, 0x5c, 0x3c, 0x86, 0x65, 0x41, 0x38, 0x20, 0x9e, 0x88, 0x28, 0x19,
          0xc2, 0x5f, 0xa3, 0x7c, 0xca);
  uint64_t scalar = 5;
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vx v2, v4, %[A], v0.t" ::[A] "r"(scalar));
  VCMP_U8(13, v2, 0, 0x01, 0, 0x01, 0, 0x01, 0, 0x03, 0, 0x00, 0, 0x03, 0, 0x03,
          0, 0x03);

  VSET(16, e16, m2);
  VLOAD_16(v4, 0x4e7f, 0xfe41, 0x1346, 0x6c1a, 0x38ce, 0x5fa7, 0x5e39, 0xf7a2,
           0x61aa, 0x0a3a, 0xfe0a, 0x30f1, 0x5852, 0xbb6b, 0x42f7, 0x58d9);
  scalar = 816;
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vx v2, v4, %[A], v0.t" ::[A] "r"(scalar));
  VCMP_U16(14, v2, 0, 0x032a, 0, 0x0158, 0, 0x0130, 0, 0x0315, 0, 0x0020, 0,
           0x009c, 0, 0x0255, 0, 0x011b);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0x15c6221c, 0x0d704417, 0x3d90ffd1, 0x4e168273, 0xc3bd5e20,
           0xd75f62df, 0x3002ed42, 0x74269b1d, 0xc77bc0dd, 0x36f2552d,
           0x71b5888c, 0x02eb291b, 0x790cb3b1, 0xa3cf03c4, 0x8f90730a,
           0xf41b555a);
  scalar = 7389998;
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v2);
  asm volatile("vmulhu.vx v2, v4, %[A], v0.t" ::[A] "r"(scalar));
  VCMP_U32(15, v2, 0, 0x0005eb5c, 0, 0x00226562, 0, 0x005eddef, 0, 0x00332972,
           0, 0x001833e9, 0, 0x0001491b, 0, 0x00482775, 0, 0x006b8612);

  /* #if ELEN == 64 */
  /*   VSET(16, e64, m2); */
  /*   VLOAD_64(v4, 0x96304201a90be11f, 0x11654d4226322e4b, 0xe16e5cf2c1183b63,
   */
  /*            0x447b5f4710764817, 0xb62589a3d309672c, 0x5ddec2e6716fd0d3, */
  /*            0xf31034a096a6d0fa, 0x9cb4dca46ce577f7, 0x30cf2e2dc6773d82, */
  /*            0x6129247d49c42f4b, 0x3d9ee22336a4e216, 0x3c9b9d533797be90, */
  /*            0x0c0c54042a20ddc8, 0xf309bda968a3a583, 0x550697570a1e9645, */
  /*            0x5beaf5933973231f); */
  /*   scalar = 321156886679781445; */
  /*   VLOAD_8(v0, 0xAA, 0xAA); */
  /*   VCLEAR(v2); */
  /*   asm volatile("vmulhu.vx v2, v4, %[A], v0.t" ::[A] "r"(scalar)); */
  /*   VCMP_U64(16, v2, 0, 0x004d882170361dd2, 0, 0x013138661b0ea1a1, 0, */
  /*            0x01a25fd52874e6a2, 0, 0x02ba6ebb77802a7c, 0,
   * 0x01b10a47f99f8c44, 0, */
  /*            0x010e20461059ad6b, 0, 0x043b352e6dc32a00, 0,
   * 0x0199ac3dc8053978); */
  /* #endif */
};

int main(void) {
  INIT_CHECK();
  enable_vec();

  TEST_CASE1();
  // TEST_CASE2();
  TEST_CASE3();
  // TEST_CASE4();

  EXIT_CHECK();
}
