
#include <cmath>
#include <float.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>


#ifndef VP_SIMD128
#define VP_SIMD128 1
#endif


#ifndef VP_SIMD256
#define VP_SIMD256 0
#endif

#ifndef VP_SIMD512
#define VP_SIMD512 0
#endif


#if VP_SIMD512 
#define VP_SIMD_NAMESPACE simd512
namespace VP_SIMD_NAMESPACE {
    #define VP_SIMD 1
    #define VP_SIMD_WIDTH 64

	struct v_uint8x64
	{
		typedef unsigned char lane_type;
		enum { nlanes = 64 };
		__m512i val;

		explicit v_uint8x64(__m512i v) : val(v) {}
		v_uint8x64(unsigned char v0,  unsigned char v1,  unsigned char v2,  unsigned char v3,
			unsigned char v4,  unsigned char v5,  unsigned char v6,  unsigned char v7,
			unsigned char v8,  unsigned char v9,  unsigned char v10, unsigned char v11,
			unsigned char v12, unsigned char v13, unsigned char v14, unsigned char v15,
			unsigned char v16, unsigned char v17, unsigned char v18, unsigned char v19,
			unsigned char v20, unsigned char v21, unsigned char v22, unsigned char v23,
			unsigned char v24, unsigned char v25, unsigned char v26, unsigned char v27,
			unsigned char v28, unsigned char v29, unsigned char v30, unsigned char v31,
			unsigned char v32, unsigned char v33, unsigned char v34, unsigned char v35,
			unsigned char v36, unsigned char v37, unsigned char v38, unsigned char v39,
			unsigned char v40, unsigned char v41, unsigned char v42, unsigned char v43,
			unsigned char v44, unsigned char v45, unsigned char v46, unsigned char v47,
			unsigned char v48, unsigned char v49, unsigned char v50, unsigned char v51,
			unsigned char v52, unsigned char v53, unsigned char v54, unsigned char v55,
			unsigned char v56, unsigned char v57, unsigned char v58, unsigned char v59,
			unsigned char v60, unsigned char v61, unsigned char v62, unsigned char v63)
		{
			val = _v512_set_epu8(v63, v62, v61, v60, v59, v58, v57, v56, v55, v54, v53, v52, v51, v50, v49, v48,
				v47, v46, v45, v44, v43, v42, v41, v40, v39, v38, v37, v36, v35, v34, v33, v32,
				v31, v30, v29, v28, v27, v26, v25, v24, v23, v22, v21, v20, v19, v18, v17, v16,
				v15, v14, v13, v12, v11, v10, v9,  v8,  v7,  v6,  v5,  v4,  v3,  v2,  v1,  v0);
		}
		v_uint8x64() {}

		unsigned char get0() const { return (unsigned char)_mm_cvtsi128_si32(_mm512_castsi512_si128(val)); }
	};

	struct v_uint16x32
	{
		typedef unsigned short lane_type;
		enum { nlanes = 32 };
		__m512i val;

		explicit v_uint16x32(__m512i v) : val(v) {}
		v_uint16x32(unsigned short v0,  unsigned short v1,  unsigned short v2,  unsigned short v3,
			unsigned short v4,  unsigned short v5,  unsigned short v6,  unsigned short v7,
			unsigned short v8,  unsigned short v9,  unsigned short v10, unsigned short v11,
			unsigned short v12, unsigned short v13, unsigned short v14, unsigned short v15,
			unsigned short v16, unsigned short v17, unsigned short v18, unsigned short v19,
			unsigned short v20, unsigned short v21, unsigned short v22, unsigned short v23,
			unsigned short v24, unsigned short v25, unsigned short v26, unsigned short v27,
			unsigned short v28, unsigned short v29, unsigned short v30, unsigned short v31)
		{
			val = _v512_set_epu16(v31, v30, v29, v28, v27, v26, v25, v24, v23, v22, v21, v20, v19, v18, v17, v16,
				v15, v14, v13, v12, v11, v10, v9,  v8,  v7,  v6,  v5,  v4,  v3,  v2,  v1,  v0);
		}
		v_uint16x32() {}

