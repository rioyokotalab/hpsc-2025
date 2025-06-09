#include <cstdio>
#include <cstdlib>
#include <cmath>

int main() {
  const int N = 16;
  float x[N], y[N], m[N], fx[N], fy[N];
  for(int i=0; i<N; i++) {
    x[i] = drand48();
    y[i] = drand48();
    m[i] = drand48();
    fx[i] = fy[i] = 0;
  }
  for(int i=0; i<N; i++) {
    __m512 xi  = _mm512_set1_ps(x[i]);
    __m512 yi  = _mm512_set1_ps(y[i]);
    __m512 fxi = _mm512_setzero_ps();
    __m512 fyi = _mm512_setzero_ps();
    for(int j=0; j<N; j+=16) {
       __m512 xj = _mm512_loadu_ps(&x[j]);
      __m512 yj = _mm512_loadu_ps(&y[j]);
      __m512 mj = _mm512_loadu_ps(&m[j]);

      __m512 rx = _mm512_sub_ps(xi, xj);
      __m512 ry = _mm512_sub_ps(yi, yj);
      __m512 r2 = _mm512_fmadd_ps(ry, ry, _mm512_mul_ps(rx, rx));

      __m512 inv_r = _mm512_rsqrt14_ps(r2);            
      __m512 inv_r2 = _mm512_mul_ps(inv_r, inv_r);
      __m512 inv_r3 = _mm512_mul_ps(inv_r, inv_r2);    

      __m512 coef = _mm512_mul_ps(mj, inv_r3);         
      __m512 fxv  = _mm512_mul_ps(rx, coef);
      __m512 fyv  = _mm512_mul_ps(ry, coef);


      __m512i idx = _mm512_set_epi32(
        j+15,j+14,j+13,j+12,j+11,j+10,j+9,j+8,
        j+7,j+6,j+5,j+4,j+3,j+2,j+1,j+0);
      __mmask16 self = _mm512_cmpeq_epi32_mask(idx, _mm512_set1_epi32(i));
      fxv = _mm512_mask_blend_ps(self, fxv, _mm512_setzero_ps());
      fyv = _mm512_mask_blend_ps(self, fyv, _mm512_setzero_ps());

      fxi = _mm512_add_ps(fxi, fxv);
      fyi = _mm512_add_ps(fyi, fyv);
    }
    fx[i] -= _mm512_reduce_add_ps(fxi);
    fy[i] -= _mm512_reduce_add_ps(fyi);
    printf("%d %g %g\n",i,fx[i],fy[i]);
  }
}
