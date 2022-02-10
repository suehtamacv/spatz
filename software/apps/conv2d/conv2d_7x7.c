#include "conv2d.h"
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define R 64
#define C 64
#define F 7

void conv2d_7x7(int32_t *o, int32_t *i, int32_t *f) {
  // We work on 2 rows of the output matrix at once
  int32_t block_size_o = 2;
  // We work on block_size_o + F - 1 rows of the input matrix at once

  int32_t block_size_c;

  // Set the vector configuration
  asm volatile("vsetvli %0, %1, e32, m2, ta, ma" : "=r"(block_size_c) : "r"(C+F-1));

  block_size_c = block_size_c - F + 1;

  for (int32_t c = 0; c < C; c += block_size_c) {

    int32_t c_ = MIN(C - c, block_size_c);

    // First iteration round, r = 0
    int32_t *i_ = i + c;
    int32_t *o_ = o + c;

    // For simplicity, compute over the padding rows as well
    //conv2d_vec_4xC_slice_init_7x7(o_, c_);
    // Preload the first two input rows -> This is not needed in the other rounds
    conv2d_vec_4xC_slice_preload_7x7(i_, c_);
    // The first F-1 rows have already been loaded by conv2d_vec_4xC_slice_init()
    int32_t *i__ = i_ + (F - 1) * (C + F - 1);
    conv2d_vec_4xC_7x7(o_, i__, f, c_);
    // Re-use some of the already-loa);
    // Re-use some of the already-loaded input rows
    //conv2d_vec_4xC_slice_move_7x7(C, F);

    // Iterate over the output rows
    for (int32_t r = block_size_o; r < R; r += block_size_o) {
      i_ = i + r * (C + F - 1) + c;
      o_ = o + r * C + c;

      // For simplicity, compute over the padding rows as well
      //conv2d_vec_4xC_slice_init_7x7(o_, C);
      // The first F-1 rows have already been loaded by
      // conv2d_vec_4xC_slice_init()
      i__ = i_ + (F - 1) * (C + F - 1);
      conv2d_vec_4xC_7x7(o_, i__, f, c_);
      // Re-use some of the already-loaded input rows
      //conv2d_vec_4xC_slice_move_7x7(C, F);
    }
  }
}

// Load 2 rows of the output matrix
void conv2d_vec_4xC_slice_init_7x7(int32_t c) {
  // Set the vector configuration
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(c));
  // Fetch 2 output rows
  asm volatile("vmv.v.i v0,  0");
  asm volatile("vmv.v.i v2,  0");
}