		unsigned short get0() const { return (unsigned short)_mm_cvtsi128_si32(_mm512_castsi512_si128(val)); }
	};

	struct v_uint32x16
	{
		typedef unsigned lane_type;
		enum { nlanes = 16 };
		__m512i val;

		explicit v_uint32x16(__m512i v) : val(v) {}
		v_uint32x16(unsigned v0,  unsigned v1,  unsigned v2,  unsigned v3,
			unsigned v4,  unsigned v5,  unsigned v6,  unsigned v7,
			unsigned v8,  unsigned v9,  unsigned v10, unsigned v11,
			unsigned v12, unsigned v13, unsigned v14, unsigned v15)
		{
			val = _mm512_setr_epi32((int)v0,  (int)v1,  (int)v2,  (int)v3, (int)v4,  (int)v5,  (int)v6,  (int)v7,
				(int)v8,  (int)v9,  (int)v10, (int)v11, (int)v12, (int)v13, (int)v14, (int)v15);
		}
		v_uint32x16() {}

		unsigned get0() const { return (unsigned)_mm_cvtsi128_si32(_mm512_castsi512_si128(val)); }
	};

	struct v_int32x16
	{
		typedef int lane_type;
		enum { nlanes = 16 };
		__m512i val;

		explicit v_int32x16(__m512i v) : val(v) {}
		v_int32x16(int v0, int v1, int v2,  int v3,  int v4,  int v5,  int v6,  int v7,
			int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15)
		{
			val = _mm512_setr_epi32(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15);
		}
		v_int32x16() {}

		int get0() const { return _mm_cvtsi128_si32(_mm512_castsi512_si128(val)); }
	};

    //! @brief Maximum available vector register capacity 8-bit unsigned integer values
    typedef v_uint8x64    v_uint8;
	//! @brief Maximum available vector register capacity 16-bit unsigned integer values
	typedef v_uint16x32   v_uint16;
    //! @brief Maximum available vector register capacity 32-bit unsigned integer values
    typedef v_uint32x16   v_uint32;
    //! @brief Maximum available vector register capacity 32-bit signed integer values
    typedef v_int32x16    v_int32;

} // namespace
using namespace VP_SIMD_NAMESPACE;
#elif VP_SIMD256 
#define VP_SIMD_NAMESPACE simd256
namespace VP_SIMD_NAMESPACE {
    #define VP_SIMD 1
    #define VP_SIMD_WIDTH 32

	struct v_uint8x32
	{
		typedef unsigned char lane_type;
		enum { nlanes = 32 };
		__m256i val;

		explicit v_uint8x32(__m256i v) : val(v) {}
		v_uint8x32(unsigned char v0,  unsigned char v1,  unsigned char v2,  unsigned char v3,
			unsigned char v4,  unsigned char v5,  unsigned char v6,  unsigned char v7,
			unsigned char v8,  unsigned char v9,  unsigned char v10, unsigned char v11,
			unsigned char v12, unsigned char v13, unsigned char v14, unsigned char v15,
			unsigned char v16, unsigned char v17, unsigned char v18, unsigned char v19,
			unsigned char v20, unsigned char v21, unsigned char v22, unsigned char v23,
			unsigned char v24, unsigned char v25, unsigned char v26, unsigned char v27,
			unsigned char v28, unsigned char v29, unsigned char v30, unsigned char v31)
		{
			val = _mm256_setr_epi8((char)v0, (char)v1, (char)v2, (char)v3,
				(char)v4,  (char)v5,  (char)v6 , (char)v7,  (char)v8,  (char)v9,
				(char)v10, (char)v11, (char)v12, (char)v13, (char)v14, (char)v15,
				(char)v16, (char)v17, (char)v18, (char)v19, (char)v20, (char)v21,
				(char)v22, (char)v23, (char)v24, (char)v25, (char)v26, (char)v27,
				(char)v28, (char)v29, (char)v30, (char)v31);
		}
		/* coverity[uninit_ctor]: suppress warning */
		v_uint8x32() {}

