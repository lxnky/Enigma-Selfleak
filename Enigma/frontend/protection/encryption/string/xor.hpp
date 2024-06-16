#ifndef XORSTR_H
#define XORSTR_H

#include <Windows.h>

#define XORSTR_INLINE	__forceinline
#define XORSTR_NOINLINE __declspec( noinline )
#define XORSTR_CONST	constexpr
#define XORSTR_VOLATILE volatile

#define XORSTR_CONST_INLINE \
XORSTR_INLINE XORSTR_CONST 

#define XORSTR_CONST_NOINLINE \
XORSTR_NOINLINE XORSTR_CONST

#define XORSTR_FNV_OFFSET_BASIS 0xCBF29CE484222325
#define XORSTR_FNV_PRIME 0x100000001B3

#define XORSTR_TYPE_SIZEOF( _VALUE ) \
sizeof( decltype( _VALUE ) )

#define XORSTR_BYTE( _VALUE, _IDX )	\
( ( _VALUE >> ( __min( _IDX, ( XORSTR_TYPE_SIZEOF( _VALUE ) ) - 1)  * 8 ) ) & 0xFF )

#define XORSTR_NIBBLE( _VALUE, _IDX ) \
( ( _VALUE >> ( __min( _IDX, ( XORSTR_TYPE_SIZEOF( _VALUE ) * 2 ) - 1 ) * 4 ) ) & 0xF )

#define XORSTR_MAKE_INTEGER_SEQUENCE( _LEN_ ) \
__make_integer_seq< XORSTR_INT_SEQ, SIZE_T, _LEN_ >( )

#define XORSTR_INTEGER_SEQUENCE( _INDICES_ ) \
XORSTR_INT_SEQ< SIZE_T, _INDICES_... >

template< typename _Ty, _Ty... Types >
struct XORSTR_INT_SEQ
{
};

XORSTR_CONST_NOINLINE
INT XORSTR_ATOI8(
	IN CHAR Character
) noexcept
{
	return (Character >= '0' && Character <= '9') ?
		(Character - '0') : NULL;
}


XORSTR_CONST_NOINLINE
UINT64 XORSTR_KEY(
	IN SIZE_T CryptStrLength
) noexcept
{
	UINT64 KeyHash = XORSTR_FNV_OFFSET_BASIS;

	for (SIZE_T i = NULL; i < sizeof(__TIME__); i++) {
		KeyHash = KeyHash ^ (XORSTR_ATOI8(__TIME__[i]) + (CryptStrLength * i)) & 0xFF;
		KeyHash = KeyHash * XORSTR_FNV_PRIME;
	}

	return KeyHash;
}

template< typename _CHAR_TYPE_,
	SIZE_T _STR_LENGTH_ >
class _XORSTR_
{
public:
	XORSTR_CONST_INLINE _XORSTR_(
		IN _CHAR_TYPE_ CONST(&String)[_STR_LENGTH_]
	) : _XORSTR_(String, XORSTR_MAKE_INTEGER_SEQUENCE(_STR_LENGTH_))
	{
	}


	XORSTR_INLINE
		CONST _CHAR_TYPE_* String(
			VOID
		)
	{
		for (SIZE_T i = NULL; i < _STR_LENGTH_; i++) {
			StringData[i] = CRYPT_CHAR(StringData[i], i);
		}

		return (_CHAR_TYPE_*)(StringData);
	}

private:


	static XORSTR_CONST UINT64 Key = XORSTR_KEY(_STR_LENGTH_);

	static XORSTR_CONST_INLINE
		_CHAR_TYPE_ CRYPT_CHAR(
			IN _CHAR_TYPE_ Character,
			IN SIZE_T KeyIndex
		)
	{
		return (Character ^ ((Key + KeyIndex) ^
			(XORSTR_NIBBLE(Key, KeyIndex % 16))));
	}

	template< SIZE_T... _INDEX_ >
	XORSTR_CONST_INLINE _XORSTR_(
		IN _CHAR_TYPE_ CONST(&String)[_STR_LENGTH_],
		IN XORSTR_INTEGER_SEQUENCE(_INDEX_) IntSeq
	) : StringData{ CRYPT_CHAR(String[_INDEX_], _INDEX_)... }
	{
	}

	XORSTR_VOLATILE _CHAR_TYPE_ StringData[_STR_LENGTH_];
};

template< SIZE_T _STR_LEN_ >
XORSTR_CONST_INLINE
_XORSTR_< CHAR, _STR_LEN_ > XorStr(
	IN CHAR CONST(&String)[_STR_LEN_]
)
{
	return _XORSTR_< CHAR, _STR_LEN_ >(String);
}

template< SIZE_T _STR_LEN_ >
XORSTR_CONST_INLINE
_XORSTR_< WCHAR, _STR_LEN_ > XorStr(
	IN WCHAR CONST(&String)[_STR_LEN_]
)
{
	return _XORSTR_< WCHAR, _STR_LEN_ >(String);
}

template< SIZE_T _STR_LEN_ >
XORSTR_CONST_INLINE
_XORSTR_< char32_t, _STR_LEN_ > XorStr(
	IN char32_t CONST(&String)[_STR_LEN_]
)
{
	return _XORSTR_< char32_t, _STR_LEN_ >(String);
}

#define _( _STR_ ) XorStr( _STR_ ).String( )

#endif