// Load 4 rows of the output matrix
void conv2d_vec_4xC_slice_preload_7x7(int32_t *i, int32_t c) {
  // Helper variables
  int32_t lwi = (C + F - 1) << 2;

  // Set the vector configuration
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(c + F - 1));
  // Fetch the first F-1 = 6 input rows
  asm volatile("vle32.v v4,  (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("vle32.v v6,  (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("vle32.v v8,  (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("vle32.v v10, (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("vle32.v v12, (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("vle32.v v14, (%0)" :: "r"(i));
}

// Calculate 4 output matrix rows
void conv2d_vec_4xC_7x7(int32_t *o, int32_t *i, int32_t *f, int32_t c) {

  // Temporary variables (one filter column)
  int32_t t0, t1, t2, t3, t4, t5, t6;
  int32_t slamt = 1;

  // Helper variables
  int32_t lwo = C << 2;
  int32_t lwi = (C + F - 1) << 2;
  int32_t lwf = F << 2;
  int32_t *f_;

  // Compute on C elements
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(c + F - 1));
  // Fetch other 2 rows of the input matrix
  asm volatile("vle32.v v16, (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("vmv.v.i v0,  0");
  f_ = f;
  asm volatile("vle32.v v18, (%0); add %0, %0, %1" : "+&r"(i) : "r"(lwi));
  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t0) : "r"(lwf));
  asm volatile("vmv.v.i v2,  0");

  // Compute on C elements
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(c));
  // Fetch the first column of the filter, and start calculating its
  // contribution on the two output rows (v0, v2)
  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t1) : "r"(lwf));
  asm volatile("vmacc.vx v0, %0, v4" ::"r"(t0));
  asm volatile("vmacc.vx v2, %0, v6" ::"r"(t0));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t2) : "r"(lwf));
  asm volatile("vmacc.vx v0, %0, v6" ::"r"(t1));
  asm volatile("vmacc.vx v2, %0, v8" ::"r"(t1));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t3) : "r"(lwf));
  asm volatile("vmacc.vx v0, %0, v8" ::"r"(t2));
  asm volatile("vmacc.vx v2, %0, v10" ::"r"(t2));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t4) : "r"(lwf));
  asm volatile("vmacc.vx v0, %0, v10" ::"r"(t3));
  asm volatile("vmacc.vx v2, %0, v12" ::"r"(t3));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t5) : "r"(lwf));
  asm volatile("vmacc.vx v0, %0, v12" ::"r"(t4));
  asm volatile("vmacc.vx v2, %0, v14" ::"r"(t4));

  asm volatile("lw %1, (%0);" : "+&r"(f_), "=&r"(t6));
  asm volatile("vmacc.vx v0, %0, v14" ::"r"(t5));
  asm volatile("vmacc.vx v2, %0, v16" ::"r"(t5));

  asm volatile("vmacc.vx v0, %0, v16" ::"r"(t6));
  //slamt += 1;
  asm volatile("vslidedown.vx v20, v4, %0" ::"r"(slamt));
  f_ = f+1;
  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t0) : "r"(lwf));
  asm volatile("vmacc.vx v2, %0, v18" ::"r"(t6));

  for (int32_t idx = 1; idx < F - 1; ++idx) {
    // Fetch the other columns of the filter (except for the last one), and
    // start calculating their contributions on the two output rows (v0, v2) To
    // do so, at each iteration slide down the input rows by one
    asm volatile("lw %1, (%0); add %0, %0, %2"
                 : "+&r"(f_), "=&r"(t1)
                 : "r"(lwf));
    asm volatile("vslidedown.vx v22, v6,  %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v20" ::"r"(t0));

    asm volatile("lw %1, (%0); add %0, %0, %2"
                 : "+&r"(f_), "=&r"(t2)
                 : "r"(lwf));
    asm volatile("vslidedown.vx v24, v8,  %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v22" ::"r"(t1));
    asm volatile("vmacc.vx v2, %0, v22" ::"r"(t0));

    asm volatile("lw %1, (%0); add %0, %0, %2"
                 : "+&r"(f_), "=&r"(t3)
                 : "r"(lwf));
    asm volatile("vslidedown.vx v26, v10, %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v24" ::"r"(t2));
    asm volatile("vmacc.vx v2, %0, v24" ::"r"(t1));

    asm volatile("lw %1, (%0); add %0, %0, %2"
                 : "+&r"(f_), "=&r"(t4)
                 : "r"(lwf));
    asm volatile("vslidedown.vx v28, v12, %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v26" ::"r"(t3));
    asm volatile("vmacc.vx v2, %0, v26" ::"r"(t2));

    asm volatile("lw %1, (%0); add %0, %0, %2"
                 : "+&r"(f_), "=&r"(t5)
                 : "r"(lwf));
    asm volatile("vslidedown.vx v30, v14, %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v28" ::"r"(t4));
    asm volatile("vmacc.vx v2, %0, v28" ::"r"(t3));

    asm volatile("lw %1, (%0);" : "+&r"(f_), "=&r"(t6));
    asm volatile("vslidedown.vx v20, v16, %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v30" ::"r"(t5));
    asm volatile("vmacc.vx v2, %0, v30" ::"r"(t4));

    asm volatile("vslidedown.vx v22, v18, %0" ::"r"(slamt));
    asm volatile("vmacc.vx v0, %0, v20" ::"r"(t6));
    f_ = f+idx+1;
    asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t0) : "r"(lwf));
    asm volatile("vmacc.vx v2, %0, v20" ::"r"(t5));

    slamt += 1;
    asm volatile("vslidedown.vx v20, v4,  %0" ::"r"(slamt));
    asm volatile("vmacc.vx v2, %0, v22" ::"r"(t6));
  }

  //f_ = f + (F - 1) + lwf;
  //slamt = F - 1;
  // Repeat for the last filter column, and then store the output rows
  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t1) : "r"(lwf));
  asm volatile("vslidedown.vx v22, v6,  %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v20" ::"r"(t0));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t2) : "r"(lwf));
  asm volatile("vslidedown.vx v24, v8,  %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v22" ::"r"(t1));
  asm volatile("vmacc.vx v2, %0, v22" ::"r"(t0));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t3) : "r"(lwf));
  asm volatile("vslidedown.vx v26, v10, %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v24" ::"r"(t2));
  asm volatile("vmacc.vx v2, %0, v24" ::"r"(t1));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t4) : "r"(lwf));
  asm volatile("vslidedown.vx v28, v12, %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v26" ::"r"(t3));
  asm volatile("vmacc.vx v2, %0, v26" ::"r"(t2));

  asm volatile("lw %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t5) : "r"(lwf));
  asm volatile("vslidedown.vx v30, v14, %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v28" ::"r"(t4));
  asm volatile("vmacc.vx v2, %0, v28" ::"r"(t3));

  asm volatile("lw %1, (%0);" : "+&r"(f_), "=&r"(t6));
  asm volatile("vslidedown.vx v20, v16, %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v30" ::"r"(t5));
  asm volatile("vmacc.vx v2, %0, v30" ::"r"(t4));

  asm volatile("vslidedown.vx v22, v18, %0" ::"r"(slamt));
  asm volatile("vmacc.vx v0, %0, v20" ::"r"(t6));
  asm volatile("vse32.v  v0, (%0); add %0, %0, %1" : "+&r"(o) : "r"(lwo));
  asm volatile("vmacc.vx v2, %0, v20" ::"r"(t5));

  asm volatile("vmacc.vx v2, %0, v22" ::"r"(t6));
  asm volatile("vse32.v  v2, (%0); add %0, %0, %1" : "+&r"(o) : "r"(lwo));

  // Move C+F-1 elements
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(c + F - 1));
  // Move the last F-1 input rows
  asm volatile("vmv.v.v v4, v8");
  asm volatile("vmv.v.v v6, v10");
  asm volatile("vmv.v.v v8, v12");
  asm volatile("vmv.v.v v10, v14");
  asm volatile("vmv.v.v v12, v16");
  asm volatile("vmv.v.v v14, v18");
}

void conv2d_vec_4xC_slice_move_7x7(int32_t c) {
  // Move C+F-1 elements
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(c + F - 1));
  // Move the last F-1 input rows
  asm volatile("vmv.v.v v4, v8");
  asm volatile("vmv.v.v v6, v10");
  asm volatile("vmv.v.v v8, v12");
  asm volatile("vmv.v.v v10, v14");
  asm volatile("vmv.v.v v12, v16");
  asm volatile("vmv.v.v v14, v18");
}

#undef R
#undef C
#undef F