		unsigned char get0() const { return (unsigned char)_mm_cvtsi128_si32(_mm256_castsi256_si128(val)); }
	};

	struct v_uint16x16
	{
		typedef unsigned short lane_type;
		enum { nlanes = 16 };
		__m256i val;

		explicit v_uint16x16(__m256i v) : val(v) {}
		v_uint16x16(unsigned short v0,  unsigned short v1,  unsigned short v2,  unsigned short v3,
			unsigned short v4,  unsigned short v5,  unsigned short v6,  unsigned short v7,
			unsigned short v8,  unsigned short v9,  unsigned short v10, unsigned short v11,
			unsigned short v12, unsigned short v13, unsigned short v14, unsigned short v15)
		{
			val = _mm256_setr_epi16((short)v0, (short)v1, (short)v2, (short)v3,
				(short)v4,  (short)v5,  (short)v6,  (short)v7,  (short)v8,  (short)v9,
				(short)v10, (short)v11, (short)v12, (short)v13, (short)v14, (short)v15);
		}
		/* coverity[uninit_ctor]: suppress warning */
		v_uint16x16() {}

		unsigned short get0() const { return (unsigned short)_mm_cvtsi128_si32(_mm256_castsi256_si128(val)); }
	};

	struct v_uint32x8
	{
		typedef unsigned lane_type;
		enum { nlanes = 8 };
		__m256i val;

		explicit v_uint32x8(__m256i v) : val(v) {}
		v_uint32x8(unsigned v0, unsigned v1, unsigned v2, unsigned v3,
			unsigned v4, unsigned v5, unsigned v6, unsigned v7)
		{
			val = _mm256_setr_epi32((unsigned)v0, (unsigned)v1, (unsigned)v2,
				(unsigned)v3, (unsigned)v4, (unsigned)v5, (unsigned)v6, (unsigned)v7);
		}
		/* coverity[uninit_ctor]: suppress warning */
		v_uint32x8() {}

		unsigned get0() const { return (unsigned) _mm_cvtsi128_si32(_mm256_castsi256_si128(val)); }
	};

	struct v_int32x8
	{
		typedef int lane_type;
		enum { nlanes = 8 };
		__m256i val;

		explicit v_int32x8(__m256i v) : val(v) {}
		v_int32x8(int v0, int v1, int v2, int v3,
			int v4, int v5, int v6, int v7)
		{
			val = _mm256_setr_epi32(v0, v1, v2, v3, v4, v5, v6, v7);
		}
		/* coverity[uninit_ctor]: suppress warning */
		v_int32x8() {}

		int get0() const { return _mm_cvtsi128_si32(_mm256_castsi256_si128(val)); }
	};


//! @addtogroup core_hal_intrin
//! @{
    //! @brief Maximum available vector register capacity 8-bit unsigned integer values
    typedef v_uint8x32   v_uint8;
	//! @brief Maximum available vector register capacity 16-bit unsigned integer values
	typedef v_uint16x16  v_uint16;
    //! @brief Maximum available vector register capacity 32-bit unsigned integer values
    typedef v_uint32x8   v_uint32;
    //! @brief Maximum available vector register capacity 32-bit signed integer values
    typedef v_int32x8    v_int32;

} // namespace
using namespace VP_SIMD_NAMESPACE;
#elif VP_SIMD128 
#define VP_SIMD_NAMESPACE simd128
namespace VP_SIMD_NAMESPACE {
    #define VP_SIMD VP_SIMD128
    #define VP_SIMD_WIDTH 16

	struct v_uint8x16
	{
		typedef unsigned char lane_type;
		typedef __m128i vector_type;
		enum { nlanes = 16 };

		/* coverity[uninit_ctor]: suppress warning */
		v_uint8x16() {}
		explicit v_uint8x16(__m128i v) : val(v) {}
		v_uint8x16(unsigned char v0, unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4, unsigned char v5, unsigned char v6, unsigned char v7,
			unsigned char v8, unsigned char v9, unsigned char v10, unsigned char v11, unsigned char v12, unsigned char v13, unsigned char v14, unsigned char v15)
		{
			val = _mm_setr_epi8((char)v0, (char)v1, (char)v2, (char)v3,
				(char)v4, (char)v5, (char)v6, (char)v7,
				(char)v8, (char)v9, (char)v10, (char)v11,
				(char)v12, (char)v13, (char)v14, (char)v15);
		}

		unsigned char get0() const
		{
			return (unsigned char)_mm_cvtsi128_si32(val);
		}

		__m128i val;
	};

	struct v_uint16x8
	{
		typedef unsigned short lane_type;
		typedef __m128i vector_type;
		enum { nlanes = 8 };

		/* coverity[uninit_ctor]: suppress warning */
		v_uint16x8() {}
		explicit v_uint16x8(__m128i v) : val(v) {}
		v_uint16x8(unsigned short v0, unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4, unsigned short v5, unsigned short v6, unsigned short v7)
		{
			val = _mm_setr_epi16((short)v0, (short)v1, (short)v2, (short)v3,
				(short)v4, (short)v5, (short)v6, (short)v7);
		}

		unsigned short get0() const
		{
			return (unsigned short)_mm_cvtsi128_si32(val);
		}

		__m128i val;
	};

	struct v_uint32x4
	{
		typedef unsigned lane_type;
		typedef __m128i vector_type;
		enum { nlanes = 4 };

		/* coverity[uninit_ctor]: suppress warning */
		v_uint32x4() {}
		explicit v_uint32x4(__m128i v) : val(v) {}
		v_uint32x4(unsigned v0, unsigned v1, unsigned v2, unsigned v3)
		{
			val = _mm_setr_epi32((int)v0, (int)v1, (int)v2, (int)v3);
		}

		unsigned get0() const
		{
			return (unsigned)_mm_cvtsi128_si32(val);
		}

		__m128i val;
	};

	struct v_int32x4
	{
		typedef int lane_type;
		typedef __m128i vector_type;
		enum { nlanes = 4 };

		/* coverity[uninit_ctor]: suppress warning */
		v_int32x4() {}
		explicit v_int32x4(__m128i v) : val(v) {}
		v_int32x4(int v0, int v1, int v2, int v3)
		{
			val = _mm_setr_epi32(v0, v1, v2, v3);
		}

		int get0() const
		{
			return _mm_cvtsi128_si32(val);
		}

		__m128i val;
	};
    //! @brief Maximum available vector register capacity 8-bit unsigned integer values
    typedef v_uint8x16  v_uint8;
	//! @brief Maximum available vector register capacity 16-bit unsigned integer values
	typedef v_uint16x8  v_uint16;
    //! @brief Maximum available vector register capacity 32-bit unsigned integer values
    typedef v_uint32x4  v_uint32;
    //! @brief Maximum available vector register capacity 32-bit signed integer values
    typedef v_int32x4   v_int32;

} // namespace
using namespace VP_SIMD_NAMESPACE;

#endif


#if VP_SIMD

namespace VP_SIMD_NAMESPACE {

    //! @name Wide load from memory
    //! @{
    //! @brief Load maximum available capacity register contents from memory
	//template<typename _Tp, typename _Tvec> static inline
    inline v_uint8 vp_load(const unsigned char * ptr) 
	{
#if VP_SIMD512
		return (v_uint8)_mm512_loadu_si512((const __m512i*)ptr);

#elif VP_SIMD256
		return (v_uint8)_mm256_loadu_si256((const __m256i*)ptr);

#elif VP_SIMD128 
		return (v_uint8)_mm_loadu_si128((const __m128i*)ptr); 
#endif
	}

	inline v_uint16 vp_load(const unsigned short * ptr) 
	{
#if VP_SIMD512
		return (v_uint16)_mm512_loadu_si512((const __m512i*)ptr);

#elif VP_SIMD256
		return (v_uint16)_mm256_loadu_si256((const __m256i*)ptr);

#elif VP_SIMD128 
		return (v_uint16)_mm_loadu_si128((const __m128i*)ptr); 
#endif
	}
	
    inline v_int32 vp_load(const int * ptr) 
	{
#if VP_SIMD512
		return (v_int32)_mm512_loadu_si512((const __m512i*)ptr);

#elif VP_SIMD256
		return (v_int32)_mm256_loadu_si256((const __m256i*)ptr);

#elif VP_SIMD128 
		return (v_int32)_mm_loadu_si128((const __m128i*)ptr); 
#endif
	}
	
    inline v_uint32 vp_load(const unsigned * ptr) 
	{
#if VP_SIMD512
		return (v_uint32)_mm512_loadu_si512((const __m512i*)ptr);

#elif VP_SIMD256
		return (v_uint32)_mm256_loadu_si256((const __m256i*)ptr);

#elif VP_SIMD128 
		return (v_uint32)_mm_loadu_si128((const __m128i*)ptr); 
#endif
	}


//! @cond IGNORED

    //// backward compatibility
    template<typename _Tp, typename _Tvec> static inline
    void vp_store(_Tp* dst, const _Tvec& v) 
	{
#if VP_SIMD512
		return _mm512_storeu_si512((__m512i*)dst, v.val); ;

#elif VP_SIMD256
		return  _mm256_storeu_si256((__m256i*)dst, v.val);

#elif VP_SIMD128 
		return _mm_storeu_si128((__m128i*)dst,v.val); 
#endif
	}

	inline v_uint8 vp_reverse(const v_uint8 &a)
	{
#if VP_SIMD512
		static const __m512i shuf = _mm512_set_epi32(
			0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
			0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
			0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
			0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f);
		static const __m512i perm = _mm512_set_epi64(1, 0, 3, 2, 5, 4, 7, 6);
		__m512i vec = _mm512_shuffle_epi8(a.val, shuf);
		return v_uint8(_mm512_permutexvar_epi64(perm, vec));

#elif VP_SIMD256
		static const __m256i perm = _mm256_setr_epi8(
			15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
			15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
		__m256i vec = _mm256_shuffle_epi8(a.val, perm);
		return v_uint8(_mm256_permute2x128_si256(vec, vec, 1));

#elif VP_SIMD128
		static const __m128i perm = _mm_setr_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
		return v_uint8(_mm_shuffle_epi8(a.val, perm));
#endif
	}

	inline v_uint16 vp_reverse(const v_uint16 &a)
	{
#if VP_SIMD512
		static const __m512i shuf = _mm512_set_epi32(
			0x01000302, 0x05040706, 0x09080b0a, 0x0d0c0f0e,
			0x01000302, 0x05040706, 0x09080b0a, 0x0d0c0f0e,
			0x01000302, 0x05040706, 0x09080b0a, 0x0d0c0f0e,
			0x01000302, 0x05040706, 0x09080b0a, 0x0d0c0f0e);
		static const __m512i perm = _mm512_set_epi64(1, 0, 3, 2, 5, 4, 7, 6);
		__m512i vec = _mm512_shuffle_epi8(a.val, shuf);
		return v_uint16(_mm512_permutexvar_epi64(perm, vec));

#elif VP_SIMD256
		static const __m256i perm = _mm256_setr_epi8(
			14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1,
			14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
		__m256i vec = _mm256_shuffle_epi8(a.val, perm);
		return v_uint16(_mm256_permute2x128_si256(vec, vec, 1));

#elif VP_SIMD128
		static const __m128i perm = _mm_setr_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
		return v_uint16(_mm_shuffle_epi8(a.val, perm));
#endif
	}

	inline v_uint32 vp_reverse(const v_uint32 &a)
	{
#if VP_SIMD512
		static const __m512i perm = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,14, 15);
		return v_uint32(_mm512_permutexvar_epi32(perm, a.val));

#elif VP_SIMD256
		static const __m256i perm = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
		return v_uint32(_mm256_permutevar8x32_epi32(a.val, perm));

#elif VP_SIMD128
		return v_uint32(_mm_shuffle_epi32(a.val, _MM_SHUFFLE(0, 1, 2, 3)));

#endif
	}

} // namespace

#endif

#ifndef VP_SIMD
#define VP_SIMD 0
#endif



